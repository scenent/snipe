#pragma once
#ifndef DIALOG_H
#define DIALOG_H

#include <queue>

#include "objects/object2d.h"
#include "objects/label.h"
#include "objects/sprite2d.h"

enum class DialogState {
	Idle, Reading, Finished
};

class Dialog : public Object2D {
public:
	bool progressed = false;
	bool can_skip = true;
	std::queue<std::pair<std::wstring, std::wstring>> sayQueue{};
	Label* textLabel = nullptr;
	Label* nameLabel = nullptr;
	Sprite2D* background = nullptr;
	Sprite2D* namebox = nullptr;
	std::wstring fontPath = L"";
	int fontSize = 32;
	std::wstring nameFontPath = L"";
	int nameFontSize = 24;
	DialogState state = DialogState::Idle;
	double speed = 1;

	vec2 nameLabelPos;
public:
	Dialog(Scene* _owner, std::wstring _name);
	~Dialog();
	void update() override;
	void draw() override;
	void say(std::wstring _name, std::wstring _content, bool _can_skip = true);
	void reset();
};

#endif