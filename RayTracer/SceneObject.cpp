#include "SceneObject.h"


SceneObject::SceneObject() {
	mtl_index = -1;
	tex_index = -1;
}

SceneObject::SceneObject(const SceneObject& obj) {
	mtl_index = obj.mtl_index;
	tex_index = obj.tex_index;
}

void SceneObject::setTextureIndex(const int tex_index_) {
	tex_index = tex_index_;
}

void SceneObject::setMaterialIndex(const int mtl_index_) {
	mtl_index = mtl_index_;
}

bool SceneObject::isTextured() const {
	return (tex_index >= 0);
}