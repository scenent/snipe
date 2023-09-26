#include "objects/dialog.h"
#include "scene/scene.h"
#include "root.h"

extern bool SNIPE_USING_MULTITHREAD;

Dialog::Dialog(Scene* _owner, std::wstring _name) : Object2D(_owner, _name) {
	type = L"Dialog";
	textLabel = new Label(_owner, L"DialogTextLabel");
	nameLabel = new Label(_owner, L"DialogNameLabel");
	nameLabel->position = position;
	nameLabel->update();
	nameLabel->size = vec2(200, 50);
	nameLabel->align_mode = LabelAlignMode::Center;
	if (nameLabel->align_mode == LabelAlignMode::Begin) nameLabel->position.x = nameLabel->position.x;
	else if (nameLabel->align_mode == LabelAlignMode::Center) nameLabel->position.x = (nameLabel->position.x + (nameLabel->size.x / 2.0f) - (nameLabel->glyph_size.x / 2.0f));
	else nameLabel->position.x = nameLabel->position.x + nameLabel->size.x - nameLabel->glyph_size.x;
	namebox = new Sprite2D(_owner, L"DialogNameBox");
	background = new Sprite2D(_owner, L"DialogBackground");
	textLabel->fontPath = fontPath;
	nameLabel->fontPath = nameFontPath;
}
Dialog::~Dialog() {
	delete nameLabel, textLabel, namebox, background;
}
void Dialog::update() {
	if (((Scene2D*)owner)->dialogPaused == true) return;
	FrameEvent event = owner->root->currentFrameEvent;
	if (event.mouse.buttons[GLFW_MOUSE_BUTTON_2].action == GLFW_PRESS) {
		visible = false;
	}
	if (visible == false && (event.justKeys[GLFW_KEY_SPACE].action == GLFW_PRESS || event.mouse.buttons[GLFW_MOUSE_BUTTON_1].action == GLFW_PRESS)) {
		visible = true;
		return;
	}
	if (!visible)return;
	textLabel->position = position;
	textLabel->size = size;
	double speedEach = speed / textLabel->text.size();
	if (state == DialogState::Reading) {
		if (textLabel->percent_visible + speedEach >= 1.0) {
			state = DialogState::Finished;
		}
		textLabel->percent_visible += speedEach;
	}
	if (owner->currentFocused == nullptr && (event.mouse.buttons[GLFW_MOUSE_BUTTON_1].action == GLFW_PRESS || event.justKeys[GLFW_KEY_SPACE].action == GLFW_PRESS)) {
		if (state == DialogState::Reading) {
			if (can_skip) {
				textLabel->percent_visible = 1.0;
				state = DialogState::Finished;
			}
		}
		else if (state == DialogState::Finished) {
			progressed = true;
			if (sayQueue.empty() == false) {
				nameLabel->text = sayQueue.front().first;
				nameLabel->fontSize = nameFontSize;
				textLabel->text = sayQueue.front().second; sayQueue.pop();
				textLabel->fontSize = fontSize;
				textLabel->percent_visible = 0.0f;
				textLabel->load(fontPath);
				nameLabel->load(nameFontPath);
				state = DialogState::Reading;
				nameLabel->position = nameLabelPos;
				if (nameLabel->align_mode == LabelAlignMode::Begin) nameLabel->position.x = nameLabelPos.x;
				else if (nameLabel->align_mode == LabelAlignMode::Center) nameLabel->position.x = (nameLabelPos.x + (nameLabel->size.x / 2.0f) - (nameLabel->glyph_size.x / 2.0f));
				else nameLabel->position.x = nameLabelPos.x + nameLabel->size.x - nameLabel->glyph_size.x;
			}
		}
	}
	if (state == DialogState::Idle && sayQueue.empty() == false) {
		nameLabel->text = sayQueue.front().first;
		nameLabel->fontSize = nameFontSize;
		textLabel->text = sayQueue.front().second; sayQueue.pop();
		textLabel->fontSize = fontSize;
		textLabel->percent_visible = 0.0f;
		textLabel->load(fontPath);
		nameLabel->load(nameFontPath);
		nameLabel->draw();
		state = DialogState::Reading;
		nameLabel->position = nameLabelPos;
		if (nameLabel->align_mode == LabelAlignMode::Begin) nameLabel->position.x = nameLabelPos.x;
		else if (nameLabel->align_mode == LabelAlignMode::Center) nameLabel->position.x = (nameLabelPos.x + (nameLabel->size.x / 2.0f) - (nameLabel->glyph_size.x / 2.0f));
		else nameLabel->position.x = nameLabelPos.x + nameLabel->size.x - nameLabel->glyph_size.x;
	}
}
void Dialog::draw() {
	if (!visible)return;
	background->draw();
	namebox->draw();
	textLabel->draw();
	nameLabel->draw();
}
void Dialog::say(std::wstring _name, std::wstring _content, bool _can_skip) {
	if ((_name == L"" && _content == L"")) {
		return;
	}
	nameLabel->visible = true;
	textLabel->visible = true;
	if (fontPath == L"") alert(L"there's no assigned font path");
	if (nameFontPath == L"") alert(L"there's no assigned font path for name box");
	if (_name == L"") {
		namebox->visible = false;
	}
	else {
		namebox->visible = true;
	}
	_content = replaceAll(_content, L"\\n", L"\n");
	can_skip = _can_skip;
	if (SNIPE_USING_MULTITHREAD == false){
		if (state == DialogState::Reading) {
			sayQueue.push({ _name, _content });
		}
		else {
			nameLabel->text = _name;
			textLabel->text = _content;
			nameLabel->fontSize = nameFontSize;
			textLabel->fontSize = fontSize;
			textLabel->percent_visible = 0.0f;
			textLabel->load(fontPath);
			nameLabel->load(nameFontPath);
			state = DialogState::Reading;
		}
	}
	else {
		if (progressed == true) progressed = false;
		sayQueue.push({ _name, _content });
		state = DialogState::Idle;
	}
}

void Dialog::reset() {
	visible = true;
	textLabel->color = vec4(1, 1, 1, 1);
	nameLabel->text = L"";
	textLabel->text = L"";
	textLabel->percent_visible = 0.0f;
	textLabel->load(fontPath);
	nameLabel->load(nameFontPath);
	while (sayQueue.empty() == false) sayQueue.pop();
	textLabel->text = L"";
	textLabel->fontPath = fontPath;
	textLabel->load(fontPath);
	nameLabel->text = L"";
	nameLabel->fontPath = nameFontPath;
	nameLabel->load(nameFontPath);
	textLabel->visible = false;
	nameLabel->visible = false;
	state = DialogState::Idle;
}