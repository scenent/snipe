#include "objects/history_container.h"
#include "scene/scene.h"
#include "root.h"

HistoryContainer::HistoryContainer(Scene* _owner, std::wstring _name) : ContainerBase(_owner, _name){
	type = L"HistoryContainer";
	slider = new Vslider(_owner, L"HistoryContainerVslider");
	size = { 100, 500 };
}
HistoryContainer::~HistoryContainer() {
	for (auto& p : historyObjectList) {
		if (p.first != nullptr) delete p.first;
		if (p.second != nullptr) delete p.second;
		p.first = nullptr;
		p.second = nullptr;
	}
	historyObjectList.clear();
	delete slider;
}
void HistoryContainer::update() {
	while (tempHistoryQueue.empty() == false) {
		std::pair<std::wstring, std::wstring> tempHistory = tempHistoryQueue.front();
		tempHistoryQueue.pop();
		Label* nameBox = new Label(owner, L"HistoryName");
		Label* contentBox = new Label(owner, L"HistoryContent");
		nameBox->text = tempHistory.first;
		contentBox->text = tempHistory.second;
		nameBox->fontSize = fontSize;
		contentBox->fontSize = fontSize;
		nameBox->load(fontPath);
		contentBox->load(fontPath);
		nameBox->draw();
		nameBox->position.x = position.x;
		contentBox->position.x = position.x + space;
		nameBox->size.x = size.x;
		contentBox->size.x = size.x - 200;
		nameBox->clipping_rect = { position, size };
		contentBox->clipping_rect = { position, size + vec2(50, 0)};
		historyObjectList.push_back({ nameBox, contentBox });
		tempHistory.first = L"";
		tempHistory.second = L"";
	}
	FrameEvent event = owner->root->currentFrameEvent;
	vec2 mousePos = vec2(event.mouse.pos.xPos, event.mouse.pos.yPos);
	if (owner->root->WINDOW_FULLSCREEN == true) {
		mousePos.scaleBased(vec2((float)owner->root->WINDOW_WIDTH_ORIGIN / (float)owner->root->WINDOW_WIDTH, (float)owner->root->WINDOW_HEIGHT_ORIGIN / (float)owner->root->WINDOW_HEIGHT), vec2(0, 0));
	}
	Camera* cam = owner->currentCamera;
	slider->position = vec2(position.x + size.x - slider->size.x, position.y);
	slider->size.y = size.y;
	float begin, center, end;
	float children_size = 0;
	for (int i = 0; i < historyObjectList.size(); i++) { children_size += ((Label*)historyObjectList[i].second)->glyph_size.y; children_size += separation; }
	begin = position.y;
	center = position.y + (size.y / 2);
	end = position.y + size.y - children_size;
	float start_pos;
	if (alignMode == ContainerAlignMode::Begin) start_pos = begin;
	else if (alignMode == ContainerAlignMode::Center) start_pos = center - (children_size / 2);
	else start_pos = end;
	int index = 0;
	float y = start_pos;
	for (; index < historyObjectList.size();) {
		historyObjectList[index].first->position.y = y;
		historyObjectList[index].second->position.y = y;
		y += ((Label*)historyObjectList[index].second)->glyph_size.y;
		y += separation;
		index++;
	}
	slider->visible = children_size > size.y;
	if (children_size > size.y) {
		for (auto& p : historyObjectList) {
			p.first->position.y -= (children_size - size.y) * slider->percent;
			p.second->position.y -= (children_size - size.y) * slider->percent;
		}
	}
	slider->grabber_size.y = 1000 / children_size * size.y;
	slider->update();
	if (event.mouse.scroll.yOffset != 0.0f) {
		polygon containerPoly;
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
		containerPoly.vertices = { _0, _1, _2, _3 };
		if (polygon::pointVspolygon(mousePos, containerPoly)) {
			slider->percent += event.mouse.scroll.yOffset > 0 ? -scroll_amount : scroll_amount;
			slider->percent = clamp(slider->percent, 0.0f, 1.0f);
		}
	}
}
void HistoryContainer::draw() {
	if (!visible) return;
	for (auto& p : historyObjectList) {
		p.first->draw();
		p.second->draw();
	}
	slider->draw();
}
void HistoryContainer::set_position(vec2 _position) {
	vec2 degree = _position - position;
	for (auto& p : objectList) {
		p->position += degree;
	}
	position = _position;
}

void HistoryContainer::add_history(std::wstring _name, std::wstring _content) {
	tempHistoryQueue.push({ _name, _content });
}

void HistoryContainer::reset_history() {
	for (auto& p : historyObjectList) {
		delete p.first, p.second;
		p.first = nullptr, p.second = nullptr;
	}
	historyObjectList.clear();
	tempHistoryQueue = std::queue<std::pair<std::wstring, std::wstring>>();
}