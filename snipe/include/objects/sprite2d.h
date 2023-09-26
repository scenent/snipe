#pragma once
#ifndef SPRITE2D_H
#define SPRITE2D_H

#include <stb_image/stb_image.h>

#include "objects/object2d.h"

class Sprite2D : public Object2D {
private:
public:
	std::wstring tag       = L"";
	std::wstring filePath  = L"";

	unsigned int textureID = NULL;
	vec2 pivot_offset      = { 0, 0 };
	vec4 modulate          = { 1, 1, 1, 1 };
	bool pixel_filter      = true;
public:
	Sprite2D(Scene* _owner, std::wstring _name);
	~Sprite2D();
	void load(std::wstring _filePath);
	void update() override;
	void draw() override;
};


#endif