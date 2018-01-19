/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: ResourceManager.h
Purpose: Loads Meshes and other commonly used game resources into
memory and manages their instances as well as returns references to them.
Language: C++
Project: GAM541
Author: Holden Profit
Creation date: 1/17/18
- End Header --------------------------------------------------------*/

#pragma once

#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <map>
#include <utility>
#include "Mesh.h"
#include "STBSurface.h"

// TODO: Remove this
struct TextureInfo {
	bool hasAlpha;
	float frameWidth, frameHeight;
	int rows, cols;
};

class ResourceManager
{
private:
	std::map<std::string, Mesh*> m_meshes;
	std::map<std::string, SurfaceTextureBuffer * > m_textures;

	ResourceManager();
	~ResourceManager();

	//struct TextureInfo {
	//	bool hasAlpha;
	//	float frameWidth, frameHeight;
	//	int rows, cols;
	//};

	GLuint _CreateTextureBuffer(const STB_Surface * const stbSurface);
public:
	ResourceManager(const ResourceManager &) = delete;
	void operator=(const ResourceManager &) = delete;

	static ResourceManager& GetInstance()
	{
		static ResourceManager instance;
		return instance;
	}

	Mesh * LoadMesh(std::string meshName);
	Mesh * GetMesh(std::string meshName);
	void UnloadMesh(std::string meshName);

	SurfaceTextureBuffer * LoadTexture(std::string textureName, std::string fileName, bool hasAlpha);
	SurfaceTextureBuffer * GetTexture(const std::string textureName);
	void UnloadTexture(std::string textureName);

	void LoadTexturesFromFile(std::string fileName);

	void UnloadAll();
};

#endif