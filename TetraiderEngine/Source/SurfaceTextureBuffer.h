/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: SurfaceTextureBuffer.h
Purpose: Structs containing SDL_Surface data and an OpenGL bufferID
Language: C++
Project: GAM541
Author: Holden Profit
Creation date: 1/17/18
- End Header --------------------------------------------------------*/

#pragma once

#ifndef SURFACE_TEXTURE_BUFFER_H
#define SURFACE_TEXTURE_BUFFER_H

#include <glew.h>
#include <SDL.h>

struct SurfaceTextureBuffer {
	explicit SurfaceTextureBuffer(SDL_Surface * _surface, GLuint _bufferId, int _alphaMode) :
		surface(_surface), bufferId(_bufferId), alphaMode(_alphaMode){};

	int alphaMode;
	SDL_Surface * surface;
	GLuint bufferId;
};

#endif