/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Moodie Ghaddar>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

#define EPSILON 0.0001

// Seperating axis theorem functions
void SetAxis(std::vector<Vector3D>& result, const std::vector<Vector3D> vertx);
void ProjectOnAxis(Projection& result, const Vector3D& axis, const std::vector<Vector3D>& vertx);
bool ProjectionsOverlap(const Projection& projectionA, const Projection& projectionB);
float GetOverlap(const Projection& projectionA, const Projection& projectionB);
bool ProjectVerticesOnAxisAndDetectCollision(const std::vector<Vector3D>& axises, const std::vector<Vector3D>& vertxA, const std::vector<Vector3D>& vertxB, MTV& mtv);

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

bool StaticCircleToStaticCircle(const Vector3D &center0, float radius0, const Vector3D &center1, float radius1, MTV& mtv) {
	if (Vector3D::SquareDistance(center0, center1) <= (radius0 + radius1)*(radius0 + radius1)) {
		Vector3D normal = center0 - center1;
		normal.Normalize();
		Vector3D contactPoint = normal*radius1 + center1;
		mtv.normal = normal;
		mtv.penetration = fabsf(radius0 - Vector3D::Distance(contactPoint, center0));
		return true;
	}

	return false;
}

bool StaticRectToStaticRect(const Vector3D &rect0, float halfWidth0, float halfHeight0, const Vector3D &rect1, float halfWidth1, float halfHeight1, MTV& mtv) {
	/*float rect0LeftX = rect0.x - halfWidth0;
	float rect0TopY = rect0.y + halfHeight0;
	float rect0RightX = rect0.x + halfWidth0;
	float rect0BottomY = rect0.y - halfHeight0;

	float rect1LeftX = rect1.x - halfWidth1;
	float rect1TopY = rect1.y + halfHeight1;
	float rect1RightX = rect1.x + halfWidth1;
	float rect1BottomY = rect1.y - halfHeight1;

	return (rect0LeftX <= rect1RightX && rect1LeftX <= rect0RightX && rect0TopY >= rect1BottomY && rect1TopY >= rect0BottomY);*/

	// Using SAT to determine MTV. Replace if a more effecient way is found. This can reduce to testing on two axis only
	std::vector<Vector3D> rectVertx0(4);
	rectVertx0[0] = Vector3D(-halfWidth0, -halfHeight0, 0);
	rectVertx0[1] = Vector3D(-halfWidth0, halfHeight0, 0);
	rectVertx0[2] = Vector3D(halfWidth0, halfHeight0, 0);
	rectVertx0[3] = Vector3D(halfWidth0, -halfHeight0, 0);

	std::vector<Vector3D> rectVertx1(4);
	rectVertx1[0] = Vector3D(-halfWidth1, -halfHeight1, 0);
	rectVertx1[1] = Vector3D(-halfWidth1, halfHeight1, 0);
	rectVertx1[2] = Vector3D(halfWidth1, halfHeight1, 0);
	rectVertx1[3] = Vector3D(halfWidth1, -halfHeight1, 0);

	return SeperatingAxisTheorom::SAT(rect0, rectVertx0, rect1, rectVertx1, mtv);
}

