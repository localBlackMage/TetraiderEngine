/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Vector2D.h
Purpose: Provides a 3 Dimensional Vector (with w coordinate for Homogeneous uses)
Language: C++
Project: GAM541
Author: Holden Profit
Creation date: 1/17/18
- End Header --------------------------------------------------------*/

#pragma once

#ifndef VECTOR3_H
#define VECTOR3_H

class Vector3D
{
public:
	float x, y, z, w;
	
	Vector3D();
	Vector3D(const aiVector3t<ai_real>& v);
	Vector3D(const aiVector3t<ai_real>& v, const float& _w);
	Vector3D(const Vector3D& vec, const float& _w);
	Vector3D(const float& _x, const float& _y, const float& _z);
	Vector3D(const float& _x, const float& _y, const float& _z, const float& _w);
	Vector3D(const Vector3D & rhs);
	Vector3D(const Vector2D & rhs);
	Vector3D& operator= (const Vector3D& rhs);
	~Vector3D();

	void Set(float _x, float _y, float _z);
	void Set(float _x, float _y, float _z, float _w);

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

	float AngleRadians() const;
	float AngleDegrees() const;

	void Zero();
	void Negate();
	float SquareLength() const;
	float Length() const;
	void Normalize();
	bool IsVectorZero(float error = EPSILON) const;

	static float SquareDistance(const Vector3D& vector0, const Vector3D& vector1);
	static float Distance(const Vector3D& vector0, const Vector3D& vector1);
	static Vector3D Normalize(const Vector3D& vector);
	static float Dot(const Vector3D& vector0, const Vector3D& vector1);
	static Vector3D Cross(const Vector3D& vector0, const Vector3D& vector1);
	static Vector3D VectorFromAngleDegrees(float degrees);
	static Vector3D AngleRadians(float radians, Vector3D& axis);
	static Vector3D AngleDegrees(float degrees, Vector3D& axis);
};

Vector3D operator*(const float scalar, const Vector3D& other);
std::ostream& operator<<(std::ostream& out, const Vector3D& vec);

static const Vector3D XAXIS = Vector3D(1, 0, 0, 0);
static const Vector3D YAXIS = Vector3D(0, 1, 0, 0);
static const Vector3D ZAXIS = Vector3D(0, 0, 1, 0);

void Vector3DTests();

#endif