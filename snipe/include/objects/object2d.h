#pragma once
#ifndef OBJECT2D_H
#define OBJECT2D_H

#include "objects/object.h"

class Object2D : public Object {
public:
	vec2 position  = { 0, 0 };
	vec2 size = { 0, 0 };
	vec2 pivot_offset = { 0, 0 };
	float rotation = 0.0f;
	vec2 scale     = { 1, 1 };
	int z_order    = 0;
public:
	Object2D(Scene* _owner, std::wstring _name);
};

#endif