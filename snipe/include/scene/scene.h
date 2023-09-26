#pragma once
#ifndef SCENE_H
#define SCENE_H

#include "utils.h"
#include "math_utils.h"
#include "objects/object.h"
#include "objects/color_rect.h"
#include "objects/sprite2d.h"
#include "objects/label.h"
#include "objects/camera.h"
#include "objects/camera2d.h"
#include "objects/button.h"
#include "objects/texture_button.h"
#include "objects/hslider.h"
#include "objects/vslider.h"
#include "objects/vbox_container.h"
#include "objects/dialog.h"
#include "objects/choice_box.h"
#include "objects/tween.h"
#include "objects/text_edit.h"
#include "objects/text_button.h"
#include "audio/audio.h"
#include "objects/history_container.h"

enum class SceneState {
	FrameIdle,
	FrameUpdate,
	FrameUpdateEnd,
	FrameDraw,
	FrameDrawEnd
};

class Root;

class Scene {
public:
	std::wstring type = L"Scene";
	Root* root;
	std::wstring name;

	Object2D* currentFocused = nullptr;

	Camera* currentCamera = nullptr;

	SceneState state = SceneState::FrameIdle;
	bool is2d = true;
public:
	Scene(Root* _root, std::wstring _name);
	virtual void update() = 0;
	virtual void draw() = 0;

	void makeCameraCurrent(Camera* _cam);
};


#endif