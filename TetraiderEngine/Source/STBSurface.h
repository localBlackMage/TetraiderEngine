/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: STBSurface.h
Purpose: Structs for loading STB_Surfaces
Language: C++
Project: GAM541
Author: Holden Profit
Creation date: 1/17/18
- End Header --------------------------------------------------------*/

#pragma once

#ifndef STB_SURFACE_H
#define STB_SURFACE_H

#include <glew.h>

// Data captured by stbi_load
struct STB_Surface {
	unsigned char * data;
	int width;
	int height;
	int channels;
	bool hasAlpha;
	float frameWidth; 
	float frameHeight;
	int rows;
	int columns;
};

struct SurfaceTextureBuffer {
	explicit SurfaceTextureBuffer(STB_Surface * _surface, GLuint _bufferId) :
		surface(_surface), bufferId(_bufferId) {};
	STB_Surface * surface;
	GLuint bufferId;
};

#endif