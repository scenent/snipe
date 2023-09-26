#include "scene/scene3d.h"

Scene3D::Scene3D(Root* _root, std::wstring _name) : Scene(_root, _name){
	type = L"Scene2D";
}

void Scene3D::update() {
	state = SceneState::FrameUpdate;
	for (auto& p : objectList) p->update();
	state = SceneState::FrameUpdateEnd;
}

void Scene3D::draw() {
	state = SceneState::FrameDraw;
	for (auto& p : objectList) p->draw();
	state = SceneState::FrameDrawEnd;
}