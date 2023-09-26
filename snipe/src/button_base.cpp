#include "objects/button_base.h"

ButtonBase::ButtonBase(Scene* _owner, std::wstring _name) : Object2D(_owner, _name){
	type = L"ButtonBase";
	size = { 100, 40 };
}
ButtonBase::~ButtonBase() {

}
void ButtonBase::update() {

}
void ButtonBase::draw() {

}