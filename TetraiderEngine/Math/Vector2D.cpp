#include "Vector2D.h"
#include "MathDefs.h"
//#include <stdio.h>
//#include <stdlib.h>
//#include <math.h>
//#include <algorithm>
#include <iostream>

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

Vector2D::Vector2D() : x(0.f), y(0.f), w(1.f) { }

Vector2D::Vector2D(float _x, float _y) : x(_x), y(_y), w(1.f) { }

Vector2D::Vector2D(float _x, float _y, float _w) : x(_x), y(_y), w(_w) { }

Vector2D::Vector2D(const Vector2D & rhs) : x(rhs.x), y(rhs.y), w(rhs.w) { }

Vector2D& Vector2D::operator=(Vector2D rhs)
{
	x = rhs.x;
	y = rhs.y;
	w = rhs.w;
	return *this;
}

Vector2D::~Vector2D() { }

void Vector2D::Set(float _x, float _y)
{
	x = _x;
	y = _y;
}

void Vector2D::Set(float _x, float _y, float _w)
{
	x = _x;
	y = _y;
	w = _w;
}

#pragma region Operator Overrides
Vector2D::operator float*()
{
	return &(x);
}

bool Vector2D::operator==(const Vector2D& other) const
{
	return fabs(x - other.x) + fabs(y - other.y) + fabs(w - other.w) < EPSILON;
}

bool Vector2D::operator!=(const Vector2D& other) const
{
	return fabs(x - other.x) + fabs(y - other.y) + fabs(w - other.w) >= EPSILON;
}

Vector2D Vector2D::operator+(const Vector2D& other) const
{
	return Vector2D(
		x + other.x,
		y + other.y
	);
}

Vector2D& Vector2D::operator+=(const Vector2D & other)
{
	x += other.x;
	y += other.y;
	return *this;
}

Vector2D Vector2D::operator-(const Vector2D& other) const
{
	return Vector2D(
		x - other.x,
		y - other.y
	);
}

Vector2D& Vector2D::operator-=(const Vector2D & other)
{
	x -= other.x;
	y -= other.y;
	return *this;
}

Vector2D Vector2D::operator*(const float scalar) const
{
	return Vector2D(
		x * scalar,
		y * scalar
	);
}

Vector2D Vector2D::operator/(const float divisor) const
{
	return Vector2D(
		x / divisor,
		y / divisor
	);
}
#pragma endregion

void Vector2D::Add(const Vector2D& other)
{
	x += other.x;
	y += other.y;
}

void Vector2D::Sub(const Vector2D& other)
{
	x -= other.x;
	y -= other.y;
}

void Vector2D::Mul(const float scalar)
{
	x *= scalar;
	y *= scalar;
}

void Vector2D::Div(const float divisor)
{
	x /= divisor;
	y /= divisor;
}

void Vector2D::Print()
{
	std::cout << "<X: " << x << ", Y: " << y << ", W: " << w << ">" << std::endl;
}

void Vector2D::Zero()
{
	x = y = 0.0f;
}

void Vector2D::Negate()
{
	x = -x;
	y = -y;
}

float Vector2D::SquareLength()
{
	return x * x + y * y;
}

float Vector2D::Length()
{
	return sqrtf(SquareLength());
}

float Vector2D::SquareDistance(const Vector2D& vector0, const Vector2D& vector1)
{
	float x = vector1.x - vector0.x;
	float y = vector1.y - vector0.y;
	return x * x + y * y;
}

float Vector2D::Distance(const Vector2D& vector0, const Vector2D& vector1)
{
	return sqrtf(SquareDistance(vector0, vector1));
}

void Vector2D::Normalize()
{
	Div(Length());
}

float Vector2D::GetRadians()
{
	return atan2f(y, x);
}

float Vector2D::GetDegrees()
{
	return GetRadians() * DEG_TO_RAD;
}

#pragma region Static Methods

Vector2D Vector2D::Normalize(Vector2D& vector)
{
	float len = vector.Length();
	return vector / len;
}

float Vector2D::Dot(const Vector2D& vector0, const Vector2D& vector1)
{
	return vector0.x * vector1.x + vector0.y * vector1.y;
}

