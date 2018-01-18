/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Vector2D.h
Purpose: Provides a 2 Dimensional Vector (with w coordinate for Homogeneous uses)
Language: C++
Project: GAM541
Author: Holden Profit
Creation date: 1/12/18
- End Header --------------------------------------------------------*/

//#pragma once

#ifndef VECTOR2_H
#define VECTOR2_H

class Vector2D 
{
public:
	float x, y, w;

	Vector2D();
	Vector2D(float _x, float _y);
	Vector2D(float _x, float _y, float _w);
	Vector2D(const Vector2D & rhs);
	Vector2D& operator= (Vector2D rhs);
	~Vector2D();

	void Set(float _x, float _y);
	void Set(float _x, float _y, float _w);

	operator float*();
	bool operator== (const Vector2D& other) const;
	bool operator!= (const Vector2D& other) const;
	Vector2D operator+ (const Vector2D& other) const;
	Vector2D& operator+= (const Vector2D& other);
	Vector2D operator- (const Vector2D& other) const;
	Vector2D& operator-= (const Vector2D& other);
	Vector2D operator* (const float scalar) const;
	Vector2D operator/ (const float divisor) const;

	void Add(const Vector2D& other);
	void Sub(const Vector2D& other);
	void Mul(const float scalar);
	void Div(const float divisor);
	void Print();

	void Zero();
	void Negate();
	float SquareLength();
	float Length();
	void Normalize();
	float GetRadians();
	float GetDegrees();

	static float SquareDistance(const Vector2D& vector0, const Vector2D& vector1);
	static float Distance(const Vector2D& vector0, const Vector2D& vector1);
	static Vector2D Normalize(Vector2D& vector);
	static float Dot(const Vector2D& vector0, const Vector2D& vector1);
	static Vector2D AngleRadians(float radians);
	static Vector2D AngleDegrees(float degrees);
};

void Vector2DTests();

#endif