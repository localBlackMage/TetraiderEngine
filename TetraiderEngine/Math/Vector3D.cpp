#include "Vector3D.h"
#include "AcrylicorTypedefs.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <algorithm>

Vector3D::Vector3D() : m_x(0.f), m_y(0.f), m_z(0.f), m_w(1.f) { }

Vector3D::Vector3D(float x, float y, float z) : m_x(x), m_y(y), m_z(z), m_w(1.f) { }

Vector3D::Vector3D(float x, float y, float z, float w) : m_x(x), m_y(y), m_z(z), m_w(w) { }

Vector3D::Vector3D(const Vector3D & rhs) : m_x(rhs.m_x), m_y(rhs.m_y), m_z(rhs.m_z), m_w(rhs.m_w) { }

Vector3D::Vector3D(const Vector2D & rhs) : m_x(rhs.getX()), m_y(rhs.getY()), m_z(0.0f), m_w(rhs.getW()) { }

Vector3D& Vector3D::operator=(const Vector3D rhs)
{
	m_x = rhs.m_x;
	m_y = rhs.m_y;
	m_z = rhs.m_z;
	m_w = rhs.m_w;
	return *this;
}

Vector3D::~Vector3D() { }

void Vector3D::Set(float x, float y, float z)
{
	m_x = x;
	m_y = y;
	m_z = z;
}

void Vector3D::Set(float x, float y, float z, float w)
{
	m_x = x;
	m_y = y;
	m_z = z;
	m_w = w;
}

#pragma region Operator Overrides
Vector3D::operator float*()
{
	return &(m_x);
}

bool Vector3D::operator==(const Vector3D& other) const
{
	double d = 0.0f;

	d += fabs(m_x - other.m_x);
	d += fabs(m_y - other.m_y);
	d += fabs(m_z - other.m_z);
	d += fabs(m_w - other.m_w);

	return d < EPSILON;
}

bool Vector3D::operator!=(const Vector3D& other) const
{
	double d = 0.0f;

	d += fabs(m_x - other.m_x);
	d += fabs(m_y - other.m_y);
	d += fabs(m_z - other.m_z);
	d += fabs(m_w - other.m_w);

	return d >= EPSILON;
}

Vector3D Vector3D::operator+(const Vector3D& other) const
{
	return Vector3D(
		m_x + other.m_x,
		m_y + other.m_y,
		m_z + other.m_z
	);
}

Vector3D & Vector3D::operator+=(const Vector3D & other)
{
	m_x += other.m_x;
	m_y += other.m_y;
	m_z += other.m_z;
	return *this;
}

Vector3D Vector3D::operator-(const Vector3D& other) const
{
	return Vector3D(
		m_x - other.m_x,
		m_y - other.m_y,
		m_z - other.m_z
	);
}

Vector3D & Vector3D::operator-=(const Vector3D & other)
{
	m_x -= other.m_x;
	m_y -= other.m_y;
	m_z -= other.m_z;
	return *this;
}

Vector3D Vector3D::operator*(const float scalar) const
{
	return Vector3D(
		m_x * scalar,
		m_y * scalar,
		m_z * scalar
	);
}

Vector3D Vector3D::operator/(const float divisor) const
{
	return Vector3D(
		m_x / divisor,
		m_y / divisor,
		m_z / divisor
	);
}

float Vector3D::operator[](const int idx) const
{
	return *(&m_x + idx);
}

float& Vector3D::operator[](const int idx)
{
	return *(&m_x + idx);
}
#pragma endregion

void Vector3D::Add(const Vector3D& other)
{
	m_x += other.m_x;
	m_y += other.m_y;
	m_z += other.m_z;
}

void Vector3D::Sub(const Vector3D& other)
{
	m_x -= other.m_x;
	m_y -= other.m_y;
	m_z -= other.m_z;
}

void Vector3D::Mul(const float scalar)
{
	m_x *= scalar;
	m_y *= scalar;
	m_z *= scalar;
}

void Vector3D::Div(const float divisor)
{
	if (divisor == 0.0f) {
		//throw "Division by zero error!";
		return;
	}
	m_x /= divisor;
	m_y /= divisor;
	m_z /= divisor;
}

void Vector3D::Print() const
{
	printf("(X: %f, Y: %f, Z: %f, W: %f)\n", m_x, m_y, m_z, m_w);
}

