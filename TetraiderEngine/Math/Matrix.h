#pragma once

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

#ifndef MATRIX_H
#define MATRIX_H

#include "Vector2D.h"
#include "Vector3D.h"
#include <math.h>

class Matrix
{
public:
	int m_rows, m_cols;
	float ** m_matrix;

	Matrix(int rows, int cols);
	Matrix(const Matrix &other); // Copy Constructor
	Matrix& operator= (const Matrix &other); // Assignment Operator
	~Matrix();

	void Transpose();
	void Print();
	void Zero();
	void Identity();

	bool operator== (const  Matrix& other);
	bool operator!= (const  Matrix& other);
	Matrix operator+ (const Matrix& other);
	Matrix operator- (const Matrix& other);
	Matrix operator* (const Matrix& other);
	Matrix operator* (const float scalar);
	Matrix operator/ (const float divisor);

	bool operator== (const  Vector2D& other);
	bool operator!= (const  Vector2D& other);
	Matrix operator+ (const Vector2D& other);
	Matrix operator- (const Vector2D& other);
	Matrix operator* (const Vector2D& other);

	bool operator== (const  Vector3D& other);
	bool operator!= (const  Vector3D& other);
	Matrix operator+ (const Vector3D& other);
	Matrix operator- (const Vector3D& other);
	Matrix operator* (const Vector3D& other);

	static Matrix Zero(int rows, int cols);
	static Matrix Identity(int rows_cols);
};

#if TEST_MODE
void MatrixTests();
#endif

#endif