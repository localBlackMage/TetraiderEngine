#include "Math3D.h"
#include "Matrix4x4.h"
#include <math.h>
#include <algorithm>
#include "AcrylicorTypedefs.h"
#include <random>

static std::random_device rd;
static std::mt19937 mt(rd());

float GetRandomFloat(float lo, float hi)
{
	std::uniform_real_distribution<float> dist(lo, hi);
	return dist(mt);
}

bool FloatEquals(float value, float targetValue)
{
	return ((value >= targetValue - EPSILON) && (value <= targetValue + EPSILON));
}

static Vector3D _SnapPointStaticCircleToStaticRect(const Vector3D & Circle, const Vector3D & Rect, const float halfWidth, const float halfHeight)
{
	Vector3D snappedPoint = Circle;

	float left = Rect.getX() - halfWidth;
	float right = Rect.getX() + halfWidth;
	float top = Rect.getY() + halfHeight;
	float bottom = Rect.getY() - halfHeight;


	if (snappedPoint.getX() > right)	snappedPoint.setX(right);
	else if (snappedPoint.getX() < left)	snappedPoint.setX(left);

	if (snappedPoint.getY() > top)	snappedPoint.setY(top);
	else if (snappedPoint.getY() < bottom)	snappedPoint.setY(bottom);

	return snappedPoint;
}

#pragma region Static-On-Static Collisions

#pragma region Static Point
bool StaticPointToStaticCircle(const Vector3D & pos, const Vector3D & center, float radius)
{
	return Vector3D::SquareDistance(pos, center) <= (radius * radius);
}

bool StaticPointToStaticRect(const Vector3D & pos, const Vector3D & rect, const float halfWidth, const float halfHeight)
{
	float left = rect.getX() - halfWidth,
		right = rect.getX() + halfWidth,
		top = rect.getY() + halfHeight,
		bottom = rect.getY() - halfHeight;
	return (pos.getX() >= left && pos.getX() <= right && pos.getY() <= top && pos.getY() >= bottom);
}

float StaticPointToStaticLineSegment(const Vector3D & P, const LineSegment3D & LS)
{
	return Vector3D::Dot(P, LS.getNorm()) - LS.getNDotP0();
}
#pragma endregion

#pragma region Static Circle
bool StaticCircleToStaticCircle(const Vector3D & center0, const float radius0, const Vector3D & center1, const float radius1)
{
	float radiusSum = radius0 + radius1;
	return Vector3D::SquareDistance(center0, center1) <= (radiusSum * radiusSum);
}
#pragma endregion

#pragma region Static Rect
bool StaticRectToStaticRect(const Vector3D & rect0, const float halfWidth0, const float halfHeight0, const Vector3D & rect1, const float halfWidth1, const float halfHeight1)
{
	float left0 = rect0.getX() - halfWidth0,
		right0 = rect0.getX() + halfWidth0,
		top0 = rect0.getY() + halfHeight0,
		bottom0 = rect0.getY() - halfHeight0,
		left1 = rect1.getX() - halfWidth1,
		right1 = rect1.getX() + halfWidth1,
		top1 = rect1.getY() + halfHeight1,
		bottom1 = rect1.getY() - halfHeight1;

	return !(top1 < bottom0 || left1 > right0 || right1 < left0 || bottom1 > top0);
}