void Vector3D::Zero()
{
	m_x = m_y = m_z = 0.0f;
}

void Vector3D::Negate()
{
	m_x = -m_x;
	m_y = -m_y;
	m_z = -m_z;
}

float Vector3D::SquareLength() const
{
	return m_x * m_x + m_y * m_y + m_z * m_z;
}

float Vector3D::Length() const
{
	return sqrtf(SquareLength());
}

void Vector3D::Normalize()
{
	Div(Length());
}

#pragma region Static Methods
float Vector3D::SquareDistance(const Vector3D& vector0, const Vector3D& vector1)
{
	float x = vector1.m_x - vector0.m_x;
	float y = vector1.m_y - vector0.m_y;
	float z = vector1.m_z - vector0.m_z;
	return x * x + y * y + z * z;
}

float Vector3D::Distance(const Vector3D& vector0, const Vector3D& vector1)
{
	return sqrtf(SquareDistance(vector0, vector1));
}

Vector3D Vector3D::Normalize(const Vector3D& vector)
{
	float length = vector.Length();
	if (length == 0.0f)
		return Vector3D();
	return vector / length;
}

float Vector3D::Dot(const Vector3D& vector0, const Vector3D& vector1)
{
	return vector0.m_x * vector1.m_x + vector0.m_y * vector1.m_y + vector0.m_z * vector1.m_z;
}

Vector3D Vector3D::Cross(const Vector3D& vector0, const Vector3D& vector1)
{
	return Vector3D(
		vector0.m_y * vector1.m_z - vector0.m_z * vector1.m_y,
		vector0.m_z * vector1.m_x - vector0.m_x * vector1.m_z,
		vector0.m_x * vector1.m_y - vector0.m_y * vector1.m_x
	);
}

Vector3D Vector3D::AngleRadians(float radians, Vector3D& axis)
{
	axis.Print();
	printf("radians: %f", radians);
	return Vector3D();
}

Vector3D Vector3D::AngleDegrees(float degrees, Vector3D& axis)
{
	return AngleRadians(degrees * DEG_TO_RAD, axis);
}
#pragma endregion

Vector3D operator*(const float scalar, const Vector3D& other)
{
	return Vector3D(
		other.getX() * scalar,
		other.getY() * scalar,
		other.getZ() * scalar
	);
}

