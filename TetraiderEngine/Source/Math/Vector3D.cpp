#include "Vector3D.h"
#include "MathDefs.h"
#include <iostream>

Vector3D::Vector3D() : x(0.f), y(0.f), w(1.f) { }

Vector3D::Vector3D(float _x, float _y, float _z) : x(_x), y(_y), z(_z), w(1.f) { }

Vector3D::Vector3D(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) { }

Vector3D::Vector3D(const Vector3D & rhs) : x(rhs.x), y(rhs.y), w(rhs.w) { }

Vector3D& Vector3D::operator=(const Vector3D rhs)
{
	x = rhs.x;
	y = rhs.y;
	z = rhs.z;
	w = rhs.w;
	return *this;
}

Vector3D::~Vector3D() { }

void Vector3D::Set(float _x, float _y, float _z)
{
	x = _x;
	y = _y;
	z = _z;
}

void Vector3D::Set(float _x, float _y, float _z, float _w)
{
	x = _x;
	y = _y;
	z = _z;
	w = _w;
}

#pragma region Operator Overrides
Vector3D::operator float*()
{
	return &(x);
}

bool Vector3D::operator==(const Vector3D& other) const
{
	double d = 0.0f;

	d += fabs(x - other.x);
	d += fabs(y - other.y);
	d += fabs(z - other.z);
	d += fabs(w - other.w);

	return d < EPSILON;
}

bool Vector3D::operator!=(const Vector3D& other) const
{
	double d = 0.0f;

	d += fabs(x - other.x);
	d += fabs(y - other.y);
	d += fabs(z - other.z);
	d += fabs(w - other.w);

	return d >= EPSILON;
}

Vector3D Vector3D::operator+(const Vector3D& other) const
{
	return Vector3D(
		x + other.x,
		y + other.y,
		z + other.z
	);
}

Vector3D & Vector3D::operator+=(const Vector3D & other)
{
	x += other.x;
	y += other.y;
	z += other.z;
	return *this;
}

Vector3D Vector3D::operator-(const Vector3D& other) const
{
	return Vector3D(
		x - other.x,
		y - other.y,
		z - other.z
	);
}

Vector3D & Vector3D::operator-=(const Vector3D & other)
{
	x -= other.x;
	y -= other.y;
	z -= other.z;
	return *this;
}

Vector3D Vector3D::operator*(const float scalar) const
{
	return Vector3D(
		x * scalar,
		y * scalar,
		z * scalar
	);
}

Vector3D Vector3D::operator/(const float divisor) const
{
	return Vector3D(
		x / divisor,
		y / divisor,
		z / divisor
	);
}

float Vector3D::operator[](const int idx) const
{
	return *(&x + idx);
}

float& Vector3D::operator[](const int idx)
{
	return *(&x + idx);
}
#pragma endregion

void Vector3D::Add(const Vector3D& other)
{
	x += other.x;
	y += other.y;
	z += other.z;
}

void Vector3D::Sub(const Vector3D& other)
{
	x -= other.x;
	y -= other.y;
	z -= other.z;
}

void Vector3D::Mul(const float scalar)
{
	x *= scalar;
	y *= scalar;
	z *= scalar;
}

void Vector3D::Div(const float divisor)
{
	x /= divisor;
	y /= divisor;
	z /= divisor;
}

void Vector3D::Print() const
{
	std::cout << "<X: " << x << ", Y: " << y << ", Z: " << z << ", W: " << w << ">" << std::endl;
}

void Vector3D::Zero()
{
	x = y = z = 0.0f;
}

void Vector3D::Negate()
{
	x = -x;
	y = -y;
	z = -z;
}

float Vector3D::SquareLength() const
{
	return x * x + y * y + z * z;
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
	float x = vector1.x - vector0.x;
	float y = vector1.y - vector0.y;
	float z = vector1.z - vector0.z;
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
	return vector0.x * vector1.x + vector0.y * vector1.y + vector0.z * vector1.z;
}

