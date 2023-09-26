#pragma once
#ifndef OBJECT3D_H
#define OBJECT3D_H

#include "objects/object.h"

class Object3D : public Object {
public:
	vec3 position  = { 0, 0, 0 };
	vec3 rotation  = { 0, 0, 0 };
	vec3 scale     = { 0, 0, 0 };
public:
	Object3D(Scene* _owner, std::wstring _name);
};

#endif