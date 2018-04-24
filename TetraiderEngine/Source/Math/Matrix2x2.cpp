/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

Matrix2x2::Matrix2x2() {}

Matrix2x2::Matrix2x2(float x1, float y1, float x2, float y2) 
{
	m_matrix[0][0] = x1;
	m_matrix[0][1] = y1;
	m_matrix[1][0] = x2;
	m_matrix[1][1] = y2;
}

Matrix2x2::Matrix2x2(const Matrix2x2 &other)
{
	m_matrix[0][0] = other.m_matrix[0][0];
	m_matrix[0][1] = other.m_matrix[0][1];
	m_matrix[1][0] = other.m_matrix[1][0];
	m_matrix[1][1] = other.m_matrix[1][1];
}

Matrix2x2& Matrix2x2::operator=(const Matrix2x2& other)
{
	m_matrix[0][0] = other.m_matrix[0][0];
	m_matrix[0][1] = other.m_matrix[0][1];
	m_matrix[1][0] = other.m_matrix[1][0];
	m_matrix[1][1] = other.m_matrix[1][1];
	return *this;
}

Matrix2x2::~Matrix2x2(){}

float Matrix2x2::Get(int row, int col) const
{
	return m_matrix[row][col];
}

void Matrix2x2::Set(int row, int col, float val)
{
	m_matrix[row][col] = val;
}

void Matrix2x2::Transpose()
{
	float tmp = m_matrix[1][0];

	m_matrix[1][0] = m_matrix[0][1];
	m_matrix[0][1] = tmp;
}

void Matrix2x2::Zero()
{
	m_matrix[0][0] = 0.0f;
	m_matrix[0][1] = 0.0f;
	m_matrix[1][0] = 0.0f;
	m_matrix[1][1] = 0.0f;
}

void Matrix2x2::Identity()
{
	m_matrix[0][0] = 1.0f;
	m_matrix[0][1] = 0.0f;
	m_matrix[1][0] = 0.0f;
	m_matrix[1][1] = 1.0f;
}

float Matrix2x2::Determinant() const
{
	return m_matrix[0][0] * m_matrix[1][1] - m_matrix[0][1] * m_matrix[1][0];
}

void Matrix2x2::Print() const
{
	std::cout << "[ " << m_matrix[0][0] << ", " << m_matrix[0][1] << "]" << std::endl;
	std::cout << "[ " << m_matrix[1][0] << ", " << m_matrix[1][1] << "]" << std::endl;
}

#pragma region Static Methods
Matrix2x2 Matrix2x2::Zero2D()
{
	return Matrix2x2(0.0f, 0.0f, 0.0f, 0.0f);
}

Matrix2x2 Matrix2x2::Identity2D()
{
	return Matrix2x2(
		1.0f, 0.0f, 
		0.0f, 1.0f
	);
}

float Matrix2x2::Determinant(const Matrix2x2& other)
{
	return other.m_matrix[0][0] * other.m_matrix[1][1] - other.m_matrix[0][1] * other.m_matrix[1][0];
}
#pragma endregion

#pragma region Operator Overrides
bool Matrix2x2::operator==(const Matrix2x2& other)
{
	double d = 0.0f;

	d += fabs(m_matrix[0][0] - other.m_matrix[0][0]);
	d += fabs(m_matrix[0][1] - other.m_matrix[0][1]);

	d += fabs(m_matrix[1][0] - other.m_matrix[1][0]);
	d += fabs(m_matrix[1][1] - other.m_matrix[1][1]);
	return d < EPSILON;
}

bool Matrix2x2::operator!=(const Matrix2x2& other)
{
	double d = 0.0f;

	d += fabs(m_matrix[0][0] - other.m_matrix[0][0]);
	d += fabs(m_matrix[0][1] - other.m_matrix[0][1]);

	d += fabs(m_matrix[1][0] - other.m_matrix[1][0]);
	d += fabs(m_matrix[1][1] - other.m_matrix[1][1]);
	return d >= EPSILON;
}

