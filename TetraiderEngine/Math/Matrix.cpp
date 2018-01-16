#include <stdio.h>
#include <stdlib.h>
#include "Matrix.h"
#include "AcrylicorTypedefs.h"

Matrix::Matrix(int rows, int cols)
{
	m_rows = rows;
	m_cols = cols;

	m_matrix = (float **)malloc(sizeof(float*) * rows);
	if (!m_matrix)
		throw "Not enough memory.";
	for(int row = 0; row < rows; row++) 
	{
		m_matrix[row] = (float *)malloc(sizeof(float) * cols);
		if (!m_matrix[row])
			throw "Not enough memory.";
	}
}

Matrix::Matrix(const Matrix &other)
{
	m_rows = other.m_rows;
	m_cols = other.m_cols;

	m_matrix = (float **)malloc(sizeof(float*) * m_rows);
	if (!m_matrix)
		throw "Not enough memory.";
	for (int row = 0; row < m_rows; row++)
	{
		m_matrix[row] = (float *)malloc(sizeof(float) * m_cols);
		if (!m_matrix[row])
			throw "Not enough memory.";
		for (int col = 0; col < m_cols; col++)
		{
			m_matrix[row][col] = other.m_matrix[row][col];
		}
	}
}

Matrix& Matrix::operator=(const Matrix& other)
{
	// Free the original memory
	for (int row = 0; row < m_rows; row++)
	{
		free(m_matrix[row]);
	}
	free(m_matrix);

	// Re-set rows and columns
	m_rows = other.m_rows;
	m_cols = other.m_cols;

	// Re-allocate memory and set values
	m_matrix = (float **)malloc(sizeof(float*) * m_rows);
	if (!m_matrix)
		throw "Not enough memory.";
	for (int row = 0; row < m_rows; row++)
	{
		m_matrix[row] = (float *)malloc(sizeof(float) * m_cols);
		if (!m_matrix[row])
			throw "Not enough memory.";
		for (int col = 0; col < m_cols; col++)
		{
			m_matrix[row][col] = other.m_matrix[row][col];
		}
	}
	return *this;
}

Matrix::~Matrix()
{
	for (int row = 0; row < m_rows; row++)
	{
		free(m_matrix[row]);
	}
	free(m_matrix);
}

void Matrix::Transpose()
{
	// Make a copy of the Matrix's transposed form
	Matrix tmp = Matrix(m_cols, m_rows);
	for (int row = 0; row < m_rows; row++) 
	{
		for (int col = 0; col < m_cols; col++)
		{
			tmp.m_matrix[col][row] = m_matrix[row][col];
		}
	}
	// Free the original memory
	for (int row = 0; row < m_rows; row++)
	{
		free(m_matrix[row]);
	}
	free(m_matrix);
	// Flip rows and cols
	int rows = m_rows;
	m_rows = m_cols;
	m_cols = rows;

	// Re-initialize the Matrix's memory
	m_matrix = (float **)malloc(sizeof(float*) * m_rows);
	if (!m_matrix)
		throw "Not enough memory.";
	for (int row = 0; row < m_rows; row++)
	{
		m_matrix[row] = (float *)malloc(sizeof(float) * m_cols);
		if (!m_matrix[row])
			throw "Not enough memory.";
		// Copy tmp's values back into the Matrix
		for (int col = 0; col < m_cols; col++)
		{
			m_matrix[row][col] = tmp.m_matrix[row][col];
		}
	}
}

void Matrix::Print()
{
	for (int row = 0; row < m_rows; row++)
	{
		printf("(");
		for (int col = 0; col < m_cols; col++)
		{
			printf(" %f ", m_matrix[row][col]);
		}
		printf(")\n");
	}
	printf("\n");
}

void Matrix::Zero()
{
	for (int row = 0; row < m_rows; row++)
	{
		for (int col = 0; col < m_cols; col++)
		{
			m_matrix[row][col] = 0.0f;
		}
	}
}

