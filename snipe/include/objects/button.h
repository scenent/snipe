#pragma once
#ifndef BUTTON_H
#define BUTTON_H

#include "objects/button_base.h"
#include "objects/color_rect.h"
#include "objects/label.h"

class Button : public ButtonBase {
public:
	ColorRect* background = nullptr;
	Label* textLabel = nullptr;
public:
	Button(Scene* _owner, std::wstring _name);
	~Button();
	void load(std::wstring fontPath);
	void update() override;
	void draw() override;
};

#endif