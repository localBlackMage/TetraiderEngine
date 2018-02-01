#pragma once
#ifndef SHAPE_H
#define SHAPE_H

class Body;

#include "Math/MathLibs.h"
#include <vector>

enum ShapeType {
	ST_Circle,
	ST_AABB,
	ST_POLYGON,

	ST_Count
};

class Shape {
public:
	Shape(ShapeType t) : type(t) {}
	virtual ~Shape() {}
	ShapeType type;
	Body* pBody;
	friend Body;
};

class Circle : public Shape {
public:
	Circle(float _radius) : Shape(ST_Circle), radius(_radius) {}
	virtual ~Circle() {}
	float radius;
private:
};

class AABB : public Shape {
public:
	AABB(float _width, float _height) : Shape(ST_AABB), width(_width), height(_height), halfWidth(_width/2.0f), halfHeight(_height/2.0f) {}
	virtual ~AABB() {}
	float halfWidth;
	float halfHeight;
	float width;
	float height;
private:
};

class Polygon : public Shape {
public:
	Polygon() : Shape(ST_POLYGON) {}
	virtual ~Polygon() {}
	std::vector<Vector3D> m_vertices;
private:
};

#endif