void Matrix::Identity()
{
	if (m_rows != m_cols)
		throw "Cannot create identity of a non-square Matrix.";

	for (int row = 0; row < m_rows; row++)
	{
		for (int col = 0; col < m_cols; col++)
		{
			m_matrix[row][col] = row == col ? 1.0f : 0.0f;
		}
	}
}

/* Matrix Operations */
bool Matrix::operator==(const Matrix& other)
{
	if (other.m_rows != m_rows || other.m_cols != m_cols) 
	{
		return false;
	}
	else 
	{ 
		double d = 0.0f;
		for (int row = 0; row < m_rows; row++)
		{
			for (int col = 0; col < m_cols; col++)
			{
				d += fabs(m_matrix[row][col] - other.m_matrix[row][col]);
			}
		}
		return d < EPSILON;
	}
}

bool Matrix::operator!=(const Matrix& other)
{
	if (other.m_rows != m_rows || other.m_cols != m_cols)
	{
		return true;
	}
	else
	{
		double d = 0.0f;
		for (int row = 0; row < m_rows; row++)
		{
			for (int col = 0; col < m_cols; col++)
			{
				d += fabs(m_matrix[row][col] - other.m_matrix[row][col]);
			}
		}
		return d >= EPSILON;
	}
}

Matrix Matrix::operator+(const Matrix& other)
{
	if (other.m_rows != m_rows || other.m_cols != m_cols)
		throw "Cannot add Matricies with differing numbers of rows or columns.";

	Matrix result = Matrix(m_rows, m_cols);
	for (int row = 0; row < m_rows; row++)
	{
		for (int col = 0; col < m_cols; col++)
		{
			result.m_matrix[row][col] = m_matrix[row][col] + other.m_matrix[row][col];
		}
	}
	return result;
}

Matrix Matrix::operator-(const Matrix& other)
{
	if (other.m_rows != m_rows || other.m_cols != m_cols)
		throw "Cannot subtract Matricies with differing numbers of rows or columns.";

	Matrix result = Matrix(m_rows, m_cols);
	for (int row = 0; row < m_rows; row++)
	{
		for (int col = 0; col < m_cols; col++)
		{
			result.m_matrix[row][col] = m_matrix[row][col] - other.m_matrix[row][col];
		}
	}
	return result;
}

Matrix Matrix::operator*(const Matrix& other)
{
	// mXn * nXo
	// rowXcol * rowXcol
	if (other.m_rows != m_cols)
		throw "Cannot multiply Matricies with differing numbers of rows or columns.";

	// TODO: FIX 
	Matrix result = Matrix(m_rows, m_cols);
	for (int row = 0; row < m_rows; row++)
	{
		for (int col = 0; col < m_cols; col++)
		{
			result.m_matrix[row][col] = 0.0f;
		}
	}
	return result;
}

Matrix Matrix::operator*(const float scalar)
{
	Matrix result = Matrix(m_rows, m_cols);
	for (int row = 0; row < m_rows; row++)
	{
		for (int col = 0; col < m_cols; col++)
		{
			result.m_matrix[row][col] = m_matrix[row][col] * scalar;
		}
	}
	return result;
}

Matrix Matrix::operator/(const float divisor)
{
	if (divisor == 0.0f)
		throw "Division by zero error!";
	Matrix result = Matrix(m_rows, m_cols);
	for (int row = 0; row < m_rows; row++)
	{
		for (int col = 0; col < m_cols; col++)
		{
			result.m_matrix[row][col] = m_matrix[row][col] / divisor;
		}
	}
	return result;
}

/* Vector2D operations */
bool Matrix::operator==(const Vector2D& other)
{
	if (m_rows != 2 || m_cols != 1)
	{
		return false;
	}
	else
	{
		double d = 0.0f;
		d += fabs(m_matrix[0][0] - other.getX());
		d += fabs(m_matrix[1][0] - other.getY());
		return d < EPSILON;
	}
}

