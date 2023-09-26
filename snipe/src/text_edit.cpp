#include "objects/text_edit.h"
#include "scene/scene.h"
#include "root.h"

TextEdit::TextEdit(Scene* _owner, std::wstring _name) : Object2D(_owner, _name) {
	type = L"TextEdit";
	textLabel = new Label(_owner, L"TextEditTextLabel");
	limit = 100;
	visible = false;
}

TextEdit::~TextEdit() {
	if (owner->currentFocused == this) {
		owner->currentFocused = nullptr;
	}
	delete textLabel;
}

void TextEdit::input(std::wstring* result, unsigned int limit) {
	visible = true;
	inputResult = result;
	this->limit = limit;
	owner->currentFocused = this;
}

void TextEdit::setCharCallback(unsigned int codepoint) {
	if (text.size() + 1 > limit) {
		return;
	}
	if (owner->currentFocused == this) {
		if (fontPath == L"") alert(L"please assign a font path to text edit");
		if (wchar_t(codepoint) == L' ') {
			if (index == 0) {
				return;
			}
			text.insert(text.begin() + index, L' ');
			index++;
		}
		else {
			text.insert(text.begin() + index, wchar_t(codepoint));
			index++;
		}
		textLabel->text = text;
		textLabel->load(fontPath);
	}
}

void TextEdit::update() {
	textLabel->position = position;
	textLabel->size = size;
	textLabel->fontSize = fontSize;
	if (!visible) return;
	FrameEvent event = owner->root->currentFrameEvent;
	Camera* cam = owner->currentCamera;
	vec2 mousePos = vec2(event.mouse.pos.xPos, event.mouse.pos.yPos);
	if (owner->root->WINDOW_FULLSCREEN == true) {
		mousePos.scaleBased(vec2((float)owner->root->WINDOW_WIDTH_ORIGIN / (float)owner->root->WINDOW_WIDTH, (float)owner->root->WINDOW_HEIGHT_ORIGIN / (float)owner->root->WINDOW_HEIGHT), vec2(0, 0));
	}
	polygon editPoly;
	{
		vec2 _0 = (vec2(this->position)).rotatedBased(-rotation, position + pivot_offset) - owner->currentCamera->position;
		vec2 _1 = (vec2(this->position) + vec2(size.x, 0.0f)).rotatedBased(-rotation, position + pivot_offset) - owner->currentCamera->position;
		vec2 _2 = (vec2(this->position) + size).rotatedBased(-rotation, position + pivot_offset) - owner->currentCamera->position;
		vec2 _3 = (vec2(this->position) + vec2(0.0f, size.y)).rotatedBased(-rotation, position + pivot_offset) - owner->currentCamera->position;
		_0.rotateBased(rad2deg(-cam->rotation), cam->offset - cam->position);
		_1.rotateBased(rad2deg(-cam->rotation), cam->offset - cam->position);
		_2.rotateBased(rad2deg(-cam->rotation), cam->offset - cam->position);
		_3.rotateBased(rad2deg(-cam->rotation), cam->offset - cam->position);
		_0.scale(cam->zoom - vec2(1.0, 1.0), cam->offset);
		_1.scale(cam->zoom - vec2(1.0, 1.0), cam->offset);
		_2.scale(cam->zoom - vec2(1.0, 1.0), cam->offset);
		_3.scale(cam->zoom - vec2(1.0, 1.0), cam->offset);
		editPoly.vertices = { _0, _1, _2, _3 };
	}
	if (event.mouse.buttons[0].action == GLFW_PRESS) {
		if (polygon::pointVspolygon(mousePos, editPoly)) {
			owner->currentFocused = this;
		}
		else {
			if (owner->currentFocused == this) {
				owner->currentFocused = nullptr;
			}
		}
	}
	if (fast_backspace ? (event.pressedKeys[GLFW_KEY_BACKSPACE]) : (event.justKeys[GLFW_KEY_BACKSPACE].action == GLFW_PRESS)){
		if (!(text == L"") && (index == 0) == false) {
			index--;
			text.erase(text.begin() + index);
			textLabel->text = text;
			textLabel->load(fontPath);
		}
	}
	if (event.justKeys[GLFW_KEY_ENTER].action == GLFW_PRESS) {
		if (owner->currentFocused == this) owner->currentFocused = nullptr;
		visible = false;
		*inputResult = text;
		limit = 10;
		text = L"";
		textLabel->text = L"";
		textLabel->load(textLabel->fontPath);
		index = 0;
		return;
	}
	if (event.justKeys[GLFW_KEY_LEFT].action == GLFW_PRESS) {
		if (index - 1 >= 0) {
			index--;
			textLabel->text = text;
			textLabel->load(fontPath);
		}
	}
	if (event.justKeys[GLFW_KEY_RIGHT].action == GLFW_PRESS) {
		if (index + 1 < text.size()+1) {
			index++;
			textLabel->text = text;
			textLabel->load(fontPath);
		}
	}
}

void TextEdit::draw() {
	if (!visible) return;
	textLabel->draw();
	vec2 cursorPos = position;
	for (int i = 0; i < index; i++) {
		Character cur = textLabel->characters[i];
		if (cur.data == L'\n') {
			cursorPos = { 0.0f, cursorPos.y + (float)fontSize - textLabel->y_separation * 2};
		}
		else {
			cursorPos.x += (cur.advance >> 6) * textLabel->fontScale;
		}
		if (cursorPos.x > position.x + size.x) {
			cursorPos = { 0.0f, cursorPos.y + (float)fontSize - textLabel->y_separation  * 2};
		}
	}
	drawLine(owner->root, cursorPos, cursorPos + vec2(0, fontSize), 1, { 1, 1, 1, 1 });
}

void TextEdit::reset() {
	visible = false;
	textLabel->text = L"";
	textLabel->load(fontPath);
}