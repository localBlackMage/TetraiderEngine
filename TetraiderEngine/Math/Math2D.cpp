#include "Math2D.h"
#include "stdio.h"
#include <math.h>
#include "AcrylicorTypedefs.h"

/*
This function checks if the point P is colliding with the circle whose
center is "Center" and radius is "radius"
*/
bool StaticPointToStaticCircle(const Vector2D &pos, const Vector2D &center, float radius)
{
	return Vector2D::SquareDistance(pos, center) <= (radius * radius);
}

/*
This function checks if the point Pos is colliding with the rectangle
whose center is Rect, width is "width" and height is height
*/
bool StaticPointToStaticRect(const Vector2D &pos, const Vector2D &rect, const float width, const float height)
{
	float	halfwidth = width / 2.0f, 
			halfheight = height / 2.0f,
			left = rect.getX() - halfwidth,
			right = rect.getX() + halfwidth,
			top = rect.getY() + halfheight,
			bottom = rect.getY() - halfheight;
	return (pos.getX() >= left && pos.getX() <= right && pos.getY() <= top && pos.getY() >= bottom);
}

/*
This function checks for collision between 2 circles.
Circle0: Center is Center0, radius is "radius0"
Circle1: Center is Center1, radius is "radius1"
*/
bool StaticCircleToStaticCircle(const Vector2D &center0, const float radius0, const Vector2D &center1, const float radius1)
{
	float radiusSum = radius0 + radius1;
	return Vector2D::SquareDistance(center0, center1) <= (radiusSum * radiusSum);
}

/*
This functions checks if 2 rectangles are colliding
Rectangle0: Center is rect0, width is "width0" and height is "height0"
Rectangle1: Center is rect1, width is "width1" and height is "height1"
*/
bool StaticRectToStaticRect(const Vector2D &rect0, const float width0, const float height0, const Vector2D &rect1, const float width1, const float height1)
{
	float	halfwidth0 = width0 / 2.0f, halfheight0 = height0 / 2.0f,
			halfwidth1 = width1 / 2.0f, halfheight1 = height1 / 2.0f,
			left0 = rect0.getX() - halfwidth0,
			right0 = rect0.getX() + halfwidth0,
			top0 = rect0.getY() + halfheight0,
			bottom0 = rect0.getY() - halfheight0,
			left1 = rect1.getX() - halfwidth1,
			right1 = rect1.getX() + halfwidth1,
			top1 = rect1.getY() + halfheight1,
			bottom1 = rect1.getY() - halfheight1;

	return !(top1 < bottom0 || left1 > right0 || right1 < left0 || bottom1 > top0);
}

/*
This function determines the distance separating a point from a line

- Parameters
- P:		The point whose location should be determined
- LS:		The line segment

- Returned value: The distance. Note that the returned value should be:
- Negative if the point is in the line's inside half plane
- Positive if the point is in the line's outside half plane
- Zero if the point is on the line
*/
float StaticPointToStaticLineSegment(const Vector2D& P, const LineSegment2D& LS)
{
	return Vector2D::Dot(P, LS.getNorm()) - LS.getNDotP0();
}

/*
This function checks whether an animated point is colliding with a line segment

- Parameters
- Ps:		The point's starting location
- Pe:		The point's ending location
- LS:		The line segment
- Pi:		This will be used to store the intersection point's coordinates (In case there's an intersection)

- Returned value: Intersection time t
- -1.0f:				If there's no intersection
- Intersection time:	If there's an intersection
*/
float AnimatedPointToStaticLineSegment(const Vector2D& Ps, const Vector2D& Pe, const LineSegment2D& LS, Vector2D& Pi)
{
	Vector2D PsPe = Pe - Ps;
	float	nDotP0 = LS.getNDotP0(),
			nDotPs = Vector2D::Dot(LS.getNorm(), Ps),
			nDotPe = Vector2D::Dot(LS.getNorm(), Pe),
			nDotPsPe = Vector2D::Dot(LS.getNorm(), PsPe);

	// Point will not collide in this iteration
	if ((nDotPs < nDotP0 && nDotPe < nDotP0) || (nDotPs > nDotP0 && nDotPe > nDotP0) || nDotPsPe == 0.0f)
		return -1.0f;

	float t = (nDotP0 - Vector2D::Dot(LS.getNorm(), Ps)) / (nDotPsPe);
	Vector2D	collisionPoint = (PsPe * t) - Ps, 
				LSPeSubLSPs = LS.getP1() - LS.getP0();

	// Point of intersection with the infinite line but not finite line
	if (Vector2D::Dot(collisionPoint - LS.getP0(), LSPeSubLSPs) < 0 || 
		Vector2D::Dot(collisionPoint - LS.getP1(), LSPeSubLSPs) < 0)
		return -1.0f;

	Pi = collisionPoint;
	return t;
}

