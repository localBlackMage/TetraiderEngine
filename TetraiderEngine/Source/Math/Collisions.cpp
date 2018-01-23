#include "Collisions.h"
#include "Vector3D.h"
#include "LineSegment2D.h"
#include "..\DebugManager.h"
#include <math.h>

#define EPSILON 0.0001

bool StaticPointToStaticCircle(const Vector3D &p, const Vector3D &center, float radius) {
	return (Vector3D::SquareDistance(p, center) <= radius*radius);
}

bool StaticPointToStaticRect(const Vector3D &pos, const Vector3D &rect, float halfWidth, float halfHeight) {
	float leftX = rect.x - halfWidth;
	float topY = rect.y + halfHeight;
	float rightX = rect.x + halfWidth;
	float bottomY = rect.y - halfHeight;

	return (pos.x >= leftX && pos.x <= rightX && pos.y >= bottomY && pos.y <= topY);
}

bool StaticCircleToStaticCircle(const Vector3D &center0, float radius0, const Vector3D &center1, float radius1) {
	return (Vector3D::SquareDistance(center0, center1) <= (radius0 + radius1)*(radius0 + radius1));
}

bool StaticRectToStaticRect(const Vector3D &rect0, float halfWidth0, float halfHeight0, const Vector3D &rect1, float halfWidth1, float halfHeight1) {
	float rect0LeftX = rect0.x - halfWidth0;
	float rect0TopY = rect0.y + halfHeight0;
	float rect0RightX = rect0.x + halfWidth0;
	float rect0BottomY = rect0.y - halfHeight0;

	float rect1LeftX = rect1.x - halfWidth1;
	float rect1TopY = rect1.y + halfHeight1;
	float rect1RightX = rect1.x + halfWidth1;
	float rect1BottomY = rect1.y - halfHeight1;

	return (rect0LeftX <= rect1RightX && rect1LeftX <= rect0RightX && rect0TopY >= rect1BottomY && rect1TopY >= rect0BottomY);
}

bool StaticCircleToStaticRect(const Vector3D &circle, float radius, const Vector3D &rectangle, float halfWidth, float halfHeight) {
	// Find closest point to the rectangle
	Vector3D closestPoint;
	SnapPointToAABB(closestPoint, circle, rectangle, halfWidth, halfHeight);

	// Check if closest point on rectangle intersects with circle
	return StaticPointToStaticCircle(closestPoint, circle, radius);
}

void SnapPointToAABB(Vector3D &result, const Vector3D &point, const Vector3D &rectangle, float halfWidth, float halfHeight) {
	float rectLeftX = rectangle.x - halfWidth;
	float rectRightX = rectangle.x + halfWidth;
	float rectTopY = rectangle.y + halfHeight;
	float rectBottomY = rectangle.y - halfHeight;

	if (point.x < rectLeftX)
		result.x = rectLeftX;
	else if (point.x > rectRightX)
		result.x = rectRightX;
	else
		result.x = point.x;

	if (point.y < rectBottomY)
		result.y = rectBottomY;
	else if (point.y > rectTopY)
		result.y = rectTopY;
	else
		result.y = point.y;
}

bool StaticAABBToRay(const Vector3D& AABB, float halfWidth, float halfHeight, const LineSegment2D &line) {
	double tmin = -INFINITY;
	double tmax = +INFINITY;

	double minX = AABB.x - halfWidth;
	double maxY = AABB.y + halfHeight;
	double maxX = AABB.x + halfWidth;
	double minY = AABB.y - halfHeight;

	if (fabsf(line.getDirection().x) != EPSILON) {
		double tx1 = (minX - line.getP0().x) / line.getDirection().x;
		double tx2 = (maxX - line.getP0().x) / line.getDirection().x;

		tmin = fmax(tmin, fmin(tx1, tx2));
		tmax = fmin(tmax, fmax(tx1, tx2));
	}

	if (fabsf(line.getDirection().y) != EPSILON) {
		double ty1 = (minY - line.getP0().y) / line.getDirection().y;
		double ty2 = (maxY - line.getP0().y) / line.getDirection().y;

		tmin = fmax(tmin, fmin(ty1, ty2));
		tmax = fmin(tmax, fmax(ty1, ty2));
	}

	if (tmin > tmax)
		return false;
	else if (tmax > 1 || tmax < 0)
		return false;

	return true;
}

bool StaticCircleToRay(const Vector3D& circle, float radius, const LineSegment2D& line) {
	Vector2D circlePos(circle.x, circle.y);
	Vector2D PsC(circle.x - line.getP0().x, circle.y -line.getP0().y);
	Vector2D PsPe(line.getP1().x - line.getP0().x, line.getP1().y - line.getP0().y);

	float m = Vector2D::Dot(PsC, line.getDirection());

	if (m < 0 && Vector2D::SquareDistance(line.getP0(), circlePos) > radius*radius)
		return false;

	float nSqaured = PsC.SquareLength() - m*m;

	if (nSqaured > radius*radius)
		return false;

	float a = Vector2D::Dot(PsPe, PsPe);
	float c = Vector2D::Dot(PsC, PsC) - radius*radius;
	PsC.Mul(-2.0f);
	float b = Vector2D::Dot(PsC, PsPe);

	float delta = b*b - 4 * a*c;
	if (delta < 0)
		return false;
	else if (delta < EPSILON) {
		float t = -b / (2 * a);
		if (t > 1.0f || t < 0.0f)
			return false;
	}
	else {
		float t1 = (-b + sqrtf(delta)) / (2 * a);
		float t2 = (-b - sqrtf(delta)) / (2 * a);
		float t = t1 > t2 ? t2 : t1;
		if (t > 1.0f || t < 0.0f)
			return false;
	}

	return true;
}