static bool _StaticRectToStaticCircle(const Vector3D& rectCenter, const float halfWidth, const float halfHeight, const Rect3D& rect, const Vector3D& circle, const float radius, const Vector3D snappedPoint)
{
	float sqRadius = radius * radius;

	if (Vector3D::SquareDistance(circle, snappedPoint) > sqRadius) // gauranteed no hit
		return false;
	else {
		Rect3D tallRect = rect.StretchHeight(radius);
		tallRect.Offset(rectCenter);
		Rect3D wideRect = rect.StretchWidth(radius);
		wideRect.Offset(rectCenter);
		float dX, dY;

		if (snappedPoint.getX() > rectCenter.getX())	dX = StaticPointToStaticLineSegment(circle, wideRect.Right()); // to the right
		else dX = StaticPointToStaticLineSegment(circle, wideRect.Left());	// to the left

		if (snappedPoint.getY() > rectCenter.getY()) 	dY = StaticPointToStaticLineSegment(circle, tallRect.Top()); // above
		else dY = StaticPointToStaticLineSegment(circle, tallRect.Bottom()); // below

		return FloatEquals(dX, 0.f) || FloatEquals(dY, 0.f);
	}
}

bool StaticRectToStaticCircle(const Vector3D & rectCenter, const float halfWidth, const float halfHeight, const Rect3D& rect, const Vector3D circle, const float radius)
{
	Vector3D snappedPoint = _SnapPointStaticCircleToStaticRect(circle, rectCenter, halfWidth, halfHeight);

	return _StaticRectToStaticCircle(rectCenter, halfWidth, halfHeight, rect, circle, radius, snappedPoint);
}
#pragma endregion

#pragma endregion

#pragma region Animated-On-Static Collisions

#pragma region Animated Point
CollisionResult AnimatedPointToStaticLineSegment(const Vector3D & Ps, const Vector3D & Pe, const LineSegment3D & LS)
{
	CollisionResult cr = CollisionResult();
	Vector3D PsPe = Pe - Ps;
	float	nDotP0 = LS.getNDotP0(),
		nDotPs = Vector3D::Dot(LS.getNorm(), Ps),
		nDotPe = Vector3D::Dot(LS.getNorm(), Pe),
		nDotPsPe = Vector3D::Dot(LS.getNorm(), PsPe);

	// Point will not collide in this iteration
	if ((nDotPs < nDotP0 && nDotPe < nDotP0) || (nDotPs > nDotP0 && nDotPe > nDotP0) || nDotPsPe == 0.0f)
		return cr;

	float t = (nDotP0 - Vector3D::Dot(LS.getNorm(), Ps)) / (nDotPsPe);
	Vector3D	collisionPoint = (PsPe * t) - Ps,
		LSPeSubLSPs = LS.getP1() - LS.getP0();

	// Point of intersection with the infinite line but not finite line
	if (Vector3D::Dot(collisionPoint - LS.getP0(), LSPeSubLSPs) < 0 ||
		Vector3D::Dot(collisionPoint - LS.getP1(), LSPeSubLSPs) < 0)
		return cr;

	cr.Set(t, collisionPoint, collisionPoint, collisionPoint);
	return cr;
}

CollisionResult AnimatedPointToStaticCircle(const Vector3D & Ps, const Vector3D & Pe, const Vector3D & Center, const float Radius)
{
	// r' = ARadius
	// r'' = SRadius + ARadius = r + r'
	// s^2 = r''^2 - n^2 => s = sqrt(r''^2 - n^2)
	// v = Pe - Ps
	// m = (Ps - Center) dot (v / |v|)
	// ti0 = (m - s)/ |v|
	// ti1 = (m + s)/ |v|
	// n^2 = |(ACenter0s - SCenter)|^2 - m^2 => n = sqrt(|(ACenterS - SCenter)|^2 - m^2)
	// if ti0 > 1.0f, no collision
	
	CollisionResult cr = CollisionResult();
	Vector3D SCsubACS = Center - Ps, v = Pe - Ps;
	float vLen = v.Length();

	Vector3D vNorm = v * (1.0f / vLen);

	float m = Vector3D::Dot(SCsubACS, vNorm);
	if (m < 0.0f)
		return cr;

	float SCsubACSLength = SCsubACS.Length(),
		nSquared = SCsubACSLength * SCsubACSLength - m * m,
		sSquared = Radius * Radius - nSquared,
		s = sqrtf(sSquared),
		ti0 = (m - s) / vLen;

	if (ti0 < 0.0f || 1.0f < ti0 || sSquared < 0.0f)
		return cr;

	Vector3D collisionPoint = (vNorm * ti0) + Ps;
	cr.Set(ti0, collisionPoint, collisionPoint, Center);
	return cr;
}
#pragma endregion

