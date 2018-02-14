#pragma once

#ifndef GAME_CONFIG_H
#define GAME_CONFIG_H

#include <string>
#include "JsonReader.h"

using json = nlohmann::json;

class GameConfig
{
private:
	std::string m_texturesDir;
	std::string m_levelFilesDir;
	std::string m_prefabsDir;
	std::string m_sfxDir;
	std::string m_shadersDir;
	std::string m_roomFilesDir;

public:
	GameConfig();
	~GameConfig();
	GameConfig(const GameConfig &) = delete;
	void operator=(const GameConfig &) = delete;

	void LoadConfig(std::string);

	std::string TexturesDir() const { return m_texturesDir; }
	std::string LevelFilesDir() const { return m_levelFilesDir; }
	std::string PrefabsDir() const { return m_prefabsDir; }
	std::string SFXDir() const { return m_sfxDir; }
	std::string ShadersDir() const { return m_shadersDir; }
	std::string RoomFilesDir() const { return m_roomFilesDir; }
};

#endif