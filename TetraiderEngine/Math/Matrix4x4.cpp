#include "Matrix4x4.h"
#include "Matrix3x3.h"
#include "AcrylicorTypedefs.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

Matrix4x4::Matrix4x4() {}

Matrix4x4::Matrix4x4(
	float x1, float y1, float z1, float w1, 
	float x2, float y2, float z2, float w2, 
	float x3, float y3, float z3, float w3, 
	float x4, float y4, float z4, float w4)
{
	m_matrix[0][0] = x1;
	m_matrix[0][1] = y1;
	m_matrix[0][2] = z1;
	m_matrix[0][3] = w1;

	m_matrix[1][0] = x2;
	m_matrix[1][1] = y2;
	m_matrix[1][2] = z2;
	m_matrix[1][3] = w2;

	m_matrix[2][0] = x3;
	m_matrix[2][1] = y3;
	m_matrix[2][2] = z3;
	m_matrix[2][3] = w3;

	m_matrix[3][0] = x4;
	m_matrix[3][1] = y4;
	m_matrix[3][2] = z4;
	m_matrix[3][3] = w4;
}

Matrix4x4::Matrix4x4(const Matrix4x4 &other)
{
	m_matrix[0][0] = other.m_matrix[0][0];
	m_matrix[0][1] = other.m_matrix[0][1];
	m_matrix[0][2] = other.m_matrix[0][2];
	m_matrix[0][3] = other.m_matrix[0][3];

	m_matrix[1][0] = other.m_matrix[1][0];
	m_matrix[1][1] = other.m_matrix[1][1];
	m_matrix[1][2] = other.m_matrix[1][2];
	m_matrix[1][3] = other.m_matrix[1][3];

	m_matrix[2][0] = other.m_matrix[2][0];
	m_matrix[2][1] = other.m_matrix[2][1];
	m_matrix[2][2] = other.m_matrix[2][2];
	m_matrix[2][3] = other.m_matrix[2][3];

	m_matrix[3][0] = other.m_matrix[3][0];
	m_matrix[3][1] = other.m_matrix[3][1];
	m_matrix[3][2] = other.m_matrix[3][2];
	m_matrix[3][3] = other.m_matrix[3][3];
}

Matrix4x4::Matrix4x4(const Matrix3x3 & other)
{
	m_matrix[0][0] = other.Get(0, 0);
	m_matrix[0][1] = other.Get(0, 1);
	m_matrix[0][2] = other.Get(0, 2);
	m_matrix[0][3] = 0.0f;

	m_matrix[1][0] = other.Get(1, 0);
	m_matrix[1][1] = other.Get(1, 1);
	m_matrix[1][2] = other.Get(1, 2);
	m_matrix[1][3] = 0.0f;

	m_matrix[2][0] = other.Get(2, 0);
	m_matrix[2][1] = other.Get(2, 1);
	m_matrix[2][2] = other.Get(2, 2);
	m_matrix[2][3] = 0.0f;

	m_matrix[3][0] = 0.0f;
	m_matrix[3][1] = 0.0f;
	m_matrix[3][2] = 0.0f;
	m_matrix[3][3] = 1.0f;
}

Matrix4x4& Matrix4x4::operator=(const Matrix4x4& other)
{
	m_matrix[0][0] = other.m_matrix[0][0];
	m_matrix[0][1] = other.m_matrix[0][1];
	m_matrix[0][2] = other.m_matrix[0][2];
	m_matrix[0][3] = other.m_matrix[0][3];

	m_matrix[1][0] = other.m_matrix[1][0];
	m_matrix[1][1] = other.m_matrix[1][1];
	m_matrix[1][2] = other.m_matrix[1][2];
	m_matrix[1][3] = other.m_matrix[1][3];

	m_matrix[2][0] = other.m_matrix[2][0];
	m_matrix[2][1] = other.m_matrix[2][1];
	m_matrix[2][2] = other.m_matrix[2][2];
	m_matrix[2][3] = other.m_matrix[2][3];

	m_matrix[3][0] = other.m_matrix[3][0];
	m_matrix[3][1] = other.m_matrix[3][1];
	m_matrix[3][2] = other.m_matrix[3][2];
	m_matrix[3][3] = other.m_matrix[3][3];
	return *this;
}