#pragma region Animated Circle
CollisionResult AnimatedCircleToStaticCircle(const Vector3D & ACenterS, const Vector3D & ACenterE, const float ARadius, const Vector3D & SCenter, const float SRadius)
{
	return AnimatedPointToStaticCircle(ACenterS, ACenterE, SCenter, ARadius + SRadius);
}

CollisionResult AnimatedCircleToStaticRect(const Vector3D & CStart, const Vector3D & CEnd, const float Radius, const Vector3D & RectCenter, const float halfWidth, const float halfHeight, const Rect3D & rect)
{
	Rect3D localRect = Rect3D(rect);
	localRect.Offset(RectCenter);

	CollisionResult collisionResults[4];
	collisionResults[0] = AnimatedCircleToStaticLineSegment(CStart, CEnd, Radius, localRect.Bottom());
	collisionResults[1] = AnimatedCircleToStaticLineSegment(CStart, CEnd, Radius, localRect.Top());
	collisionResults[2] = AnimatedCircleToStaticLineSegment(CStart, CEnd, Radius, localRect.Left());
	collisionResults[3] = AnimatedCircleToStaticLineSegment(CStart, CEnd, Radius, localRect.Right());
	
	int idx = 0;
	for (int i = 1; i < 4; ++i) {
		if (collisionResults[i].timeOfImpact >= 0.f && (collisionResults[i].timeOfImpact < collisionResults[idx].timeOfImpact || collisionResults[idx].timeOfImpact < 0.f))
			idx = i;
	}
	return collisionResults[idx];
}

CollisionResult AnimatedCircleToStaticLineSegment(const Vector3D & Ps, const Vector3D & Pe, const float Radius, const LineSegment3D & LS)
{
	CollisionResult cr = CollisionResult();
	Vector3D v = Pe - Ps, LSNorm = LS.getNorm(),
		collisionPoint, cPsubLSPs, cPsubLSPe, lineP1, lineP0;

	float	nDotP0 = LS.getNDotP0(),
		dSign = StaticPointToStaticLineSegment(Ps, LS) < 0.0f ? -1.0f : 1.0f, // -radius if inside, radius if outside
		nDotPs = Vector3D::Dot(LSNorm, Ps), // n dot Ps
		nDotPe = Vector3D::Dot(LSNorm, Pe), // n dot Pe
		t = (LS.getNDotP0() - Vector3D::Dot(LSNorm, Ps) + (dSign * Radius)) /
		(Vector3D::Dot(LSNorm, v)); // (n dot P0 - n dot Ps + D) / (n dot v)

	collisionPoint = (v * t) + Ps;	// Ps + (v * t)

	if (t > 1.0f ||
		(nDotPs - nDotP0 < -Radius && nDotPe - nDotP0 < -Radius) ||
		(nDotPs - nDotP0 > Radius && nDotPe - nDotP0 > Radius) ||	// Ball will not collide in this iteration
		(Vector3D::Dot(collisionPoint - LS.getP1(), LS.getP0() - LS.getP1()) < 0.0f) ||
		(Vector3D::Dot(collisionPoint - LS.getP0(), LS.getP1() - LS.getP0()) < 0.0f)) // Point of intersection with the infinite line but not finite line
		return cr;

	cr.Set(t, collisionPoint, collisionPoint, collisionPoint);
	return cr;
}
#pragma endregion

#pragma endregion

