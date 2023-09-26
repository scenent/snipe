#pragma once
#ifndef SLIDERBASE_H
#define SLIDERBASE_H

#include "objects/object2d.h"

enum class SliderState {
	Idle, Hover, Down
};

class SliderBase : public Object2D {
public:
	vec4 backgroundColor = { 1, 1, 1, 1 };
	vec4 idleColor = { 0.7568, 0.7568, 0.7568, 1.0 };
	vec4 hoverColor = { 0.6588, 0.6588, 0.6588, 1.0 };
	vec4 downColor = { 0.4705, 0.4705, 0.4705, 1.0 };

	vec4 color = idleColor;

	vec2 pivot_offset = { 0, 0 };
	double percent = 0.0;
	SliderState state = SliderState::Idle;
	std::vector<std::function<void(Object*)>> funcCallbackList{};
	std::vector<std::wstring> callbackList{};
	std::wstring callbackValue = L"";
public:
	SliderBase(Scene* _owner, std::wstring _name);
	~SliderBase();
	void update() override;
	void draw() override;
};







#endif