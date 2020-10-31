#include "Vector3.h"
#include <math.h>

Vector3::Vector3(const Vector3& v) {
  x = v.x;
  y = v.y;
  z = v.z;
}

Vector3 Vector3::operator+(const Vector3& v) {
  Vector3 vec;
  vec.x = this->x + v.x;
  vec.y = this->y + v.y;
  vec.z = this->z + v.z;
  return vec;
}

Vector3 Vector3::operator-(const Vector3& v) {
  Vector3 vec;
  vec.x = this->x - v.x;
  vec.y = this->y - v.y;
  vec.z = this->z - v.z;
  return vec;
}

Vector3 Vector3::operator*(const Vector3& v) {
  Vector3 vec;
  vec.x = this->x * v.x;
  vec.y = this->y * v.y;
  vec.z = this->z * v.z;
  return vec;
}

Vector3 Vector3::operator/(const Vector3& v) {
  Vector3 vec;
  vec.x = this->x / v.x;
  vec.y = this->y / v.y;
  vec.z = this->z / v.z;
  return vec;
}

Vector3 Vector3::operator*(float c) {
  Vector3 vec;
  vec.x = this->x * c;
  vec.y = this->y * c;
  vec.z = this->z * c;
  return vec;
}

Vector3 Vector3::operator/(float c) {
  Vector3 vec;
  vec.x = this->x / c;
  vec.y = this->y / c;
  vec.z = this->z / c;
  return vec;
}


Vector3 Vector3::cross(const Vector3& v) {
  return Vector3((y * v.z) - (z * v.y),
    (z * v.x) - (x * v.z),
    (x * v.y) - (y * v.x));
}

float Vector3::dot(const Vector3& v) const {
  return (x * v.x) + (y * v.y) + (z * v.z);
}

float Vector3::length() const {
  return (float)sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
}


Vector3 Vector3::toUnit() const {
  Vector3 vec;
  float len = this->length();
  vec.x = x / len;
  vec.y = y / len;
  vec.z = z / len;
  return vec;
}

Vector3 Vector3::reverse() const {
  return Vector3(-x, -y, -z);
}