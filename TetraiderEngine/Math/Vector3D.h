/* Start Header -------------------------------------------------------
Copyright (C) 2017 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Vector3D.h
Purpose: Provides a 3 Dimensional Vector (with w coordinate for Homogeneous uses)
Language: C++
Platform: Windows 8.1 / 10.0.15063.0  or  GCC 7.1.0
Project: CS529_holdenprofit_
Author: Holden Profit, holden.profit, 42833
Creation date: 9/29/17
- End Header --------------------------------------------------------*/

#pragma once

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

#ifndef VECTOR3_H
#define VECTOR3_H

#include "Vector2D.h"

class Vector3D
{
private:
	float m_x, m_y, m_z, m_w;

public:
	Vector3D();
	Vector3D(float x, float y, float z);
	Vector3D(float x, float y, float z, float w);
	Vector3D(const Vector3D & rhs);
	Vector3D(const Vector2D & rhs);
	Vector3D& operator= (Vector3D rhs);
	~Vector3D();

	void Set(float x, float y, float z);
	void Set(float x, float y, float z, float w);
	float getX() const { return m_x; };
	void setX(float x) { m_x = x; };
	float getY() const { return m_y; };
	void setY(float y) { m_y = y; };
	float getZ() const { return m_z; };
	void setZ(float z) { m_z = z; };
	float getW() const { return m_w; };
	void setW(float w) { m_w = w; };

	operator float*();
	bool operator== (const  Vector3D& other) const;
	bool operator!= (const  Vector3D& other) const;
	Vector3D operator+ (const Vector3D& other) const;
	Vector3D& operator+= (const Vector3D& other);
	Vector3D operator- (const Vector3D& other) const;
	Vector3D& operator-= (const Vector3D& other);
	Vector3D operator* (const float scalar) const;
	Vector3D operator/ (const float divisor) const;
	float operator[](const int idx) const;
	float& operator[](const int idx);

	void Add(const Vector3D& other);
	void Sub(const Vector3D& other);
	void Mul(const float scalar);
	void Div(const float divisor);
	void Print() const;

	void Zero();
	void Negate();
	float SquareLength() const;
	float Length() const;
	void Normalize();

	static float SquareDistance(const Vector3D& vector0, const Vector3D& vector1);
	static float Distance(const Vector3D& vector0, const Vector3D& vector1);
	static Vector3D Normalize(const Vector3D& vector);
	static float Dot(const Vector3D& vector0, const Vector3D& vector1);
	static Vector3D Cross(const Vector3D& vector0, const Vector3D& vector1);
	static Vector3D AngleRadians(float radians, Vector3D& axis);
	static Vector3D AngleDegrees(float degrees, Vector3D& axis);
};

Vector3D operator*(const float scalar, const Vector3D& other);

void Vector3DTests();

#endif