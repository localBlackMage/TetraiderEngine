#ifndef MATH_FUNCTIONS_H
#define MATH_FUNCTIONS_H

#include "Vector3D.h"
#include "Vector2D.h"

Vector3D Lerp(const Vector3D& vectorA, const Vector3D& vectorB, float t);
Vector2D Lerp(const Vector2D& vectorA, const Vector2D& vectorB, float t);
float Lerp(float valueA, float valueB, float t);

#endif