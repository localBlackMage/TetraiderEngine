/* Start Header -------------------------------------------------------
Copyright (C) 2017 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Math3D.h
Purpose: Provides 3D math functionality such as collision
Language: C++
Platform: Windows 8.1 / 10.0.15063.0  or  GCC 7.1.0
Project: CS529_holdenprofit_
Author: Holden Profit, holden.profit, 42833
Creation date: 11/03/17
- End Header --------------------------------------------------------*/

#pragma once

#ifndef MATH3D_H
#define MATH3D_H

#include "Vector3D.h"
#include "LineSegment3D.h"
#include "Rect3D.h"
#include "CollisionResult.h"

float GetRandomFloat(float lo, float hi);

bool FloatEquals(float value, float targetValue);

/*
Static on Static methods are a "are these two objects colliding this very instant" style methods.

They will only return a true or false value.
*/
#pragma region Static-On-Static Collisions

#pragma region Static Point
/*
This function checks if the point P is colliding with the circle whose
center is "Center" and radius is "Radius"
*/
bool StaticPointToStaticCircle(const Vector3D &pos, const Vector3D &center, float radius);

/*
This function checks if the point Pos is colliding with the rectangle
whose center is Rect
*/
bool StaticPointToStaticRect(const Vector3D &pos, const Vector3D &rect, const float halfWidth, const float halfHeight);

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
float StaticPointToStaticLineSegment(const Vector3D& P, const LineSegment3D& LS);

#pragma endregion

#pragma region Static Circle

/*
This function checks for collision between 2 circles.
Circle0: Center is Center0, radius is "Radius0"
Circle1: Center is Center1, radius is "Radius1"
*/
bool StaticCircleToStaticCircle(const Vector3D &center0, const float radius0, const Vector3D &center1, const float radius1);

#pragma endregion

#pragma region Static Rect

/*
This functions checks if 2 rectangles are colliding
Rectangle0: Center is pRect0, halfWidth is "halfWidth0" and halfHeight is "halfHeight0"
Rectangle1: Center is pRect1, halfHeight is "halfHeight1" and halfHeight is "halfHeight1"
*/
bool StaticRectToStaticRect(const Vector3D &rect0, const float halfWidth0, const float halfHeight0, const Vector3D &rect1, const float halfWidth1, const float halfHeight1);

bool StaticRectToStaticCircle(const Vector3D & rectCenter, const float halfWidth, const float halfHeight, const Rect3D& rect, const Vector3D circle, const float radius);

#pragma endregion

#pragma endregion

/*
Animated on Static methods will find if an animated object will collide with a non-moving object during a period of time and where.

They will return a CollisionResult value.
*/
#pragma region Animated-On-Static Collisions

#pragma region Animated Point
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
CollisionResult AnimatedPointToStaticCircle(const Vector3D& Ps, const Vector3D& Pe, const Vector3D& Center, const float Radius);

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
CollisionResult AnimatedPointToStaticLineSegment(const Vector3D& Ps, const Vector3D& Pe, const LineSegment3D& LS);

#pragma endregion

#pragma region Animated Circle
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
CollisionResult AnimatedCircleToStaticCircle(const Vector3D& ACenterS, const Vector3D& ACenterE, const float ARadius, const Vector3D& SCenter, const float SRadius);

CollisionResult AnimatedCircleToStaticRect(const Vector3D& CStart, const Vector3D& CEnd, const float Radius, const Vector3D& RectCenter, const float halfWidth, const float halfHeight, const Rect3D& rect);

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
CollisionResult AnimatedCircleToStaticLineSegment(const Vector3D& Ps, const Vector3D& Pe, const float Radius, const LineSegment3D& LS);
#pragma endregion

#pragma endregion

/*
Animated on Animated methods will find if two animated objects will collide during a period of time and where.

They will return a CollisionResult value.
*/
#pragma region Animated-On-Animated Collisions

CollisionResult AnimatedCircleToAnimatedRect(const Vector3D& Start0, const Vector3D& End0, const float Radius, const Vector3D& Start1, const Vector3D& End1, const float halfWidth, const float halfHeight, const Rect3D & rect);

CollisionResult AnimatedCircleToAnimatedCircle(const Vector3D& Start0, const Vector3D& End0, const float Radius0, const Vector3D& Start1, const Vector3D& End1, const float Radius);

#pragma endregion

/*
Reflection methods will, given a CollisionResult and two sets of start/end points, reflect the movement represented by the start/end points at the CollisionResult's pointOfImpact
*/
#pragma region Reflection Methods

#pragma region Point
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
Vector3D ReflectAnimatedPointOnStaticCircle(const Vector3D& Ps, const Vector3D& Pe, const Vector3D& Center, const CollisionResult& CR);

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
Vector3D ReflectAnimatedPointOnStaticLineSegment(const Vector3D& Pe, const LineSegment3D& LS, const CollisionResult& CR);
#pragma endregion

#pragma region Circle
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
Vector3D ReflectAnimatedCircleOnStaticLineSegment(const Vector3D& Pe, const LineSegment3D& LS, const CollisionResult& CR);

Vector3D ReflectAnimatedCircleOnStaticRect(const Vector3D& Pe, const Vector3D & Pi, const Vector3D& RectCenter, const float halfWidth, const float halfHeight, const Rect3D & rect);

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
Vector3D ReflectAnimatedCircleOnStaticCircle(const Vector3D& ACenterS, const Vector3D& ACenterE, const float ARadius, const Vector3D& SCenter, const float SRadius, const CollisionResult& CR);
#pragma endregion

#pragma endregion


#pragma region Push Methods
#pragma region Circle
Vector3D PushCircleFromCircle(const float Radius, const float OtherRadius, const Vector3D & poi, const CollisionResult & CR);

Vector3D PushCircleFromRect(const Vector3D & CircleEnd, const float Radius,
	const Vector3D& RectCenter, const float halfWidth, const float halfHeight, const CollisionResult& CR);

#pragma endregion

#pragma endregion
#endif