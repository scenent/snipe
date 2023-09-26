#include "objects/container_base.h"

ContainerBase::ContainerBase(Scene* _owner, std::wstring _name) : Object2D(_owner, _name){
	type = L"ContainerBase";
}
ContainerBase::~ContainerBase() {

}
void ContainerBase::update() {

}
void ContainerBase::draw() {

}