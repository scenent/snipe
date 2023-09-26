#pragma once
#ifndef BUTTONBASE_H
#define BUTTONBASE_H

#include "objects/object2d.h"
#include "objects/color_rect.h"
#include "objects/label.h"

enum class ButtonState {
	Idle, Hover, Down
};

enum class ButtonTextAlign {
	Begin, Center, End
};

class ButtonBase : public Object2D {
public:
	std::wstring text = L"";
	std::wstring fontPath = L"";
	float fontSize = 16;

	bool pressed = false;

	vec4 idleColor = { 0.7568, 0.7568, 0.7568, 1.0 };
	vec4 hoverColor = { 0.6588, 0.6588, 0.6588, 1.0 };
	vec4 downColor = { 0.4705, 0.4705, 0.4705, 1.0 };

	vec4 idleOutlineColor = { 1.0, 1.0, 1.0, 1.0 };
	vec4 hoverOutlineColor = { 1.0, 1.0, 1.0, 1.0 };
	vec4 downOutlineColor = { 1.0, 1.0, 1.0, 1.0 };

	vec4 idleModulate = { 1, 1, 1, 1 };
	vec4 hoverModulate = { 1, 1, 1, 1 };
	vec4 downModulate = { 1, 1, 1, 1 };

	vec4 color = idleColor;
	vec4 outline_color = idleOutlineColor;
	ButtonState state = ButtonState::Idle;
	ButtonTextAlign textAlign = ButtonTextAlign::Center;

	std::vector<std::function<void(Object*)>> funcCallbackList{};
	std::vector<std::wstring> callbackList{};
public:
	ButtonBase(Scene* _owner, std::wstring _name);
	~ButtonBase();
	void update() override;
	void draw() override;
};

#endif