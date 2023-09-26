#pragma once
#ifndef HISTORYCONTAINER_H
#define HISTORYCONTAINER_H

#include <queue>

#include "objects/container_base.h"
#include "objects/vslider.h"
#include "objects/label.h"

class HistoryContainer : public ContainerBase {
public:
	std::queue<std::pair<std::wstring, std::wstring>> tempHistoryQueue{};
	std::vector<std::pair<Object2D*, Object2D*>> historyObjectList{};
	Vslider* slider = nullptr;
	std::wstring fontPath = L"";
	int fontSize = 16;
	float space = 10.0f;
public:
	HistoryContainer(Scene* _owner, std::wstring _name);
	~HistoryContainer();
	void update() override;
	void draw() override;
	void set_position(vec2 _position);
	void add_history(std::wstring _name, std::wstring _content);
	void reset_history();
};


#endif