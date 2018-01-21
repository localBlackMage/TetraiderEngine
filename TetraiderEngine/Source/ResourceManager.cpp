#include "ResourceManager.h"
#include <iostream>
#include "JsonReader.h"
#include "GameConfig.h"
#define STB_IMAGE_IMPLEMENTATION
#include "External\stb_image.h"

ResourceManager::ResourceManager()
{
	Mesh * quad = LoadMesh("quad");

	quad->AddTriangle(
		-0.5f, -0.5f, 0.0f, .0f, 1.f, 0xFFFFFFFF,
		0.5f, -0.5f, 0.0f, 1.f, 1.f, 0xFFFFFFFF,
		-0.5f, 0.5f, 0.0f, .0f, .0f, 0xFFFFFFFF
	);
	quad->AddTriangle(
		0.5f, -0.5f, 0.0f, 1.f, 1.f, 0xFFFFFFFF,
		0.5f, 0.5f, 0.0f, 1.f, .0f, 0xFFFFFFFF,
		-0.5f, 0.5f, 0.0f, .0f, .0f, 0xFFFFFFFF
	);

	quad->FinishMesh();
}

ResourceManager::~ResourceManager() 
{
	for (auto comp : m_meshes) {
		if (comp.second)
			delete comp.second;
	}
	m_meshes.clear();

	for (auto comp : m_textures) {
		if (comp.second) {
			stbi_image_free(comp.second->surface->data);
			delete comp.second->surface;
			delete comp.second;
		}
	}
	m_textures.clear();
}

GLuint ResourceManager::_CreateTextureBuffer(const STB_Surface * const stbSurface)
{
	GLuint textureBuffer;
	glGenTextures(1, &textureBuffer);
	glBindTexture(GL_TEXTURE_2D, textureBuffer);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glTexImage2D(GL_TEXTURE_2D, 0,
		stbSurface->hasAlpha ? GL_RGBA : GL_RGB,
		stbSurface->width, stbSurface->height, 0,
		stbSurface->hasAlpha ? GL_RGBA : GL_RGB,
		GL_UNSIGNED_BYTE, stbSurface->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	return textureBuffer;
}

ResourceManager::TextureInfo ResourceManager::_LoadTextureInfoFile(std::string textureInfoFilePath, std::string texturesDir)
{
	json j = JsonReader::OpenJsonFile(texturesDir + textureInfoFilePath + ".json");
	TextureInfo info;
	if (j.is_object()) {
		info.filename = texturesDir + JsonReader::ParseString(j, "filename");
		info.frameWidth = JsonReader::ParseFloat(j, "frameWidth");
		info.frameHeight = JsonReader::ParseFloat(j, "frameHeight");
		info.rows = JsonReader::ParseInt(j, "rows");
		info.cols = JsonReader::ParseInt(j, "columns");
		info.hasAlpha = JsonReader::ParseBool(j, "alpha");
	}
	return info;
}

SurfaceTextureBuffer * ResourceManager::_LoadTexture(std::string textureName)
{
	SurfaceTextureBuffer * stbuff = m_textures[textureName];

	if (stbuff)
		return stbuff;

	STB_Surface * surface = new STB_Surface();
	GameConfig& gameConfig = GameConfig::GetInstance();
	if (surface) {
		ResourceManager::TextureInfo info = _LoadTextureInfoFile(textureName, gameConfig.m_texturesDir);

		surface->hasAlpha = info.hasAlpha;
		surface->data = stbi_load(info.filename.c_str(),
			&surface->width, &surface->height,
			&surface->channels,
			info.hasAlpha ? STBI_rgb_alpha : STBI_rgb);

		if (!surface->data) {
			std::cerr << "Failed to read file: " << textureName << std::endl;
			return nullptr;
		}
		/*surface->frameWidth = info.frameWidth / surface->width;
		surface->frameHeight = info.frameHeight / surface->height;
		surface->rows = info.rows;
		surface->columns = info.cols;*/
		GLuint bufferId = _CreateTextureBuffer(surface);

		stbuff = new SurfaceTextureBuffer(surface, bufferId);
		m_textures[textureName] = stbuff;
		return stbuff;
	}
	else {
		std::cerr << "Failed to create texture: " << textureName << std::endl;
		return nullptr;
	}
}


Mesh * ResourceManager::LoadMesh(std::string meshName)
{
	Mesh * mesh = m_meshes[meshName];

	if (mesh)
		return mesh;

	mesh = new Mesh();
	if (mesh)
		m_meshes[meshName] = mesh;

	return mesh;
}

Mesh * ResourceManager::GetMesh(std::string meshName)
{
	Mesh * mesh = m_meshes[meshName];

	if (mesh)
		return mesh;
	else {
		std::cerr << meshName << " has not yet been created." << std::endl;
		return nullptr;
	}
}

void ResourceManager::UnloadMesh(std::string meshName)
{
	if (m_meshes[meshName]) {
		delete m_meshes[meshName];
		m_meshes.erase(meshName);
	}
}

SurfaceTextureBuffer * ResourceManager::GetTexture(const std::string textureName)
{
	if (textureName == "") return nullptr;

	SurfaceTextureBuffer * stbuff = m_textures[textureName];

	if (stbuff)
		return stbuff;
	else
		return _LoadTexture(textureName);
}

void ResourceManager::UnloadTexture(std::string textureName)
{
	if (m_textures[textureName]) {
		stbi_image_free(m_textures[textureName]->surface->data);
		delete m_textures[textureName]->surface;
		delete m_textures[textureName];

		m_textures.erase(textureName);
	}
}

/*
void ResourceManager::LoadTexturesFromFile(std::string fileName)
{
	try {
		json j = JsonReader::OpenJsonFile(fileName);

		if (j.is_object()) {
			for (json::iterator it = j.begin(); it != j.end(); ++it) {
				std::string key = it.key();
				TextureInfo info;
				info.frameWidth = JsonReader::ParseFloat(j, key, "frameWidth");
				info.frameHeight = JsonReader::ParseFloat(j, key, "frameHeight");
				info.rows = JsonReader::ParseInt(j, key, "rows");
				info.cols = JsonReader::ParseInt(j, key, "columns");
				info.hasAlpha = JsonReader::ParseBool(j, key, "alpha");
				LoadTexture(key, JsonReader::ParseString(j, key, "filename"), info.hasAlpha);
			}
		}
	}
	catch (const json::parse_error& ex) {
		std::cerr << ex.what() << std::endl;
	}
}
*/

void ResourceManager::UnloadAll()
{
	for (auto comp : m_meshes) {
		if (comp.second)
			delete comp.second;
	}
	m_meshes.clear();

	for (auto comp : m_textures) {
		if (comp.second) {
			stbi_image_free(comp.second->surface->data);
			delete comp.second->surface;
			delete comp.second;
		}
	}
	m_textures.clear();
}
