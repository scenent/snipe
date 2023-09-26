#include "objects/object3d.h"


Object3D::Object3D(Scene* _owner, std::wstring _name) : Object(_owner, _name) {
	type = L"Object3D";
}