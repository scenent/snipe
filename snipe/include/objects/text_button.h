#pragma once
#ifndef TEXTBUTTON_H
#define TEXTBUTTON_H

#include "objects/button_base.h"
#include "objects/label.h"

class TextButton : public ButtonBase {
public:
	Label* textLabel = nullptr;
public:
	TextButton(Scene* _owner, std::wstring _name);
	~TextButton();
	void load(std::wstring fontPath);
	void update() override;
	void draw() override;
};


#endif