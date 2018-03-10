#pragma once

#ifndef GAME_CONFIG_H
#define GAME_CONFIG_H

//#include <string>
//#include <utility>
//#include "JsonReader.h"
//
//using json = nlohmann::json;

class GameConfig
{
private:
	std::string m_texturesDir;
	std::string m_levelFilesDir;
	std::string m_prefabsDir;
	std::string m_sfxDir;
	std::string m_shadersDir;
	std::string m_roomFilesDir;
	std::string m_fontsDir;
	
	bool m_soundsMute;

	unsigned short m_cellWidth, m_cellHeight;	// Cell dimensions in pixels (ex. 96x96)
	unsigned short m_roomWidth, m_roomHeight;	// Room dimensions in cells (ex. 12x8)
	unsigned short m_roomsX, m_roomsY;			// Number of rooms in the x and y directions on the grid

	int m_screenWidth, m_screenHeight;
public:
	GameConfig();
	~GameConfig();
	GameConfig(const GameConfig &) = delete;
	void operator=(const GameConfig &) = delete;

	void LoadConfig(std::string);

	inline std::string TexturesDir() const { return m_texturesDir; }
	inline std::string LevelFilesDir() const { return m_levelFilesDir; }
	inline std::string PrefabsDir() const { return m_prefabsDir; }
	inline std::string SFXDir() const { return m_sfxDir; }
	inline std::string ShadersDir() const { return m_shadersDir; }
	inline std::string RoomFilesDir() const { return m_roomFilesDir; }
	inline std::string FontsDir() const { return m_fontsDir; }

	inline bool GetsoundsMute(){ return m_soundsMute; }
	inline unsigned short CellWidth() const { return m_cellWidth; }
	inline unsigned short CellHeight() const { return m_cellHeight; }
	inline unsigned short RoomWidth() const { return m_roomWidth; }
	inline unsigned short RoomHeight() const { return m_roomHeight; }
	inline unsigned int LevelWidth() const { return int(m_cellWidth) * int(m_roomWidth) * int(m_roomsX); }
	inline unsigned int LevelHeight() const { return int(m_cellHeight) * int(m_roomHeight) * int(m_roomsY); }
};

#endif