/*
This function checks whether an animated circle is colliding with a line segment

- Parameters
- Ps:		The center's starting location
- Pe:		The center's ending location
- Radius:	The circle's radius
- LS:		The line segment
- Pi:		This will be used to store the intersection point's coordinates (In case there's an intersection)

- Returned value: Intersection time t
- -1.0f:				If there's no intersection
- Intersection time:	If there's an intersection
*/
float AnimatedCircleToStaticLineSegment(const Vector2D& Ps, const Vector2D& Pe, const float Radius, const LineSegment2D& LS, Vector2D& Pi)
{
	Vector2D v = Pe - Ps, LSNorm = LS.getNorm(),
		collisionPoint, cPsubLSPs, cPsubLSPe, lineP1, lineP0;

	float	nDotP0 = LS.getNDotP0(),
		dSign = StaticPointToStaticLineSegment(Ps, LS) < 0.0f ? -1.0f : 1.0f, // -radius if inside, radius if outside
		nDotPs = Vector2D::Dot(LSNorm, Ps), // n dot Ps
		nDotPe = Vector2D::Dot(LSNorm, Pe), // n dot Pe
		t = (LS.getNDotP0() - Vector2D::Dot(LSNorm, Ps) + (dSign * Radius)) /
		(Vector2D::Dot(LSNorm, v)); // (n dot P0 - n dot Ps + D) / (n dot v)

	collisionPoint = (v * t) + Ps;	// Ps + (v * t)

	if (t > 1.0f ||
		(nDotPs - nDotP0 < -Radius && nDotPe - nDotP0 < -Radius) ||
		(nDotPs - nDotP0 > Radius && nDotPe - nDotP0 > Radius) ||	// Ball will not collide in this iteration
		(Vector2D::Dot(collisionPoint - LS.getP1(), LS.getP0() - LS.getP1()) < 0.0f) ||
		(Vector2D::Dot(collisionPoint - LS.getP0(), LS.getP1() - LS.getP0()) < 0.0f)) // Point of intersection with the infinite line but not finite line
		return -1.0f;

	Pi = collisionPoint;
	return t;
}

/*
This function reflects an animated point on a line segment.
It should first make sure that the animated point is intersecting with the line

- Parameters
- Ps:		The point's starting location
- Pe:		The point's ending location
- LS:		The line segment
- Pi:		This will be used to store the intersection point's coordinates (In case there's an intersection)
- R:		Reflected vector R

- Returned value: Intersection time t
- -1.0f:				If there's no intersection
- Intersection time:	If there's an intersection
*/
float ReflectAnimatedPointOnStaticLineSegment(const Vector2D& Ps, const Vector2D& Pe, const LineSegment2D& LS, Vector2D& Pi, Vector2D& R)
{
	float timeOfIntersection = AnimatedPointToStaticLineSegment(Ps, Pe, LS, Pi);
	if (timeOfIntersection == -1.0f)
		return -1.0f;

	// r = P`e - Pi
	// i = Pe - Pi
	// s = (i dot n) * n
	// m = i - s
	// r = m - s => i - 2s => i - 2(i dot n) * n
	// P`e = Pi + r => Pi + i - 2(i dot n) * n

	Vector2D i = Pe - Pi,
			 r = i - ((LS.getNorm() * Vector2D::Dot(i, LS.getNorm())) * 2.0f);
	//R.Set(r.getX(), r.getY());
	R = r + Pi;

	return timeOfIntersection;
}

