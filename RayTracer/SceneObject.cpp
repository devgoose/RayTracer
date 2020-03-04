#include "SceneObject.h"

SceneObject::SceneObject(const SceneObject& obj) {
	pos = obj.pos;
	mtlcolor = obj.mtlcolor;
}