#pragma once

/*
Main class for holding a vector in 3d space.
*/
class Vector3
{
private:
	float x;
	float y;
	float z;

public:
	Vector3() {
		x = 0;
		y = 0;
		z = 0;
	}

	Vector3(float x_, float y_, float z_) {
		x = x_;
		y = y_;
		z = z_;
	}

	Vector3(const Vector3& v);

	float getX() const { return x; } 
	float getY() const { return y; }
	float getZ() const { return z; }

	void setX(float x_) { x = x_; }
	void setY(float y_) { y = y_; }
	void setZ(float z_) { z = z_; }

	Vector3 operator+(const Vector3& v);
	Vector3 operator-(const Vector3& v);
	Vector3 operator*(const Vector3& v);
	Vector3 operator/(const Vector3& v);

	Vector3 operator*(float c);
	Vector3 operator/(float c);


	Vector3 cross(const Vector3& v);
	float dot(const Vector3& v);
	
	float length() const;

	Vector3 toUnit() const;


};