Matrix4x4::~Matrix4x4() {}

float Matrix4x4::Get(int row, int col) const
{
	if (row < 0 || row > 3 || col < 0 || col > 3)
		throw "Invalid row or column access.";
	return m_matrix[row][col];
}

void Matrix4x4::Set(int row, int col, float val)
{
	if (row < 0 || row > 3 || col < 0 || col > 3)
		throw "Invalid row or column access.";
	m_matrix[row][col] = val;
}

void Matrix4x4::Transpose()
{
	Matrix4x4 transposed = Matrix4x4(
		0.0f,			m_matrix[1][0], m_matrix[2][0], m_matrix[3][0],
		m_matrix[0][1], 0.0f,			m_matrix[2][1], m_matrix[3][1],
		m_matrix[0][2], m_matrix[1][2], 0.0f,			m_matrix[3][2],
		m_matrix[0][3], m_matrix[1][3], m_matrix[2][3], 0.0f
	);

	m_matrix[0][1] = transposed.m_matrix[0][1];
	m_matrix[0][2] = transposed.m_matrix[0][2];
	m_matrix[0][3] = transposed.m_matrix[0][3];

	m_matrix[1][0] = transposed.m_matrix[1][0];
	m_matrix[1][2] = transposed.m_matrix[1][2];
	m_matrix[1][3] = transposed.m_matrix[1][3];

	m_matrix[2][0] = transposed.m_matrix[2][0];
	m_matrix[2][1] = transposed.m_matrix[2][1];
	m_matrix[2][3] = transposed.m_matrix[2][3];

	m_matrix[3][0] = transposed.m_matrix[3][0];
	m_matrix[3][1] = transposed.m_matrix[3][1];
	m_matrix[3][2] = transposed.m_matrix[3][2];
}

void Matrix4x4::Zero()
{
	m_matrix[0][0] = 0.0f;
	m_matrix[0][1] = 0.0f;
	m_matrix[0][2] = 0.0f;
	m_matrix[0][3] = 0.0f;

	m_matrix[1][0] = 0.0f;
	m_matrix[1][1] = 0.0f;
	m_matrix[1][2] = 0.0f;
	m_matrix[1][3] = 0.0f;

	m_matrix[2][0] = 0.0f;
	m_matrix[2][1] = 0.0f;
	m_matrix[2][2] = 0.0f;
	m_matrix[2][3] = 0.0f;

	m_matrix[3][0] = 0.0f;
	m_matrix[3][1] = 0.0f;
	m_matrix[3][2] = 0.0f;
	m_matrix[3][3] = 0.0f;
}

void Matrix4x4::Identity()
{
	m_matrix[0][0] = 1.0f;
	m_matrix[0][1] = 0.0f;
	m_matrix[0][2] = 0.0f;
	m_matrix[0][3] = 0.0f;

	m_matrix[1][0] = 0.0f;
	m_matrix[1][1] = 1.0f;
	m_matrix[1][2] = 0.0f;
	m_matrix[1][3] = 0.0f;

	m_matrix[2][0] = 0.0f;
	m_matrix[2][1] = 0.0f;
	m_matrix[2][2] = 1.0f;
	m_matrix[2][3] = 0.0f;

	m_matrix[3][0] = 0.0f;
	m_matrix[3][1] = 0.0f;
	m_matrix[3][2] = 0.0f;
	m_matrix[3][3] = 1.0f;
}

float Matrix4x4::Determinant() const
{
	float total = 0.0f;
	for (unsigned int headCol = 0; headCol < 4; headCol++) {
		Matrix3x3 x33;
		for (unsigned int currCol = 0; currCol < 4; currCol++) {
			if (currCol == headCol)
				continue;
			x33.Set(0, currCol, m_matrix[1][currCol]);
			x33.Set(1, currCol, m_matrix[2][currCol]);
			x33.Set(2, currCol, m_matrix[3][currCol]);
		}
		total += x33.Determinant();
	}
	return total;
}

