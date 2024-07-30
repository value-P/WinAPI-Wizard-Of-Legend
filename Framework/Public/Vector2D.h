#pragma once
#include "Define.h"
class Vector2D
{
public:
	Vector2D();
	Vector2D(float _x, float _y);
	~Vector2D();

public:
	float		x, y;

	Vector2D	operator+(const Vector2D& _Vector2D);
	Vector2D	operator+(const float& _Float);
	void		operator+=(const Vector2D& _Vector2D);
	void		operator+=(const float& _Float);

	Vector2D	operator-(const Vector2D& _Vector2D);
	Vector2D	operator-(const float& _Float);
	void		operator-=(const Vector2D& _Vector2D);
	void		operator-=(const float& _Float);

	Vector2D	operator*(const Vector2D& _Vector2D);
	Vector2D	operator*(const float& _Float);
	void		operator*=(const Vector2D& _Vector2D);
	void		operator*=(const float& _Float);

	Vector2D	Normalize();							// 정규화
	float		ToAngle_Rad();							// 방향벡터 끼임각화(라디안)
	float		Magnitude();							// 길이
	float		Distance(const Vector2D& _Vector2D);	// 특정 벡터와의 거리
	float		Angle_Rad(const Vector2D& _Vector2D);	// 특정 벡터와의 각도(라디안)
	float		Angle_Deg(const Vector2D& _Vector2D);	// 특정 벡터와의 각도(디그리)	

	static Vector2D Zero() { return Vector2D(0.f, 0.f); }
};