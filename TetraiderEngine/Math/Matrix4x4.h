/* Start Header -------------------------------------------------------
Copyright (C) 2017 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Matrix4x4.h
Purpose: Provides a 4 by 4 Matrix and associated functionality
Language: C++
Platform: Windows 8.1 / 10.0.15063.0  or  GCC 7.1.0
Project: CS529_holdenprofit_
Author: Holden Profit, holden.profit, 42833
Creation date: 9/29/17
- End Header --------------------------------------------------------*/

#pragma once

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

#ifndef MATRIX4X4_H
#define MATRIX4X4_H

#include "Vector3D.h"
#include "Matrix3x3.h"

class Matrix4x4
{
private:
	float m_matrix[4][4];

public:
	Matrix4x4();
	Matrix4x4(float x1, float y1, float z1, float w1, float x2, float y2, float z2, float w2, float x3, float y3, float z3, float w3, float x4, float y4, float z4, float w4);
	Matrix4x4(const Matrix4x4 &other); // Copy Ctor
	Matrix4x4(const Matrix3x3 &other); // Copy Ctor
	Matrix4x4& operator= (const Matrix4x4 &other); // Assignment Operator
	~Matrix4x4();

	float Get(int row, int col) const;
	void Set(int row, int col, float val);
	void Transpose();
	void Zero();
	void Identity();
	float Determinant() const;
	void Print() const;

	static Matrix4x4 Zero4D();
	static Matrix4x4 Identity4D();
	static float Determinant(const Matrix4x4& other);
	static Matrix4x4 Transpose(const Matrix4x4& other);
	static Matrix4x4 Transpose3x3(const Matrix4x4& other);
	static Matrix4x4 Inverse(const Matrix4x4& other);
	static Matrix4x4 Inverse3x3(const Matrix4x4& other);
	static Matrix4x4 Rotate(const float degrees, const Vector3D& axis);
	static Matrix4x4 Translate(const Vector3D& amt);
	static Matrix4x4 Scale(const float scalar);
	static Matrix4x4 Scale(const float scaleX, const float scaleY, const float scaleZ);
	static Matrix4x4 Perspective(const float fov, const float aspect, const float near, const float far);
	static Matrix4x4 Perspective(const float fov, const float aspect, const float near);
	static Matrix4x4 Orthographic(const int width, const int height, const float near, const float far);
	static Matrix4x4 Orthographic(const int width, const int height, const float near);

	operator float*();
	bool operator== (const  Matrix4x4& other) const;
	bool operator!= (const  Matrix4x4& other) const;
	Matrix4x4 operator+ (const Matrix4x4& other) const;
	Matrix4x4 operator- (const Matrix4x4& other) const;
	Matrix4x4 operator* (const Matrix4x4& other) const;
	Matrix4x4 operator* (const float scalar) const;
	Matrix4x4 operator/ (const float divisor) const;
	
	Vector3D operator* (const Vector3D& other) const;
};

#if TEST_MODE
void Matrix4x4Tests();
#endif

#endif