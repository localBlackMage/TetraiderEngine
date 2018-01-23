#ifndef COLLISIONS_H
#define COLLISIONS

#include <vector>
#include "Vector3D.h"

class LineSegment2D;

bool StaticPointToStaticCircle(const Vector3D &p, const Vector3D &center, float radius);
bool StaticPointToStaticRect(const Vector3D &pos, const Vector3D &rect, float halfWidth, float halfHeight);

bool StaticCircleToStaticCircle(const Vector3D &center0, float radius0, const Vector3D &center1, float radius1);
bool StaticRectToStaticRect(const Vector3D &rect0, float halfWidth0, float halfHeight0, const Vector3D &rect1, float halfWidth1, float halfHeight1);
bool StaticCircleToStaticRect(const Vector3D &circle, float radius, const Vector3D &rectangle, float halfWidth, float halfHeight);

void SnapPointToAABB(Vector3D &result, const Vector3D &point, const Vector3D &rectangle, float halfWidth, float halfHeight);

bool StaticAABBToRay(const Vector3D &AABB, float halfWidth, float halfHeight, const LineSegment2D &line);
bool StaticCircleToRay(const Vector3D& circle, float radius, const LineSegment2D& line);

bool SAT(const Vector3D shapeA, const std::vector<Vector3D>& shapeAvert, const Vector3D shapeB, const std::vector<Vector3D>& shapeBvert);
void SetAxis(std::vector<Vector3D>& result, const std::vector<Vector3D> vertx);

struct Projection {
	float min, max = 0;
};

struct MTV {
	Vector3D normal;
	float overlap;
};

void ProjectOnAxis(Projection& result, const Vector3D& axis, const std::vector<Vector3D>& vertx);
bool ProjectionsOverlap(const Projection& projectionA, const Projection& projectionB);
float GetOverlap(const Projection& projectionA, const Projection& projectionB);

#endif // !COLLISIONS_H
