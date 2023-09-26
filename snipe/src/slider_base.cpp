#include "objects/slider_base.h"
#include "scene/scene.h"
#include "root.h"

SliderBase::SliderBase(Scene* _owner, std::wstring _name) : Object2D(_owner, _name){
	type = L"SliderBase";
}
SliderBase::~SliderBase() {

}
void SliderBase::update() {

}
void SliderBase::draw() {

}