Vector3D Vector3D::Cross(const Vector3D& vector0, const Vector3D& vector1)
{
	return Vector3D(
		vector0.y * vector1.z - vector0.z * vector1.y,
		vector0.z * vector1.x - vector0.x * vector1.z,
		vector0.x * vector1.y - vector0.y * vector1.x
	);
}

Vector3D Vector3D::AngleRadians(float radians, Vector3D& axis)
{
	axis.Print();
	std::cout << "Radians: " << radians << std::endl;
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
		other.x * scalar,
		other.y * scalar,
		other.z * scalar
	);
}

#if TEST_MODE
void Vector3DTests()
{
	std::cout << "\n========== Running Vector3D tests ==========\n\n");

	Vector3D * createdVector = new Vector3D();
	std::cout << "Created Vector(): %s\n", (createdVector->getX() == 0.0f && createdVector->getY() == 0.0f && createdVector->getW() == 1.0f) ? PASS : FAIL);
	delete createdVector;

	createdVector = new Vector3D(1.0f, 2.0f, 3.0f);
	std::cout << "Created Vector(x, y): %s\n", (createdVector->getX() == 1.0f && createdVector->getY() == 2.0f && createdVector->getZ() == 3.0f && createdVector->getW() == 1.0f) ? PASS : FAIL);
	delete createdVector;

	createdVector = new Vector3D(2.0f, 3.0f, 4.0f, 0.0f);
	std::cout << "Created Vector(x, y, w): %s\n", (createdVector->getX() == 2.0f && createdVector->getY() == 3.0f && createdVector->getZ() == 4.0f && createdVector->getW() == 0.0f) ? PASS : FAIL);
	delete createdVector;

	Vector3D v1, v2, v3, result;
	float scalar;

	v1.Set(1.0f, 2.0f, 3.0f);
	v2.Set(1.0f, 2.0f, 3.0f);
	std::cout << "Vector3D == equal: %s\n", (v1 == v2) ? PASS : FAIL);

	v2.setX(1.1f);
	std::cout << "Vector3D == not equal: %s\n", (v1 == v2) ? FAIL : PASS);

	v2.Set(2.0f, 3.0f, 4.0f);
	std::cout << "Vector3D != not equal: %s\n", (v1 != v2) ? PASS : FAIL);

	v2.Set(1.0f, 2.0f, 3.0f);
	std::cout << "Vector3D != equal: %s\n", (v1 != v2) ? FAIL : PASS);

	v1.Set(0.1f, 2.0f, 1.0f);
	v2.Set(1.0f, 3.5f, -3.0f);
	result.Set(1.1f, 5.5f, -2.0f);
	std::cout << "Vector3D +: %s\n", (v1 + v2 == result) ? PASS : FAIL);

	v1.Set(-0.1f, 1.0f, -1.0f);
	v2.Set(1.0f, -2.0f, 1.0f);
	result.Set(0.9f, -1.0f, 0.0f);
	std::cout << "Vector3D +: %s\n", (v1 + v2 == result) ? PASS : FAIL);

	v1.Set(1.0f, 2.0f, -1.0f);
	v2.Set(1.0f, -2.0f, -1.0f);
	result.Set(0.0f, 4.0f, 0.0f);
	std::cout << "Vector3D -: %s\n", (v1 - v2 == result) ? PASS : FAIL);

	v1.Set(-2.0f, 2.5f, 1.0f);
	v2.Set(-2.0f, 2.0f, -1.0f);
	result.Set(0.0f, 0.5f, 2.0f);
	std::cout << "Vector3D -: %s\n", (v1 - v2 == result) ? PASS : FAIL);

	v1.Set(-2.0f, 2.5f, 1.0f);
	scalar = 2.0f;
	result.Set(-4.0f, 5.0f, 2.0f);
	std::cout << "Vector3D *: %s\n", (v1 * scalar == result) ? PASS : FAIL);

	v1.Set(-2.0f, 2.5f, 1.0f);
	scalar = 0.5f;
	result.Set(-1.0f, 1.25f, 0.5f);
	std::cout << "Vector3D *: %s\n", (v1 * scalar == result) ? PASS : FAIL);

	v1.Set(-2.0f, 10.0f, 4.5f);
	scalar = 2.0f;
	result.Set(-1.0f, 5.0f, 2.25f);
	std::cout << "Vector3D /: %s\n", (v1 / scalar == result) ? PASS : FAIL);

	v1.Set(-2.0f, 10.0f, 1.0f);
	scalar = 0.5f;
	result.Set(-4.0f, 20.0f, 2.0f);
	std::cout << "Vector3D /: %s\n", (v1 / scalar == result) ? PASS : FAIL);

	v1.Set(0.1f, 2.0f, 1.0f);
	v2.Set(1.0f, 3.5f, -3.0f);
	result.Set(1.1f, 5.5f, -2.0f);
	v1.Add(v2);
	std::cout << "Vector3D Add: %s\n", (v1 == result) ? PASS : FAIL);

	v1.Set(-0.1f, 1.0f, -1.0f);
	v2.Set(1.0f, -2.0f, 1.0f);
	result.Set(0.9f, -1.0f, 0.0f);
	v1.Add(v2);
	std::cout << "Vector3D Add: %s\n", (v1 == result) ? PASS : FAIL);

	v1.Set(1.0f, 2.0f, -1.0f);
	v2.Set(1.0f, -2.0f, -1.0f);
	result.Set(0.0f, 4.0f, 0.0f);
	v1.Sub(v2);
	std::cout << "Vector3D Sub: %s\n", (v1 == result) ? PASS : FAIL);

	v1.Set(-2.0f, 2.5f, 1.0f);
	v2.Set(-2.0f, 2.0f, -1.0f);
	result.Set(0.0f, 0.5f, 2.0f);
	v1.Sub(v2);
	std::cout << "Vector3D Sub: %s\n", (v1 == result) ? PASS : FAIL);

	v1.Set(-2.0f, 2.5f, 1.0f);
	scalar = 2.0f;
	result.Set(-4.0f, 5.0f, 2.0f);
	v1.Mul(scalar);
	std::cout << "Vector3D Mul: %s\n", (v1 == result) ? PASS : FAIL);

	v1.Set(-2.0f, 2.5f, 1.0f);
	scalar = 0.5f;
	result.Set(-1.0f, 1.25f, 0.5f);
	v1.Mul(scalar);
	std::cout << "Vector3D Mul: %s\n", (v1 == result) ? PASS : FAIL);

	v1.Set(-2.0f, 10.0f, 4.5f);
	scalar = 2.0f;
	result.Set(-1.0f, 5.0f, 2.25f);
	v1.Div(scalar);
	std::cout << "Vector3D Div: %s\n", (v1 == result) ? PASS : FAIL);

	v1.Set(-2.0f, 10.0f, 1.0f);
	scalar = 0.5f;
	result.Set(-4.0f, 20.0f, 2.0f);
	v1.Div(scalar);
	std::cout << "Vector3D Div: %s\n", (v1 == result) ? PASS : FAIL);

	v1.Set(1.0f, 1.0f, 1.0f);
	result.Set(0.0f, 0.0f, 0.0f);
	v1.Zero();
	std::cout << "Vector3D Zero: %s\n", (v1 == result) ? PASS : FAIL);

	v1.Set(0.0f, 0.0f, 0.0f);
	result.Set(1.0f, 2.0f, 3.0f);
	v1.Set(1.0f, 2.0f, 3.0f);
	std::cout << "Vector3D Set(x, y, z): %s\n", (v1 == result) ? PASS : FAIL);

	v1.Set(0.0f, 0.0f, 0.0f, 0.0f);
	result.Set(1.0f, 2.0f, 3.0f, 1.0f);
	v1.Set(1.0f, 2.0f, 3.0f, 1.0f);
	std::cout << "Vector3D Set(x, y, z, w): %s\n", (v1 == result) ? PASS : FAIL);

	v1.Set(1.0f, 2.0f, -3.0f, 1.0f);
	result.Set(-1.0f, -2.0f, 3.0f, 1.0f);
	v1.Negate();
	std::cout << "Vector3D Negate: %s\n", (v1 == result) ? PASS : FAIL);

	v1.Set(1.0f, 0.0f, -3.0f, 1.0f);
	result.Set(-1.0f, 0.0f, 3.0f, 1.0f);
	v1.Negate();
	std::cout << "Vector3D Negate: %s\n", (v1 == result) ? PASS : FAIL);

	v1.Set(-3.0f, 4.0f, 5.0f);
	std::cout << "Vector3D SquareLength: %s\n", (v1.SquareLength() == 50.0f) ? PASS : FAIL);

	v1.Set(-4.0f, 3.0f, -5.0f);
	std::cout << "Vector3D Length: %s\n", (v1.Length() == 7.0710678f) ? PASS : FAIL);

	v1.Set(-3.0f, 4.0f, 5.0f);
	v2.Set(3.0f, -4.0f, -5.0f);
	std::cout << "Vector3D SquareDistance: %s\n", (Vector3D::SquareDistance(v1, v2) == 200.0f) ? PASS : FAIL);

	v1.Set(-3.0f, 4.0f, 5.0f);
	v2.Set(3.0f, -4.0f, -5.0f);
	std::cout << "Vector3D Distance: %s\n", ((Vector3D::Distance(v1, v2) - 14.1421356f) < EPSILON) ? PASS : FAIL);

	v1.Set(-3.0f, 4.0f, 5.0f);
	result.Set(-0.424264f, 0.565685f, 0.707107f);
	v1.Normalize();
	std::cout << "Vector3D Normalize: %s\n", (v1 == result) ? PASS : FAIL);

	v1.Set(-3.0f, 4.0f, -5.0f);
	v1.Normalize();
	std::cout << "Vector3D Normalize Length Check: %s\n", (v1.Length() == 1.0f) ? PASS : FAIL);

	v1.Set(-3.0f, 4.0f, 5.0f);
	result.Set(-0.424264f, 0.565685f, 0.707107f);
	std::cout << "Vector3D Normalize(vector): %s\n", (Vector3D::Normalize(v1) == result) ? PASS : FAIL);

	v1.Set(-3.0f, 4.0f, -5.0f);
	std::cout << "Vector3D Normalize(vector) Length Check: %s\n", (Vector3D::Normalize(v1).Length() == 1.0f) ? PASS : FAIL);

	v1.Set(2.0f, 3.0f, 3.0f);
	v2.Set(4.0f, 5.0f, 3.0f);
	std::cout << "Vector3D Dot: %s\n", ((Vector3D::Dot(v1, v2) - 32.0f) < EPSILON) ? PASS : FAIL);

	v1.Set(1.0f, 0.0f, 0.0f);
	v2.Set(0.0f, 1.0f, 0.0f);
	result.Set(0.0f, 0.0f, 1.0f);
	std::cout << "Vector3D Cross: %s\n", (Vector3D::Cross(v1, v2) == result) ? PASS : FAIL);

	v1.Set(1.0f, 0.0f, 0.0f);
	v2.Set(0.0f, 0.0f, -1.0f);
	result.Set(0.0f, 1.0f, 0.0f);
	std::cout << "Vector3D Cross: %s\n", (Vector3D::Cross(v1, v2) == result) ? PASS : FAIL);

	v1.Set(0.0f, 1.0f, 0.0f);
	v2.Set(0.0f, 0.0f, 1.0f);
	result.Set(1.0f, 0.0f, 0.0f);
	std::cout << "Vector3D Cross: %s\n", (Vector3D::Cross(v1, v2) == result) ? PASS : FAIL);

