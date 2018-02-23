#include "MathFunctions.h"
#include <cmath>
#include <stdlib.h>

Vector3D Lerp(const Vector3D& vectorA, const Vector3D& vectorB, float t) {
	return Vector3D(Lerp(vectorA.x, vectorB.x, t), Lerp(vectorA.y, vectorB.y, t), Lerp(vectorA.z, vectorB.z, t), Lerp(vectorA.w, vectorB.w, t));
}

Vector2D Lerp(const Vector2D& vectorA, const Vector2D& vectorB, float t) {
	return Vector2D(Lerp(vectorA.x, vectorB.x, t), Lerp(vectorA.y, vectorB.y, t), Lerp(vectorA.w, vectorB.w, t));
}

float Lerp(float valueA, float valueB, float t) {
	if (t > 1)
		t = 1;
	else if (t < 0)
		t = 0;

	return valueA*(1-t) + (t)* valueB;
}

float SinWave(float amplitude, float frequency, float phaseShift, float time) {
	return amplitude*sinf(frequency*time + phaseShift);
}

float CosWave(float amplitude, float frequency, float phaseShift, float time) {
	return amplitude*cosf(frequency*time + phaseShift);
}

float RandomFloat(float min, float max) {
	srand(TETRA_FRAMERATE.GetElapsedTime());
	return static_cast<float>(rand()) / max + min;
}

int RandomInt(int min, int max) {
	srand(TETRA_FRAMERATE.GetElapsedTime());
	return rand() / max + min;
}