#pragma region Animated-On-Animated Collisions
CollisionResult AnimatedCircleToAnimatedRect(const Vector3D & CircleStart, const Vector3D & CircleEnd, const float Radius, const Vector3D & RectStart, const Vector3D & RectEnd, const float halfWidth, const float halfHeight, const Rect3D & rect)
{
	CollisionResult cr = CollisionResult();
	Vector3D vCircle = CircleEnd - CircleStart; // circle
	Vector3D vRect = RectEnd - RectStart; // rect
	float vCircleLen = vCircle.Length();
	float vRectLen = vRect.Length();

	vCircle = vCircleLen != 0.f ? vCircle / vCircleLen : vCircle;
	vRect = vRectLen != 0.f ? vRect / vRectLen : vRect;
	float t = 0.0f;

	for (t = 0.0f; t <= 1.0f; t += 0.05f) {
		Vector3D ct = CircleStart + (vCircle * (t * vCircleLen));
		Vector3D rt = RectStart + (vRect * (t * vRectLen));
		if (StaticRectToStaticCircle(rt, halfWidth, halfHeight, rect, ct, Radius)) {
			cr.Set(t, _SnapPointStaticCircleToStaticRect(ct, rt, halfWidth, halfHeight), ct, rt);
			break;
		}
	}
	return cr;
}

static bool _AnimatedCircleToAnimatedCircle(const Vector3D & Start0, const Vector3D & End0, const float Radius0, const Vector3D & Start1, const Vector3D & End1, const float Radius1)
{
	float moveRadius0 = (End0 - Start0).SquareLength() + (2.f * Radius0 * Radius0);
	float moveRadius1 = (End1 - Start1).SquareLength() + (2.f * Radius1 * Radius1);
	float totalRadius = moveRadius0 + moveRadius1;

	float distS0S1 = Vector3D::SquareDistance(Start0, Start1);
	float distS0E1 = Vector3D::SquareDistance(Start0, End1);
	float distE0S1 = Vector3D::SquareDistance(End0, Start1);
	float distE0E1 = Vector3D::SquareDistance(End0, End1);

	return (distS0S1 < totalRadius || distS0E1 < totalRadius || distE0S1 < totalRadius || distE0E1 < totalRadius);
}

CollisionResult AnimatedCircleToAnimatedCircle(const Vector3D & Start0, const Vector3D & End0, const float Radius0, const Vector3D & Start1, const Vector3D & End1, const float Radius1)
{
	CollisionResult cr = CollisionResult();
	if (!_AnimatedCircleToAnimatedCircle(Start0, End0, Radius0, Start1, End1, Radius1))
		return cr;

	Vector3D v0 = End0 - Start0;
	Vector3D v1 = End1 - Start1;
	float v0Len = v0.Length();
	float v1Len = v1.Length();
	v0 = v0Len != 0.f ? v0 / v0Len : v0;
	v1 = v1Len != 0.f ? v1 / v1Len : v1;
	float t = 0.0f;

	for (t = 0.0f; t <= 1.0f; t += 0.05f) {
		Vector3D ct0 = Start0 + (v0 * (t * v0Len));
		Vector3D ct1 = Start1 + (v1 * (t * v1Len));
		if (StaticCircleToStaticCircle(ct0, Radius0, ct1, Radius1)) {
			cr.Set(t, ct0 + (Vector3D::Normalize(ct1 - ct0) * Radius0), ct0, ct1);
			break;
		}
	}
	return cr;
}
#pragma endregion

#pragma region Reflection Methods

Vector3D ReflectAnimatedPointOnStaticLineSegment(const Vector3D & Pe, const LineSegment3D & LS, const CollisionResult& CR)
{
	// r = P`e - Pi
	// i = Pe - Pi
	// s = (i dot n) * n
	// m = i - s
	// r = m - s => i - 2s => i - 2(i dot n) * n
	// P`e = Pi + r => Pi + i - 2(i dot n) * n

	Vector3D i = Pe - CR.pointOfImpact,
		r = i - ((LS.getNorm() * Vector3D::Dot(i, LS.getNorm())) * 2.0f);
	return r + CR.pointOfImpact;
}

