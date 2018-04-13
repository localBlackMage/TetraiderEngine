/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: MathDefs.h
Purpose: Provides several predefined mathematical definitions
Language: C++
Project: GAM541
Author: Holden Profit
Creation date: 1/12/18
- End Header --------------------------------------------------------*/

//#pragma once

#ifndef MATH_DEFINES_H
#define MATH_DEFINES_H

#define TEST_MODE 1
#define PASS "PASS"
#define FAIL "!!!!! FAIL !!!!!"
#define EPSILON 0.0001
#define PI       3.14159265358979323846f   // pi
#define PI_2     1.57079632679489661923f   // pi/2
#define PI_4     0.785398163397448309616f  // pi/4

#define DEG_TO_RAD (3.14159265358979323846f / 180.0f)
#define RAD_TO_DEG (180.0f / 3.14159265358979323846f)

#define TETRA_RAND_MAX 0x7FFF

struct Point {
	float x, y;
};

typedef std::vector<Point> ControlPoints;

struct InterpolationItem {
	ControlPoints points;	// Control Points for bezier curve interpolation between start and end
	float amplitude;		// How much of an offset to give the result from the curve
};

#endif
