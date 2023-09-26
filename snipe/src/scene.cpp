#include "scene/scene.h"


Scene::Scene(Root* _root, std::wstring _name) : root(_root), name(_name) {
	
}

void Scene::update() {
	state = SceneState::FrameUpdate;
	state = SceneState::FrameUpdateEnd;
}

void Scene::draw() {
	state = SceneState::FrameDraw;
	state = SceneState::FrameDrawEnd;
}

void Scene::makeCameraCurrent(Camera* _cam) {
	_cam->current = true;
	currentCamera = _cam;
}
