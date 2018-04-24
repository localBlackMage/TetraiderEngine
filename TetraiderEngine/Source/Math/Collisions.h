/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Moodie Ghaddar>
- End Header --------------------------------------------------------*/

#pragma once

#ifndef COLLISIONS_H
#define COLLISIONS

//#include <vector>
//#include "Vector3D.h"

class LineSegment2D;

struct MTV {
	MTV() {}
	MTV(Vector3D _normal, float _penetration) :
		normal(_normal), penetration(_penetration) {}
	Vector3D normal;
	float penetration;
};

struct Projection {
	float min, max = 0;
};

bool StaticPointToStaticCircle(const Vector3D &p, const Vector3D &center, float radius);
bool StaticPointToStaticRect(const Vector3D &pos, const Vector3D &rect, float halfWidth, float halfHeight);

bool StaticCircleToStaticCircle(const Vector3D &center0, float radius0, const Vector3D &center1, float radius1, MTV& mtv);
bool StaticRectToStaticRect(const Vector3D &rect0, float halfWidth0, float halfHeight0, const Vector3D &rect1, float halfWidth1, float halfHeight1, MTV& mtv);
bool StaticCircleToStaticRect(const Vector3D &circle, float radius, const Vector3D &rectangle, float halfWidth, float halfHeight, MTV& mtv);

void SnapPointToAABB(Vector3D &result, const Vector3D &point, const Vector3D &rectangle, float halfWidth, float halfHeight);

bool StaticAABBToRay(const Vector3D &AABB, float halfWidth, float halfHeight, const LineSegment2D &line);
bool StaticCircleToRay(const Vector3D& circle, float radius, const LineSegment2D& line);
bool StaticPolygonToRay(const Vector3D& shapeA, const std::vector<Vector3D>& shapeAvert, const LineSegment2D& line);

bool StaticPolygonToStaticAABB(const Vector3D& shapeA, const std::vector<Vector3D>& shapeAvert, const Vector3D &rectangle, float halfWidth, float halfHeight, MTV& mtv);
bool StaticPolygonToStaticCircle(const Vector3D& shapeA, const std::vector<Vector3D>& shapeAvert, const Vector3D &circle, float radius, MTV& mtv);

bool IsPointInCone(const Vector3D& point, const Vector3D& center, float radius, Vector3D& dir, float angle);

class SeperatingAxisTheorom {
public:
	SeperatingAxisTheorom() {}
	~SeperatingAxisTheorom() {}
	static bool SAT(const Vector3D& shapeA, const std::vector<Vector3D>& shapeAvert, const Vector3D& shapeB, const std::vector<Vector3D>& shapeBvert, MTV& mtv);	
};

#endif // !COLLISIONS_H