bool Matrix::operator!=(const Vector2D& other)
{
	if (m_rows != 2 || m_cols != 1)
	{
		return true;
	}
	else
	{
		double d = 0.0f;
		d += fabs(m_matrix[0][0] - other.getX());
		d += fabs(m_matrix[1][0] - other.getY());
		return d >= EPSILON;
	}
}

Matrix Matrix::operator+(const Vector2D& other)
{
	if (m_rows != 2 || m_cols != 1)
		throw "Cannot add Matricies and Vector2D if the Matrix does not have 2 rows and 1 column.";

	Matrix result = Matrix(2, 1);
	result.m_matrix[0][0] = m_matrix[0][0] + other.getX();
	result.m_matrix[1][0] = m_matrix[1][0] + other.getY();
	return result;
}

Matrix Matrix::operator-(const Vector2D& other)
{
	if (m_rows != 2 || m_cols != 1)
		throw "Cannot subtract Matricies and Vector2D if the Matrix does not have 2 rows and 1 column.";

	Matrix result = Matrix(2, 1);
	result.m_matrix[0][0] = m_matrix[0][0] - other.getX();
	result.m_matrix[1][0] = m_matrix[1][0] - other.getY();
	return result;
}

Matrix Matrix::operator*(const Vector2D& other)
{
	// mXn * nXo
	// mX2 * 2X1
	if (m_cols != 2)
		throw "Cannot subtract Matricies and Vector2D if the Matrix does not have 2 columns.";

	Matrix result = Matrix(m_rows, 1);
	for (int row = 0; row < m_rows; row++)
	{
		result.m_matrix[row][0] = m_matrix[row][0] * other.getX() + m_matrix[row][1] * other.getY();
	}
	return result;
}


/* Vector3D operations */
bool Matrix::operator==(const Vector3D& other)
{
	if (m_rows != 3 || m_cols != 1)
	{
		return false;
	}
	else
	{
		double d = 0.0f;
		d += fabs(m_matrix[0][0] - other.getX());
		d += fabs(m_matrix[1][0] - other.getY());
		d += fabs(m_matrix[2][0] - other.getZ());
		return d < EPSILON;
	}
}

bool Matrix::operator!=(const Vector3D& other)
{
	if (m_rows != 3 || m_cols != 1)
	{
		return true;
	}
	else
	{
		double d = 0.0f;
		d += fabs(m_matrix[0][0] - other.getX());
		d += fabs(m_matrix[1][0] - other.getY());
		d += fabs(m_matrix[2][0] - other.getZ());
		return d >= EPSILON;
	}
}

Matrix Matrix::operator+(const Vector3D& other)
{
	if (m_rows != 3 || m_cols != 1)
		throw "Cannot add Matricies and Vector3D if the Matrix does not have 3 rows and 1 column.";

	Matrix result = Matrix(3, 1);
	result.m_matrix[0][0] = m_matrix[0][0] + other.getX();
	result.m_matrix[1][0] = m_matrix[1][0] + other.getY();
	result.m_matrix[2][0] = m_matrix[2][0] + other.getZ();
	return result;
}

Matrix Matrix::operator-(const Vector3D& other)
{
	if (m_rows != 3 || m_cols != 1)
		throw "Cannot subtract Matricies and Vector3D if the Matrix does not have 3 rows and 1 column.";

	Matrix result = Matrix(3, 1);
	result.m_matrix[0][0] = m_matrix[0][0] - other.getX();
	result.m_matrix[1][0] = m_matrix[1][0] - other.getY();
	result.m_matrix[2][0] = m_matrix[2][0] - other.getZ();
	return result;
}

Matrix Matrix::operator*(const Vector3D& other)
{
	// mXn * nXo
	// mX3 * 3X1
	if (m_cols != 3)
		throw "Cannot subtract Matricies and Vector3D if the Matrix does not have 3 columns.";

	Matrix result = Matrix(m_rows, 1);
	for (int row = 0; row < m_rows; row++)
	{
		result.m_matrix[row][0] = m_matrix[row][0] * other.getX() + m_matrix[row][1] * other.getY() + m_matrix[row][2] * other.getZ();
	}
	return result;
}


