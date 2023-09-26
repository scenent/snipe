#pragma once
#ifndef CONTAINERBASE_H
#define CONTAINERBASE_H

#include "objects/object2d.h"
#include "objects/button.h"
#include "objects/slider_base.h"

enum class ContainerAlignMode {
	Begin, Center, End
};

class ContainerBase : public Object2D {
public:
	float scroll_amount = 0.1f;
	double percent = 0.0;
	std::vector<Object2D*> objectList{};
	float separation = 0.0f;
	ContainerAlignMode alignMode = ContainerAlignMode::Begin;

	std::vector<std::function<void(Object*)>> callbackList{};
public:
	ContainerBase(Scene* _owner, std::wstring _name);
	~ContainerBase();
	void update() override;
	void draw() override;
};


#endif