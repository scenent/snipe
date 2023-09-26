#include "objects/object2d.h"

Object2D::Object2D(Scene* _owner, std::wstring _name) : Object(_owner, _name){
	type = L"Object2D";
}