Matrix2x2 Matrix2x2::operator+(const Matrix2x2& other)
{
	return Matrix2x2(
		m_matrix[0][0] + other.m_matrix[0][0], m_matrix[0][1] + other.m_matrix[0][1],
		m_matrix[1][0] + other.m_matrix[1][0], m_matrix[1][1] + other.m_matrix[1][1]
	);
}

Matrix2x2 Matrix2x2::operator-(const Matrix2x2& other)
{
	return Matrix2x2(
		m_matrix[0][0] - other.m_matrix[0][0], m_matrix[0][1] - other.m_matrix[0][1],
		m_matrix[1][0] - other.m_matrix[1][0], m_matrix[1][1] - other.m_matrix[1][1]
	);
}

Matrix2x2 Matrix2x2::operator*(const Matrix2x2& other)
{
	return Matrix2x2(
		(m_matrix[0][0] * other.m_matrix[0][0]) + (m_matrix[0][1] * other.m_matrix[1][0]),
		(m_matrix[0][0] * other.m_matrix[0][1]) + (m_matrix[0][1] * other.m_matrix[1][1]),

		(m_matrix[1][0] * other.m_matrix[0][0]) + (m_matrix[1][1] * other.m_matrix[1][0]),
		(m_matrix[1][0] * other.m_matrix[0][1]) + (m_matrix[1][1] * other.m_matrix[1][1])
	);
}

Matrix2x2 Matrix2x2::operator*(const float scalar)
{
	return Matrix2x2(
		m_matrix[0][0] * scalar, m_matrix[0][1] * scalar,
		m_matrix[1][0] * scalar, m_matrix[1][1] * scalar
	);
}

Matrix2x2 Matrix2x2::operator/(const float divisor)
{
	return Matrix2x2(
		m_matrix[0][0] / divisor, m_matrix[0][1] / divisor,
		m_matrix[1][0] / divisor, m_matrix[1][1] / divisor
	);
}

/* Vector2D operations */
Vector2D Matrix2x2::operator*(const Vector2D& other)
{
	return Vector2D(
		m_matrix[0][0] * other.x + m_matrix[0][1] * other.y,
		m_matrix[1][0] * other.x + m_matrix[1][1] * other.y
	);
}
#pragma endregion