/* Static Matrix Functions */
Matrix Matrix::Zero(int rows, int cols)
{
	Matrix zero = Matrix(rows, cols);

	for (int row = 0; row < rows; row++)
	{
		for (int col = 0; col < cols; col++)
		{
			zero.m_matrix[row][col] = 0.0f;
		}
	}

	return zero;
}

Matrix Matrix::Identity(int rows_cols)
{
	Matrix identity = Matrix(rows_cols, rows_cols);

	for (int row = 0; row < rows_cols; row++)
	{
		for (int col = 0; col < rows_cols; col++)
		{
			identity.m_matrix[row][col] = row == col ? 1.0f : 0.0f;
		}
	}

	return identity;
}

#if TEST_MODE
void MatrixTests()
{
	printf("\n========== Running Matrix tests ==========\n\n");

#pragma region Methods
	Matrix m0 = Matrix(4, 4);
	Matrix m1 = Matrix(4, 4);
	long i, j;

	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			m0.m_matrix[i][j] = i * 2 + j;

	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			m1.m_matrix[i][j] = i + j * 2;

	m1.Transpose();
	printf("Matrix Transpose: %s\n", (m0 == m1) ? PASS : FAIL);

	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			m0.m_matrix[i][j] = 0.0f;

	m1.Zero();
	printf("Matrix Zero: %s\n", (m0 == m1) ? PASS : FAIL);

	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			m0.m_matrix[i][j] = i == j ? 1.0f : 0.0f;

	m1.Identity();
	printf("Matrix Identity: %s\n", (m0 == m1) ? PASS : FAIL);
#pragma endregion Methods
#pragma region StaticMethods
	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			m0.m_matrix[i][j] = 0.0f;

	Matrix zero = Matrix::Zero(4, 4);
	printf("Matrix::Zero: %s\n", (zero == m0) ? PASS : FAIL);

	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			m0.m_matrix[i][j] = i == j ? 1.0f : 0.0f;

	Matrix identity = Matrix::Identity(4);
	printf("Matrix::Identity: %s\n", (m0 == identity) ? PASS : FAIL);
#pragma endregion StaticMethods
#pragma region Operations
	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			m0.m_matrix[i][j] = i + j;

	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			m1.m_matrix[i][j] = i - j;
	printf("Matrix == Matrix equal: %s\n", (m0 == m0) ? PASS : FAIL);

	printf("Matrix == Matrix not equal: %s\n", (m0 == m1) ? FAIL : PASS);

	printf("Matrix != Matrix not equal: %s\n", (m0 != m1) ? PASS : FAIL);

	printf("Matrix != Matrix equal: %s\n", (m0 != m0) ? FAIL : PASS);

	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			m0.m_matrix[i][j] = i + j;

	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			m1.m_matrix[i][j] = -(i + j);

	printf("Matrix +: %s\n", (zero == m0 + m1) ? PASS : FAIL);

	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			m0.m_matrix[i][j] = i + j;

	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			m1.m_matrix[i][j] = i + j;

	printf("Matrix -: %s\n", (zero == m0 - m1) ? PASS : FAIL);


	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			m0.m_matrix[i][j] = i + j;

	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			m1.m_matrix[i][j] = i + j;

	printf("Matrix * Matrix: %s\n", FAIL);// (zero == m0 * m1) ? PASS : FAIL);

	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			m0.m_matrix[i][j] = i + j;

	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			m1.m_matrix[i][j] = (i + j) * 2.0f;

	printf("Matrix * scalar: %s\n", ((m0 * 2.0f) == m1) ? PASS : FAIL);

	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			m0.m_matrix[i][j] = i + j;

	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			m1.m_matrix[i][j] = (i + j) / 2.0f;

	printf("Matrix / divisor: %s\n", ((m0 / 2.0f) == m1) ? PASS : FAIL);
#pragma endregion Operations
#pragma region Vector2D
	printf("\n========== Running Matrix - Vector2D tests ==========\n\n");

	Matrix x21 = Matrix(2, 1);
	Matrix x22 = Matrix(2, 2);
	Matrix x21Test = Matrix(2, 1);
	Vector2D v2 = Vector2D(1, 2);

	x21.m_matrix[0][0] = 1.0f;
	x21.m_matrix[1][0] = 2.0f;
	v2.Set(1.0f, 2.0f);
	printf("Matrix(2X1) == Vector2D equal: %s\n", (x21 == v2) ? PASS : FAIL);

	v2.Set(2.0f, 1.0f);
	printf("Matrix(2X1) == Vector2D not equal: %s\n", (x21 == v2) ? FAIL : PASS);

	v2.Zero();
	printf("Matrix(mXn) == Vector2D where (m != 2 and n != 1): %s\n", (x22 == v2) ? FAIL : PASS);

	v2.Set(1.0f, 1.0f);
	printf("Matrix(2X1) != Vector2D not equal: %s\n", (x21 != v2) ? PASS : FAIL);

	v2.Set(1.0f, 2.0f);
	printf("Matrix(2X1) != Vector2D equal: %s\n", (x21 != v2) ? FAIL : PASS);

	v2.Set(0.0f, 0.0f, 0.0f);
	printf("Matrix(mXn) != Vector2D where (m != 2 and n != 1): %s\n", (x22 != v2) ? PASS : FAIL);


	x21.m_matrix[0][0] = 1.0f;
	x21.m_matrix[1][0] = 2.0f;
	v2.Set(1.0f, 2.0f);

	x21Test.m_matrix[0][0] = 2.0f;
	x21Test.m_matrix[1][0] = 4.0f;
	printf("Matrix(2X1) + Vector2D: %s\n", ((x21 + v2) == x21Test) ? PASS : FAIL);

	try {
		x22 + v2;
		printf("Matrix(mXn) + Vector2D where n != 2: %s\n", FAIL);
	}
	catch (...) {
		printf("Matrix(mXn) + Vector2D where n != 2: %s\n", PASS);
	}


	x21.m_matrix[0][0] = 1.0f;
	x21.m_matrix[1][0] = 2.0f;
	v2.Set(1.0f, 2.0f);

	x21Test.m_matrix[0][0] = 0.0f;
	x21Test.m_matrix[1][0] = 0.0f;
	printf("Matrix(2X1) - Vector2D: %s\n", ((x21 - v2) == x21Test) ? PASS : FAIL);

	try {
		x22 - v2;
		printf("Matrix(mXn) - Vector2D where n != 2: %s\n", FAIL);
	}
	catch (...) {
		printf("Matrix(mXn) - Vector2D where n != 2: %s\n", PASS);
	}


	x22.m_matrix[0][0] = 1.0f;
	x22.m_matrix[0][1] = 2.0f;
	x22.m_matrix[1][0] = 3.0f;
	x22.m_matrix[1][1] = 4.0f;
	v2.Set(2.0f, 3.0f);

	x21Test.m_matrix[0][0] = 8.0f;
	x21Test.m_matrix[1][0] = 18.0f;
	printf("Matrix(2X2) * Vector2D: %s\n", ((x22 * v2) == x21Test) ? PASS : FAIL);

	try {
		x21 * v2;
		printf("Matrix(mXn) - Vector2D where n != 2: %s\n", FAIL);
	}
	catch (...) {
		printf("Matrix(mXn) - Vector2D where n != 2: %s\n", PASS);
	}

#pragma endregion Vector2D
#pragma region Vector3D
	printf("\n========== Running Matrix - Vector3D tests ==========\n\n");
	Matrix x31 = Matrix(3, 1);
	Matrix x33 = Matrix(3, 3);
	Matrix x31Test = Matrix(3, 1);
	Vector3D v3 = Vector3D(1, 2, 3);

	x31.m_matrix[0][0] = 1.0f;
	x31.m_matrix[1][0] = 2.0f;
	x31.m_matrix[2][0] = 3.0f;
	printf("Matrix(3X1) == Vector3D equal: %s\n", (x31 == v3) ? PASS : FAIL);

	x31.m_matrix[0][0] = 2.0f;
	printf("Matrix(3X1) == Vector3D not equal: %s\n", (x31 == v3) ? FAIL : PASS);

	v3.Zero();
	printf("Matrix(mXn) == Vector3D where (m != 3 and n != 1): %s\n", (x33 == v3) ? FAIL : PASS);

	x31.m_matrix[0][0] = 1.0f;
	x31.m_matrix[1][0] = 2.0f;
	x31.m_matrix[2][0] = 3.0f;
	printf("Matrix(3X1) != Vector3D not equal: %s\n", (x31 != v3) ? PASS : FAIL);

	v3.Set(1.0f, 2.0f, 3.0f);
	printf("Matrix(3X1) != Vector3D equal: %s\n", (x31 != v3) ? FAIL : PASS);

	v3.Set(0.0f, 0.0f, 0.0f);
	printf("Matrix(mXn) != Vector3D where (m != 3 and n != 1): %s\n", (x33 != v3) ? PASS : FAIL);


	x31.m_matrix[0][0] = 1.0f;
	x31.m_matrix[1][0] = 2.0f;
	x31.m_matrix[2][0] = 3.0f;
	v3.Set(1.0f, 2.0f, 3.0f);

	x31Test.m_matrix[0][0] = 2.0f;
	x31Test.m_matrix[1][0] = 4.0f;
	x31Test.m_matrix[2][0] = 6.0f;
	printf("Matrix(3X1) + Vector3D: %s\n", ((x31 + v3) == x31Test) ? PASS : FAIL);

	try {
		x33 + v3;
		printf("Matrix(mXn) + Vector3D where n != 3: %s\n", FAIL);
	}
	catch (...) {
		printf("Matrix(mXn) + Vector3D where n != 3: %s\n", PASS);
	}


	x31.m_matrix[0][0] = 1.0f;
	x31.m_matrix[1][0] = 2.0f;
	x31.m_matrix[2][0] = 3.0f;
	v3.Set(1.0f, 2.0f, 3.0f);

	x31Test.m_matrix[0][0] = 0.0f;
	x31Test.m_matrix[1][0] = 0.0f;
	x31Test.m_matrix[2][0] = 0.0f;
	printf("Matrix(3X1) - Vector3D: %s\n", ((x31 - v3) == x31Test) ? PASS : FAIL);

	try {
		x33 - v3;
		printf("Matrix(mXn) - Vector3D where n != 3: %s\n", FAIL);
	}
	catch (...) {
		printf("Matrix(mXn) - Vector3D where n != 3: %s\n", PASS);
	}


	x33.m_matrix[0][0] = 1.0f;
	x33.m_matrix[0][1] = 2.0f;
	x33.m_matrix[0][2] = 3.0f;

	x33.m_matrix[1][0] = 4.0f;
	x33.m_matrix[1][1] = 5.0f;
	x33.m_matrix[1][2] = 6.0f;

	x33.m_matrix[2][0] = 7.0f;
	x33.m_matrix[2][1] = 8.0f;
	x33.m_matrix[2][2] = 9.0f;
	v3.Set(1.0f, 2.0f, 3.0f);

	x31Test.m_matrix[0][0] = 14.0f;
	x31Test.m_matrix[1][0] = 32.0f;
	x31Test.m_matrix[2][0] = 50.0f;
	printf("Matrix(3X3) * Vector3D: %s\n", ((x33 * v3) == x31Test) ? PASS : FAIL);

	try {
		x31 * v3;
		printf("Matrix(mXn) - Vector3D where n != 3: %s\n", FAIL);
	}
	catch (...) {
		printf("Matrix(mXn) - Vector3D where n != 3: %s\n", PASS);
	}

#pragma endregion Vector3D
}
#endif