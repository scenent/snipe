#include "objects/tween.h"
#include "scene/scene.h"
#include "root.h"

void Tween::linear_interpolate(Object2D* target, std::wstring property, std::any begin, std::any end, double duration, bool* finished) {
	TweenInfo i;
	i.target = target;
	i.property = property;
	i.begin = begin;
	i.end = end;
	i.duration = duration;
	i.current = begin;
	if (finished != nullptr) {
		i.finished = finished;
		assert(*finished == false);
	}
	if (property == L"position" || property == L"size" || property == L"scale") {
		vec2 range = (std::any_cast<vec2>(end) - std::any_cast<vec2>(begin));
		if (duration != 1.0) range /= duration;
		i.step = range / owner->root->WINDOW_TARGET_FPS;
		if (property == L"position") target->position = std::any_cast<vec2>(begin);
		if (property == L"size") target->size = std::any_cast<vec2>(begin);
		if (property == L"scale") target->scale = std::any_cast<vec2>(begin);
	}
	else if (property == L"rotation") {
		float range = (std::any_cast<float>(end) - std::any_cast<float>(begin));
		if (duration != 1.0) range /= duration;
		i.step = range / owner->root->WINDOW_TARGET_FPS;
		target->rotation = std::any_cast<float>(begin);
	}
	else if (property == L"modulate") {
		vec4 range = (std::any_cast<vec4>(end) - std::any_cast<vec4>(begin));
		if (duration != 1.0) range /= duration;
		i.step = range / owner->root->WINDOW_TARGET_FPS;
		((Sprite2D*)target)->modulate = std::any_cast<vec4>(begin);
	}
	else if (property == L"color") {
		vec4 range = (std::any_cast<vec4>(end) - std::any_cast<vec4>(begin));
		if (duration != 1.0) range /= duration;
		i.step = range / owner->root->WINDOW_TARGET_FPS;
		((ColorRect*)target)->color = std::any_cast<vec4>(begin);
	}
	else {
		alert(L"snipe dose not supports property called " + property);
	}
	processingList.push_back(i);
}

void Tween::update() {
	std::vector<TweenInfo>::iterator iter = processingList.begin();
	while (iter != processingList.end()) {
		if ((*iter).isFinished() == false) {
			if ((*iter).property == L"position") {
				(*iter).target->position = std::any_cast<vec2>((*iter).current);
				(*iter).current = std::any_cast<vec2>((*iter).current) + std::any_cast<vec2>((*iter).step);
			}
			else if ((*iter).property == L"size") {
				(*iter).target->size = std::any_cast<vec2>((*iter).current);
				(*iter).current = std::any_cast<vec2>((*iter).current) + std::any_cast<vec2>((*iter).step);
			}
			else if ((*iter).property == L"scale") {
				(*iter).target->scale = std::any_cast<vec2>((*iter).current);
				(*iter).current = std::any_cast<vec2>((*iter).current) + std::any_cast<vec2>((*iter).step);
			}
			else if ((*iter).property == L"rotation") {
				(*iter).target->rotation = std::any_cast<float>((*iter).current);
				(*iter).current = std::any_cast<float>((*iter).current) + std::any_cast<float>((*iter).step);
			}
			else if ((*iter).property == L"modulate") {
				((Sprite2D*)(*iter).target)->modulate = std::any_cast<vec4>((*iter).current);
				(*iter).current = std::any_cast<vec4>((*iter).current) + std::any_cast<vec4>((*iter).step);
			}
			else if ((*iter).property == L"color") {
				((ColorRect*)(*iter).target)->color = std::any_cast<vec4>((*iter).current);
				(*iter).current = std::any_cast<vec4>((*iter).current) + std::any_cast<vec4>((*iter).step);
			}
			iter++;
		}
		else {
			if ((*iter).property == L"position") {
				(*iter).target->position = std::any_cast<vec2>((*iter).end);
			}
			else if ((*iter).property == L"size") {
				(*iter).target->size = std::any_cast<vec2>((*iter).end);
			}
			else if ((*iter).property == L"scale") {
				(*iter).target->scale = std::any_cast<vec2>((*iter).end);
			}
			else if ((*iter).property == L"rotation") {
				(*iter).target->rotation = std::any_cast<float>((*iter).end);
			}
			else if ((*iter).property == L"modulate") {
				((Sprite2D*)(*iter).target)->modulate = std::any_cast<vec4>((*iter).end);
			}
			else if ((*iter).property == L"modulate") {
				((ColorRect*)(*iter).target)->color = std::any_cast<vec4>((*iter).end);
			}
			if ((*iter).finished != nullptr) {
				*(*iter).finished = true;
			}
			iter = processingList.erase(iter);
		}
	}
}
void Tween::draw() {

}