bool StaticCircleToStaticRect(const Vector3D &circle, float radius, const Vector3D &rectangle, float halfWidth, float halfHeight, MTV& mtv) {
	// Find closest point to the rectangle
	Vector3D closestPoint;
	SnapPointToAABB(closestPoint, circle, rectangle, halfWidth, halfHeight);

	// Check if closest point on rectangle intersects with circle
	if (StaticPointToStaticCircle(closestPoint, circle, radius)) {
		Vector3D normal = closestPoint - circle;
		if (normal.IsVectorZero()) {
			normal = rectangle - circle;
		}
		normal.Normalize();
		mtv.normal = normal;
		mtv.penetration = fabsf(radius - Vector3D::Distance(closestPoint, circle));
		return true;
	}

	return false;
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
	Vector3D P1 = line.getP1();

	if (StaticPointToStaticRect(P1, AABB, halfWidth, halfHeight))
		return true;
	
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
	Vector3D P0 = line.getP0();

	if (StaticPointToStaticCircle(P0, circle, radius))
		return true;
	
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

bool SeperatingAxisTheorom::SAT(const Vector3D& shapeA, const std::vector<Vector3D>& shapeAvert, const Vector3D& shapeB, const std::vector<Vector3D>& shapeBvert, MTV& mtv) {
	std::vector<Vector3D> vertxA(shapeAvert.size());
	std::vector<Vector3D> vertxB(shapeBvert.size());
	// Get vertixes in world coordinate space. TODO: If static object this calculation can be avoided
	for (unsigned int i = 0; i < shapeAvert.size(); ++i) { vertxA[i] = shapeA + shapeAvert[i]; }
	for (unsigned int i = 0; i < shapeBvert.size(); ++i) { vertxB[i] = shapeB + shapeBvert[i]; }

	// Get all axises from shape A
	std::vector<Vector3D> axisesA(shapeAvert.size());
	SetAxis(axisesA, vertxA);
	// Get all axises from shape B
	std::vector<Vector3D> axisesB(shapeBvert.size());
	SetAxis(axisesB, vertxB);

	// Set penetration to a large value
	mtv.penetration = INFINITY;
	// Check for collision overlap, TODO: check for containment
	if (!ProjectVerticesOnAxisAndDetectCollision(axisesA, vertxA, vertxB, mtv)) return false;
	if (!ProjectVerticesOnAxisAndDetectCollision(axisesB, vertxA, vertxB, mtv)) return false;

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

bool ProjectVerticesOnAxisAndDetectCollision(const std::vector<Vector3D>& axises, const std::vector<Vector3D>& vertxA, const std::vector<Vector3D>& vertxB, MTV& mtv) {
	Projection p1;
	Projection p2;
	// Check if projections are overlaping for every axis. If one fails then shapes do not collide
	for (unsigned int i = 0; i < axises.size(); ++i) {
		ProjectOnAxis(p1, axises[i], vertxA);
		ProjectOnAxis(p2, axises[i], vertxB);

		if (!ProjectionsOverlap(p1, p2))
			return false;
		// Find the penetration depth
		else {
			float o = GetOverlap(p1, p2);
			if (o < mtv.penetration) {
				mtv.penetration = o;
				mtv.normal = axises[i];
			}
		}
	}

	return true;
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

// TODO: Check if any cases are missed
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

	//printf("SAT: Projections are not overlapping\n");
	return 0;
}

bool StaticPolygonToStaticAABB(const Vector3D& shapeA, const std::vector<Vector3D>& shapeAvert, const Vector3D &rectangle, float halfWidth, float halfHeight, MTV& mtv) {
	std::vector<Vector3D> rectVertx(4);
	rectVertx[0] = Vector3D( -halfWidth, -halfHeight, 0);
	rectVertx[1] = Vector3D( -halfWidth, halfHeight, 0);
	rectVertx[2] = Vector3D(halfWidth, halfHeight, 0);
	rectVertx[3] = Vector3D(halfWidth, -halfHeight, 0);

	return SeperatingAxisTheorom::SAT(shapeA, shapeAvert, rectangle, rectVertx, mtv);
}

bool StaticPolygonToStaticCircle(const Vector3D& shapeA, const std::vector<Vector3D>& shapeAvert, const Vector3D &circle, float radius, MTV& mtv) {
	// TODO: Voroni regions for circle vs polygon

	// Get vertixes in world coordinate space. TODO: If static object this calculation can be avoided
	std::vector<Vector3D> vertxA(shapeAvert.size());
	for (unsigned int i = 0; i < shapeAvert.size(); ++i) { vertxA[i] = shapeA + shapeAvert[i]; }

	for (unsigned int i = 0; i < vertxA.size(); ++i) {
		Vector3D edge = vertxA[i == vertxA.size() - 1 ? 0 : i + 1] - vertxA[i];
		Vector3D axis = circle - vertxA[i];
		float dot = Vector3D::Dot(axis, edge);
		float edgeSqaureLength = edge.SquareLength();
		if (dot >= 0 && dot <= edgeSqaureLength) {
			Vector3D projection = vertxA[i] + (dot / edgeSqaureLength) * edge;
			axis = projection - circle;
			if (axis.SquareLength() <= radius*radius) {
				axis.Normalize();
				mtv.normal = axis;
				mtv.penetration = fabsf(radius - Vector3D::Distance(projection, circle));
				TETRA_DEBUG.DrawLine(projection, projection + axis * -50, DebugColor::CYAN);
				return true;
			}
			else {
				axis.Normalize();
				// If circle is in outer half plane
				if (Vector3D::Dot(circle, axis) - Vector3D::Dot(vertxA[i], axis) > 0)
					return false;
			}
		}
	}
	for (unsigned int i = 0; i < vertxA.size(); ++i) {
		if (StaticPointToStaticCircle(vertxA[i], circle, radius)) {
			Vector3D normal = circle - vertxA[i];
			normal.Normalize();
			mtv.normal = normal;
			mtv.penetration = fabsf(radius - Vector3D::Distance(vertxA[i], circle));
			TETRA_DEBUG.DrawLine(vertxA[i], vertxA[i] + normal * 50, DebugColor::CYAN);
			return true;
		}
	}

	return false;
}

bool StaticPolygonToRay(const Vector3D& shapeA, const std::vector<Vector3D>& shapeAvert, const LineSegment2D& line) {
	Vector3D pos(0, 0, 0);
	std::vector<Vector3D> lineVertx(2);
	lineVertx[0] = Vector3D(line.getP0().x, line.getP0().y, 0);
	lineVertx[1] = Vector3D(line.getP1().x, line.getP1().y, 0);

	MTV mtv;
	return SeperatingAxisTheorom::SAT(shapeA, shapeAvert, pos, lineVertx, mtv);
}

// TODO: Test if this functions works for angles grater than 180
bool IsPointInCone(const Vector3D& point, const Vector3D& center, float radius, Vector3D& dir, float angle) {
	if (!StaticPointToStaticCircle(point, center, radius))
		return false;

	Vector3D dirToPoint = point - center;
	dirToPoint.Normalize();

	float a = acosf(Vector3D::Dot(dirToPoint, dir)) * 180/PI;
	return a < angle*0.5f;
}