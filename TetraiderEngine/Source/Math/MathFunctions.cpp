#include "MathFunctions.h"

Vector3D Lerp(const Vector3D& vectorA, const Vector3D& vectorB, float t) {
	return Vector3D(Lerp(vectorA.x, vectorB.x, t), Lerp(vectorA.y, vectorB.y, t), Lerp(vectorA.z, vectorB.z, t));
}

Vector2D Lerp(const Vector2D& vectorA, const Vector2D& vectorB, float t) {
	return Vector2D(Lerp(vectorA.x, vectorB.x, t), Lerp(vectorA.y, vectorB.y, t));
}

float Lerp(float valueA, float valueB, float t) {
	if (t > 1)
		t = 1;
	else if (t < 0)
		t = 0;

	return valueA*(1-t) + (t)* valueB;
}