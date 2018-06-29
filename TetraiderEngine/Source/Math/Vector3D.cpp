/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

Vector3D::Vector3D() : x(0.f), y(0.f), z(0.f), w(1.f) { }

Vector3D::Vector3D(const aiVector3t<ai_real> & v) : x(v.x), y(v.y), z(v.z), w(1.f) { }

Vector3D::Vector3D(const aiVector3t<ai_real>& v, const float & _w) : x(v.x), y(v.y), z(v.z), w(_w) { }

Vector3D::Vector3D(const Vector3D & vec, const float & _w) : x(vec.x), y(vec.y), z(vec.z), w(_w) { }

Vector3D::Vector3D(const float& _x, const float& _y, const float& _z) : x(_x), y(_y), z(_z), w(1.f) { }

Vector3D::Vector3D(const float& _x, const float& _y, const float& _z, const float& _w) : x(_x), y(_y), z(_z), w(_w) { }

Vector3D::Vector3D(const Vector3D & rhs) : x(rhs.x), y(rhs.y), z(rhs.z), w(rhs.w) { }

Vector3D::Vector3D(const Vector2D & rhs) : x(rhs.x), y(rhs.y), z(0.f), w(rhs.w) { }

Vector3D& Vector3D::operator=(const Vector3D& rhs)
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

bool Vector3D::IsVectorZero(float error) const {
	return (fabsf(x) < error && fabsf(y) < error && fabsf(z) < error);
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
	if (x == 0 && y == 0 && z == 0)
		return;
	Div(Length());
}

float Vector3D::AngleRadians() const {
	return atan2f(y, x);
}

