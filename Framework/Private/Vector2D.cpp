#include "stdafx.h"
#include "Vector2D.h"

Vector2D::Vector2D()
	: x(0), y(0)
{
}

Vector2D::Vector2D(float _x, float _y)
	: x(_x), y(_y)
{
}

Vector2D::~Vector2D()
{
}

Vector2D Vector2D::operator+(const Vector2D& _Vector2D)
{
	float tempX = x + _Vector2D.x;
	float tempY = y + _Vector2D.y;

	return Vector2D(tempX, tempY);
}

Vector2D Vector2D::operator+(const float& _Float)
{
	float tempX = x + _Float;
	float tempY = y + _Float;

	return Vector2D(tempX, tempY);
}

void Vector2D::operator+=(const Vector2D& _Vector2D)
{
	x += _Vector2D.x;
	y += _Vector2D.y;
}

void Vector2D::operator+=(const float& _Float)
{
	x += _Float;
	y += _Float;
}

Vector2D Vector2D::operator-(const Vector2D& _Vector2D)
{
	float tempX = x - _Vector2D.x;
	float tempY = y - _Vector2D.y;

	return Vector2D(tempX, tempY);

}

Vector2D Vector2D::operator-(const float& _Float)
{
	float tempX = x - _Float;
	float tempY = y - _Float;

	return Vector2D(tempX, tempY);
}

void Vector2D::operator-=(const Vector2D& _Vector2D)
{
	x -= _Vector2D.x;
	y -= _Vector2D.y;
}

void Vector2D::operator-=(const float& _Float)
{
	x -= _Float;
	y -= _Float;
}

Vector2D Vector2D::operator*(const Vector2D& _Vector2D)
{
	float tempX = x * _Vector2D.x;
	float tempY = y * _Vector2D.y;

	return Vector2D(tempX, tempY);

}

Vector2D Vector2D::operator*(const float& _Float)
{
	float tempX = x * _Float;
	float tempY = y * _Float;

	return Vector2D(tempX, tempY);
}

void Vector2D::operator*=(const Vector2D& _Vector2D)
{
	x *= _Vector2D.x;
	y *= _Vector2D.y;
}

void Vector2D::operator*=(const float& _Float)
{
	x *= _Float;
	y *= _Float;
}

Vector2D Vector2D::Normalize()
{
	float tempDiagnal = sqrtf((x * x) + (y * y));
	float tempX = x == 0 ? 0 : x / tempDiagnal;
	float tempY = y == 0 ? 0 : y / tempDiagnal;

	return Vector2D(tempX, tempY);
}

float Vector2D::ToAngle_Rad()
{
	if(this->y < 0) 
		return acosf(this->x / this->Magnitude());

	return 2 * PI - acosf(this->x / this->Magnitude());
}

float Vector2D::Magnitude()
{
	return sqrtf((x * x) + (y * y));
}

float Vector2D::Distance(const Vector2D& _Vector2D)
{
	Vector2D tempVector = *this - _Vector2D;

	return tempVector.Magnitude();
}

float Vector2D::Angle_Rad(const Vector2D& _Vector2D)
{
	Vector2D tempVector = *this - _Vector2D;

	return atan2f(tempVector.y, tempVector.x);
}

float Vector2D::Angle_Deg(const Vector2D& _Vector2D)
{
	return float((this->Angle_Rad(_Vector2D)) * RAD2DEG);
}