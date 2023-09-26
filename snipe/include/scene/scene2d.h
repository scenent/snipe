#pragma once
#ifndef SCENE2D_H
#define SCENE2D_H

#include "scene/scene.h"
#include "objects/object.h"

class Scene2D : public Scene {
public:
	bool dialogPaused = false;

	std::vector<Object2D*> objectList;
	std::vector<Audio*> audioList;
	Dialog* currentDialog = nullptr;
	ChoiceBox* currentChoiceBox = nullptr;
	HistoryContainer* currentHistory = nullptr;

	Tween* currentTween = nullptr;
public:
	Scene2D(Root* _root, std::wstring _name);
	~Scene2D();
	void update() override;
	void draw() override;

	Object* getObjectN(std::wstring _name);
	Object* getObject(std::wstring _type, std::wstring _name);
	Sprite2D* getSprite2D(std::wstring _name, std::wstring _tag);
	std::vector<Sprite2D*> getSprite2DList(std::wstring _name);
	std::vector<Sprite2D*> getSprite2DListExcept(std::wstring _name, std::wstring _exceptTag);
	std::vector<TextEdit*> getTextEditList();
	Audio* getAudio(std::wstring _name);

	ColorRect* addColorRect(std::wstring _name);
	Sprite2D* addSprite2D(std::wstring _name);
	Label* addLabel(std::wstring _name);
	Camera2D* addCamera2D(std::wstring _name);
	Button* addButton(std::wstring _name);
	TextureButton* addTextureButton(std::wstring _name);
	TextButton* addTextButton(std::wstring _name);
	Hslider* addHslider(std::wstring _name);
	Vslider* addVslider(std::wstring _name);
	VboxContainer* addVboxContainer(std::wstring _name);
	Dialog* addDialog(std::wstring _name);
	ChoiceBox* addChoiceBox(std::wstring _name);
	TextEdit* addTextEdit(std::wstring _name);
	Audio* addAudio(std::wstring _name);
	HistoryContainer* addHistoryContainer(std::wstring _name);
};


#endif