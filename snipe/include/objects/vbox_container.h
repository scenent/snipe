#pragma once
#ifndef VBOXCONTAINER_H
#define VBOXCONTAINER_H

#include "objects/container_base.h"
#include "objects/vslider.h"

class VboxContainer : public ContainerBase {
public:
	Vslider* slider = nullptr;
public:
	VboxContainer(Scene* _owner, std::wstring _name);
	~VboxContainer();
	void update() override;
	void draw() override;
	void set_position(vec2 _position);
};


#endif