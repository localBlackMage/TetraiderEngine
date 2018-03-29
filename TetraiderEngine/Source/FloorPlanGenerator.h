#pragma once

#ifndef FLOOR_PLAN_GENERATOR_H
#define FLOOR_PLAN_GENERATOR_H

const float MAX_DISTANCE = 10000000.f;

enum RoomConnections {
	LEFT = 1,
	UP = 2,
	RIGHT = 4,
	DOWN = 8,

	LEFT_UP = 3,
	LEFT_RIGHT = 5,
	UP_RIGHT = 6,
	LEFT_UP_RIGHT = 7,
	LEFT_DOWN = 9,
	UP_DOWN = 10,
	LEFT_UP_DOWN = 11,
	RIGHT_DOWN = 12,
	LEFT_RIGHT_DOWN = 13,
	UP_RIGHT_DOWN = 14,
	ALL = 15
};

std::ostream& operator<<(std::ostream& out, const RoomConnections& rc);

enum class RoomType {
	BOSS, INTERESTING, SPAWN, DEAD, ALIVE
};

std::ostream& operator<<(std::ostream& out, const RoomType& rt);

struct RoomNode {
	explicit RoomNode(RoomType type, short id, short row, short col) : 
		m_type(type), m_id(id), m_row(row), m_col(col), m_cost(MAX_DISTANCE), m_parent(nullptr){}
	short m_id, m_row, m_col;
	RoomConnections m_ConnectionType;
	RoomNode* m_Neighbors[4]; // 0 = left, 1 = up, 2 = right, 3 = down
	RoomType m_type;
	float m_cost;
	RoomNode* m_parent;
	Vector3D m_position;

	bool operator==(const RoomNode& rhs) { return m_id == rhs.m_id; }
	bool operator!=(const RoomNode& rhs) { return m_id != rhs.m_id; }
	bool operator() (const RoomNode& lhs, const RoomNode& rhs);
};

typedef std::vector<json*> RoomFiles;
typedef std::unordered_map<unsigned short, RoomFiles> DifficultyMap;

class FloorPlanGenerator {
protected:
	RoomNode** m_roomNodes;
	RoomNode* m_spawnNode;
	unsigned short m_cols, m_rows;	// How many rooms wide and tall a floor will be (ex. 5x5)
	unsigned short m_maxRowIdx, m_maxColIdx;
	int m_seed;
	unsigned short m_difficulty;

	std::unordered_map<RoomConnections, DifficultyMap > m_roomFiles;

	bool _A_Star(RoomNode& start, RoomNode& goal);

	/* Viable meaning the GO has a C_SpawnOnHealthZero component and is a T_Obstacle */
	bool _IsGOAViableObject(GameObject* pGO);
	/* Viable meaning the GO has a C_SpawnOnHealthZero component and is a T_Enemy */
	bool _IsGOAViableEnemy(GameObject* pGO);
	void _ResetNodeDistancesAndParents();
	void _ConnectNeighbors();
	std::vector<RoomNode*> _SelectNodes(bool bossLevel);
	void _ConnectSelectedNodes(std::vector<RoomNode*>& selectedNodes);
	/* First selectively unsets neighbors, then sets connection type */
	void _SetRoomConnectionTypes();
	void _UnsetNodeNeigbors(RoomNode& node);
	json* _GetRoomJsonForDifficulty(RoomConnections connection);
	json* _GetRoomJsonForSpawn(RoomConnections connection);
public:
	FloorPlanGenerator();
	~FloorPlanGenerator();
	FloorPlanGenerator(const FloorPlanGenerator &) = delete;
	void operator=(const FloorPlanGenerator &) = delete;
	
	void GenerateRoomNodes(unsigned short cols, unsigned short rows, unsigned short difficulty = 1);
	void GenerateFloorPlan(int seed = -1, bool bossLevel = false);
	void ResetAllNodes();
	void PrintFloorPlan();
	void GenerateLevelFromFloorPlan();

	RoomConnections GetRoomConnectionType(const std::string connectionType);
	
	void LoadRoomFiles();

	unsigned int FloorWidthPixels() const;
	unsigned int FloorHeightPixels() const;

	inline int Seed() const { return m_seed; }
};

#endif

