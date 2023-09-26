#pragma once
#ifndef SCENE3D_H
#define SCENE3D_H

#include <vector>

#include "scene/scene.h"
#include "objects/object3d.h"

class Scene3D : public Scene {
public:
	std::vector<Object3D*> objectList;
	std::vector<Object2D*> interfaceList;
public:
	Scene3D(Root* _root, std::wstring _name);
	void update() override;
	void draw() override;
};


#endif