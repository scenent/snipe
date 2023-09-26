#pragma once
#ifndef VSLIDER_H
#define VSLIDER_H

#include "objects/slider_base.h"

class Vslider : public SliderBase {
public:
	vec2 grabber_size = { 20, 10 };
public:
	Vslider(Scene* _owner, std::wstring _name);
	~Vslider();
	void update() override;
	void draw() override;
};


#endif