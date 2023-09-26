#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "objects/object2d.h"

class Camera : public Object2D {
public:
	bool current = false;
	vec2 offset = { 0, 0 };
	vec3 offset_3d = { 0, 0, 0 };
	vec2 zoom = { 1, 1 };
	vec3 zoom_3d = { 1,1, 1 };
public:
	Camera(Scene* _owner, std::wstring _name) : Object2D(_owner, _name) {};
	virtual glm::mat4 getViewMatrix() = 0;
};

#endif