Vector2D Vector2D::AngleRadians(float radians)
{
	return Vector2D(cosf(radians), sinf(radians));
}

Vector2D Vector2D::AngleDegrees(float degrees)
{
	return AngleRadians(degrees * DEG_TO_RAD);
}

#pragma endregion

#if TEST_MODE
#define PASS "PASS"
#define FAIL "!!!!! FAIL !!!!!"

void Vector2DTests()
{
	printf("\n========== Running Vector2D tests ==========\n\n");

	Vector2D * createdVector = new Vector2D();
	printf("Created Vector(): %s\n", (createdVector->x == 0.0f && createdVector->y == 0.0f && createdVector->w == 1.0f) ? PASS : FAIL);
	delete createdVector;

	createdVector = new Vector2D(1.0f, 2.0f);
	printf("Created Vector(_x, _y): %s\n", (createdVector->x == 1.0f && createdVector->y == 2.0f && createdVector->w == 1.0f) ? PASS : FAIL);
	delete createdVector;

	createdVector = new Vector2D(2.0f, 3.0f, 0.0f);
	printf("Created Vector(_x, _y, _w): %s\n", (createdVector->x == 2.0f && createdVector->y == 3.0f && createdVector->w == 0.0f) ? PASS : FAIL);
	delete createdVector;

	Vector2D v1, v2, v3, result;
	float scalar;
	double angle;

	v1.Set(1.0f, 2.0f);
	v2.Set(1.0f, 2.0f);
	printf("Vector2D == equal: %s\n", (v1 == v2) ? PASS : FAIL);

	v2.Set(1.1f, 2.0f);
	printf("Vector2D == not equal: %s\n", (v1 == v2) ? FAIL : PASS);

	v2.Set(2.0f, 3.0f);
	printf("Vector2D != not equal: %s\n", (v1 != v2) ? PASS : FAIL);

	v2.Set(1.0f, 2.0f);
	printf("Vector2D != equal: %s\n", (v1 != v2) ? FAIL : PASS);

	v1.Set(0.1f, 2.0f);
	v2.Set(1.0f, 3.5f);
	result.Set(1.1f, 5.5f);
	printf("Vector2D +: %s\n", (v1 + v2 == result) ? PASS : FAIL);

	v1.Set(-0.1f, 1.0f);
	v2.Set(1.0f, -2.0f);
	result.Set(0.9f, -1.0f);
	printf("Vector2D +: %s\n", (v1 + v2 == result) ? PASS : FAIL);

	v1.Set(1.0f, 2.0f);
	v2.Set(1.0f, -2.0f);
	result.Set(0.0f, 4.0f);
	printf("Vector2D -: %s\n", (v1 - v2 == result) ? PASS : FAIL);

	v1.Set(-2.0f, 2.5f);
	v2.Set(-2.0f, 2.0f);
	result.Set(0.0f, 0.5f);
	printf("Vector2D -: %s\n", (v1 - v2 == result) ? PASS : FAIL);

	scalar = 2.0f;
	result.Set(-4.0f, 5.0f);
	printf("Vector2D *: %s\n", (v1 * scalar == result) ? PASS : FAIL);

	scalar = 0.5f;
	result.Set(-1.0f, 1.25f);
	printf("Vector2D *: %s\n", (v1 * scalar == result) ? PASS : FAIL);

	scalar = 2.0f;
	result.Set(-1.0f, 1.25f);
	printf("Vector2D /: %s\n", (v1 / scalar == result) ? PASS : FAIL);

	v1.Set(-2.0f, 10.0f);
	scalar = 0.5f;
	result.Set(-4.0f, 20.0f);
	printf("Vector2D /: %s\n", (v1 / scalar == result) ? PASS : FAIL);

	v1.Set(0.1f, 2.0f);
	v2.Set(1.0f, 3.5f);
	result.Set(1.1f, 5.5f);
	v1.Add(v2);
	printf("Vector2D Add: %s\n", (v1 == result) ? PASS : FAIL);

	v1.Set(-0.1f, 1.0f);
	v2.Set(1.0f, -2.0f);
	result.Set(0.9f, -1.0f);
	v1.Add(v2);
	printf("Vector2D Add: %s\n", (v1 == result) ? PASS : FAIL);

	v1.Set(1.0f, 2.0f);
	v2.Set(1.0f, -2.0f);
	result.Set(0.0f, 4.0f);
	v1.Sub(v2);
	printf("Vector2D Sub: %s\n", (v1 == result) ? PASS : FAIL);

	v1.Set(-2.0f, 2.5f);
	v2.Set(-2.0f, 2.0f);
	result.Set(0.0f, 0.5f);
	v1.Sub(v2);
	printf("Vector2D Sub: %s\n", (v1 == result) ? PASS : FAIL);

	v1.Set(-2.0f, 2.5f);
	scalar = 2.0f;
	result.Set(-4.0f, 5.0f);
	v1.Mul(scalar);
	printf("Vector2D Mul: %s\n", (v1 == result) ? PASS : FAIL);

	v1.Set(-2.0f, 2.5f);
	scalar = 0.5f;
	result.Set(-1.0f, 1.25f);
	v1.Mul(scalar);
	printf("Vector2D Mul: %s\n", (v1 == result) ? PASS : FAIL);

	v1.Set(-2.0f, 10.0f);
	scalar = 2.0f;
	result.Set(-1.0f, 5.0f);
	v1.Div(scalar);
	printf("Vector2D Div: %s\n", (v1 == result) ? PASS : FAIL);

	v1.Set(-2.0f, 10.0f);
	scalar = 0.5f;
	result.Set(-4.0f, 20.0f);
	v1.Div(scalar);
	printf("Vector2D Div: %s\n", (v1 == result) ? PASS : FAIL);

	v1.Set(1.0f, 1.0f);
	result.Set(0.0f, 0.0f);
	v1.Zero();
	printf("Vector2D Zero: %s\n", (v1 == result) ? PASS : FAIL);

	v1.Set(1.0f, 0.0f, 1.0f);
	result.Set(1.0f, 2.0f, 1.0f);
	v1.Set(1.0f, 2.0f);
	printf("Vector2D Set(_x, _y): %s\n", (v1 == result) ? PASS : FAIL);

	v1.Set(0.0f, 0.0f, 0.0f);
	result.Set(1.0f, 2.0f, 1.0f);
	v1.Set(1.0f, 2.0f, 1.0f);
	printf("Vector2D Set(_x, _y, _w): %s\n", (v1 == result) ? PASS : FAIL);

	v1.Set(1.0f, -1.0f, 1.0f);
	result.Set(-1.0f, 1.0f, 1.0f);
	v1.Negate();
	printf("Vector2D Negate: %s\n", (v1 == result) ? PASS : FAIL);

	v1.Set(-1.0f, 0.0f, 1.0f);
	result.Set(1.0f, 0.0f, 1.0f);
	v1.Negate();
	printf("Vector2D Negate: %s\n", (v1 == result) ? PASS : FAIL);

	v1.Set(-3.0f, 4.0f);
	printf("Vector2D SquareLength: %s\n", (v1.SquareLength() == 25.0) ? PASS : FAIL);

	v1.Set(-4.0f, 3.0f);
	printf("Vector2D Length: %s\n", (v1.Length() == 5.0) ? PASS : FAIL);

	v1.Set(-3.0f, 4.0f);
	v2.Set(3.0f, -4.0f);
	printf("Vector2D SquareDistance: %s\n", (Vector2D::SquareDistance(v1, v2) == 100.0f) ? PASS : FAIL);

	printf("Vector2D Distance: %s\n", ((Vector2D::Distance(v1, v2) - 10.0f) < EPSILON) ? PASS : FAIL);

	result.Set(-0.6f, 0.8f);
	v1.Normalize();
	printf("Vector2D Normalize: %s\n", (v1 == result) ? PASS : FAIL);

	v1.Set(3.0f, -4.0f);
	v1.Normalize();
	printf("Vector2D Normalize Length Check: %s\n", (v1.Length() == 1.0f) ? PASS : FAIL);

	v1.Set(3.0f, -4.0f);
	result.Set(0.6f, -0.8f);
	printf("Vector2D Normalize(vector): %s\n", (Vector2D::Normalize(v1) == result) ? PASS : FAIL);

	v1.Set(-3.0f, 4.0f);
	printf("Vector2D Normalize(vector) Length Check: %s\n", (Vector2D::Normalize(v1).Length() == 1.0f) ? PASS : FAIL);

	v1.Set(2.0f, 3.0f);
	v2.Set(4.0f, 5.0f);
	printf("Vector2D Dot: %s\n", ((Vector2D::Dot(v1, v2) - 23.0f) < EPSILON) ? PASS : FAIL);

	result.Set(0.707106f, 0.707106f, 1.0f);
	angle = PI_4;
	printf("Vector2D AngleRadians (%f): %s\n", angle, (Vector2D::AngleRadians(angle) == result) ? PASS : FAIL);

	result.Set(0.0f, 1.0f, 1.0f);
	angle = PI_2;
	printf("Vector2D AngleRadians (%f): %s\n", angle, (Vector2D::AngleRadians(angle) == result) ? PASS : FAIL);

	result.Set(-1.0f, 0.0f, 1.0f);
	angle = PI;
	printf("Vector2D AngleRadians (%f): %s\n", angle, (Vector2D::AngleRadians(angle) == result) ? PASS : FAIL);

	result.Set(-1.0f, 0.0f, 1.0f);
	angle = -PI;
	printf("Vector2D AngleRadians (%f): %s\n", angle, (Vector2D::AngleRadians(angle) == result) ? PASS : FAIL);

	result.Set(0.0f, -1.0f, 1.0f);
	angle = 1.5f * PI;
	printf("Vector2D AngleRadians (%f): %s\n", angle, (Vector2D::AngleRadians(angle) == result) ? PASS : FAIL);

	result.Set(1.0f, 0.0f, 1.0f);
	angle = 2.0f * PI;
	printf("Vector2D AngleRadians (%f): %s\n", angle, (Vector2D::AngleRadians(angle) == result) ? PASS : FAIL);

	result.Set(1.0f, 0.0f, 1.0f);
	angle = 0.0f;
	printf("Vector2D AngleRadians (%f): %s\n", angle, (Vector2D::AngleRadians(angle) == result) ? PASS : FAIL);

	result.Set(0.707106f, 0.707106f, 1.0f);
	angle = 45.0f;
	printf("Vector2D AngleDegrees (%f): %s\n", angle, (Vector2D::AngleDegrees(angle) == result) ? PASS : FAIL);

	result.Set(0.0f, 1.0f, 1.0f);
	angle = 90.0f;
	printf("Vector2D AngleDegrees (%f): %s\n", angle, (Vector2D::AngleDegrees(angle) == result) ? PASS : FAIL);

	result.Set(-1.0f, 0.0f, 1.0f);
	angle = 180.0f;
	printf("Vector2D AngleDegrees (%f): %s\n", angle, (Vector2D::AngleDegrees(angle) == result) ? PASS : FAIL);

	result.Set(-1.0f, 0.0f, 1.0f);
	angle = -180.0f;
	printf("Vector2D AngleDegrees (%f): %s\n", angle, (Vector2D::AngleDegrees(angle) == result) ? PASS : FAIL);

	result.Set(0.0f, -1.0f, 1.0f);
	angle = 270.0f;
	printf("Vector2D AngleDegrees (%f): %s\n", angle, (Vector2D::AngleDegrees(angle) == result) ? PASS : FAIL);

	result.Set(1.0f, 0.0f, 1.0f);
	angle = 360.0f;
	printf("Vector2D AngleDegrees (%f): %s\n", angle, (Vector2D::AngleDegrees(angle) == result) ? PASS : FAIL);

	result.Set(1.0f, 0.0f, 1.0f);
	angle = 0.0f;
	printf("Vector2D AngleDegrees (%f): %s\n", angle, (Vector2D::AngleDegrees(angle) == result) ? PASS : FAIL);
}

#undef PASS
#undef FAIL
#endif