#if TEST_MODE
void Matrix2x2Tests()
{
	std::cout << "\n========== Running Matrix2x2 tests ==========\n\n";

#pragma region Methods
	Matrix2x2 m0 = Matrix2x2();
	Matrix2x2 m1 = Matrix2x2();
	long i, j;

	for (i = 0; i < 2; i++)
		for (j = 0; j < 2; j++)
			m0.Set(i, j, float(i * 2 + j));

	for (i = 0; i < 2; i++)
		for (j = 0; j < 2; j++)
			m1.Set(i, j, float(i + j * 2));

	m1.Transpose();
	std::cout << "Matrix2x2 Transpose: " << ((m0 == m1) ? PASS : FAIL) << std::endl;

	for (i = 0; i < 2; i++)
		for (j = 0; j < 2; j++)
			m0.Set(i, j, 0.0f);

	m1.Zero();
	std::cout << "Matrix2x2 Zero: " << ((m0 == m1) ? PASS : FAIL) << std::endl;

	for (i = 0; i < 2; i++)
		for (j = 0; j < 2; j++)
			m0.Set(i, j, i == j ? 1.0f : 0.0f);

	m1.Identity();
	std::cout << "Matrix2x2 Identity: " << ((m0 == m1) ? PASS : FAIL) << std::endl;
#pragma endregion Methods
#pragma region StaticMethods
	for (i = 0; i < 2; i++)
		for (j = 0; j < 2; j++)
			m0.Set(i, j, 0.0f);

	Matrix2x2 zero = Matrix2x2::Zero2D();
	std::cout << "Matrix2x2::Zero: " << ((zero == m0) ? PASS : FAIL) << std::endl;

	for (i = 0; i < 2; i++)
		for (j = 0; j < 2; j++)
			m0.Set(i, j, i == j ? 1.0f : 0.0f);

	Matrix2x2 identity = Matrix2x2::Identity2D();
	std::cout << "Matrix2x2::Identity: " << ((m0 == identity) ? PASS : FAIL) << std::endl;
#pragma endregion StaticMethods
#pragma region Operations
	for (i = 0; i < 2; i++)
		for (j = 0; j < 2; j++)
			m0.Set(i, j, float(i + j));

	for (i = 0; i < 2; i++)
		for (j = 0; j < 2; j++)
			m1.Set(i, j, float(i - j));
	std::cout << "Matrix2x2 == Matrix2x2 equal: " << ((m0 == m0) ? PASS : FAIL) << std::endl;

	std::cout << "Matrix2x2 == Matrix2x2 not equal: " << ((m0 == m1) ? FAIL : PASS);

	std::cout << "Matrix2x2 != Matrix2x2 not equal: " << ((m0 != m1) ? PASS : FAIL) << std::endl;

	std::cout << "Matrix2x2 != Matrix2x2 equal: " << ((m0 != m0) ? FAIL : PASS);

	for (i = 0; i < 2; i++)
		for (j = 0; j < 2; j++)
			m0.Set(i, j, float(i + j));

	for (i = 0; i < 2; i++)
		for (j = 0; j < 2; j++)
			m1.Set(i, j, float(-(i + j)));
	std::cout << "Matrix2x2 + Matrix2x2: " << ((zero == m0 + m1) ? PASS : FAIL) << std::endl;

	for (i = 0; i < 2; i++)
		for (j = 0; j < 2; j++)
			m0.Set(i, j, float(i + j));
	for (i = 0; i < 2; i++)
		for (j = 0; j < 2; j++)
			m1.Set(i, j, float(i + j));
	std::cout << "Matrix2x2 - Matrix2x2: " << ((zero == m0 - m1) ? PASS : FAIL) << std::endl;

	/*for (i = 0; i < 2; i++)
	for (j = 0; j < 2; j++)
	m0.Set(i, j, i + j;

	for (i = 0; i < 2; i++)
	for (j = 0; j < 2; j++)
	m1.Set(i, j, i + j;
	Matrix2x2 result = Matrix2x2(1.0f, 2.0f, 5.0f, 11.0f);
	std::cout << "Matrix2x2 * Matrix2x2: " << ((m0 * m1 == result) ? PASS : FAIL) << std::endl;

	for (i = 0; i < 2; i++)
	for (j = 0; j < 2; j++)
	m0.Set(i, j, i + j;

	for (i = 0; i < 2; i++)
	for (j = 0; j < 2; j++)
	m1.Set(i, j, (i + j) * 2.0f;
	std::cout << "Matrix2x2 * scalar: " << (((m0 * 2.0f) == m1) ? PASS : FAIL) << std::endl;

	for (i = 0; i < 2; i++)
	for (j = 0; j < 2; j++)
	m0.Set(i, j, i + j;

	for (i = 0; i < 2; i++)
	for (j = 0; j < 2; j++)
	m1.Set(i, j, (i + j) / 2.0f;
	std::cout << "Matrix2x2 / divisor: " << (((m0 / 2.0f) == m1) ? PASS : FAIL) << std::endl;
	#pragma endregion Operations
	#pragma region Vector2D
	std::cout << "\n========== Running Matrix2x2 - Vector2D tests ==========\n\n");
	Matrix2x2 x22 = Matrix2x2();
	Vector2D v2 = Vector2D(1, 2);

	x22.m_matrix[0][0] = 1.0f;
	x22.m_matrix[0][1] = 2.0f;
	x22.m_matrix[1][0] = 3.0f;
	x22.m_matrix[1][1] = 4.0f;
	v2.x = 2.0f;	v2.y = 3.0f;

	Vector2D v2Test = Vector2D(8.0f, 18.0f);
	std::cout << "Matrix2x2 * Vector2D: " << ((v2Test == (x22 * v2)) ? PASS : FAIL) << std::endl;
	*/
#pragma endregion Vector2D
}
#endif