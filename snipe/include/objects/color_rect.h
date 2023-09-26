#pragma once
#ifndef COLORRECT_H
#define COLORRECT_H

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "objects/object2d.h"
#include "shader.h"

class ColorRect : public Object2D {
public:
	vec2 pivot_offset = { 0, 0 };
	vec4 color        = { 1, 1, 1, 1 };
public:
	ColorRect(Scene* _owner, std::wstring _name);
	~ColorRect();
	void update() override;
	void draw() override;
};


#endif