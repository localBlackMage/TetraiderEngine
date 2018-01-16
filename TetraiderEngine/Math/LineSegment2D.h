/* Start Header -------------------------------------------------------
Copyright (C) 2017 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: LineSegment2D.h
Purpose: Provides a 2 Dimensional Line Segment
Language: C++
Platform: Windows 8.1 / 10.0.15063.0  or  GCC 7.1.0
Project: CS529_holdenprofit_
Author: Holden Profit, holden.profit, 42833
Creation date: 10/6/17
- End Header --------------------------------------------------------*/

#pragma once

#ifndef LINESEGMENT2D_H
#define LINESEGMENT2D_H

#include "Vector2D.h"

class LineSegment2D
{
private:
	Vector2D m_p0, m_p1, m_norm;
	float m_nDotP0;

public:
	LineSegment2D(float x0, float y0, float x1, float y1);
	LineSegment2D(Vector2D Point0, Vector2D Point1);
	~LineSegment2D();

	Vector2D getP0() const { return m_p0; }
	void setP0(Vector2D p0);
	Vector2D getP1() const { return m_p1; }
	void setP1(Vector2D p01);
	Vector2D getNorm() const { return m_norm; }
	float getNDotP0() const { return m_nDotP0; }
};

#endif