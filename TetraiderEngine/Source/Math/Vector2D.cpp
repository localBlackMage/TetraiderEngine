/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

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
	std::cout << "\n========== Running Vector2D tests ==========\n\n";

	Vector2D * createdVector = new Vector2D();
	std::cout << "Created Vector(): " << ((createdVector->x == 0.0f && createdVector->y == 0.0f && createdVector->w == 1.0f) ? PASS : FAIL) << std::endl;
	delete createdVector;

	createdVector = new Vector2D(1.0f, 2.0f);
	std::cout << "Created Vector(_x, _y): " << ((createdVector->x == 1.0f && createdVector->y == 2.0f && createdVector->w == 1.0f) ? PASS : FAIL) << std::endl;
	delete createdVector;

	createdVector = new Vector2D(2.0f, 3.0f, 0.0f);
	std::cout << "Created Vector(_x, _y, _w): " << ((createdVector->x == 2.0f && createdVector->y == 3.0f && createdVector->w == 0.0f) ? PASS : FAIL) << std::endl;
	delete createdVector;

	Vector2D v1, v2, v3, result;
	float scalar;
	float angle;

	v1.Set(1.0f, 2.0f);
	v2.Set(1.0f, 2.0f);
	std::cout << "Vector2D == equal: " << ((v1 == v2) ? PASS : FAIL) << std::endl;

	v2.Set(1.1f, 2.0f);
	std::cout << "Vector2D == not equal: " << ((v1 == v2) ? FAIL : PASS);

	v2.Set(2.0f, 3.0f);
	std::cout << "Vector2D != not equal: " << ((v1 != v2) ? PASS : FAIL) << std::endl;

	v2.Set(1.0f, 2.0f);
	std::cout << "Vector2D != equal: " << ((v1 != v2) ? FAIL : PASS);

	v1.Set(0.1f, 2.0f);
	v2.Set(1.0f, 3.5f);
	result.Set(1.1f, 5.5f);
	std::cout << "Vector2D +: " << ((v1 + v2 == result) ? PASS : FAIL) << std::endl;

	v1.Set(-0.1f, 1.0f);
	v2.Set(1.0f, -2.0f);
	result.Set(0.9f, -1.0f);
	std::cout << "Vector2D +: " << ((v1 + v2 == result) ? PASS : FAIL) << std::endl;

	v1.Set(1.0f, 2.0f);
	v2.Set(1.0f, -2.0f);
	result.Set(0.0f, 4.0f);
	std::cout << "Vector2D -: " << ((v1 - v2 == result) ? PASS : FAIL) << std::endl;

	v1.Set(-2.0f, 2.5f);
	v2.Set(-2.0f, 2.0f);
	result.Set(0.0f, 0.5f);
	std::cout << "Vector2D -: " << ((v1 - v2 == result) ? PASS : FAIL) << std::endl;

	scalar = 2.0f;
	result.Set(-4.0f, 5.0f);
	std::cout << "Vector2D *: " << ((v1 * scalar == result) ? PASS : FAIL) << std::endl;

	scalar = 0.5f;
	result.Set(-1.0f, 1.25f);
	std::cout << "Vector2D *: " << ((v1 * scalar == result) ? PASS : FAIL) << std::endl;

	scalar = 2.0f;
	result.Set(-1.0f, 1.25f);
	std::cout << "Vector2D /: " << ((v1 / scalar == result) ? PASS : FAIL) << std::endl;

	v1.Set(-2.0f, 10.0f);
	scalar = 0.5f;
	result.Set(-4.0f, 20.0f);
	std::cout << "Vector2D /: " << ((v1 / scalar == result) ? PASS : FAIL) << std::endl;

	v1.Set(0.1f, 2.0f);
	v2.Set(1.0f, 3.5f);
	result.Set(1.1f, 5.5f);
	v1.Add(v2);
	std::cout << "Vector2D Add: " << ((v1 == result) ? PASS : FAIL) << std::endl;

	v1.Set(-0.1f, 1.0f);
	v2.Set(1.0f, -2.0f);
	result.Set(0.9f, -1.0f);
	v1.Add(v2);
	std::cout << "Vector2D Add: " << ((v1 == result) ? PASS : FAIL) << std::endl;

	v1.Set(1.0f, 2.0f);
	v2.Set(1.0f, -2.0f);
	result.Set(0.0f, 4.0f);
	v1.Sub(v2);
	std::cout << "Vector2D Sub: " << ((v1 == result) ? PASS : FAIL) << std::endl;

	v1.Set(-2.0f, 2.5f);
	v2.Set(-2.0f, 2.0f);
	result.Set(0.0f, 0.5f);
	v1.Sub(v2);
	std::cout << "Vector2D Sub: " << ((v1 == result) ? PASS : FAIL) << std::endl;

	v1.Set(-2.0f, 2.5f);
	scalar = 2.0f;
	result.Set(-4.0f, 5.0f);
	v1.Mul(scalar);
	std::cout << "Vector2D Mul: " << ((v1 == result) ? PASS : FAIL) << std::endl;

	v1.Set(-2.0f, 2.5f);
	scalar = 0.5f;
	result.Set(-1.0f, 1.25f);
	v1.Mul(scalar);
	std::cout << "Vector2D Mul: " << ((v1 == result) ? PASS : FAIL) << std::endl;

	v1.Set(-2.0f, 10.0f);
	scalar = 2.0f;
	result.Set(-1.0f, 5.0f);
	v1.Div(scalar);
	std::cout << "Vector2D Div: " << ((v1 == result) ? PASS : FAIL) << std::endl;

	v1.Set(-2.0f, 10.0f);
	scalar = 0.5f;
	result.Set(-4.0f, 20.0f);
	v1.Div(scalar);
	std::cout << "Vector2D Div: " << ((v1 == result) ? PASS : FAIL) << std::endl;

	v1.Set(1.0f, 1.0f);
	result.Set(0.0f, 0.0f);
	v1.Zero();
	std::cout << "Vector2D Zero: " << ((v1 == result) ? PASS : FAIL) << std::endl;

	v1.Set(1.0f, 0.0f, 1.0f);
	result.Set(1.0f, 2.0f, 1.0f);
	v1.Set(1.0f, 2.0f);
	std::cout << "Vector2D Set(_x, _y): " << ((v1 == result) ? PASS : FAIL) << std::endl;

	v1.Set(0.0f, 0.0f, 0.0f);
	result.Set(1.0f, 2.0f, 1.0f);
	v1.Set(1.0f, 2.0f, 1.0f);
	std::cout << "Vector2D Set(_x, _y, _w): " << ((v1 == result) ? PASS : FAIL) << std::endl;

	v1.Set(1.0f, -1.0f, 1.0f);
	result.Set(-1.0f, 1.0f, 1.0f);
	v1.Negate();
	std::cout << "Vector2D Negate: " << ((v1 == result) ? PASS : FAIL) << std::endl;

	v1.Set(-1.0f, 0.0f, 1.0f);
	result.Set(1.0f, 0.0f, 1.0f);
	v1.Negate();
	std::cout << "Vector2D Negate: " << ((v1 == result) ? PASS : FAIL) << std::endl;

	v1.Set(-3.0f, 4.0f);
	std::cout << "Vector2D SquareLength: " << ((v1.SquareLength() == 25.0) ? PASS : FAIL) << std::endl;

	v1.Set(-4.0f, 3.0f);
	std::cout << "Vector2D Length: " << ((v1.Length() == 5.0) ? PASS : FAIL) << std::endl;

	v1.Set(-3.0f, 4.0f);
	v2.Set(3.0f, -4.0f);
	std::cout << "Vector2D SquareDistance: " << ((Vector2D::SquareDistance(v1, v2) == 100.0f) ? PASS : FAIL) << std::endl;

	std::cout << "Vector2D Distance: " << (((Vector2D::Distance(v1, v2) - 10.0f) < EPSILON) ? PASS : FAIL) << std::endl;

	result.Set(-0.6f, 0.8f);
	v1.Normalize();
	std::cout << "Vector2D Normalize: " << ((v1 == result) ? PASS : FAIL) << std::endl;

	v1.Set(3.0f, -4.0f);
	v1.Normalize();
	std::cout << "Vector2D Normalize Length Check: " << ((v1.Length() == 1.0f) ? PASS : FAIL) << std::endl;

	v1.Set(3.0f, -4.0f);
	result.Set(0.6f, -0.8f);
	std::cout << "Vector2D Normalize(vector): " << ((Vector2D::Normalize(v1) == result) ? PASS : FAIL) << std::endl;

	v1.Set(-3.0f, 4.0f);
	std::cout << "Vector2D Normalize(vector) Length Check: " << ((Vector2D::Normalize(v1).Length() == 1.0f) ? PASS : FAIL) << std::endl;

	v1.Set(2.0f, 3.0f);
	v2.Set(4.0f, 5.0f);
	std::cout << "Vector2D Dot: " << (((Vector2D::Dot(v1, v2) - 23.0f) < EPSILON) ? PASS : FAIL) << std::endl;

	result.Set(0.707106f, 0.707106f, 1.0f);
	angle = PI_4;
	std::cout << "Vector2D AngleRadians " << ((Vector2D::AngleRadians(angle) == result) ? PASS : FAIL) << std::endl;

	result.Set(0.0f, 1.0f, 1.0f);
	angle = PI_2;
	std::cout << "Vector2D AngleRadians " << ((Vector2D::AngleRadians(angle) == result) ? PASS : FAIL) << std::endl;

	result.Set(-1.0f, 0.0f, 1.0f);
	angle = PI;
	std::cout << "Vector2D AngleRadians " << ((Vector2D::AngleRadians(angle) == result) ? PASS : FAIL) << std::endl;

	result.Set(-1.0f, 0.0f, 1.0f);
	angle = -PI;
	std::cout << "Vector2D AngleRadians " << ((Vector2D::AngleRadians(angle) == result) ? PASS : FAIL) << std::endl;

	result.Set(0.0f, -1.0f, 1.0f);
	angle = 1.5f * PI;
	std::cout << "Vector2D AngleRadians " << ((Vector2D::AngleRadians(angle) == result) ? PASS : FAIL) << std::endl;

	result.Set(1.0f, 0.0f, 1.0f);
	angle = 2.0f * PI;
	std::cout << "Vector2D AngleRadians " << ((Vector2D::AngleRadians(angle) == result) ? PASS : FAIL) << std::endl;

	result.Set(1.0f, 0.0f, 1.0f);
	angle = 0.0f;
	std::cout << "Vector2D AngleRadians " << ((Vector2D::AngleRadians(angle) == result) ? PASS : FAIL) << std::endl;

	result.Set(0.707106f, 0.707106f, 1.0f);
	angle = 45.0f;
	std::cout << "Vector2D AngleDegrees " << ((Vector2D::AngleDegrees(angle) == result) ? PASS : FAIL) << std::endl;

	result.Set(0.0f, 1.0f, 1.0f);
	angle = 90.0f;
	std::cout << "Vector2D AngleDegrees " << ((Vector2D::AngleDegrees(angle) == result) ? PASS : FAIL) << std::endl;

	result.Set(-1.0f, 0.0f, 1.0f);
	angle = 180.0f;
	std::cout << "Vector2D AngleDegrees " << ((Vector2D::AngleDegrees(angle) == result) ? PASS : FAIL) << std::endl;

	result.Set(-1.0f, 0.0f, 1.0f);
	angle = -180.0f;
	std::cout << "Vector2D AngleDegrees " << ((Vector2D::AngleDegrees(angle) == result) ? PASS : FAIL) << std::endl;

	result.Set(0.0f, -1.0f, 1.0f);
	angle = 270.0f;
	std::cout << "Vector2D AngleDegrees " << ((Vector2D::AngleDegrees(angle) == result) ? PASS : FAIL) << std::endl;

	result.Set(1.0f, 0.0f, 1.0f);
	angle = 360.0f;
	std::cout << "Vector2D AngleDegrees " << ((Vector2D::AngleDegrees(angle) == result) ? PASS : FAIL) << std::endl;

	result.Set(1.0f, 0.0f, 1.0f);
	angle = 0.0f;
	std::cout << "Vector2D AngleDegrees " << ((Vector2D::AngleDegrees(angle) == result) ? PASS : FAIL) << std::endl;
}

#undef PASS
#undef FAIL
#endif