Vector3D ReflectAnimatedCircleOnStaticLineSegment(const Vector3D & Pe, const LineSegment3D & LS, const CollisionResult& CR)
{
	// r = P`e - Pi
	// i = Pe - Pi
	// s = (i dot n) * n
	// m = i - s
	// r = m - s => i - 2s => i - 2(i dot n) * n
	// P`e = Pi + r => Pi + i - 2(i dot n) * n

	Vector3D i = Pe - CR.pointOfImpact,
		r = i - ((LS.getNorm() * Vector3D::Dot(i, LS.getNorm())) * 2.0f);

	return r + CR.pointOfImpact;
}

Vector3D ReflectAnimatedCircleOnStaticRect(const Vector3D & Pe, const Vector3D & Pi, const Vector3D & RectCenter, const float halfWidth, const float halfHeight, const Rect3D & rect)
{
	// TODO: Fill me out

	Vector3D snappedPoint = _SnapPointStaticCircleToStaticRect(Pi, RectCenter, halfWidth, halfHeight);


	//if (snappedPoint.getX() > RectCenter.getX())	dX = StaticPointToStaticLineSegment(circle, rect.Right()); // to the right
	//else dX = StaticPointToStaticLineSegment(circle, rect.Left());	// to the left

	//if (snappedPoint.getY() > RectCenter.getY()) 	dY = StaticPointToStaticLineSegment(circle, rect.Top()); // above
	//else dY = StaticPointToStaticLineSegment(circle, rect.Bottom()); // below
	return Vector3D();
}

Vector3D ReflectAnimatedPointOnStaticCircle(const Vector3D & Ps, const Vector3D & Pe, const Vector3D & Center, const CollisionResult& CR)
{
	// n = Center - Pi
	// m = Ps - Pi = u + s
	// u = (m dot n) * n
	// s = Ps - u
	// r = u - s => 2(m dot n) * n - m
	// k = |PsPe|
	// R = Pi + k * rNorm * (1 - timeOfIntersection)

	Vector3D v = Pe - Ps,
		n = Vector3D::Normalize(Center - CR.pointOfImpact),
		m = Ps - CR.pointOfImpact,
		u = n * Vector3D::Dot(m, n),
		r = Vector3D::Normalize((u * 2.0f) - m) * v.Length();
	return r * (1.0f - CR.timeOfImpact);
}

Vector3D ReflectAnimatedCircleOnStaticCircle(const Vector3D& ACenterS, const Vector3D& ACenterE, const float ARadius, const Vector3D& SCenter, const float SRadius, const CollisionResult& CR)
{
	return ReflectAnimatedPointOnStaticCircle(ACenterS, ACenterE, SCenter, CR);
}
#pragma endregion

#pragma region Push Methods
#pragma region Circle
Vector3D PushCircleFromCircle(const float Radius, const float OtherRadius, const Vector3D & poi, const CollisionResult & CR)
{
	float expectedDistance = (Radius + OtherRadius);
	float actualDistance = Vector3D::Distance(CR.rhs_poi, CR.lhs_poi);
	float offset = (expectedDistance - actualDistance);

	return Vector3D::Normalize(poi - CR.pointOfImpact) * (offset + 1.f);
}

Vector3D PushCircleFromRect(const Vector3D & CircleEnd, const float Radius, const Vector3D& RectCenter, const float halfWidth, const float halfHeight, const CollisionResult& CR) {
	Vector3D snappedPoint = _SnapPointStaticCircleToStaticRect(CircleEnd, RectCenter, halfWidth, halfHeight);
	Vector3D pushDir = CircleEnd - snappedPoint;
	float pushLen = pushDir.Length();
	float pushForce = Radius - pushLen;
	return (pushDir / pushLen) * (pushForce + .5f);
}

#pragma endregion

#pragma endregion