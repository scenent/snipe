#include "objects/button.h"
#include "objects/object.h"
#include "root.h"
#include "script/interpreter.h"
#include "singleton.h"

Button::Button(Scene* _owner, std::wstring _name) : ButtonBase(_owner, _name){
	type = L"Button";
	background = new ColorRect(owner, L"ButtonBackground");
	textLabel = new Label(owner, L"ButtonTextLabel");
}
Button::~Button() {
	if (owner->currentFocused == this) {
		owner->currentFocused = nullptr;
	}
	delete background, textLabel;
}
void Button::load(std::wstring fontPath) {
	this->fontPath = fontPath;
	background->position = this->position;
	background->size = this->size;
	background->color = this->idleColor;
	background->rotation = this->rotation;
	background->scale = this->scale;
	background->pivot_offset = this->pivot_offset;
	textLabel->position = this->position;
	textLabel->size = this->size;
	textLabel->rotation = this->rotation;
	textLabel->scale = this->scale;
	textLabel->text = this->text;
	textLabel->fontSize = this->fontSize;
	textLabel->pivot_offset = this->pivot_offset;
	textLabel->load(this->fontPath);
	textLabel->draw();
}
void Button::update() {
	if (pressed == true) pressed = false;
	background->position = this->position;
	background->size = this->size;
	background->rotation = this->rotation;
	background->scale = this->scale;
	background->pivot_offset = this->pivot_offset;
	if (textAlign == ButtonTextAlign::Begin) textLabel->position.x = position.x;
	else if (textAlign == ButtonTextAlign::Center) textLabel->position.x = position.x + (size.x / 2) - (textLabel->glyph_size.x / 2);
	else textLabel->position.x = position.x + size.x - textLabel->glyph_size.x;
	textLabel->position.y = position.y;
	textLabel->size = this->size;
	textLabel->rotation = this->rotation;
	textLabel->scale = this->scale;
	textLabel->pivot_offset = this->pivot_offset;
	if (!visible) {
		if (owner->currentFocused == this) {
			owner->currentFocused = nullptr;
		}
		state = ButtonState::Idle;
		return;
	}
	if (state == ButtonState::Idle) color = idleColor;
	else if (state == ButtonState::Hover) color = hoverColor;
	else if (state == ButtonState::Down) color = downColor;
	background->color = this->color;
	Camera* cam = owner->currentCamera;
	FrameEvent event = owner->root->currentFrameEvent;
	vec2 mousePos = vec2(event.mouse.pos.xPos, event.mouse.pos.yPos);
	if (root->WINDOW_FULLSCREEN == true) {
		mousePos.scaleBased(vec2((float)root->WINDOW_WIDTH_ORIGIN / (float)root->WINDOW_WIDTH, (float)root->WINDOW_HEIGHT_ORIGIN / (float)root->WINDOW_HEIGHT), vec2(0, 0));
	}
	rect buttonRect = { this->position - owner->currentCamera->position, this->size };
	polygon buttonPoly;
	vec2 _0 = (vec2(this->position)).rotatedBased(-rotation, position + pivot_offset) - owner->currentCamera->position;
	vec2 _1 = (vec2(this->position) + vec2(size.x , 0.0f)).rotatedBased(-rotation, position + pivot_offset) - owner->currentCamera->position;
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
	buttonPoly.vertices = { _0, _1, _2, _3 };
	if (polygon::pointVspolygon(mousePos, buttonPoly)) {
		if (state == ButtonState::Idle) {
			state = ButtonState::Hover;
			owner->currentFocused = this;
		}
	}
	else {
		state = ButtonState::Idle;
		if (owner->currentFocused == this) {
			owner->currentFocused = nullptr;
		}
	}
	if (event.mouse.buttons[0].action == GLFW_PRESS) {
		if (polygon::pointVspolygon(mousePos, buttonPoly)) {
			state = ButtonState::Down;
		}
	}
	else if (event.mouse.buttons[0].action == GLFW_RELEASE) {
		if (state == ButtonState::Down) {
			state = ButtonState::Idle;
			if (polygon::pointVspolygon(mousePos, buttonPoly)) {
				state = ButtonState::Hover;
				pressed = true;
				for (auto& p : funcCallbackList) {
					p(this);
				}
				for (std::wstring p : callbackList) {
					if (Sgt::instance().labelExists(p)) {
						std::thread t(interpret_script, Sgt::instance().labelMap[p], false, false, false, true);
						t.detach();
					}
					else {
						alert(L"failed to process callback :: there is no label called " + p);
					}
				}
			}
		}
	}
}
void Button::draw() {
	if (!visible) return;
	background->draw();
	textLabel->draw();
}