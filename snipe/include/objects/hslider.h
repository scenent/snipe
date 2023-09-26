#pragma once
#ifndef HSLIDER_H
#define HSLIDER_H

#include "objects/slider_base.h"

class Hslider : public SliderBase {
public:
	vec2 grabber_size = { 10, 20 };
public:
	Hslider(Scene* _owner, std::wstring _name);
	~Hslider();
	void update() override;
	void draw() override;
};

#endif