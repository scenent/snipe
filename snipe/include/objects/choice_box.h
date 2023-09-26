#pragma once
#ifndef CHOICEBOX_H
#define CHOICEBOX_H

#include "objects/object2d.h"
#include "objects/texture_button.h"

enum class ChoiceBoxAlignMode {
	Begin, Center, End
};

enum class ChoiceBoxState {
	Idle, Wait 
};

class ChoiceBox : public Object2D {
private:
	std::vector<std::wstring> tempChoiceInfo{};
	std::vector<TextureButton*> buttonList{};
	ChoiceBoxState state = ChoiceBoxState::Idle;
	int* result = nullptr;
public:
	float separation = 10.0f;
	ChoiceBoxAlignMode alignMode = ChoiceBoxAlignMode::Center;
	float eachBoxSize = 20.0f;
	float fontSize = 16.0f;
	std::wstring fontPath = L"";
	vec4 fontColor = { 0, 0, 0, 1 };
	vec4 outline_color = { 1, 1, 1, 1 };
	float outline_size = 0;
	std::wstring idleFilePath = L"";
	std::wstring hoverFilePath = L"";
	std::wstring downFilePath = L"";
public:
	ChoiceBox(Scene* _owner, std::wstring _name);
	~ChoiceBox();
	void update() override;
	void draw() override;
	void choice(std::vector<std::wstring> _messages , int* _result);
	void reset();
};


#endif