#pragma region Rotation Tests

	//result.x = 0.707106f;	result.y = 0.707106f;	result.w = 1.0f;
	//angle = M_PI_4;
	//std::cout << "Vector3D AngleRadians (%f): %s\n", angle, (Vector3D::AngleRadians(angle) == result) ? PASS : FAIL);

	//result.x = 0.0f;	result.y = 1.0f;	result.w = 1.0f;
	//angle = M_PI_2;
	//std::cout << "Vector3D AngleRadians (%f): %s\n", angle, (Vector3D::AngleRadians(angle) == result) ? PASS : FAIL);

	//result.x = -1.0f;	result.y = 0.0f;	result.w = 1.0f;
	//angle = M_PI;
	//std::cout << "Vector3D AngleRadians (%f): %s\n", angle, (Vector3D::AngleRadians(angle) == result) ? PASS : FAIL);

	//result.x = -1.0f;	result.y = 0.0f;	result.w = 1.0f;
	//angle = -M_PI;
	//std::cout << "Vector3D AngleRadians (%f): %s\n", angle, (Vector3D::AngleRadians(angle) == result) ? PASS : FAIL);

	//result.x = 0.0f;	result.y = -1.0f;	result.w = 1.0f;
	//angle = 1.5f * M_PI;
	//std::cout << "Vector3D AngleRadians (%f): %s\n", angle, (Vector3D::AngleRadians(angle) == result) ? PASS : FAIL);

	//result.x = 1.0f;	result.y = 0.0f;	result.w = 1.0f;
	//angle = 2.0f * M_PI;
	//std::cout << "Vector3D AngleRadians (%f): %s\n", angle, (Vector3D::AngleRadians(angle) == result) ? PASS : FAIL);

	//result.x = 1.0f;	result.y = 0.0f;	result.w = 1.0f;
	//angle = 0.0f;
	//std::cout << "Vector3D AngleRadians (%f): %s\n", angle, (Vector3D::AngleRadians(angle) == result) ? PASS : FAIL);

	//result.x = 0.707106f;	result.y = 0.707106f;	result.w = 1.0f;
	//angle = 45.0f;
	//std::cout << "Vector3D AngleDegrees (%f): %s\n", angle, (Vector3D::AngleDegrees(angle) == result) ? PASS : FAIL);

	//result.x = 0.0f;	result.y = 1.0f;	result.w = 1.0f;
	//angle = 90.0f;
	//std::cout << "Vector3D AngleDegrees (%f): %s\n", angle, (Vector3D::AngleDegrees(angle) == result) ? PASS : FAIL);

	//result.x = -1.0f;	result.y = 0.0f;	result.w = 1.0f;
	//angle = 180.0f;
	//std::cout << "Vector3D AngleDegrees (%f): %s\n", angle, (Vector3D::AngleDegrees(angle) == result) ? PASS : FAIL);

	//result.x = -1.0f;	result.y = 0.0f;	result.w = 1.0f;
	//angle = -180.0f;
	//std::cout << "Vector3D AngleDegrees (%f): %s\n", angle, (Vector3D::AngleDegrees(angle) == result) ? PASS : FAIL);

	//result.x = 0.0f;	result.y = -1.0f;	result.w = 1.0f;
	//angle = 270.0f;
	//std::cout << "Vector3D AngleDegrees (%f): %s\n", angle, (Vector3D::AngleDegrees(angle) == result) ? PASS : FAIL);

	//result.x = 1.0f;	result.y = 0.0f;	result.w = 1.0f;
	//angle = 360.0f;
	//std::cout << "Vector3D AngleDegrees (%f): %s\n", angle, (Vector3D::AngleDegrees(angle) == result) ? PASS : FAIL);

	//result.x = 1.0f;	result.y = 0.0f;	result.w = 1.0f;
	//angle = 0.0f;
	//std::cout << "Vector3D AngleDegrees (%f): %s\n", angle, (Vector3D::AngleDegrees(angle) == result) ? PASS : FAIL);
#pragma endregion
}
#endif