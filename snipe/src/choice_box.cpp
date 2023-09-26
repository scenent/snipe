#include "objects/choice_box.h"
#include "scene/scene.h"
#include "root.h"

extern bool SNIPE_USING_MULTITHREAD;

ChoiceBox::ChoiceBox(Scene* _owner, std::wstring _name) : Object2D(_owner, _name){
	type = L"ChoiceBox";
	size.x = 100;
}

ChoiceBox::~ChoiceBox() {
	for (auto& p : buttonList) {
		delete p;
		p = nullptr;
	}
	buttonList.clear();
}

void ChoiceBox::update() {
	if (tempChoiceInfo.empty() == false) {
		state = ChoiceBoxState::Wait;
		for (auto& p : buttonList) {
			delete p;
			p = nullptr;
		}
		buttonList.clear();
		float yPos = position.y;
		for (int i = 0; i < tempChoiceInfo.size(); i++) {
			TextureButton* b = new TextureButton(owner, tempChoiceInfo[i]);
			b->position = { position.x, yPos };
			b->size.y = eachBoxSize;
			b->size.x = size.x;
			b->text = tempChoiceInfo[i];
			b->fontSize = fontSize;
			b->textLabel->color = fontColor;
			b->textLabel->outline_color = outline_color;
			b->textLabel->outline_size = outline_size;
			b->textAlign = ButtonTextAlign::Center;
			b->idlePath = idleFilePath;
			b->load(fontPath, idleFilePath, hoverFilePath, downFilePath);
			buttonList.push_back(b);
			yPos += eachBoxSize + separation;
		}
		tempChoiceInfo.clear();
	}
	for (auto& p : buttonList) {
		p->update();
	}
	if (state == ChoiceBoxState::Wait) {
		int index = 0;
		for (auto& p : buttonList) {
			if (p->pressed == true) {
				state = ChoiceBoxState::Idle;
				*result = index;
				for (auto& p : buttonList) {
					delete p;
					p = nullptr;
				}
				buttonList.clear();
				result = nullptr;
				break;
			}
			index++;
		}
	}
}

void ChoiceBox::draw() {
	if (!visible) return;
	if (state == ChoiceBoxState::Wait) {
		for (auto& p : buttonList) {
			p->draw();
		}
	}
}

void ChoiceBox::choice(std::vector<std::wstring> _messages, int* _result) {
	if (fontPath == L"") alert(L"there's no font path : choicebox : called : " + name);
	result = _result;
	if (SNIPE_USING_MULTITHREAD == false) {
		state = ChoiceBoxState::Wait;
		for (auto& p : buttonList) {
			delete p;
			p = nullptr;
		}
		buttonList.clear();
		float yPos = position.y;
		for (int i = 0; i < _messages.size(); i++) {
			TextureButton* b = new TextureButton(owner, _messages[i]);
			b->position = { position.x, yPos };
			b->size.y = eachBoxSize;
			b->size.x = size.x;
			b->text = _messages[i];
			b->textLabel->color = fontColor;
			b->textLabel->outline_color = outline_color;
			b->textLabel->outline_size = outline_size;
			b->textLabel->align_mode = LabelAlignMode::Center;
			b->idlePath = idleFilePath;
			b->load(fontPath, idleFilePath, hoverFilePath, downFilePath);
			buttonList.push_back(b);
			yPos += eachBoxSize + separation;
		}
	}
	else {
		tempChoiceInfo = _messages;
	}
}

void ChoiceBox::reset() {
	for (auto& p : buttonList) {
		delete p;
		p = nullptr;
	}
	buttonList.clear();
}