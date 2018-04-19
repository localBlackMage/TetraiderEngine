/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#pragma once

#ifndef TETRA_COLOR_H
#define TETRA_COLOR_H

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