bool SAT(const Vector3D shapeA, const std::vector<Vector3D>& shapeAvert, const Vector3D shapeB, const std::vector<Vector3D>& shapeBvert) {
	DebugManager& debugManager = DebugManager::GetInstance();

	std::vector<Vector3D> axesA(shapeAvert.size());
	std::vector<Vector3D> vertxA(shapeAvert.size());
	std::vector<Vector3D> axesB(shapeBvert.size());
	std::vector<Vector3D> vertxB(shapeBvert.size());

	for (unsigned int i = 0; i < shapeAvert.size(); ++i) { vertxA[i] = shapeA + shapeAvert[i]; }
	for (unsigned int i = 0; i < shapeBvert.size(); ++i) { vertxB[i] = shapeB + shapeBvert[i]; }

	SetAxis(axesA, vertxA);
	SetAxis(axesB, vertxB);

	// Debug
	for (unsigned int i = 0; i < axesA.size(); ++i) {
		Vector3D pA = ((vertxA[i == vertxA.size() - 1 ? 0 : i + 1] - vertxA[i])* 0.5f) + vertxA[i];
		Vector3D pB = pA + axesA[i] * 50;
		debugManager.DrawLine(pA, pB, DebugColor::CYAN);
	}

	for (unsigned int i = 0; i < axesB.size(); ++i) {
		Vector3D pA = ((vertxB[i == vertxB.size() - 1 ? 0 : i + 1] - vertxB[i])* 0.5f) + vertxB[i];
		Vector3D pB = pA + axesB[i] * 50;
		debugManager.DrawLine(pA, pB, DebugColor::CYAN);
	}
	// Debug done
	MTV mtv;
	mtv.overlap = INFINITY;
	for (unsigned int i = 0; i < axesA.size(); ++i) {
		Projection p1;
		Projection p2;
		ProjectOnAxis(p1, axesA[i], vertxA);
		ProjectOnAxis(p2, axesA[i], vertxB);
		 
		if (!ProjectionsOverlap(p1, p2))
			return false;
		else {
			float o = GetOverlap(p1, p2);
			if (o < mtv.overlap) {
				mtv.overlap = o;
				mtv.normal = -1*axesA[i];
			}
		}
	}

	// TO DO check for containment
	for (unsigned int i = 0; i < axesB.size(); ++i) {
		Projection p1;
		Projection p2;
		ProjectOnAxis(p1, axesB[i], vertxA);
		ProjectOnAxis(p2, axesB[i], vertxB);

		if (!ProjectionsOverlap(p1, p2))
			return false;
		else {
			float o = GetOverlap(p1, p2);
			if (o < mtv.overlap) {
				mtv.overlap = o;
				mtv.normal = axesB[i];
			}
		}
	}
	
	printf("overlap: %f\n", mtv.overlap);
	return true;
}

// Assumes vertx has the same size as result
void SetAxis(std::vector<Vector3D>& result, const std::vector<Vector3D> vertx) {
	for (unsigned int i = 0; i < vertx.size(); ++i) {
		// Get the edge 
		Vector3D edge = vertx[i == vertx.size() - 1 ? 0 : i + 1] - vertx[i];
		Vector3D normal(-edge.y, edge.x, 0);
		normal.Normalize();
		result[i] = normal;
	}
}

void ProjectOnAxis(Projection& result, const Vector3D& axis, const std::vector<Vector3D>& vertx) {
	result.min = Vector3D::Dot(axis, vertx[0]);
	result.max = result.min;

	for (unsigned int i = 1; i < vertx.size(); ++i) {
		float p = Vector3D::Dot(axis, vertx[i]);

		if (p < result.min) result.min = p;
		else if (p > result.max) result.max = p;
	}
}

bool ProjectionsOverlap(const Projection& projectionA, const Projection& projectionB) {
	if (projectionA.max < projectionB.min || projectionB.max < projectionA.min)
		return false;
	else
		return true;
}

// TO DO check if this is correct
float GetOverlap(const Projection& projectionA, const Projection& projectionB) {
	if (projectionA.min < projectionB.min) {
		if (projectionA.max > projectionB.min && projectionA.max < projectionB.max)
			return projectionA.max - projectionB.min;
		else if (projectionA.max >= projectionB.max)
			return projectionB.max - projectionB.min;
	}
	else if (projectionA.min >= projectionB.min) {
		if (projectionA.max < projectionB.max)
			return projectionA.max - projectionA.min;
		else if (projectionA.max >= projectionB.max)
			return projectionB.max - projectionA.min;
	}
	else {
		printf("Something wrong in finding overlap\n");
		return 0;
	}
}