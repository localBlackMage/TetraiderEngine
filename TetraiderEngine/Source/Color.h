#pragma once

#ifndef TETRA_COLOR_H
#define TETRA_COLOR_H

#include <glew.h>

struct Color {
	Color() : r(0), g(0), b(0), a(0) {}
	Color(GLubyte _r, GLubyte _g, GLubyte _b, GLubyte _a) : r(_r), g(_g), b(_b), a(_a) {}
	GLubyte r, g, b, a;

	GLubyte operator[] (const int& idx) const {
		return *(&r + idx);
	}

	GLubyte& operator[] (const int& idx) {
		return *(&r + idx);
	}
};

#endif