#pragma once
#ifndef TWEEN_H
#define TWEEN_H

#include <vector>
#include <string>
#include <iostream>
#include <math.h>
#include <algorithm>
#include <any>

#include "objects/object2d.h"
#include "objects/sprite2d.h"
#include "objects/color_rect.h"

struct TweenInfo {
	Object2D* target;
	std::wstring property;
	std::any begin, end;
	std::any step;
	std::any current;
	double duration;
	bool* finished = nullptr;
	void print() {
		std::wcout << L"property = " << property << "\n";
		std::wcout << L"duration = " << duration << "\n";
		std::wcout << L"step = ";
		if (property == L"position" || property == L"size" || property == L"scale") {
			std::wcout << std::any_cast<vec2>(step).x << L"  " << std::any_cast<vec2>(step).y << "\n";
		}
		else if (property == L"rotation") {
			std::wcout << std::any_cast<float>(step) << "\n";
		}
		else if (property == L"modulate") {
			std::any_cast<vec4>(step).print();
		}
		else if (property == L"color") {
			std::any_cast<vec4>(step).print();
		}
	}
	bool isFinished() {
		if (property == L"position") {
			vec2 endv = std::any_cast<vec2>(end);
			return abs(target->position.x - endv.x) < 0.001 && abs(target->position.y - endv.y) < 0.001;
		}
		else if (property == L"size") {
			vec2 endv = std::any_cast<vec2>(end);
			return abs(target->size.x - endv.x) < 0.001 && abs(target->size.y - endv.y) < 0.001;
		}
		else if (property == L"scale") {
			vec2 endv = std::any_cast<vec2>(end);
			return abs(target->scale.x - endv.x) < 0.001 && abs(target->scale.y - endv.y) < 0.001;
		}
		else if (property == L"rotation") {
			float endv = std::any_cast<float>(end);
			return abs(target->rotation - endv) < 0.001;
		}
		else if (property == L"modulate") {
			Sprite2D* s = ((Sprite2D*)target);
			vec4 endv = std::any_cast<vec4>(end);
			return abs(s->modulate.x - endv.x) < 0.001 && abs(s->modulate.y - endv.y) < 0.001 && abs(s->modulate.z - endv.z) < 0.001 && abs(s->modulate.w - endv.w) < 0.001;
		}
		else if (property == L"color") {
			ColorRect* c = ((ColorRect*)target);
			vec4 endv = std::any_cast<vec4>(end);
			return abs(c->color.x - endv.x) < 0.001 && abs(c->color.y - endv.y) < 0.001 && abs(c->color.z - endv.z) < 0.001 && abs(c->color.w - endv.w) < 0.001;
		}
		else {
			alert(L"there is no property called " + property);
		}
	}
};

class Tween : public Object2D{
public:
	std::vector<TweenInfo> processingList;
public:
	void linear_interpolate(Object2D* target, std::wstring property, std::any begin, std::any end, double duration, bool* finished);
	Tween(Scene* _owner) : Object2D(_owner, L"") { type = L"Tween"; };
	~Tween() {};
	void update();
	void draw();
};


#endif