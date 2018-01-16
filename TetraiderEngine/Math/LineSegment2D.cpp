#include "LineSegment2D.h"

LineSegment2D::LineSegment2D(float x0, float y0, float x1, float y1) : 
	m_p0(Vector2D(x0, y0)), 
	m_p1(Vector2D(x1, y1)),
	m_norm(Vector2D::Normalize(Vector2D(y0 - y1, -(x0 - x1)))),
	m_nDotP0(Vector2D::Dot(m_p0, m_norm))
{ }

LineSegment2D::LineSegment2D(Vector2D Point0, Vector2D Point1) :
	m_p0(Point0),
	m_p1(Point1),
	m_norm(Vector2D::Normalize(m_p0 - m_p1)),
	m_nDotP0(Vector2D::Dot(m_p0, m_norm))
{ }

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