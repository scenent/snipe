#include "objects/vslider.h"
#include "scene/scene.h"
#include "root.h"
#include "script/interpreter.h"


Vslider::Vslider(Scene* _owner, std::wstring _name) : SliderBase(_owner, _name){
	type = L"Vslider";
	size = { 20, 100 };
}
Vslider::~Vslider() {

}
void Vslider::update() {
	if (!visible) return;
	Camera* cam = owner->currentCamera;
	FrameEvent event = owner->root->currentFrameEvent;
	vec2 mousePos = vec2(event.mouse.pos.xPos, event.mouse.pos.yPos);
	if (root->WINDOW_FULLSCREEN == true) {
		mousePos.scaleBased(vec2((float)root->WINDOW_WIDTH_ORIGIN / (float)root->WINDOW_WIDTH, (float)root->WINDOW_HEIGHT_ORIGIN / (float)root->WINDOW_HEIGHT), vec2(0, 0));
	}
	polygon grabberPoly;
	vec2 _0 = (vec2(this->position)                              + vec2(0.0, size.y * percent - (grabber_size.y / 2))).rotatedBased(-rotation, position + pivot_offset) - owner->currentCamera->position;
	vec2 _1 = (vec2(this->position) + vec2(grabber_size.x, 0.0f) + vec2(0.0, size.y * percent - (grabber_size.y / 2))).rotatedBased(-rotation, position + pivot_offset) - owner->currentCamera->position;
	vec2 _2 = (vec2(this->position) + grabber_size               + vec2(0.0, size.y * percent - (grabber_size.y / 2))).rotatedBased(-rotation, position + pivot_offset) - owner->currentCamera->position;
	vec2 _3 = (vec2(this->position) + vec2(0.0f, grabber_size.y) + vec2(0.0, size.y * percent - (grabber_size.y / 2))).rotatedBased(-rotation, position + pivot_offset) - owner->currentCamera->position;
	_0.rotateBased(rad2deg(-cam->rotation), cam->offset - cam->position);
	_1.rotateBased(rad2deg(-cam->rotation), cam->offset - cam->position);
	_2.rotateBased(rad2deg(-cam->rotation), cam->offset - cam->position);
	_3.rotateBased(rad2deg(-cam->rotation), cam->offset - cam->position);
	_0.scale(cam->zoom - vec2(1.0, 1.0), cam->offset);
	_1.scale(cam->zoom - vec2(1.0, 1.0), cam->offset);
	_2.scale(cam->zoom - vec2(1.0, 1.0), cam->offset);
	_3.scale(cam->zoom - vec2(1.0, 1.0), cam->offset);
	grabberPoly.vertices = { _0, _1, _2, _3 };
	if (polygon::pointVspolygon(mousePos, grabberPoly)) {
		if (state == SliderState::Idle) state = SliderState::Hover;
	}
	else {
		if (state == SliderState::Hover) {
			state = SliderState::Idle;
		}
	}
	if (event.mouse.buttons->action == GLFW_PRESS) {
		if (polygon::pointVspolygon(mousePos, grabberPoly)) {
			state = SliderState::Down;
		}
	}
	else if (event.mouse.buttons->action == GLFW_RELEASE) {
		if (state == SliderState::Down) {
			state = SliderState::Hover;
			for (auto& p : callbackList) {
				std::thread t(interpret_script, Sgt::instance().labelMap[p], false, false, false, true);
				t.detach();
				if (t.joinable()) t.join();
			}
			for (auto& p : funcCallbackList) {
				p(this);
			}
			if (callbackValue != L"") {
				Sgt::instance().varMap[callbackValue] = std::to_wstring(percent);
			}
		}
	}
	if (state == SliderState::Down) {
		percent = ((mousePos - position).y + cam->position.y) / size.y;
		if (percent > 1.0f) percent = 1.0f;
		if (percent < 0.0f) percent = 0.0f;

	}
	if (state == SliderState::Idle) color = idleColor;
	else if (state == SliderState::Hover) color = hoverColor;
	else if (state == SliderState::Down) color = downColor;
}
void Vslider::draw() {
	if (!visible) return;
	drawRectEx(owner->root, position, size, rotation, pivot_offset, scale, backgroundColor);
	vec2 tempPos = position + vec2(0.0, size.y * percent - (grabber_size.y / 2));
	vec2 tempSize = grabber_size;
	if (tempPos.y < position.y) {
		float bias = position.y - tempPos.y;
		tempPos.y += bias;
		tempSize.y -= bias;
	}
	if (tempPos.y + tempSize.y > position.y + size.y) {
		tempSize.y -= (tempPos.y + tempSize.y)- (position.y + size.y);
	}
	drawRectEx(owner->root, tempPos, tempSize, rotation, pivot_offset, scale, color);
}