/*
This function reflects an animated circle on a line segment.
It should first make sure that the animated point is intersecting with the line

- Parameters
- Ps:		The center's starting location
- Pe:		The center's ending location
- Radius:	The circle's radius
- LS:		The line segment
- Pi:		This will be used to store the intersection point's coordinates (In case there's an intersection)
- R:		Reflected vector R

- Returned value: Intersection time t
- -1.0f:				If there's no intersection
- Intersection time:	If there's an intersection
*/
float ReflectAnimatedCircleOnStaticLineSegment(const Vector2D& Ps, const Vector2D& Pe, const float Radius, const LineSegment2D& LS, Vector2D& Pi, Vector2D& R)
{
	float timeOfIntersection = AnimatedCircleToStaticLineSegment(Ps, Pe, Radius, LS, Pi);
	if (timeOfIntersection == -1.0f)
		return -1.0f;

	// r = P`e - Pi
	// i = Pe - Pi
	// s = (i dot n) * n
	// m = i - s
	// r = m - s => i - 2s => i - 2(i dot n) * n
	// P`e = Pi + r => Pi + i - 2(i dot n) * n

	Vector2D i = Pe - Pi, 
			 r = i - ((LS.getNorm() * Vector2D::Dot(i, LS.getNorm())) * 2.0f);

	R = r + Pi;
	return timeOfIntersection;
}

/*
This function checks whether an animated point is colliding with a static circle

- Parameters
- Ps:		The point's starting location
- Pe:		The point's ending location
- Center:	The circle's center
- Radius:	The circle's radius
- Pi:		This will be used to store the intersection point's coordinates (In case there's an intersection)

- Returned value: Intersection time t
- -1.0f:		If there's no intersection
- Intersection time:	If there's an intersection
*/
float AnimatedPointToStaticCircle(const Vector2D& Ps, const Vector2D& Pe, const Vector2D& Center, const float Radius, Vector2D& Pi)
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

	Vector2D SCsubACS = Center - Ps, v = Pe - Ps;
	float vLen = v.Length();

	Vector2D vNorm = v * (1.0f / vLen);

	float m = Vector2D::Dot(SCsubACS, vNorm);
	if (m < 0.0f)
		return -1.0f;

	float SCsubACSLength = SCsubACS.Length(),
		nSquared = SCsubACSLength * SCsubACSLength - m * m,
		sSquared = Radius * Radius - nSquared,
		s = sqrtf(sSquared),
		ti0 = (m - s) / vLen;

	if (ti0 < 0.0f || 1.0f < ti0 || sSquared < 0.0f)
		return -1.0f;

	Vector2D ti0Vec = vNorm * ti0;
	Pi = (vNorm * ti0) + Ps;
	return ti0;
}

/*
This function reflects an animated point on a static circle.
It should first make sure that the animated point is intersecting with the circle

- Parameters
- Ps:		The point's starting location
- Pe:		The point's ending location
- Center:	The circle's center
- Radius:	The circle's radius
- Pi:		This will be used to store the intersection point's coordinates (In case there's an intersection)
- R:		Reflected vector R

- Returned value: Intersection time t
- -1.0f:		If there's no intersection
- Intersection time:	If there's an intersection
*/
float ReflectAnimatedPointOnStaticCircle(const Vector2D& Ps, const Vector2D& Pe, const Vector2D& Center, const float Radius, Vector2D& Pi, Vector2D& R)
{
	float timeOfIntersection = AnimatedPointToStaticCircle(Ps, Pe, Center, Radius, Pi);
	if (timeOfIntersection < 0.0f)
		return -1.0f;

	// n = Center - Pi
	// m = Ps - Pi = u + s
	// u = (m dot n) * n
	// s = Ps - u
	// r = u - s => 2(m dot n) * n - m
	// k = |PsPe|
	// R = Pi + k * rNorm * (1 - timeOfIntersection)

	Vector2D v = Pe - Ps, 
		n = Vector2D::Normalize(Center - Pi),
		m = Ps - Pi, 
		u = n * Vector2D::Dot(m, n),
		r = Vector2D::Normalize((u * 2.0f) - m) * v.Length();
	R = r * (1.0f - timeOfIntersection);
	return timeOfIntersection;
}


