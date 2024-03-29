/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Matrix2x2.h
Purpose: Provides a 2 x 2 Matrix and associated functionality
Language: C++
Project: GAM541
Author: Holden Profit
Creation date: 1/17/18
- End Header --------------------------------------------------------*/

//#pragma once

#ifndef MATRIX2X2_H
#define MATRIX2X2_H

//#include "Vector2D.h"

class Matrix2x2
{
private:
	float m_matrix[2][2];

public:
	Matrix2x2();
	Matrix2x2(float x1, float y1, float x2, float y2);
	Matrix2x2(const Matrix2x2 &other); // Copy Ctor
	Matrix2x2& operator= (const Matrix2x2 &other); // Assignment Operator
	~Matrix2x2();

	float Get(int row, int col) const;
	void Set(int row, int col, float val);
	void Transpose();
	void Zero();
	void Identity();
	float Determinant() const;
	void Print() const;

	static Matrix2x2 Zero2D();
	static Matrix2x2 Identity2D();
	static float Determinant(const Matrix2x2& other);

	bool operator== (const  Matrix2x2& other);
	bool operator!= (const  Matrix2x2& other);
	Matrix2x2 operator+ (const Matrix2x2& other);
	Matrix2x2 operator- (const Matrix2x2& other);
	Matrix2x2 operator* (const Matrix2x2& other);	
	Matrix2x2 operator* (const float scalar);
	Matrix2x2 operator/ (const float divisor);

	Vector2D operator* (const Vector2D& other);
};

#if TEST_MODE
void Matrix2x2Tests();
#endif

#endif