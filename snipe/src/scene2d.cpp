#include "scene/scene2d.h"
#include "objects/texture_button.h"
#include "audio/audio.h"


Scene2D::Scene2D(Root* _root, std::wstring _name) : Scene(_root, _name){
	type = L"Scene2D";
	currentTween = new Tween(this);
}

Scene2D::~Scene2D() {
	for (auto& p : objectList) {
		if (p->type == L"Audio") {
			ma_device_uninit(&((Audio*)p)->device);
			ma_decoder_uninit(&((Audio*)p)->decoder);
		}
	}
	for (auto& p : objectList) {
		delete p;
		p = nullptr;
	}
	objectList.clear();
	delete currentTween;
}

void Scene2D::update() {
	state = SceneState::FrameUpdate;
	currentTween->update();
	std::vector<Object2D*> zorderSorted = objectList;
	stable_sort(zorderSorted.begin(), zorderSorted.end(), [](const Object2D* left, const Object2D* right) { return left->z_order < right->z_order; });
	for (auto& p : zorderSorted) {
		if (p->type != L"Dialog")
			p->update();
	}
	for (auto& p : zorderSorted) {
		if (p->type == L"Dialog")
			p->update();
	}
	state = SceneState::FrameUpdateEnd;
}

void Scene2D::draw() {
	state = SceneState::FrameDraw;
	std::vector<Object2D*> zorderSorted = objectList;
	stable_sort(zorderSorted.begin(), zorderSorted.end(), [](const Object2D* left, const Object2D* right) { return left->z_order < right->z_order; });
	for (auto& p : zorderSorted) p->draw();
	state = SceneState::FrameDrawEnd;
}
Object* Scene2D::getObjectN(std::wstring _name) {
	for (auto& p : objectList) {
		if (p->name == _name) {
			return p;
		}
	}
	return nullptr;
}

Object* Scene2D::getObject(std::wstring _type, std::wstring _name) {
	for (auto& p : objectList) {
		if (p->type == _type && p->name == _name) {
			return p;
		}
	}
	return nullptr;
}

Sprite2D* Scene2D::getSprite2D(std::wstring _name, std::wstring _tag) {
	for (auto& p : objectList) {
		if (p->type == L"Sprite2D") {
			if (((Sprite2D*)p)->name == _name && ((Sprite2D*)p)->tag == _tag) {
				return ((Sprite2D*)p);
			}
		}
	}
	return nullptr;
}
std::vector<Sprite2D*> Scene2D::getSprite2DList(std::wstring _name) {
	std::vector<Sprite2D*> result{};
	for (auto& p : objectList) {
		if (p->type == L"Sprite2D" && p->name == _name) {
			result.push_back((Sprite2D*)p);
		}
	}
	return result;
}
std::vector<Sprite2D*> Scene2D::getSprite2DListExcept(std::wstring _name, std::wstring _exceptTag) {
	std::vector<Sprite2D*> result{};
	for (auto& p : objectList) {
		if (p->type == L"Sprite2D" && p->name == _name && ((Sprite2D*)p)->tag != _exceptTag) {
			result.push_back((Sprite2D*)p);
		}
	}
	return result;
}
std::vector<TextEdit*> Scene2D::getTextEditList() {
	std::vector<TextEdit*> result;
	for (auto& p : objectList) {
		if (p->type == L"TextEdit") {
			result.push_back((TextEdit*)p);
		}
	}
	return result;
}

Audio* Scene2D::getAudio(std::wstring _name) {
	for (auto& p : audioList) {
		if (p->name == _name) {
			return p;
		}
	}
	alert(L"there is no audio called " + _name);
	return nullptr;
}

ColorRect* Scene2D::addColorRect(std::wstring _name) {
	ColorRect* c = new ColorRect(this, _name);
	objectList.push_back(c);
	return c;
}

Sprite2D* Scene2D::addSprite2D(std::wstring _name) {
	Sprite2D* t = new Sprite2D(this, _name);
	objectList.push_back(t);
	return t;
}

Label* Scene2D::addLabel(std::wstring _name) {
	Label* l = new Label(this, _name);
	objectList.push_back(l);
	return l;
}

Camera2D* Scene2D::addCamera2D(std::wstring _name) {
	Camera2D* c = new Camera2D(this, _name);
	objectList.push_back(c);
	return c;
}

Button* Scene2D::addButton(std::wstring _name) {
	Button* b = new Button(this, _name);
	objectList.push_back(b);
	return b;
}

TextureButton* Scene2D::addTextureButton(std::wstring _name) {
	TextureButton* b = new TextureButton(this, _name);
	objectList.push_back(b);
	return b;
}

TextButton* Scene2D::addTextButton(std::wstring _name) {
	TextButton* b = new TextButton(this, _name);
	objectList.push_back(b);
	return b;
}

Hslider* Scene2D::addHslider(std::wstring _name) {
	Hslider* h = new Hslider(this, _name);
	objectList.push_back(h);
	return h;
}

Vslider* Scene2D::addVslider(std::wstring _name) {
	Vslider* v = new Vslider(this, _name);
	objectList.push_back(v);
	return v;
}

VboxContainer* Scene2D::addVboxContainer(std::wstring _name) {
	VboxContainer* v = new VboxContainer(this, _name);
	objectList.push_back(v);
	return v;
}

Dialog* Scene2D::addDialog(std::wstring _name) {
	Dialog* d = new Dialog(this, _name);
	objectList.push_back(d);
	return d;
}

ChoiceBox* Scene2D::addChoiceBox(std::wstring _name) {
	ChoiceBox* c = new ChoiceBox(this, _name);
	objectList.push_back(c);
	return c;
}

TextEdit* Scene2D::addTextEdit(std::wstring _name) {
	TextEdit* t = new TextEdit(this, _name);
	objectList.push_back(t);
	return t;
}

Audio* Scene2D::addAudio(std::wstring _name) {
	Audio* a = new Audio(_name);
	audioList.push_back(a);
	return a;
}

HistoryContainer* Scene2D::addHistoryContainer(std::wstring _name) {
	HistoryContainer* h = new HistoryContainer(this, _name);
	objectList.push_back(h);
	return h;
}