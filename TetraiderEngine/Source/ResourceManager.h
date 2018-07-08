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

enum Sound_Category { SFX, SONG, CATEGORY_COUNT };
struct Font_Size {
	std::string name;
	int size;

	explicit Font_Size(std::string _name, int _size) : name(_name), size(_size) {}

	bool operator==(const Font_Size& other) const {
		return (name == other.name && size == other.size);
	}
};

struct Font_SizeHasher {
	std::size_t operator() (const Font_Size& k) const {
		// Compute individual hash values for first,
		// second and combine them using XOR
		// and bit shifting:

		return ((std::hash<std::string>()(k.name) ^ (std::hash<int>()(k.size) << 1)) >> 1);
	}
};

class ResourceManager
{
private:
	struct TextureInfo {
		std::string filename;
		bool hasAlpha;
	};

	Assimp::Importer m_importer;
	
	DebugLineMesh * m_pDebugLineMesh;
	std::unordered_map<std::string, std::shared_ptr<Mesh> > m_meshes;
	std::unordered_map<std::string, std::shared_ptr<Scene> > m_scenes;
	std::unordered_map< Font_Size, FontInfo*, Font_SizeHasher> m_fonts;
	std::unordered_map<std::string, SurfaceTextureBuffer * > m_textures;
	std::map<std::string, json*> m_prefabs;
	std::vector<std::string> m_prefabStrings;

	std::unordered_map<std::string, FMOD::Sound*> m_Sounds[CATEGORY_COUNT];
	
	void _RenderFont(FontInfo& fontInfo);
	FontInfo* _LoadFont(const Font_Size& font_size);

	GLuint _CreateTextureBuffer(const SDL_Surface * const sdlSurface, int alphaMode);
	SurfaceTextureBuffer* _LoadTexture(std::string textureName);
	std::shared_ptr<Mesh> _CreateMesh(const std::string& meshName);
	std::shared_ptr<Mesh> _CreateMesh(const std::string& meshName, const aiMesh* aiMeshPtr);
	std::shared_ptr<Scene> _CreateScene(const std::string& sceneName, unsigned short meshCount);
public:
	ResourceManager();
	~ResourceManager();
	ResourceManager(const ResourceManager &) = delete;
	void operator=(const ResourceManager &) = delete;

	bool Init();

	DebugLineMesh* GetDebugLineMesh();
	std::shared_ptr<Mesh> LoadInternalMesh(const std::string& meshName);
	std::shared_ptr<Mesh> LoadMesh(const std::string& meshName);
	std::shared_ptr<Mesh> GetInternalMesh(const std::string& meshName);
	std::shared_ptr<Mesh> GetMesh(const std::string& meshName);

	std::shared_ptr<Scene> LoadScene(const std::string& sceneName);
	std::shared_ptr<Scene> GetScene(const std::string& meshSceneName);
	void UnloadMesh(const std::string& meshName);

	FontInfo* GetFont(const Font_Size& font_size);

	SurfaceTextureBuffer* GetTexture(const std::string& textureName);
	void UnloadTexture(const std::string& textureName);

	void UnloadAll();
	void LoadPrefabFiles();
	json* GetPrefabFile(const std::string& path);
	const std::vector<std::string>& GetPrefabStrings() { return m_prefabStrings; }

	void Load(Sound_Category type, const std::string& path);
	void LoadSFX(const std::string& path);
	void LoadSong(const std::string& path);
	FMOD::Sound* GetSFX(const std::string& path, Sound_Category type);
	//void Set3DListener();
};

#endif