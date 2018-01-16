/* Start Header -------------------------------------------------------
Copyright (C) 2017 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Math2D.h
Purpose: Provides 2D math functionality such as collision
Language: C++
Platform: Windows 8.1 / 10.0.15063.0  or  GCC 7.1.0
Project: CS529_holdenprofit_
Author: Holden Profit, holden.profit, 42833
Creation date: 9/29/17
- End Header --------------------------------------------------------*/

#pragma once

#ifndef MATH2D_H
#define MATH2D_H

#include "Vector2D.h"
#include "LineSegment2D.h"

/*
This function checks if the point P is colliding with the circle whose
center is "Center" and radius is "Radius"
*/
bool StaticPointToStaticCircle(const Vector2D &pos, const Vector2D &center, float radius);

/*
This function checks if the point Pos is colliding with the rectangle
whose center is Rect, width is "Width" and height is Height
*/
bool StaticPointToStaticRect(const Vector2D &pos, const Vector2D &rect, const float width, const float height);

/*
This function checks for collision between 2 circles.
Circle0: Center is Center0, radius is "Radius0"
Circle1: Center is Center1, radius is "Radius1"
*/
bool StaticCircleToStaticCircle(const Vector2D &center0, const float radius0, const Vector2D &center1, const float radius1);

/*
This functions checks if 2 rectangles are colliding
Rectangle0: Center is pRect0, width is "Width0" and height is "Height0"
Rectangle1: Center is pRect1, width is "Width1" and height is "Height1"
*/
bool StaticRectToStaticRect(const Vector2D &rect0, const float width0, const float height0, const Vector2D &rect1, const float width1, const float height1);

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
float StaticPointToStaticLineSegment(const Vector2D& P, const LineSegment2D& LS);

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
float AnimatedPointToStaticLineSegment(const Vector2D& Ps, const Vector2D& Pe, const LineSegment2D& LS, Vector2D& Pi);

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
float AnimatedCircleToStaticLineSegment(const Vector2D& Ps, const Vector2D& Pe, const float Radius, const LineSegment2D& LS, Vector2D& Pi);

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
float ReflectAnimatedPointOnStaticLineSegment(const Vector2D& Ps, const Vector2D& Pe, const LineSegment2D& LS, Vector2D& Pi, Vector2D& R);

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
float ReflectAnimatedCircleOnStaticLineSegment(const Vector2D& Ps, const Vector2D& Pe, const float Radius, const LineSegment2D& LS, Vector2D& Pi, Vector2D& R);

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
float AnimatedPointToStaticCircle(const Vector2D& Ps, const Vector2D& Pe, const Vector2D& Center, const float Radius, Vector2D& Pi);

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
float ReflectAnimatedPointOnStaticCircle(const Vector2D& Ps, const Vector2D& Pe, const Vector2D& Center, const float Radius, Vector2D& Pi, Vector2D& R);

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
float AnimatedCircleToStaticCircle(const Vector2D& ACenterS, const Vector2D& ACenterE, const float ARadius, const Vector2D& SCenter, const float SRadius, Vector2D& Pi);

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
float ReflectAnimatedCircleOnStaticCircle(const Vector2D& ACenterS, const Vector2D& ACenterE, const float ARadius, const Vector2D& SCenter, const float SRadius, Vector2D& Pi, Vector2D& R);

void StaticCollisionTests();
#endif