float Vector3D::AngleDegrees() const {
	return AngleRadians() * RAD_TO_DEG;
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
		vector0.x * vector1.y - vector0.y * vector1.x,
		0.f
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

Vector3D Vector3D::VectorFromAngleDegrees(float degrees)
{
	float rad = degrees*DEG_TO_RAD;
	return Vector3D(cosf(rad), sinf(rad), 0);
}

#pragma endregion

Vector3D operator*(const float scalar, const Vector3D& other)
{
	return Vector3D(
		other.x * scalar,
		other.y * scalar,
		other.z * scalar,
		other.w * scalar
	);
}

std::ostream & operator<<(std::ostream & out, const Vector3D & vec)
{
	out << "X: " << vec.x << ", Y: " << vec.y << ", Z: " << vec.z;
	return out;
}

#if TEST_MODE
void Vector3DTests()
{
	std::cout << "\n========== Running Vector3D tests ==========\n\n";

	Vector3D * createdVector = new Vector3D();
	std::cout << "Created Vector(): " << ((createdVector->x == 0.0f && createdVector->y == 0.0f && createdVector->w == 1.0f) ? PASS : FAIL) << std::endl;
	delete createdVector;

	createdVector = new Vector3D(1.0f, 2.0f, 3.0f);
	std::cout << "Created Vector(x, y): " << ((createdVector->x == 1.0f && createdVector->y == 2.0f && createdVector->z == 3.0f && createdVector->w == 1.0f) ? PASS : FAIL) << std::endl;
	delete createdVector;

	createdVector = new Vector3D(2.0f, 3.0f, 4.0f, 0.0f);
	std::cout << "Created Vector(x, y, w): " << ((createdVector->x == 2.0f && createdVector->y == 3.0f && createdVector->z == 4.0f && createdVector->w == 0.0f) ? PASS : FAIL) << std::endl;
	delete createdVector;

	Vector3D v1, v2, v3, result;
	float scalar;

	v1.Set(1.0f, 2.0f, 3.0f);
	v2.Set(1.0f, 2.0f, 3.0f);
	std::cout << "Vector3D == equal: " << ((v1 == v2) ? PASS : FAIL) << std::endl;

	v2.x = 1.1f;
	std::cout << "Vector3D == not equal: " << ((v1 == v2) ? FAIL : PASS);

	v2.Set(2.0f, 3.0f, 4.0f);
	std::cout << "Vector3D != not equal: " << ((v1 != v2) ? PASS : FAIL) << std::endl;

	v2.Set(1.0f, 2.0f, 3.0f);
	std::cout << "Vector3D != equal: " << ((v1 != v2) ? FAIL : PASS);

	v1.Set(0.1f, 2.0f, 1.0f);
	v2.Set(1.0f, 3.5f, -3.0f);
	result.Set(1.1f, 5.5f, -2.0f);
	std::cout << "Vector3D +: " << ((v1 + v2 == result) ? PASS : FAIL) << std::endl;

	v1.Set(-0.1f, 1.0f, -1.0f);
	v2.Set(1.0f, -2.0f, 1.0f);
	result.Set(0.9f, -1.0f, 0.0f);
	std::cout << "Vector3D +: " << ((v1 + v2 == result) ? PASS : FAIL) << std::endl;

	v1.Set(1.0f, 2.0f, -1.0f);
	v2.Set(1.0f, -2.0f, -1.0f);
	result.Set(0.0f, 4.0f, 0.0f);
	std::cout << "Vector3D -: " << ((v1 - v2 == result) ? PASS : FAIL) << std::endl;

	v1.Set(-2.0f, 2.5f, 1.0f);
	v2.Set(-2.0f, 2.0f, -1.0f);
	result.Set(0.0f, 0.5f, 2.0f);
	std::cout << "Vector3D -: " << ((v1 - v2 == result) ? PASS : FAIL) << std::endl;

	v1.Set(-2.0f, 2.5f, 1.0f);
	scalar = 2.0f;
	result.Set(-4.0f, 5.0f, 2.0f);
	std::cout << "Vector3D *: " << ((v1 * scalar == result) ? PASS : FAIL) << std::endl;

	v1.Set(-2.0f, 2.5f, 1.0f);
	scalar = 0.5f;
	result.Set(-1.0f, 1.25f, 0.5f);
	std::cout << "Vector3D *: " << ((v1 * scalar == result) ? PASS : FAIL) << std::endl;

	v1.Set(-2.0f, 10.0f, 4.5f);
	scalar = 2.0f;
	result.Set(-1.0f, 5.0f, 2.25f);
	std::cout << "Vector3D /: " << ((v1 / scalar == result) ? PASS : FAIL) << std::endl;

	v1.Set(-2.0f, 10.0f, 1.0f);
	scalar = 0.5f;
	result.Set(-4.0f, 20.0f, 2.0f);
	std::cout << "Vector3D /: " << ((v1 / scalar == result) ? PASS : FAIL) << std::endl;

	v1.Set(0.1f, 2.0f, 1.0f);
	v2.Set(1.0f, 3.5f, -3.0f);
	result.Set(1.1f, 5.5f, -2.0f);
	v1.Add(v2);
	std::cout << "Vector3D Add: " << ((v1 == result) ? PASS : FAIL) << std::endl;

	v1.Set(-0.1f, 1.0f, -1.0f);
	v2.Set(1.0f, -2.0f, 1.0f);
	result.Set(0.9f, -1.0f, 0.0f);
	v1.Add(v2);
	std::cout << "Vector3D Add: " << ((v1 == result) ? PASS : FAIL) << std::endl;

	v1.Set(1.0f, 2.0f, -1.0f);
	v2.Set(1.0f, -2.0f, -1.0f);
	result.Set(0.0f, 4.0f, 0.0f);
	v1.Sub(v2);
	std::cout << "Vector3D Sub: " << ((v1 == result) ? PASS : FAIL) << std::endl;

	v1.Set(-2.0f, 2.5f, 1.0f);
	v2.Set(-2.0f, 2.0f, -1.0f);
	result.Set(0.0f, 0.5f, 2.0f);
	v1.Sub(v2);
	std::cout << "Vector3D Sub: " << ((v1 == result) ? PASS : FAIL) << std::endl;

	v1.Set(-2.0f, 2.5f, 1.0f);
	scalar = 2.0f;
	result.Set(-4.0f, 5.0f, 2.0f);
	v1.Mul(scalar);
	std::cout << "Vector3D Mul: " << ((v1 == result) ? PASS : FAIL) << std::endl;

	v1.Set(-2.0f, 2.5f, 1.0f);
	scalar = 0.5f;
	result.Set(-1.0f, 1.25f, 0.5f);
	v1.Mul(scalar);
	std::cout << "Vector3D Mul: " << ((v1 == result) ? PASS : FAIL) << std::endl;

	v1.Set(-2.0f, 10.0f, 4.5f);
	scalar = 2.0f;
	result.Set(-1.0f, 5.0f, 2.25f);
	v1.Div(scalar);
	std::cout << "Vector3D Div: " << ((v1 == result) ? PASS : FAIL) << std::endl;

	v1.Set(-2.0f, 10.0f, 1.0f);
	scalar = 0.5f;
	result.Set(-4.0f, 20.0f, 2.0f);
	v1.Div(scalar);
	std::cout << "Vector3D Div: " << ((v1 == result) ? PASS : FAIL) << std::endl;

	v1.Set(1.0f, 1.0f, 1.0f);
	result.Set(0.0f, 0.0f, 0.0f);
	v1.Zero();
	std::cout << "Vector3D Zero: " << ((v1 == result) ? PASS : FAIL) << std::endl;

	v1.Set(0.0f, 0.0f, 0.0f);
	result.Set(1.0f, 2.0f, 3.0f);
	v1.Set(1.0f, 2.0f, 3.0f);
	std::cout << "Vector3D Set(x, y, z): " << ((v1 == result) ? PASS : FAIL) << std::endl;

	v1.Set(0.0f, 0.0f, 0.0f, 0.0f);
	result.Set(1.0f, 2.0f, 3.0f, 1.0f);
	v1.Set(1.0f, 2.0f, 3.0f, 1.0f);
	std::cout << "Vector3D Set(x, y, z, w): " << ((v1 == result) ? PASS : FAIL) << std::endl;

	v1.Set(1.0f, 2.0f, -3.0f, 1.0f);
	result.Set(-1.0f, -2.0f, 3.0f, 1.0f);
	v1.Negate();
	std::cout << "Vector3D Negate: " << ((v1 == result) ? PASS : FAIL) << std::endl;

	v1.Set(1.0f, 0.0f, -3.0f, 1.0f);
	result.Set(-1.0f, 0.0f, 3.0f, 1.0f);
	v1.Negate();
	std::cout << "Vector3D Negate: " << ((v1 == result) ? PASS : FAIL) << std::endl;

	v1.Set(-3.0f, 4.0f, 5.0f);
	std::cout << "Vector3D SquareLength: " << ((v1.SquareLength() == 50.0f) ? PASS : FAIL) << std::endl;

	v1.Set(-4.0f, 3.0f, -5.0f);
	std::cout << "Vector3D Length: " << ((v1.Length() == 7.0710678f) ? PASS : FAIL) << std::endl;

	v1.Set(-3.0f, 4.0f, 5.0f);
	v2.Set(3.0f, -4.0f, -5.0f);
	std::cout << "Vector3D SquareDistance: " << ((Vector3D::SquareDistance(v1, v2) == 200.0f) ? PASS : FAIL) << std::endl;

	v1.Set(-3.0f, 4.0f, 5.0f);
	v2.Set(3.0f, -4.0f, -5.0f);
	std::cout << "Vector3D Distance: " << (((Vector3D::Distance(v1, v2) - 14.1421356f) < EPSILON) ? PASS : FAIL) << std::endl;

	v1.Set(-3.0f, 4.0f, 5.0f);
	result.Set(-0.424264f, 0.565685f, 0.707107f);
	v1.Normalize();
	std::cout << "Vector3D Normalize: " << ((v1 == result) ? PASS : FAIL) << std::endl;

	v1.Set(-3.0f, 4.0f, -5.0f);
	v1.Normalize();
	std::cout << "Vector3D Normalize Length Check: " << ((v1.Length() == 1.0f) ? PASS : FAIL) << std::endl;
	
	v1.Set(-3.0f, 4.0f, 5.0f);
	result.Set(-0.424264f, 0.565685f, 0.707107f);
	std::cout << "Vector3D Normalize(vector): " << ((Vector3D::Normalize(v1) == result) ? PASS : FAIL) << std::endl;

	v1.Set(-3.0f, 4.0f, -5.0f);
	std::cout << "Vector3D Normalize(vector) Length Check: " << ((Vector3D::Normalize(v1).Length() == 1.0f) ? PASS : FAIL) << std::endl;

	v1.Set(2.0f, 3.0f, 3.0f);
	v2.Set(4.0f, 5.0f, 3.0f);
	std::cout << "Vector3D Dot: " << (((Vector3D::Dot(v1, v2) - 32.0f) < EPSILON) ? PASS : FAIL) << std::endl;

	v1.Set(1.0f, 0.0f, 0.0f);
	v2.Set(0.0f, 1.0f, 0.0f);
	result.Set(0.0f, 0.0f, 1.0f);
	std::cout << "Vector3D Cross: " << ((Vector3D::Cross(v1, v2) == result) ? PASS : FAIL) << std::endl;

	v1.Set(1.0f, 0.0f, 0.0f);
	v2.Set(0.0f, 0.0f, -1.0f);
	result.Set(0.0f, 1.0f, 0.0f);
	std::cout << "Vector3D Cross: " << ((Vector3D::Cross(v1, v2) == result) ? PASS : FAIL) << std::endl;

	v1.Set(0.0f, 1.0f, 0.0f);
	v2.Set(0.0f, 0.0f, 1.0f);
	result.Set(1.0f, 0.0f, 0.0f);
	std::cout << "Vector3D Cross: " << ((Vector3D::Cross(v1, v2) == result) ? PASS : FAIL) << std::endl;

#pragma region Rotation Tests

	//result.x = 0.707106f;	result.y = 0.707106f;	result.w = 1.0f;
	//angle = M_PI_4;
	//std::cout << "Vector3D AngleRadians (%f): %s\n", angle, (Vector3D::AngleRadians(angle) == result) ? PASS : FAIL) << std::endl;

	//result.x = 0.0f;	result.y = 1.0f;	result.w = 1.0f;
	//angle = M_PI_2;
	//std::cout << "Vector3D AngleRadians (%f): %s\n", angle, (Vector3D::AngleRadians(angle) == result) ? PASS : FAIL) << std::endl;

	//result.x = -1.0f;	result.y = 0.0f;	result.w = 1.0f;
	//angle = M_PI;
	//std::cout << "Vector3D AngleRadians (%f): %s\n", angle, (Vector3D::AngleRadians(angle) == result) ? PASS : FAIL) << std::endl;

	//result.x = -1.0f;	result.y = 0.0f;	result.w = 1.0f;
	//angle = -M_PI;
	//std::cout << "Vector3D AngleRadians (%f): %s\n", angle, (Vector3D::AngleRadians(angle) == result) ? PASS : FAIL) << std::endl;

	//result.x = 0.0f;	result.y = -1.0f;	result.w = 1.0f;
	//angle = 1.5f * M_PI;
	//std::cout << "Vector3D AngleRadians (%f): %s\n", angle, (Vector3D::AngleRadians(angle) == result) ? PASS : FAIL) << std::endl;

	//result.x = 1.0f;	result.y = 0.0f;	result.w = 1.0f;
	//angle = 2.0f * M_PI;
	//std::cout << "Vector3D AngleRadians (%f): %s\n", angle, (Vector3D::AngleRadians(angle) == result) ? PASS : FAIL) << std::endl;

	//result.x = 1.0f;	result.y = 0.0f;	result.w = 1.0f;
	//angle = 0.0f;
	//std::cout << "Vector3D AngleRadians (%f): %s\n", angle, (Vector3D::AngleRadians(angle) == result) ? PASS : FAIL) << std::endl;

	//result.x = 0.707106f;	result.y = 0.707106f;	result.w = 1.0f;
	//angle = 45.0f;
	//std::cout << "Vector3D AngleDegrees (%f): %s\n", angle, (Vector3D::AngleDegrees(angle) == result) ? PASS : FAIL) << std::endl;

	//result.x = 0.0f;	result.y = 1.0f;	result.w = 1.0f;
	//angle = 90.0f;
	//std::cout << "Vector3D AngleDegrees (%f): %s\n", angle, (Vector3D::AngleDegrees(angle) == result) ? PASS : FAIL) << std::endl;

	//result.x = -1.0f;	result.y = 0.0f;	result.w = 1.0f;
	//angle = 180.0f;
	//std::cout << "Vector3D AngleDegrees (%f): %s\n", angle, (Vector3D::AngleDegrees(angle) == result) ? PASS : FAIL) << std::endl;

	//result.x = -1.0f;	result.y = 0.0f;	result.w = 1.0f;
	//angle = -180.0f;
	//std::cout << "Vector3D AngleDegrees (%f): %s\n", angle, (Vector3D::AngleDegrees(angle) == result) ? PASS : FAIL) << std::endl;

	//result.x = 0.0f;	result.y = -1.0f;	result.w = 1.0f;
	//angle = 270.0f;
	//std::cout << "Vector3D AngleDegrees (%f): %s\n", angle, (Vector3D::AngleDegrees(angle) == result) ? PASS : FAIL) << std::endl;

	//result.x = 1.0f;	result.y = 0.0f;	result.w = 1.0f;
	//angle = 360.0f;
	//std::cout << "Vector3D AngleDegrees (%f): %s\n", angle, (Vector3D::AngleDegrees(angle) == result) ? PASS : FAIL) << std::endl;

	//result.x = 1.0f;	result.y = 0.0f;	result.w = 1.0f;
	//angle = 0.0f;
	//std::cout << "Vector3D AngleDegrees (%f): %s\n", angle, (Vector3D::AngleDegrees(angle) == result) ? PASS : FAIL) << std::endl;
#pragma endregion
}
#endif