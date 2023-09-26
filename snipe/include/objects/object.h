#pragma once
#ifndef OBJECT_H
#define OBJECT_H

#include <string>
#include <vector>
#include <iostream>
#include <functional>
#include <thread>

#include <glad/include/glad/glad.h>
#include <GLFW/include/GLFW/glfw3.h>

#include "frame_event.h"
#include "math_utils.h"
#include "utils.h"

class Scene;

enum class ObjectState {
	Idle, UpdateBegin, UpdateEnd, DrawBegin, DrawEnd
};

class Object {
public:
	std::wstring type = L"Object";
	Scene*       owner   = nullptr;
	std::wstring name    = L"";
	bool         visible = true;
protected:
	ObjectState  objectState = ObjectState::Idle;
	std::map<ObjectState, std::wstring> objectStateMap = {
		{ObjectState::Idle, L"Idle"},
		{ObjectState::UpdateBegin, L"UpdateBegin"},
		{ObjectState::UpdateEnd, L"UpdateEnd"},
		{ObjectState::DrawBegin, L"DrawBegin"},
		{ObjectState::DrawEnd, L"DrwaEnd"}
	};
public:
	Object(Scene* _owner, std::wstring _name);
	virtual void update() = 0;
	virtual void draw() = 0;
};


#endif