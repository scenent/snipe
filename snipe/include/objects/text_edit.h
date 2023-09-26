#pragma once
#ifndef TEXTEDIT_H
#define TEXTEDIT_H

#include "objects/object2d.h"
#include "objects/label.h"

class TextEdit : public Object2D {
public:
	Label* textLabel = nullptr;
	std::wstring fontPath = L"";
	int fontSize = 16;

	int index = 0;
	std::wstring text = L"";
	unsigned int limit = 10;
	bool fast_backspace = false;
	std::wstring* inputResult = nullptr;
public:
	TextEdit(Scene* _owner, std::wstring _name);
	~TextEdit();
	void input(std::wstring* result, unsigned int limit);
	void setCharCallback(unsigned int codepoint);
	void update() override;
	void draw() override;
	void reset();
};

#endif