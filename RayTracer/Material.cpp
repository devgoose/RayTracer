#include "Material.h"
class Color;

Material::Material(const Color& diff, const Color& spec, float ka_, float kd_, float ks_, float exponent_) {
	diffuse = diff;
	specular = spec;
	ka = ka_;
	kd = kd_;
	ks = ks_;
	exponent = exponent_;
}

Material::Material() {
	diffuse = Color();
	specular = Color();
	ka = 0;
	kd = 0;
	ks = 0;
	exponent = 0;
}

Material::Material(const Material& m) {
	diffuse = Color( m.diffuse);
	specular = Color(m.specular);
	ka = m.ka;
	kd = m.kd;
	ks = m.ks;
	exponent = m.exponent;
}

