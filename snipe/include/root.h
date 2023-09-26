#pragma once
#ifndef ROOT_H
#define ROOT_H

#include <thread>
#include <glad/include/glad/glad.h>
#include <GLFW/include/GLFW/glfw3.h>

#include "frame_event.h"
#include "scene/scene.h"
#include "scene/scene2d.h"
#include "scene/scene3d.h"

class Root {
public:
	int         WINDOW_WIDTH_ORIGIN = 1600;
	int         WINDOW_HEIGHT_ORIGIN = 900;
	int         WINDOW_WIDTH  = 1600;
	int         WINDOW_HEIGHT = 900;
	int         WINDOW_XPOS   = 160;
	int         WINDOW_YPOS   = 90;
	std::string WINDOW_TITLE  = multibyte2utf8(L"Hello World");
	std::string WINDOW_ICON_PATH = "";
	bool        WINDOW_FULLSCREEN = false;
	bool        WINDOW_ALLOW_HIDPI = false;
	bool        WINDOW_QUIT_ON_ESC = false;
	float       WINDOW_TARGET_FPS = 60.0f;

	std::vector<Scene*> sceneList{};
	Scene* currentScene = nullptr;

	GLFWwindow* window = nullptr;
	double currentTime = 0.0;
	double lasttime = 0.0;
	int frameCount = 0;

	FrameEvent currentFrameEvent{};
public:
	Root();
	~Root();
	bool init();
	void process_frame();
	void quit();

	void setWindowFullscreen(bool flag);
	void setWindowIcon(std::string filePath);

	Scene2D* addScene2D(std::wstring _name);
	Scene3D* addScene3D(std::wstring _name);

	Scene2D* getScene2D(std::wstring _name);
};


#endif