#if TEST_MODE
void Vector3DTests()
{
	printf("\n========== Running Vector3D tests ==========\n\n");

	Vector3D * createdVector = new Vector3D();
	printf("Created Vector(): %s\n", (createdVector->getX() == 0.0f && createdVector->getY() == 0.0f && createdVector->getW() == 1.0f) ? PASS : FAIL);
	delete createdVector;

	createdVector = new Vector3D(1.0f, 2.0f, 3.0f);
	printf("Created Vector(x, y): %s\n", (createdVector->getX() == 1.0f && createdVector->getY() == 2.0f && createdVector->getZ() == 3.0f && createdVector->getW() == 1.0f) ? PASS : FAIL);
	delete createdVector;

	createdVector = new Vector3D(2.0f, 3.0f, 4.0f, 0.0f);
	printf("Created Vector(x, y, w): %s\n", (createdVector->getX() == 2.0f && createdVector->getY() == 3.0f && createdVector->getZ() == 4.0f && createdVector->getW() == 0.0f) ? PASS : FAIL);
	delete createdVector;

	Vector3D v1, v2, v3, result;
	float scalar;

	v1.Set(1.0f, 2.0f, 3.0f);
	v2.Set(1.0f, 2.0f, 3.0f);
	printf("Vector3D == equal: %s\n", (v1 == v2) ? PASS : FAIL);

	v2.setX(1.1f);
	printf("Vector3D == not equal: %s\n", (v1 == v2) ? FAIL : PASS);

	v2.Set(2.0f, 3.0f, 4.0f);
	printf("Vector3D != not equal: %s\n", (v1 != v2) ? PASS : FAIL);

	v2.Set(1.0f, 2.0f, 3.0f);
	printf("Vector3D != equal: %s\n", (v1 != v2) ? FAIL : PASS);

	v1.Set(0.1f, 2.0f, 1.0f);
	v2.Set(1.0f, 3.5f, -3.0f);
	result.Set(1.1f, 5.5f, -2.0f);
	printf("Vector3D +: %s\n", (v1 + v2 == result) ? PASS : FAIL);

	v1.Set(-0.1f, 1.0f, -1.0f);
	v2.Set(1.0f, -2.0f, 1.0f);
	result.Set(0.9f, -1.0f, 0.0f);
	printf("Vector3D +: %s\n", (v1 + v2 == result) ? PASS : FAIL);

	v1.Set(1.0f, 2.0f, -1.0f);
	v2.Set(1.0f, -2.0f, -1.0f);
	result.Set(0.0f, 4.0f, 0.0f);
	printf("Vector3D -: %s\n", (v1 - v2 == result) ? PASS : FAIL);

	v1.Set(-2.0f, 2.5f, 1.0f);
	v2.Set(-2.0f, 2.0f, -1.0f);
	result.Set(0.0f, 0.5f, 2.0f);
	printf("Vector3D -: %s\n", (v1 - v2 == result) ? PASS : FAIL);

	v1.Set(-2.0f, 2.5f, 1.0f);
	scalar = 2.0f;
	result.Set(-4.0f, 5.0f, 2.0f);
	printf("Vector3D *: %s\n", (v1 * scalar == result) ? PASS : FAIL);

	v1.Set(-2.0f, 2.5f, 1.0f);
	scalar = 0.5f;
	result.Set(-1.0f, 1.25f, 0.5f);
	printf("Vector3D *: %s\n", (v1 * scalar == result) ? PASS : FAIL);

	v1.Set(-2.0f, 10.0f, 4.5f);
	scalar = 2.0f;
	result.Set(-1.0f, 5.0f, 2.25f);
	printf("Vector3D /: %s\n", (v1 / scalar == result) ? PASS : FAIL);

	v1.Set(-2.0f, 10.0f, 1.0f);
	scalar = 0.5f;
	result.Set(-4.0f, 20.0f, 2.0f);
	printf("Vector3D /: %s\n", (v1 / scalar == result) ? PASS : FAIL);

	v1.Set(0.1f, 2.0f, 1.0f);
	v2.Set(1.0f, 3.5f, -3.0f);
	result.Set(1.1f, 5.5f, -2.0f);
	v1.Add(v2);
	printf("Vector3D Add: %s\n", (v1 == result) ? PASS : FAIL);

	v1.Set(-0.1f, 1.0f, -1.0f);
	v2.Set(1.0f, -2.0f, 1.0f);
	result.Set(0.9f, -1.0f, 0.0f);
	v1.Add(v2);
	printf("Vector3D Add: %s\n", (v1 == result) ? PASS : FAIL);

	v1.Set(1.0f, 2.0f, -1.0f);
	v2.Set(1.0f, -2.0f, -1.0f);
	result.Set(0.0f, 4.0f, 0.0f);
	v1.Sub(v2);
	printf("Vector3D Sub: %s\n", (v1 == result) ? PASS : FAIL);

	v1.Set(-2.0f, 2.5f, 1.0f);
	v2.Set(-2.0f, 2.0f, -1.0f);
	result.Set(0.0f, 0.5f, 2.0f);
	v1.Sub(v2);
	printf("Vector3D Sub: %s\n", (v1 == result) ? PASS : FAIL);

	v1.Set(-2.0f, 2.5f, 1.0f);
	scalar = 2.0f;
	result.Set(-4.0f, 5.0f, 2.0f);
	v1.Mul(scalar);
	printf("Vector3D Mul: %s\n", (v1 == result) ? PASS : FAIL);

	v1.Set(-2.0f, 2.5f, 1.0f);
	scalar = 0.5f;
	result.Set(-1.0f, 1.25f, 0.5f);
	v1.Mul(scalar);
	printf("Vector3D Mul: %s\n", (v1 == result) ? PASS : FAIL);

	v1.Set(-2.0f, 10.0f, 4.5f);
	scalar = 2.0f;
	result.Set(-1.0f, 5.0f, 2.25f);
	v1.Div(scalar);
	printf("Vector3D Div: %s\n", (v1 == result) ? PASS : FAIL);

	v1.Set(-2.0f, 10.0f, 1.0f);
	scalar = 0.5f;
	result.Set(-4.0f, 20.0f, 2.0f);
	v1.Div(scalar);
	printf("Vector3D Div: %s\n", (v1 == result) ? PASS : FAIL);

	v1.Set(1.0f, 1.0f, 1.0f);
	result.Set(0.0f, 0.0f, 0.0f);
	v1.Zero();
	printf("Vector3D Zero: %s\n", (v1 == result) ? PASS : FAIL);

	v1.Set(0.0f, 0.0f, 0.0f);
	result.Set(1.0f, 2.0f, 3.0f);
	v1.Set(1.0f, 2.0f, 3.0f);
	printf("Vector3D Set(x, y, z): %s\n", (v1 == result) ? PASS : FAIL);

	v1.Set(0.0f, 0.0f, 0.0f, 0.0f);
	result.Set(1.0f, 2.0f, 3.0f, 1.0f);
	v1.Set(1.0f, 2.0f, 3.0f, 1.0f);
	printf("Vector3D Set(x, y, z, w): %s\n", (v1 == result) ? PASS : FAIL);

	v1.Set(1.0f, 2.0f, -3.0f, 1.0f);
	result.Set(-1.0f, -2.0f, 3.0f, 1.0f);
	v1.Negate();
	printf("Vector3D Negate: %s\n", (v1 == result) ? PASS : FAIL);

	v1.Set(1.0f, 0.0f, -3.0f, 1.0f);
	result.Set(-1.0f, 0.0f, 3.0f, 1.0f);
	v1.Negate();
	printf("Vector3D Negate: %s\n", (v1 == result) ? PASS : FAIL);

	v1.Set(-3.0f, 4.0f, 5.0f);
	printf("Vector3D SquareLength: %s\n", (v1.SquareLength() == 50.0f) ? PASS : FAIL);

	v1.Set(-4.0f, 3.0f, -5.0f);
	printf("Vector3D Length: %s\n", (v1.Length() == 7.0710678f) ? PASS : FAIL);

	v1.Set(-3.0f, 4.0f, 5.0f);
	v2.Set(3.0f, -4.0f, -5.0f);
	printf("Vector3D SquareDistance: %s\n", (Vector3D::SquareDistance(v1, v2) == 200.0f) ? PASS : FAIL);

	v1.Set(-3.0f, 4.0f, 5.0f);
	v2.Set(3.0f, -4.0f, -5.0f);
	printf("Vector3D Distance: %s\n", ((Vector3D::Distance(v1, v2) - 14.1421356f) < EPSILON) ? PASS : FAIL);

	v1.Set(-3.0f, 4.0f, 5.0f);
	result.Set(-0.424264f, 0.565685f, 0.707107f);
	v1.Normalize();
	printf("Vector3D Normalize: %s\n", (v1 == result) ? PASS : FAIL);

	v1.Set(-3.0f, 4.0f, -5.0f);
	v1.Normalize();
	printf("Vector3D Normalize Length Check: %s\n", (v1.Length() == 1.0f) ? PASS : FAIL);

	v1.Set(-3.0f, 4.0f, 5.0f);
	result.Set(-0.424264f, 0.565685f, 0.707107f);
	printf("Vector3D Normalize(vector): %s\n", (Vector3D::Normalize(v1) == result) ? PASS : FAIL);

	v1.Set(-3.0f, 4.0f, -5.0f);
	printf("Vector3D Normalize(vector) Length Check: %s\n", (Vector3D::Normalize(v1).Length() == 1.0f) ? PASS : FAIL);

	v1.Set(2.0f, 3.0f, 3.0f);
	v2.Set(4.0f, 5.0f, 3.0f);
	printf("Vector3D Dot: %s\n", ((Vector3D::Dot(v1, v2) - 32.0f) < EPSILON) ? PASS : FAIL);

	v1.Set(1.0f, 0.0f, 0.0f);
	v2.Set(0.0f, 1.0f, 0.0f);
	result.Set(0.0f, 0.0f, 1.0f);
	printf("Vector3D Cross: %s\n", (Vector3D::Cross(v1, v2) == result) ? PASS : FAIL);

	v1.Set(1.0f, 0.0f, 0.0f);
	v2.Set(0.0f, 0.0f, -1.0f);
	result.Set(0.0f, 1.0f, 0.0f);
	printf("Vector3D Cross: %s\n", (Vector3D::Cross(v1, v2) == result) ? PASS : FAIL);

	v1.Set(0.0f, 1.0f, 0.0f);
	v2.Set(0.0f, 0.0f, 1.0f);
	result.Set(1.0f, 0.0f, 0.0f);
	printf("Vector3D Cross: %s\n", (Vector3D::Cross(v1, v2) == result) ? PASS : FAIL);

#pragma region Rotation Tests

	//result.m_x = 0.707106f;	result.m_y = 0.707106f;	result.m_w = 1.0f;
	//angle = M_PI_4;
	//printf("Vector3D AngleRadians (%f): %s\n", angle, (Vector3D::AngleRadians(angle) == result) ? PASS : FAIL);

	//result.m_x = 0.0f;	result.m_y = 1.0f;	result.m_w = 1.0f;
	//angle = M_PI_2;
	//printf("Vector3D AngleRadians (%f): %s\n", angle, (Vector3D::AngleRadians(angle) == result) ? PASS : FAIL);

	//result.m_x = -1.0f;	result.m_y = 0.0f;	result.m_w = 1.0f;
	//angle = M_PI;
	//printf("Vector3D AngleRadians (%f): %s\n", angle, (Vector3D::AngleRadians(angle) == result) ? PASS : FAIL);

	//result.m_x = -1.0f;	result.m_y = 0.0f;	result.m_w = 1.0f;
	//angle = -M_PI;
	//printf("Vector3D AngleRadians (%f): %s\n", angle, (Vector3D::AngleRadians(angle) == result) ? PASS : FAIL);

	//result.m_x = 0.0f;	result.m_y = -1.0f;	result.m_w = 1.0f;
	//angle = 1.5f * M_PI;
	//printf("Vector3D AngleRadians (%f): %s\n", angle, (Vector3D::AngleRadians(angle) == result) ? PASS : FAIL);

	//result.m_x = 1.0f;	result.m_y = 0.0f;	result.m_w = 1.0f;
	//angle = 2.0f * M_PI;
	//printf("Vector3D AngleRadians (%f): %s\n", angle, (Vector3D::AngleRadians(angle) == result) ? PASS : FAIL);

	//result.m_x = 1.0f;	result.m_y = 0.0f;	result.m_w = 1.0f;
	//angle = 0.0f;
	//printf("Vector3D AngleRadians (%f): %s\n", angle, (Vector3D::AngleRadians(angle) == result) ? PASS : FAIL);

	//result.m_x = 0.707106f;	result.m_y = 0.707106f;	result.m_w = 1.0f;
	//angle = 45.0f;
	//printf("Vector3D AngleDegrees (%f): %s\n", angle, (Vector3D::AngleDegrees(angle) == result) ? PASS : FAIL);

	//result.m_x = 0.0f;	result.m_y = 1.0f;	result.m_w = 1.0f;
	//angle = 90.0f;
	//printf("Vector3D AngleDegrees (%f): %s\n", angle, (Vector3D::AngleDegrees(angle) == result) ? PASS : FAIL);

	//result.m_x = -1.0f;	result.m_y = 0.0f;	result.m_w = 1.0f;
	//angle = 180.0f;
	//printf("Vector3D AngleDegrees (%f): %s\n", angle, (Vector3D::AngleDegrees(angle) == result) ? PASS : FAIL);

	//result.m_x = -1.0f;	result.m_y = 0.0f;	result.m_w = 1.0f;
	//angle = -180.0f;
	//printf("Vector3D AngleDegrees (%f): %s\n", angle, (Vector3D::AngleDegrees(angle) == result) ? PASS : FAIL);

	//result.m_x = 0.0f;	result.m_y = -1.0f;	result.m_w = 1.0f;
	//angle = 270.0f;
	//printf("Vector3D AngleDegrees (%f): %s\n", angle, (Vector3D::AngleDegrees(angle) == result) ? PASS : FAIL);

	//result.m_x = 1.0f;	result.m_y = 0.0f;	result.m_w = 1.0f;
	//angle = 360.0f;
	//printf("Vector3D AngleDegrees (%f): %s\n", angle, (Vector3D::AngleDegrees(angle) == result) ? PASS : FAIL);

	//result.m_x = 1.0f;	result.m_y = 0.0f;	result.m_w = 1.0f;
	//angle = 0.0f;
	//printf("Vector3D AngleDegrees (%f): %s\n", angle, (Vector3D::AngleDegrees(angle) == result) ? PASS : FAIL);
#pragma endregion
}
#endif