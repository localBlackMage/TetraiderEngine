#ifndef SHAPE_H
#define SHAPE_H

class Body;

enum ShapeType {
	ST_Circle,
	ST_AABB,

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
	Circle() : Shape(ST_Circle) {}
	virtual ~Circle() {}
	float radius;
private:
};

class AABB : public Shape {
public:
	AABB() : Shape(ST_AABB) {
		halfWidth = halfWidth*0.5f;
		halfHeight = halfHeight*0.5f;
	}
	virtual ~AABB() {}
	float halfWidth;
	float halfHeight;
private:
	float width;
	float height;
};

#endif