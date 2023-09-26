#include "objects/texture_button.h"
#include "scene/scene.h"
#include "root.h"
#include "singleton.h"
#include "script/interpreter.h"

TextureButton::TextureButton(Scene* _owner, std::wstring _name) : ButtonBase(_owner, _name) {
	type = L"TextureButton";
	textLabel = new Label(_owner, _name);
	idleTexture = new Sprite2D(_owner, _name);
	hoverTexture = new Sprite2D(_owner, _name);
	downTexture = new Sprite2D(_owner, _name);
}
TextureButton::~TextureButton() {
	delete textLabel, idleTexture, hoverTexture, downTexture;
}
void TextureButton::load(std::wstring _fontPath, std::wstring _idlePath, std::wstring _hoverPath, std::wstring _downPath) {
	this->fontPath = _fontPath;
	this->idlePath = _idlePath;
	this->hoverPath = _hoverPath;
	this->downPath = _downPath;
	idleTexture ->position = this->position;
	idleTexture->modulate = this->idleModulate;
	idleTexture->rotation = this->rotation;
	idleTexture->scale = this->scale;
	idleTexture->pivot_offset = this->pivot_offset;
	idleTexture->load(_idlePath);
	hoverTexture->position = this->position;
	hoverTexture->modulate = this->idleModulate;
	hoverTexture->rotation = this->rotation;
	hoverTexture->scale = this->scale;
	hoverTexture->pivot_offset = this->pivot_offset;
	hoverTexture->load(_hoverPath);
	downTexture->position = this->position;
	downTexture->modulate = this->idleModulate;
	downTexture->rotation = this->rotation;
	downTexture->scale = this->scale;
	downTexture->pivot_offset = this->pivot_offset;
	downTexture->load(_downPath);
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
void TextureButton::update() {
	if (pressed == true) pressed = false;
	idleTexture->position = this->position;
	idleTexture->modulate = this->idleModulate;
	idleTexture->rotation = this->rotation;
	idleTexture->scale = this->scale;
	idleTexture->pivot_offset = this->pivot_offset;
	hoverTexture->position = this->position;
	hoverTexture->modulate = this->idleModulate;
	hoverTexture->rotation = this->rotation;
	hoverTexture->scale = this->scale;
	hoverTexture->pivot_offset = this->pivot_offset;
	downTexture->position = this->position;
	downTexture->modulate = this->idleModulate;
	downTexture->rotation = this->rotation;
	downTexture->scale = this->scale;
	downTexture->pivot_offset = this->pivot_offset;
	textLabel->position = this->position;
	textLabel->size = this->size;
	textLabel->rotation = this->rotation;
	textLabel->scale = this->scale;
	textLabel->pivot_offset = this->pivot_offset;
	if (textAlign == ButtonTextAlign::Begin) textLabel->position.x = position.x;
	else if (textAlign == ButtonTextAlign::Center) textLabel->position.x = position.x + (size.x / 2) - (textLabel->glyph_size.x / 2);
	else textLabel->position.x = position.x + size.x - textLabel->glyph_size.x;
	if (state == ButtonState::Idle) { idleTexture->visible = true; hoverTexture->visible = false; downTexture->visible = false; }
	else if (state == ButtonState::Hover) { idleTexture->visible = false; hoverTexture->visible = true; downTexture->visible = false; }
	else if (state == ButtonState::Down) { idleTexture->visible = false; hoverTexture->visible = false; downTexture->visible = true; }
	if (!visible) {
		if (owner->currentFocused == this) {
			owner->currentFocused = nullptr;
		}
		state = ButtonState::Idle;
		return;
	}
	Camera* cam = owner->currentCamera;
	FrameEvent event = owner->root->currentFrameEvent;
	vec2 mousePos = vec2(event.mouse.pos.xPos, event.mouse.pos.yPos);
	if (root->WINDOW_FULLSCREEN == true) {
		mousePos.scaleBased(vec2((float)root->WINDOW_WIDTH_ORIGIN / (float)root->WINDOW_WIDTH, (float)root->WINDOW_HEIGHT_ORIGIN / (float)root->WINDOW_HEIGHT), vec2(0, 0));
	}
	rect buttonRect = { this->position - owner->currentCamera->position, this->size };
	polygon buttonPoly;
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
	buttonPoly.vertices = { _0, _1, _2, _3 };
	if (polygon::pointVspolygon(mousePos, buttonPoly)) {
		if (state == ButtonState::Idle) state = ButtonState::Hover;
	}
	else state = ButtonState::Idle;
	if (event.mouse.buttons->action == GLFW_PRESS) {
		if (polygon::pointVspolygon(mousePos, buttonPoly)) {
			state = ButtonState::Down;
		}
	}
	else if (event.mouse.buttons->action == GLFW_RELEASE) {
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
void TextureButton::draw() {
	if (!visible)return;
	if (state == ButtonState::Hover) hoverTexture->draw();
	else if (state == ButtonState::Down) downTexture->draw();
	else idleTexture->draw();
	textLabel->draw();
}