void Matrix4x4::Print() const
{
	printf("( %f %f %f %f )\n", m_matrix[0][0], m_matrix[0][1], m_matrix[0][2], m_matrix[0][3]);
	printf("( %f %f %f %f )\n", m_matrix[1][0], m_matrix[1][1], m_matrix[1][2], m_matrix[1][3]);
	printf("( %f %f %f %f )\n", m_matrix[2][0], m_matrix[2][1], m_matrix[2][2], m_matrix[2][3]);
	printf("( %f %f %f %f )\n", m_matrix[3][0], m_matrix[3][1], m_matrix[3][2], m_matrix[3][3]);
	printf("\n");
}

#pragma region Static Methods
Matrix4x4 Matrix4x4::Zero4D()
{
	return Matrix4x4(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
}

Matrix4x4 Matrix4x4::Identity4D()
{
	return Matrix4x4(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

float Matrix4x4::Determinant(const Matrix4x4& other)
{
	float total = 0.0f;
	for (unsigned int headCol = 0; headCol < 4; headCol++) {
		Matrix3x3 x33;
		for (unsigned int currCol = 0; currCol < 4; currCol++) {
			if (currCol == headCol)
				continue;
			x33.Set(0, currCol, other.m_matrix[1][currCol]);
			x33.Set(1, currCol, other.m_matrix[2][currCol]);
			x33.Set(2, currCol, other.m_matrix[3][currCol]);
		}
		total += x33.Determinant();
	}
	return total;
}

Matrix4x4 Matrix4x4::Transpose(const Matrix4x4& other)
{
	return Matrix4x4(
		other.m_matrix[0][0], other.m_matrix[1][0], other.m_matrix[2][0], other.m_matrix[3][0],
		other.m_matrix[0][1], other.m_matrix[1][1], other.m_matrix[2][1], other.m_matrix[3][1],
		other.m_matrix[0][2], other.m_matrix[1][2], other.m_matrix[2][2], other.m_matrix[3][2],
		other.m_matrix[0][3], other.m_matrix[1][3], other.m_matrix[2][3], other.m_matrix[3][3]
	);
}

Matrix4x4 Matrix4x4::Transpose3x3(const Matrix4x4& other)
{
	return Matrix4x4(
		other.m_matrix[0][0], other.m_matrix[1][0], other.m_matrix[2][0], other.m_matrix[0][3],
		other.m_matrix[0][1], other.m_matrix[1][1], other.m_matrix[2][1], other.m_matrix[1][3],
		other.m_matrix[0][2], other.m_matrix[1][2], other.m_matrix[2][2], other.m_matrix[2][3],
		other.m_matrix[3][0], other.m_matrix[3][1], other.m_matrix[3][2], other.m_matrix[3][3]
	);
}

Matrix4x4 Matrix4x4::Inverse(const Matrix4x4& other)
{
	//float idet = 1.0f / other.Determinant();

	//return Matrix4x4(
	//	idet*(other.Get(1, 1) * other.Get(2, 2) - other.Get(2, 1) * other.Get(1, 2)), 
	//	-idet*(other.Get(0, 1) * other.Get(2, 2) - other.Get(2, 1) * other.Get(0, 2)),
	//	idet*(other.Get(0, 1) * other.Get(1, 2) - other.Get(1, 1) * other.Get(0, 2)),
	//	0.0f,
	//	-idet*(other.Get(1, 0) * other.Get(2, 2) - other.Get(2, 0) * other.Get(1, 2)),
	//	idet*(other.Get(0, 0) * other.Get(2, 2) - other.Get(2, 0) * other.Get(0, 2)),
	//	-idet*(other.Get(0, 0) * other.Get(1, 2) - other.Get(1, 0) * other.Get(0, 2)),
	//	0.0f,
	//	idet*(other.Get(1, 0) * other.Get(2, 1) - other.Get(2, 0) * other.Get(1, 1)),
	//	-idet*(other.Get(0, 0) * other.Get(2, 1) - other.Get(2, 0) * other.Get(0, 1)),
	//	idet*(other.Get(0, 0) * other.Get(1, 1) - other.Get(1, 0) * other.Get(0, 1)),
	//	0.0f,
	//	0.0f, 0.0f, 0.0f, 1.0f
	//);

	return Matrix4x4();
}

Matrix4x4 Matrix4x4::Inverse3x3(const Matrix4x4& other)
{
	float idet = 1.0f / (other.Get(0, 0) * (other.Get(1, 1) * other.Get(2, 2) - other.Get(2, 1) * other.Get(1, 2))
		- other.Get(0, 1) * (other.Get(1, 0) * other.Get(2, 2) - other.Get(2, 0) * other.Get(1, 2))
		+ other.Get(0, 2) * (other.Get(1, 0) * other.Get(2, 1) - other.Get(1, 1) * other.Get(2, 0)));

	return Matrix4x4(
		idet*(other.Get(1, 1) * other.Get(2, 2) - other.Get(2, 1) * other.Get(1, 2)),
		-idet*(other.Get(0, 1) * other.Get(2, 2) - other.Get(2, 1) * other.Get(0, 2)),
		idet*(other.Get(0, 1) * other.Get(1, 2) - other.Get(1, 1) * other.Get(0, 2)),
		0.0f,

		-idet*(other.Get(1, 0) * other.Get(2, 2) - other.Get(2, 0) * other.Get(1, 2)),
		idet*(other.Get(0, 0) * other.Get(2, 2) - other.Get(2, 0) * other.Get(0, 2)),
		-idet*(other.Get(0, 0) * other.Get(1, 2) - other.Get(1, 0) * other.Get(0, 2)),
		0.0f,

		idet*(other.Get(1, 0) * other.Get(2, 1) - other.Get(2, 0) * other.Get(1, 1)),
		-idet*(other.Get(0, 0) * other.Get(2, 1) - other.Get(2, 0) * other.Get(0, 1)),
		idet*(other.Get(0, 0) * other.Get(1, 1) - other.Get(1, 0) * other.Get(0, 1)),
		0.0f,

		0.0f, 0.0f, 0.0f, 1.0f
	);
}

Matrix4x4 Matrix4x4::Rotate(const float degrees, const Vector3D& axis)
{
	float x = axis.getX(), y = axis.getY(), z = axis.getZ(),
		rad = degrees * DEG_TO_RAD,
		c = cosf(rad),
		a = (1 - c) / Vector3D::Dot(axis, axis),
		b = sinf(rad) / axis.Length();

	return Matrix4x4(
		c + a*x*x, a*x*y - b*z, a*x*z + b*y, 0.0f,
		a*y*x + b*z, c + a*y*y, a*y*z - b*x, 0.0f,
		a*z*x - b*y, a*z*y + b*x, c + a*z*z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

Matrix4x4 Matrix4x4::Translate(const Vector3D& amt)
{
	return Matrix4x4(
		1.0f, 0.0f, 0.0f, amt.getX(),
		0.0f, 1.0f, 0.0f, amt.getY(),
		0.0f, 0.0f, 1.0f, amt.getZ(),
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

Matrix4x4 Matrix4x4::Scale(const float scalar)
{
	return Matrix4x4(
		scalar, 0.0f, 0.0f, 0.0f,
		0.0f, scalar, 0.0f, 0.0f,
		0.0f, 0.0f, scalar, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

Matrix4x4 Matrix4x4::Scale(const float scaleX, const float scaleY, const float scaleZ)
{
	return Matrix4x4(
		scaleX, 0.0f, 0.0f, 0.0f,
		0.0f, scaleY, 0.0f, 0.0f,
		0.0f, 0.0f, scaleZ, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

Matrix4x4 Matrix4x4::Perspective(const float fov, const float aspect, const float near, const float far)
{
	float xymax = near * tanf(fov * PI / 360.0f);
	float ymin = -xymax;
	float xmin = -xymax;

	float width = xymax - xmin;
	float height = xymax - ymin;
	 
	float depth = near - far;
	float q = (near + far) / depth;
	float qn = (2 * far * near) / depth;

	float w = (2 * near) / width;
	w = w / aspect;
	float h = (2 * near) / height;

	return Matrix4x4(
		w, 0,  0, 0,
		0, h, 0,  0,
		0, 0, q, qn,
		0, 0, -1, 0
	);
}

Matrix4x4 Matrix4x4::Perspective(const float fov, const float aspect, const float near)
{
	return Perspective(fov, aspect, near, 1000.0f);
}

Matrix4x4 Matrix4x4::Orthographic(const int width, const int height, const float near, const float far)
{
	float w = 2.0f / width,
		h = 2.0f / height,
		nf = -2.0f / (far - near),
		tnf = -((far + near) / (far - near));

	return Matrix4x4(
		w, 0, 0, 0,
		0, h, 0, 0,
		0, 0, nf, tnf,
		0, 0, 0, 1
	);
}

Matrix4x4 Matrix4x4::Orthographic(const int width, const int height, const float near)
{
	return Orthographic(width, height, near, 1000.0f);
}
#pragma endregion

#pragma region Operation Overrides
Matrix4x4::operator float*()
{
	return &(m_matrix[0][0]);
}

bool Matrix4x4::operator==(const Matrix4x4& other) const
{
	double d = 0.0f;

	d += fabs(m_matrix[0][0] - other.m_matrix[0][0]);
	d += fabs(m_matrix[0][1] - other.m_matrix[0][1]);
	d += fabs(m_matrix[0][2] - other.m_matrix[0][2]);
	d += fabs(m_matrix[0][3] - other.m_matrix[0][3]);

	d += fabs(m_matrix[1][0] - other.m_matrix[1][0]);
	d += fabs(m_matrix[1][1] - other.m_matrix[1][1]);
	d += fabs(m_matrix[1][2] - other.m_matrix[1][2]);
	d += fabs(m_matrix[1][3] - other.m_matrix[1][3]);

	d += fabs(m_matrix[2][0] - other.m_matrix[2][0]);
	d += fabs(m_matrix[2][1] - other.m_matrix[2][1]);
	d += fabs(m_matrix[2][2] - other.m_matrix[2][2]);
	d += fabs(m_matrix[2][3] - other.m_matrix[2][3]);

	d += fabs(m_matrix[3][0] - other.m_matrix[3][0]);
	d += fabs(m_matrix[3][1] - other.m_matrix[3][1]);
	d += fabs(m_matrix[3][2] - other.m_matrix[3][2]);
	d += fabs(m_matrix[3][3] - other.m_matrix[3][3]);
	return d < EPSILON;
}

bool Matrix4x4::operator!=(const Matrix4x4& other) const
{
	double d = 0.0f;

	d += fabs(m_matrix[0][0] - other.m_matrix[0][0]);
	d += fabs(m_matrix[0][1] - other.m_matrix[0][1]);
	d += fabs(m_matrix[0][2] - other.m_matrix[0][2]);
	d += fabs(m_matrix[0][3] - other.m_matrix[0][3]);

	d += fabs(m_matrix[1][0] - other.m_matrix[1][0]);
	d += fabs(m_matrix[1][1] - other.m_matrix[1][1]);
	d += fabs(m_matrix[1][2] - other.m_matrix[1][2]);
	d += fabs(m_matrix[1][3] - other.m_matrix[1][3]);

	d += fabs(m_matrix[2][0] - other.m_matrix[2][0]);
	d += fabs(m_matrix[2][1] - other.m_matrix[2][1]);
	d += fabs(m_matrix[2][2] - other.m_matrix[2][2]);
	d += fabs(m_matrix[2][3] - other.m_matrix[2][3]);

	d += fabs(m_matrix[3][0] - other.m_matrix[3][0]);
	d += fabs(m_matrix[3][1] - other.m_matrix[3][1]);
	d += fabs(m_matrix[3][2] - other.m_matrix[3][2]);
	d += fabs(m_matrix[3][3] - other.m_matrix[3][3]);
	return d >= EPSILON;
}

Matrix4x4 Matrix4x4::operator+(const Matrix4x4& other) const
{
	return Matrix4x4(
		m_matrix[0][0] + other.m_matrix[0][0], m_matrix[0][1] + other.m_matrix[0][1], m_matrix[0][2] + other.m_matrix[0][2], m_matrix[0][3] + other.m_matrix[0][3],
		m_matrix[1][0] + other.m_matrix[1][0], m_matrix[1][1] + other.m_matrix[1][1], m_matrix[1][2] + other.m_matrix[1][2], m_matrix[1][3] + other.m_matrix[1][3],
		m_matrix[2][0] + other.m_matrix[2][0], m_matrix[2][1] + other.m_matrix[2][1], m_matrix[2][2] + other.m_matrix[2][2], m_matrix[2][3] + other.m_matrix[2][3],
		m_matrix[3][0] + other.m_matrix[3][0], m_matrix[3][1] + other.m_matrix[3][1], m_matrix[3][2] + other.m_matrix[3][2], m_matrix[3][3] + other.m_matrix[3][3]
	);
}

Matrix4x4 Matrix4x4::operator-(const Matrix4x4& other) const
{
	return Matrix4x4(
		m_matrix[0][0] - other.m_matrix[0][0], m_matrix[0][1] - other.m_matrix[0][1], m_matrix[0][2] - other.m_matrix[0][2], m_matrix[0][3] - other.m_matrix[0][3],
		m_matrix[1][0] - other.m_matrix[1][0], m_matrix[1][1] - other.m_matrix[1][1], m_matrix[1][2] - other.m_matrix[1][2], m_matrix[1][3] - other.m_matrix[1][3],
		m_matrix[2][0] - other.m_matrix[2][0], m_matrix[2][1] - other.m_matrix[2][1], m_matrix[2][2] - other.m_matrix[2][2], m_matrix[2][3] - other.m_matrix[2][3],
		m_matrix[3][0] - other.m_matrix[3][0], m_matrix[3][1] - other.m_matrix[3][1], m_matrix[3][2] - other.m_matrix[3][2], m_matrix[3][3] - other.m_matrix[3][3]
	);
}

Matrix4x4 Matrix4x4::operator*(const Matrix4x4& other) const
{
	return Matrix4x4(
		(m_matrix[0][0] * other.m_matrix[0][0]) + (m_matrix[0][1] * other.m_matrix[1][0]) + (m_matrix[0][2] * other.m_matrix[2][0]) + (m_matrix[0][3] * other.m_matrix[3][0]),
		(m_matrix[0][0] * other.m_matrix[0][1]) + (m_matrix[0][1] * other.m_matrix[1][1]) + (m_matrix[0][2] * other.m_matrix[2][1]) + (m_matrix[0][3] * other.m_matrix[3][1]),
		(m_matrix[0][0] * other.m_matrix[0][2]) + (m_matrix[0][1] * other.m_matrix[1][2]) + (m_matrix[0][2] * other.m_matrix[2][2]) + (m_matrix[0][3] * other.m_matrix[3][2]),
		(m_matrix[0][0] * other.m_matrix[0][3]) + (m_matrix[0][1] * other.m_matrix[1][3]) + (m_matrix[0][2] * other.m_matrix[2][3]) + (m_matrix[0][3] * other.m_matrix[3][3]),

		(m_matrix[1][0] * other.m_matrix[0][0]) + (m_matrix[1][1] * other.m_matrix[1][0]) + (m_matrix[1][2] * other.m_matrix[2][0]) + (m_matrix[1][3] * other.m_matrix[3][0]),
		(m_matrix[1][0] * other.m_matrix[0][1]) + (m_matrix[1][1] * other.m_matrix[1][1]) + (m_matrix[1][2] * other.m_matrix[2][1]) + (m_matrix[1][3] * other.m_matrix[3][1]),
		(m_matrix[1][0] * other.m_matrix[0][2]) + (m_matrix[1][1] * other.m_matrix[1][2]) + (m_matrix[1][2] * other.m_matrix[2][2]) + (m_matrix[1][3] * other.m_matrix[3][2]),
		(m_matrix[1][0] * other.m_matrix[0][3]) + (m_matrix[1][1] * other.m_matrix[1][3]) + (m_matrix[1][2] * other.m_matrix[2][3]) + (m_matrix[1][3] * other.m_matrix[3][3]),

		(m_matrix[2][0] * other.m_matrix[0][0]) + (m_matrix[2][1] * other.m_matrix[1][0]) + (m_matrix[2][2] * other.m_matrix[2][0]) + (m_matrix[2][3] * other.m_matrix[3][0]),
		(m_matrix[2][0] * other.m_matrix[0][1]) + (m_matrix[2][1] * other.m_matrix[1][1]) + (m_matrix[2][2] * other.m_matrix[2][1]) + (m_matrix[2][3] * other.m_matrix[3][1]),
		(m_matrix[2][0] * other.m_matrix[0][2]) + (m_matrix[2][1] * other.m_matrix[1][2]) + (m_matrix[2][2] * other.m_matrix[2][2]) + (m_matrix[2][3] * other.m_matrix[3][2]),
		(m_matrix[2][0] * other.m_matrix[0][3]) + (m_matrix[2][1] * other.m_matrix[1][3]) + (m_matrix[2][2] * other.m_matrix[2][3]) + (m_matrix[2][3] * other.m_matrix[3][3]),

		(m_matrix[3][0] * other.m_matrix[0][0]) + (m_matrix[3][1] * other.m_matrix[1][0]) + (m_matrix[3][2] * other.m_matrix[2][0]) + (m_matrix[3][3] * other.m_matrix[3][0]),
		(m_matrix[3][0] * other.m_matrix[0][1]) + (m_matrix[3][1] * other.m_matrix[1][1]) + (m_matrix[3][2] * other.m_matrix[2][1]) + (m_matrix[3][3] * other.m_matrix[3][1]),
		(m_matrix[3][0] * other.m_matrix[0][2]) + (m_matrix[3][1] * other.m_matrix[1][2]) + (m_matrix[3][2] * other.m_matrix[2][2]) + (m_matrix[3][3] * other.m_matrix[3][2]),
		(m_matrix[3][0] * other.m_matrix[0][3]) + (m_matrix[3][1] * other.m_matrix[1][3]) + (m_matrix[3][2] * other.m_matrix[2][3]) + (m_matrix[3][3] * other.m_matrix[3][3])
	);
}

Matrix4x4 Matrix4x4::operator*(const float scalar) const
{
	return Matrix4x4(
		m_matrix[0][0] * scalar, m_matrix[0][1] * scalar, m_matrix[0][2] * scalar, m_matrix[0][3] * scalar,
		m_matrix[1][0] * scalar, m_matrix[1][1] * scalar, m_matrix[1][2] * scalar, m_matrix[1][3] * scalar,
		m_matrix[2][0] * scalar, m_matrix[2][1] * scalar, m_matrix[2][2] * scalar, m_matrix[2][3] * scalar,
		m_matrix[3][0] * scalar, m_matrix[3][1] * scalar, m_matrix[3][2] * scalar, m_matrix[3][3] * scalar
	);
}

Matrix4x4 Matrix4x4::operator/(const float divisor) const
{
	if (divisor == 0.0f)
		throw "Division by zero error!";

	return Matrix4x4(
		m_matrix[0][0] / divisor, m_matrix[0][1] / divisor, m_matrix[0][2] / divisor, m_matrix[0][3] / divisor,
		m_matrix[1][0] / divisor, m_matrix[1][1] / divisor, m_matrix[1][2] / divisor, m_matrix[1][3] / divisor,
		m_matrix[2][0] / divisor, m_matrix[2][1] / divisor, m_matrix[2][2] / divisor, m_matrix[2][3] / divisor,
		m_matrix[3][0] / divisor, m_matrix[3][1] / divisor, m_matrix[3][2] / divisor, m_matrix[3][3] / divisor
	);
}


Vector3D Matrix4x4::operator*(const Vector3D& other) const
{
	return Vector3D(
		m_matrix[0][0] * other.getX() + m_matrix[0][1] * other.getY() + m_matrix[0][2] * other.getZ() + m_matrix[0][3] * other.getW(),
		m_matrix[1][0] * other.getX() + m_matrix[1][1] * other.getY() + m_matrix[1][2] * other.getZ() + m_matrix[1][3] * other.getW(),
		m_matrix[2][0] * other.getX() + m_matrix[2][1] * other.getY() + m_matrix[2][2] * other.getZ() + m_matrix[2][3] * other.getW(),
		m_matrix[3][0] * other.getX() + m_matrix[3][1] * other.getY() + m_matrix[3][2] * other.getZ() + m_matrix[3][3] * other.getW()
	);
}
#pragma endregion

#if TEST_MODE
void Matrix4x4Tests()
{
	printf("\n========== Running Matrix3x3 tests ==========\n\n");

#pragma region Methods
	Matrix4x4 m0 = Matrix4x4();
	Matrix4x4 m1 = Matrix4x4();
	long i, j;

	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			m0.Set(i, j, float(i * 2 + j));
			m1.Set(i, j, float(i + j * 2));
		}
	}

	m1.Transpose();
	printf("Matrix4x4 Transpose: %s\n", (m0 == m1) ? PASS : FAIL);

	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			m0.Set(i, j, 0.0f);
	m1.Zero();
	printf("Matrix4x4 Zero: %s\n", (m0 == m1) ? PASS : FAIL);

	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			m0.Set(i, j, i == j ? 1.0f : 0.0f);

	m1.Identity();
	printf("Matrix4x4 Identity: %s\n", (m0 == m1) ? PASS : FAIL);
#pragma endregion
#pragma region StaticMethods
	m0.Zero();

	Matrix4x4 zero = Matrix4x4::Zero4D();
	printf("Matrix4x4::Zero4D: %s\n", (zero == m0) ? PASS : FAIL);

	m0.Identity();

	Matrix4x4 identity = Matrix4x4::Identity4D();
	printf("Matrix4x4::Identity4D: %s\n", (m0 == identity) ? PASS : FAIL);
#pragma endregion
#pragma region Operations

	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			m0.Set(i, j, i + j);
			m1.Set(i, j, i - j);
		}
	}
	printf("Matrix4x4 == Matrix4x4 equal: %s\n", (m0 == m0) ? PASS : FAIL);

	printf("Matrix4x4 == Matrix4x4 not equal: %s\n", (m0 == m1) ? FAIL : PASS);

	printf("Matrix4x4 != Matrix4x4 not equal: %s\n", (m0 != m1) ? PASS : FAIL);

	printf("Matrix4x4 != Matrix4x4 equal: %s\n", (m0 != m0) ? FAIL : PASS);

	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			m0.Set(i, j, i + j);
			m1.Set(i, j, -(i + j));
		}
	}
	printf("Matrix4x4 + Matrix4x4: %s\n", (zero == m0 + m1) ? PASS : FAIL);

	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			m0.Set(i, j, i + j);
			m1.Set(i, j, i + j);
		}
	}
	printf("Matrix4x4 - Matrix4x4: %s\n", (zero == m0 - m1) ? PASS : FAIL);

	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			m0.Set(i, j, i + j);
			m1.Set(i, j, i + j);
		}
	}
	Matrix4x4 result = Matrix4x4(
		14.0f, 20.0f, 26.0f, 32.0f,
		20.0f, 30.0f, 40.0f, 50.0f,
		26.0f, 40.0f, 54.0f, 68.0f,
		32.0f, 50.0f, 68.0f, 86.0f
	);
	printf("Matrix4x4 * Matrix4x4: %s\n", (m0 * m1 == result) ? PASS : FAIL);

	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			m0.Set(i, j, i + j);
			m1.Set(i, j, (i + j) * 2.0f);
		}
	}
	printf("Matrix4x4 * scalar: %s\n", ((m0 * 2.0f) == m1) ? PASS : FAIL);

	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			m0.Set(i, j, i + j);
			m1.Set(i, j, (i + j) / 2.0f);
		}
	}
	printf("Matrix4x4 / divisor: %s\n", ((m0 / 2.0f) == m1) ? PASS : FAIL);
#pragma endregion
#pragma region Vector3D
	printf("\n========== Running Matrix4x4 - Vector3D tests ==========\n\n");
	Matrix4x4 x44 = Matrix4x4(
		1.0f, 2.0f, 3.0f, 4.0f, 
		5.0f, 6.0f, 7.0f, 8.0f, 
		9.0f, 8.0f, 7.0f, 6.0f,
		5.0f, 4.0f, 3.0f, 2.0f);
	Vector3D v3 = Vector3D(1.0f, 2.0f, 3.0f, 1.0f);
	Vector3D v3Test = Vector3D(42.0f, 94.0f, 88.0f, 36.0f);
	printf("Matrix4x4 * Vector3D: %s\n", (v3Test == (x44 * v3)) ? PASS : FAIL);
#pragma endregion
}
#endif