/*
This function checks whether an animated circle is colliding with a static circle

- Parameters
- Center0s:		The starting position of the animated circle's center
- Center0e:		The ending position of the animated circle's center
- Radius0:		The animated circle's radius
- Center1:		The static circle's center
- Radius1:		The static circle's radius
- Pi:			This will be used to store the intersection point's coordinates (In case there's an intersection)

- Returned value: Intersection time t
- -1.0f:		If there's no intersection
- Intersection time:	If there's an intersection
*/
float AnimatedCircleToStaticCircle(const Vector2D& ACenterS, const Vector2D& ACenterE, const float ARadius, const Vector2D& SCenter, const float SRadius, Vector2D& Pi)
{
	return AnimatedPointToStaticCircle(ACenterS, ACenterE, SCenter, ARadius + SRadius, Pi);
}


/*
This function reflects an animated circle on a static circle.
It should first make sure that the animated circle is intersecting with the static one

- Parameters
- Center0s:		The starting position of the animated circle's center
- Center0e:		The ending position of the animated circle's center
- Radius0:		The animated circle's radius
- Center1:		The static circle's center
- Radius1:		The static circle's radius
- Pi:			This will be used to store the intersection point's coordinates (In case there's an intersection)
- R:			Reflected vector R

- Returned value: Intersection time t
- -1.0f:		If there's no intersection
- Intersection time:	If there's an intersection
*/
float ReflectAnimatedCircleOnStaticCircle(const Vector2D& ACenterS, const Vector2D& ACenterE, const float ARadius, const Vector2D& SCenter, const float SRadius, Vector2D& Pi, Vector2D& R)
{
	return ReflectAnimatedPointOnStaticCircle(ACenterS, ACenterE, SCenter, ARadius + SRadius, Pi, R);
}

// ================================================================================================================================================= //
// ================================================================================================================================================= //

void StaticCollisionTests()
{
	printf("\n========== Running Static Collision tests ==========\n\n");

	Vector2D v1 = Vector2D(10.0f, 10.0f),
		v2 = Vector2D(11.4f, 11.4f);
	float radius = 2.0f;

	//StaticPointToStaticCircle
	printf("StaticPointToStaticCircle Collision: %s\n", (StaticPointToStaticCircle(v2, v1, radius)) ? PASS : FAIL);

	v2.Set(12.0f, 12.0f);
	printf("StaticPointToStaticCircle Non Collision: %s\n\n", (!StaticPointToStaticCircle(v2, v1, radius)) ? PASS : FAIL);

	//StaticPointToStaticRect
	v1.Set(1.0f, 1.0f);	// point
	v2.Set(0.0f, 0.0f);	// rect
	printf("StaticPointToStaticRect Collision: %s\n", (StaticPointToStaticRect(v1, v2, 2.0f, 2.0f) ? PASS : FAIL));
	printf("StaticPointToStaticRect Non Collision: %s\n\n", (!StaticPointToStaticRect(v1, v2, 1.0f, 1.0f) ? PASS : FAIL));

	//StaticCircleToStaticCircle
	v1.Set(2.0f, 0.0f);
	printf("StaticCircleToStaticCircle Collision Touch: %s\n", (StaticCircleToStaticCircle(v1, 1.0f, v2, 1.0f) ? PASS : FAIL));
	printf("StaticCircleToStaticCircle Collision: %s\n", (StaticCircleToStaticCircle(v1, 2.0f, v2, 1.0f) ? PASS : FAIL));
	printf("StaticCircleToStaticCircle Non Collision: %s\n\n", (!StaticCircleToStaticCircle(v1, 0.5f, v2, 1.0f) ? PASS : FAIL));

	//StaticRectToStaticRect
	v1.Set(2.0f, 2.0f);
	v2.Set(0.0f, 0.0f);
	printf("StaticRectToStaticRect Non Collision: %s\n", (!StaticRectToStaticRect(v1, 1.0f, 1.0f, v2, 1.0f, 1.0f) ? PASS : FAIL));
	printf("StaticRectToStaticRect Collision Touch: %s\n", (StaticRectToStaticRect(v1, 2.0f, 2.0f, v2, 2.0f, 2.0f) ? PASS : FAIL));
	printf("StaticRectToStaticRect Collision Intersect: %s\n", (StaticRectToStaticRect(v1, 3.0f, 3.0f, v2, 3.0f, 3.0f) ? PASS : FAIL));
}
