#ifndef MATH_FUNCTIONS_H
#define MATH_FUNCTIONS_H

#include "Vector3D.h"
#include "Vector2D.h"

struct Point {
	float x, y;
};

Vector3D Lerp(const Vector3D& vectorA, const Vector3D& vectorB, float t);
Vector2D Lerp(const Vector2D& vectorA, const Vector2D& vectorB, float t);
float Lerp(float valueA, float valueB, float t);
float SinWave(float amplitude, float frequency, float phaseShift, float time);
float CosWave(float amplitude, float frequency, float phaseShift, float time);
float RandomFloat(float min, float max);
int RandomInt(int min, int max);

unsigned int GetPascalEntry(unsigned short degree, unsigned short index);

#endif