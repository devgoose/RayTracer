#include "DirectionalLight.h"

DirectionalLight::DirectionalLight() {
  direction = Vector3();
}


DirectionalLight::DirectionalLight(const Color& color, const Vector3& direction_) 
  : Light(color) {
  direction = direction_;
}