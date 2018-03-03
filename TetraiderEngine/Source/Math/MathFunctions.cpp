#include "MathFunctions.h"
#include <cmath>
#include <ctime>
#include <math.h>
#include <stdlib.h>
#include <unordered_map>
#include <vector>

std::unordered_map<unsigned short, std::vector<unsigned int> > pascalTriangle;

static unsigned int _Factorial(unsigned int num)
{
	if (num == 0)	return 1;
	return (num == 1 ? num : num * _Factorial(num - 1));
}

static void _GeneratePascalRow(unsigned short degree) {
	std::vector<unsigned int> row;
	row.reserve(degree + 1);
	row.push_back(1);
	unsigned int dF = _Factorial(degree);
	// (d, i) = (i!) / (d! * (i - d)!)
	for (int i = 1; i < degree + 1; ++i) {
		row.push_back(dF / (_Factorial(i) * _Factorial(degree - i)));
	}
	pascalTriangle[degree] = row;
}

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

	return valueA*(1.f-t) + (t)* valueB;
}

float SinWave(float amplitude, float frequency, float phaseShift, float time) {
	return amplitude*sinf(frequency*time + phaseShift);
}

float CosWave(float amplitude, float frequency, float phaseShift, float time) {
	return amplitude*cosf(frequency*time + phaseShift);
}

float RandomFloat(float min, float max) {
	return static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * (max-min) + min;
}

int RandomInt(int min, int max) {
	return rand() % (max-min) + min;
}

unsigned int GetPascalEntry(unsigned short degree, unsigned short index)
{
	if (pascalTriangle.find(degree) == pascalTriangle.end())
		_GeneratePascalRow(degree);

	return pascalTriangle[degree][index];
}

static float BernsteinFormula(const unsigned short& d, const float& t, const float& tPow, const unsigned short& i) {
	float dci = float(GetPascalEntry(d, i));
	return std::powf((1.f - t), float(d - i)) * tPow * dci;
}

float BezierInterpolation(const std::vector<Point>& points, const float& t) {
	if (points.size() == 0)	return 0.f;

	unsigned short d = (unsigned short)points.size() - 1;
	float tPow = 1.f;
	float returnValue = points[0].y * BernsteinFormula(d, t, tPow, 0);
	for (unsigned short i = 1; i < points.size(); i++) {
		tPow *= t;
		returnValue += points[i].y * BernsteinFormula(d, t, tPow, i);
	}
	return returnValue;
}
