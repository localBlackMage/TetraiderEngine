#include "LineSegment3D.h"

void LineSegment3D::_SetNorm()
{
	// All lines are assumed to exist on a plane parallel to the XY plane (i.e. p0.z == p1.z for both points)
	m_norm = Vector3D::Normalize(Vector3D(m_p0.getY() - m_p1.getY(), -(m_p0.getX() - m_p1.getX()), 0.f));
	m_nDotP0 = Vector3D::Dot(m_p0, m_norm);

	//Vector3D p0Norm = Vector3D::Normalize(m_p0);
	//Vector3D p1Norm = Vector3D::Normalize(m_p1);

	//if (p0Norm != p1Norm) {
	//	float proj = Vector3D::Dot(m_p1, m_p0);
	//	if (proj == 0.f) {
	//		m_norm = Vector3D::Normalize(Vector3D(m_p0.getY() - m_p1.getY(), -(m_p0.getX() - m_p1.getX()), 0.f));
	//	}
	//	else {
	//		float scalar = proj / m_p0.Length();
	//		m_norm = Vector3D::Normalize(m_p1 - (scalar * m_p0));
	//	}
	//}
	//else if (m_p0.getX() != m_p1.getX() && m_p0.getY() != m_p1.getY()) {
	//	m_norm = Vector3D::Normalize(Vector3D(m_p0.getY() - m_p1.getY(), -(m_p0.getX() - m_p1.getX()), 0.f));
	//}
	//// TODO: cases for x coords and y coords being the same

	//m_nDotP0 = Vector3D::Dot(m_p0, m_norm);
}

LineSegment3D::LineSegment3D(float x0, float y0, float z0, float x1, float y1, float z1) :
	m_p0(Vector3D(x0, y0, z0)),
	m_p1(Vector3D(x1, y1, z1))
{ 
	_SetNorm();
}

LineSegment3D::LineSegment3D(Vector3D Point0, Vector3D Point1) :
	m_p0(Point0),
	m_p1(Point1)
{ 
	_SetNorm();
}

LineSegment3D::LineSegment3D(const LineSegment3D & rhs) :
	m_p0(rhs.m_p0), m_p1(rhs.m_p1), m_norm(rhs.m_norm), m_nDotP0(rhs.m_nDotP0)
{}

LineSegment3D & LineSegment3D::operator=(const LineSegment3D & rhs)
{
	m_p0 = rhs.m_p0;
	m_p1 = rhs.m_p1;
	m_norm = rhs.m_norm;
	m_nDotP0 = rhs.m_nDotP0;
	return *this;
}

LineSegment3D::~LineSegment3D() {}

LineSegment3D & LineSegment3D::operator+=(const Vector3D & rhs)
{
	offset(rhs);
	return *this;
}

LineSegment3D & LineSegment3D::operator-=(const Vector3D & rhs)
{
	offset(rhs * -1.f);
	return *this;
}

void LineSegment3D::setP0(Vector3D p0)
{
	m_p0 = p0;
	_SetNorm();
}

void LineSegment3D::setP1(Vector3D p1)
{
	m_p1 = p1;
	_SetNorm();
}

void LineSegment3D::offset(Vector3D offset)
{
	m_p0 += offset;
	m_p1 += offset;
	m_nDotP0 = Vector3D::Dot(m_p0, m_norm);
}

void LineSegment3D::shiftAlongNormal(float offset)
{
	Vector3D offsetVec = m_norm * offset;
	m_p0 += offsetVec;
	m_p1 += offsetVec;
	m_nDotP0 = Vector3D::Dot(m_p0, m_norm);
}
