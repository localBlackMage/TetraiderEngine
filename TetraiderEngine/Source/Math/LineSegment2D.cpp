/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

LineSegment2D::LineSegment2D(float x0, float y0, float x1, float y1) : 
	m_p0(Vector2D(x0, y0)), 
	m_p1(Vector2D(x1, y1)),
	m_norm(Vector2D::Normalize(Vector2D(y0 - y1, -(x0 - x1)))),
	m_nDotP0(Vector2D::Dot(m_p0, m_norm)),
	m_direction(Vector2D(x1 - x0, y1 - y0))
{
}

LineSegment2D::LineSegment2D(Vector2D Point0, Vector2D Point1) :
	m_p0(Point0),
	m_p1(Point1),
	m_norm(Vector2D::Normalize(m_p0 - m_p1)),
	m_nDotP0(Vector2D::Dot(m_p0, m_norm)),
	m_direction(Vector2D(Point1.x - Point0.x, Point1.y - Point0.y))
{ }

void LineSegment2D::SetLine(float x0, float y0, float x1, float y1) {
	m_p0.x = x0;
	m_p0.y = y0;
	m_p1.x = x1;
	m_p1.y = y1;
	m_norm = Vector2D::Normalize(Vector2D(y0 - y1, -(x0 - x1)));
	m_nDotP0 = Vector2D::Dot(m_p0, m_norm);
	m_direction = Vector2D(x1 - x0, y1 - y0);
}

LineSegment2D::~LineSegment2D() {}

void LineSegment2D::setP0(Vector2D p0)
{
	m_p0 = p0;
	m_norm = Vector2D::Normalize(m_p0 - m_p1);
	m_nDotP0 = Vector2D::Dot(m_p0, m_norm);
}

void LineSegment2D::setP1(Vector2D p1)
{
	m_p1 = p1;
	m_norm = Vector2D::Normalize(m_p0 - m_p1);
	m_nDotP0 = Vector2D::Dot(m_p0, m_norm);
}