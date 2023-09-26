#pragma once
#ifndef TEXTUREBUTTON_H
#define TEXTUREBUTTON_H

#include "objects/button_base.h"
#include "objects/sprite2d.h"
#include "objects/label.h"


class TextureButton : public ButtonBase {
public:
	Label* textLabel = nullptr;
	Sprite2D *idleTexture = nullptr, *hoverTexture = nullptr, *downTexture = nullptr;

	std::wstring idlePath = L"";
	std::wstring hoverPath = L"";
	std::wstring downPath = L"";
public:
	TextureButton(Scene* _owner, std::wstring _name);
	~TextureButton();
	void load(std::wstring _fontPath, std::wstring _idlePath, std::wstring _hoverPath, std::wstring _downPath);
	void update() override;
	void draw() override;
};


#endif