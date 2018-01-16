/* Start Header -------------------------------------------------------
Copyright (C) 2017 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: LineSegment3D.h
Purpose: Provides a 3 Dimensional Line Segment
Language: C++
Platform: Windows 8.1 / 10.0.15063.0  or  GCC 7.1.0
Project: CS529_holdenprofit_
Author: Holden Profit, holden.profit, 42833
Creation date: 11/03/17
- End Header --------------------------------------------------------*/

#pragma once

#ifndef LINESEGMENT3D_H
#define LINESEGMENT3D_H

#include "Vector3D.h"

class LineSegment3D
{
private:
	Vector3D m_p0, m_p1, m_norm;
	float m_nDotP0;

	void _SetNorm();
public:
	LineSegment3D(float x0, float y0, float z0, float x1, float y1, float z1);
	LineSegment3D(Vector3D Point0, Vector3D Point1);
	LineSegment3D(const LineSegment3D& rhs);
	LineSegment3D& operator=(const LineSegment3D& rhs);
	~LineSegment3D();

	LineSegment3D& operator+=(const Vector3D& rhs);
	LineSegment3D& operator-=(const Vector3D& rhs);

	Vector3D getP0() const { return m_p0; }
	void setP0(Vector3D p0);
	Vector3D getP1() const { return m_p1; }
	void setP1(Vector3D p01);
	Vector3D getNorm() const { return m_norm; }
	float getNDotP0() const { return m_nDotP0; }
	void offset(Vector3D offset);
	void shiftAlongNormal(float offset);
};

#endif