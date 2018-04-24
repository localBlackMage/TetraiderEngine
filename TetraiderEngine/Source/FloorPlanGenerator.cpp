/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

#define ROOM_CONNECTION_TYPE "ROOM_CONNECTION_TYPE"
#define ROOM_DIFFICULTY "DIFFICULTY"
#define ROOM_BOSS_LEVEL "BOSS_ROOM"
#define ROOM_SHOP_LEVEL "SHOP_ROOM"

static std::unordered_map<std::string, RoomConnections> ROOM_CONNECTION_STRINGS_TO_TYPES = {
	{ "LEFT", RoomConnections::LEFT },
	{ "UP", RoomConnections::UP },
	{ "LEFT_UP", RoomConnections::LEFT_UP },
	{ "RIGHT", RoomConnections::RIGHT },
	{ "LEFT_RIGHT", RoomConnections::LEFT_RIGHT },
	{ "UP_RIGHT", RoomConnections::UP_RIGHT },
	{ "LEFT_UP_RIGHT", RoomConnections::LEFT_UP_RIGHT },
	{ "DOWN", RoomConnections::DOWN },
	{ "LEFT_DOWN", RoomConnections::LEFT_DOWN },
	{ "UP_DOWN", RoomConnections::UP_DOWN },
	{ "LEFT_UP_DOWN", RoomConnections::LEFT_UP_DOWN },
	{ "RIGHT_DOWN", RoomConnections::RIGHT_DOWN },
	{ "LEFT_RIGHT_DOWN", RoomConnections::LEFT_RIGHT_DOWN },
	{ "UP_RIGHT_DOWN", RoomConnections::UP_RIGHT_DOWN },
	{ "ALL", RoomConnections::ALL }
};

static std::string ROOM_CONNECTION_STRINGS[15] = {
	"LEFT",
	"UP",
	"LEFT_UP",
	"RIGHT",
	"LEFT_RIGHT",
	"UP_RIGHT",
	"LEFT_UP_RIGHT",
	"DOWN",
	"LEFT_DOWN",
	"UP_DOWN",
	"LEFT_UP_DOWN",
	"RIGHT_DOWN",
	"LEFT_RIGHT_DOWN",
	"UP_RIGHT_DOWN",
	"ALL"
};

// small helper enum to remember directions
enum NEIGHBOR {
	N_LEFT = 0, N_UP, N_RIGHT, N_DOWN
};

const float WEIGHT = 1.5f;

// Heuristic - this can be changed as desired, just can't exceed real distance
static float _Heuristic(RoomNode& a, RoomNode& b) {
	short x = abs(a.m_row - b.m_row);
	short y = abs(a.m_col - b.m_col);
	return float(x + y) * WEIGHT;
}

#pragma region Private Methods

void FloorPlanGenerator::_PushToOpenset(RoomNode * node)
{
	node->m_set = Set::OPEN_SET;
	m_openSet[m_lastUsed++] = node;
}

RoomNode * FloorPlanGenerator::_GetCheapest()
{
	int cheapest = 0;
	for (int i = 1; i < m_lastUsed; ++i) {
		if (m_openSet[i]->m_totalCost < m_openSet[cheapest]->m_totalCost)
			cheapest = i;
	}
	RoomNode* retNode = m_openSet[cheapest];
	m_openSet[cheapest] = m_openSet[m_lastUsed - 1];
	m_openSet[m_lastUsed--] = nullptr;
	return retNode;
}

void FloorPlanGenerator::_ResetOpenset()
{
	memset(m_openSet, 0, 25);
	m_lastUsed = 0;
}

bool FloorPlanGenerator::_A_Star(RoomNode& start, RoomNode& goal)
{
	if (start == goal)	return true;
	_ResetOpenset();
	_ResetNodeDistancesAndParents();

	start.m_totalCost = _Heuristic(start, goal);
	start.m_gCost = 0;
	_PushToOpenset(&start);

	while (m_lastUsed != 0) {
		RoomNode* current = _GetCheapest(); // parent node for all neighbors
		current->m_set = Set::NO_SET;

		if (*current == goal) 
			return true;	// Found the goal

		//Cycle through neighbors
		for (int i = 0; i < 4; ++i) {
			if (!current->m_Neighbors[i])
				continue;	// current node doesn't have a neighbor here, continue on
			RoomNode* neighbor = current->m_Neighbors[i];	// short cut
			float gCost = current->m_gCost + 1.f;
			
			// neighbor not on open or closed sets, push to open set
			if (neighbor->m_set == Set::NO_SET) {
				neighbor->m_gCost = gCost;
				neighbor->m_totalCost = neighbor->m_gCost + _Heuristic(*neighbor, goal);
				neighbor->m_parent = current;

				_PushToOpenset(neighbor);
			}
			else if (neighbor->m_set != Set::NO_SET && gCost < neighbor->m_gCost) {
				float heuristic = neighbor->m_totalCost - neighbor->m_gCost;
				neighbor->m_gCost = gCost;
				neighbor->m_totalCost = neighbor->m_gCost + heuristic;
				neighbor->m_parent = current;
			}

		}

		current->m_set = Set::CLOSED_SET;
	}
	// Didn't find path
	return false;
}

bool FloorPlanGenerator::_IsGOAViableObject(GameObject * pGO)
{
	return pGO->HasComponent(C_SpawnOnHealthZero) && pGO->TagIs(GameObjectTag::T_Obstacle);
}

bool FloorPlanGenerator::_IsGOAViableEnemy(GameObject * pGO)
{
	return pGO->HasComponent(C_SpawnOnHealthZero) && pGO->TagIs(GameObjectTag::T_Enemy);
}

void FloorPlanGenerator::_ResetNodeDistancesAndParents()
{
	for (short row = 0; row < m_rows; ++row) {
		for (short col = 0; col < m_cols; ++col) {
			m_roomNodes[row][col].m_gCost = MAX_DISTANCE;
			m_roomNodes[row][col].m_totalCost = MAX_DISTANCE;
			m_roomNodes[row][col].m_parent = nullptr;
			m_roomNodes[row][col].m_set = Set::NO_SET;
		}
	}
}

void FloorPlanGenerator::_ConnectNeighbors()
{

	//Top Left
	m_roomNodes[0][0].m_Neighbors[NEIGHBOR::N_RIGHT] = &m_roomNodes[0][1];
	m_roomNodes[0][0].m_Neighbors[NEIGHBOR::N_DOWN] = &m_roomNodes[1][0];
	//Top Right
	m_roomNodes[0][m_maxColIdx].m_Neighbors[NEIGHBOR::N_LEFT] = &m_roomNodes[0][m_maxColIdx - 1];
	m_roomNodes[0][m_maxColIdx].m_Neighbors[NEIGHBOR::N_DOWN] = &m_roomNodes[1][m_maxColIdx];
	//Bottom Left
	m_roomNodes[m_maxRowIdx][0].m_Neighbors[NEIGHBOR::N_UP] = &m_roomNodes[m_maxRowIdx - 1][0];
	m_roomNodes[m_maxRowIdx][0].m_Neighbors[NEIGHBOR::N_RIGHT] = &m_roomNodes[m_maxRowIdx][1];
	//Bottom Right
	m_roomNodes[m_maxRowIdx][m_maxColIdx].m_Neighbors[NEIGHBOR::N_LEFT] = &m_roomNodes[m_maxRowIdx][m_maxColIdx - 1];
	m_roomNodes[m_maxRowIdx][m_maxColIdx].m_Neighbors[NEIGHBOR::N_UP] = &m_roomNodes[m_maxRowIdx - 1][m_maxColIdx];

	// Connect all inner nodes (avoid all edges)
	for (short row = 1; row < m_maxRowIdx; ++row) {
		for (short col = 1; col < m_maxColIdx; ++col) {
			m_roomNodes[row][col].m_Neighbors[NEIGHBOR::N_LEFT] = &m_roomNodes[row][col - 1];
			m_roomNodes[row][col].m_Neighbors[NEIGHBOR::N_UP] = &m_roomNodes[row - 1][col];
			m_roomNodes[row][col].m_Neighbors[NEIGHBOR::N_RIGHT] = &m_roomNodes[row][col + 1];
			m_roomNodes[row][col].m_Neighbors[NEIGHBOR::N_DOWN] = &m_roomNodes[row + 1][col];
		}
	}

	// Top and bottom edges
	for (short col = 1; col < m_maxColIdx; ++col) {
		//Top
		m_roomNodes[0][col].m_Neighbors[NEIGHBOR::N_LEFT] = &m_roomNodes[0][col - 1];
		m_roomNodes[0][col].m_Neighbors[NEIGHBOR::N_DOWN] = &m_roomNodes[1][col];
		m_roomNodes[0][col].m_Neighbors[NEIGHBOR::N_RIGHT] = &m_roomNodes[0][col + 1];
		//Bottom
		m_roomNodes[m_maxRowIdx][col].m_Neighbors[NEIGHBOR::N_LEFT] = &m_roomNodes[m_maxRowIdx][col - 1];
		m_roomNodes[m_maxRowIdx][col].m_Neighbors[NEIGHBOR::N_UP] = &m_roomNodes[m_maxRowIdx - 1][col];
		m_roomNodes[m_maxRowIdx][col].m_Neighbors[NEIGHBOR::N_RIGHT] = &m_roomNodes[m_maxRowIdx][col + 1];
	}

	// Left and right edges
	for (short row = 1; row < m_maxRowIdx; ++row) {
		//Left
		m_roomNodes[row][0].m_Neighbors[NEIGHBOR::N_UP] = &m_roomNodes[row - 1][0];
		m_roomNodes[row][0].m_Neighbors[NEIGHBOR::N_RIGHT] = &m_roomNodes[row][1];
		m_roomNodes[row][0].m_Neighbors[NEIGHBOR::N_DOWN] = &m_roomNodes[row + 1][0];
		//Right
		m_roomNodes[row][m_maxColIdx].m_Neighbors[NEIGHBOR::N_UP] = &m_roomNodes[row - 1][m_maxColIdx];
		m_roomNodes[row][m_maxColIdx].m_Neighbors[NEIGHBOR::N_LEFT] = &m_roomNodes[row][m_maxColIdx - 1];
		m_roomNodes[row][m_maxColIdx].m_Neighbors[NEIGHBOR::N_DOWN] = &m_roomNodes[row + 1][m_maxColIdx];
	}
}

std::vector<RoomNode*> FloorPlanGenerator::_SelectNodes(const LevelConfig& config)
{
	std::vector<RoomNode*> selectedNodes;
	std::vector<RoomType> types;
	
	short numTypes = Rand() % 3 + 2;
	types.reserve(numTypes);
	for (short idx = 0; idx < numTypes; ++idx) 
		types.push_back(RoomType::INTERESTING);

	types.push_back(RoomType::SPAWN);

	if (config.bossAndShop == BossAndShop::BOSS_ONLY || config.bossAndShop == BossAndShop::BOSS_SHOP)
		types.push_back(RoomType::BOSS);

	if (config.bossAndShop == BossAndShop::SHOP_ONLY || config.bossAndShop == BossAndShop::BOSS_SHOP)
		types.push_back(RoomType::SHOP);

	std::vector< std::pair<short, short> > chosenCoords;
	while (!types.empty()) {
		std::pair<short, short> coords;
		
		switch (types.back()) {
			case RoomType::SPAWN: {	// Spawn nodes are always in the left most column
				coords = std::pair<short, short>(short(Rand() % m_rows), 0);
				break;
			}
			case RoomType::BOSS: { // Boss nodes are always in the right most column
				coords = std::pair<short, short>(short(Rand() % m_rows), m_maxColIdx);
				break;
			}
			default: {
				coords = std::pair<short, short>(short(Rand() % m_rows), short(Rand() % m_cols));
				break;
			}
		}
		

		if (std::find(chosenCoords.begin(), chosenCoords.end(), coords) == chosenCoords.end()) {
			m_roomNodes[coords.first][coords.second].m_type = types.back();
			if (types.back() == RoomType::SPAWN)
				m_spawnNode = &m_roomNodes[coords.first][coords.second];
			else
				selectedNodes.push_back(&m_roomNodes[coords.first][coords.second]);
			types.pop_back();
			chosenCoords.push_back(coords);
		}
	}
	
	return selectedNodes;
}

void FloorPlanGenerator::_ConnectSelectedNodes(std::vector<RoomNode*>& selectedNodes)
{	
	RoomNode * node, * curPathNode;

	while (!selectedNodes.empty()) {
		node = selectedNodes.back();
		selectedNodes.pop_back();

		if (_A_Star(*m_spawnNode, *node)) {
			curPathNode = node;
			while (*curPathNode != *m_spawnNode) {
				if (curPathNode->m_type == RoomType::DEAD)
					curPathNode->m_type = RoomType::ALIVE;
				curPathNode = curPathNode->m_parent;
			}
		}
	}
}

void FloorPlanGenerator::_SetRoomConnectionTypes()
{
	RoomNode* node;
	for (short row = 0; row < m_rows; ++row) {
		for (short col = 0; col < m_cols; ++col) {
			node = &m_roomNodes[row][col];
			if (node->m_type == RoomType::DEAD) {
				_UnsetNodeNeigbors(*node);
				continue;
			}
			else {
				node->m_Neighbors[0] = !node->m_Neighbors[0] || node->m_Neighbors[0]->m_type == RoomType::DEAD ? nullptr : node->m_Neighbors[0];
				node->m_Neighbors[1] = !node->m_Neighbors[1] || node->m_Neighbors[1]->m_type == RoomType::DEAD ? nullptr : node->m_Neighbors[1];
				node->m_Neighbors[2] = !node->m_Neighbors[2] || node->m_Neighbors[2]->m_type == RoomType::DEAD ? nullptr : node->m_Neighbors[2];
				node->m_Neighbors[3] = !node->m_Neighbors[3] || node->m_Neighbors[3]->m_type == RoomType::DEAD ? nullptr : node->m_Neighbors[3];
			}


			int connection =	(node->m_Neighbors[NEIGHBOR::N_LEFT]	? 1 : 0) + 
								(node->m_Neighbors[NEIGHBOR::N_UP]		? 2 : 0) +
								(node->m_Neighbors[NEIGHBOR::N_RIGHT]	? 4 : 0) +
								(node->m_Neighbors[NEIGHBOR::N_DOWN]	? 8 : 0);

			node->m_ConnectionType = static_cast<RoomConnections>(connection);
		}
	}
}

void FloorPlanGenerator::_UnsetNodeNeigbors(RoomNode& node)
{
	node.m_Neighbors[0] = nullptr;
	node.m_Neighbors[1] = nullptr;
	node.m_Neighbors[2] = nullptr;
	node.m_Neighbors[3] = nullptr;
}

/*
Cycles through the given map of room files starting at m_difficulty and returns the highest difficulty list
If none are found, returns unsigned short max
*/
unsigned short FloorPlanGenerator::_FindRoomListForDifficulty(DifficultyMap& difficultyMap, RoomType type)
{
	unsigned short difficulty = m_difficulty;
	while (difficultyMap[difficulty].GetRooms(type).size() == 0) {
		if (--difficulty < 1)
			return -1;
	}
	return difficulty;
}

/*
Iterates through the available room files until it finds a room file of the known difficulty or lower without going to difficulty 0
If none are found, returns a nullptr
*/
json * FloorPlanGenerator::_GetRoomJsonForDifficulty(RoomConnections connection)
{
	unsigned short difficulty = _FindRoomListForDifficulty(m_roomFiles[connection], RoomType::ALIVE);
	if (difficulty > m_difficulty)	return nullptr;

	int idx = RandomInt(0, m_roomFiles[connection][difficulty].normalRooms.size());
	return m_roomFiles[connection][difficulty].normalRooms[idx];
}

/*
Finds a spawn room for the given connection 
(spawn rooms are always difficulty 0)
*/
json * FloorPlanGenerator::_GetRoomJsonForSpawn(RoomConnections connection)
{
	int idx = RandomInt(0, m_roomFiles[connection][0].normalRooms.size());
	return m_roomFiles[connection][0].normalRooms[idx];
}

/*
Finds a boss room for the given connection
Will attempt to find a room of the highest difficulty available
*/
json * FloorPlanGenerator::_GetRoomJsonForBoss(RoomConnections connection)
{
	unsigned short difficulty = _FindRoomListForDifficulty(m_roomFiles[connection], RoomType::BOSS);
	if (difficulty > m_difficulty)	return nullptr;

	int idx = RandomInt(0, m_roomFiles[connection][difficulty].bossRooms.size());
	return m_roomFiles[connection][difficulty].bossRooms[idx];
}

/*
Finds a shop room for the given connection
Will attempt to find a room of the highest difficulty available
*/
json * FloorPlanGenerator::_GetRoomJsonForShop(RoomConnections connection)
{
	unsigned short difficulty = _FindRoomListForDifficulty(m_roomFiles[connection], RoomType::SHOP);
	if (difficulty > m_difficulty)	return nullptr;

	int idx = RandomInt(0, m_roomFiles[connection][difficulty].shopRooms.size());
	return m_roomFiles[connection][difficulty].shopRooms[idx];
}

#pragma endregion

#pragma region FloorPlanGenerator Methods

FloorPlanGenerator::FloorPlanGenerator() : m_cols(0), m_rows(0) {}

FloorPlanGenerator::~FloorPlanGenerator()
{
	for (short row = 0; row < m_rows; ++row) {
		for (short col = 0; col < m_cols; ++col)
			_UnsetNodeNeigbors(m_roomNodes[row][col]);
		TETRA_MEMORY.Free(m_roomNodes[row]);
	}
	TETRA_MEMORY.Free(m_roomNodes);
}

void FloorPlanGenerator::GenerateRoomNodes(unsigned short cols, unsigned short rows, unsigned short difficulty)
{
	m_cols = cols;
	m_rows = rows;
	m_maxRowIdx = m_rows - 1;
	m_maxColIdx = m_cols - 1;
	m_difficulty = difficulty;

	short id = 0;
	m_roomNodes = (RoomNode**)TETRA_MEMORY.Alloc(sizeof(RoomNode*) * m_rows);

	for (short row = 0; row < m_rows; ++row) {
		m_roomNodes[row] = (RoomNode*)TETRA_MEMORY.Alloc(sizeof(RoomNode) * m_cols);
		for (short col = 0; col < m_cols; ++col) {
			m_roomNodes[row][col] = RoomNode(RoomType::DEAD, ++id, row, col);
			_UnsetNodeNeigbors(m_roomNodes[row][col]);
		}
	}
}

void FloorPlanGenerator::GenerateRoomNodes(const LevelConfig & config)
{
	GenerateRoomNodes(config.cols, config.rows, config.difficulty);
}

void FloorPlanGenerator::GenerateFloorPlan(const LevelConfig & config, int seed)
{
	ResetAllNodes();
	_ConnectNeighbors();
	m_seed = seed;
	std::cout << "Seed: " << m_seed << std::endl;
	SeedRand(m_seed);

	_ConnectSelectedNodes(_SelectNodes(config));
	_SetRoomConnectionTypes();
}

void FloorPlanGenerator::ResetAllNodes()
{
	for (short row = 0; row < m_rows; ++row)
		for (short col = 0; col < m_cols; ++col)
			m_roomNodes[row][col].m_type = RoomType::DEAD;
	m_spawnNode = nullptr;
}

void FloorPlanGenerator::PrintFloorPlan()
{
	for (short row = 0; row < m_rows; ++row) {
		for (short col = 0; col < m_cols; ++col) {
			std::cout << m_roomNodes[row][col].m_type;
		}
		std::cout << std::endl << std::endl;
	}
}

void FloorPlanGenerator::GenerateLevelFromFloorPlan()
{
	unsigned short cellWidth = TETRA_GAME_CONFIG.CellWidth();
	unsigned short cellHeight = TETRA_GAME_CONFIG.CellHeight();
	unsigned short roomWidth = TETRA_GAME_CONFIG.RoomWidth();
	unsigned short roomHeight = TETRA_GAME_CONFIG.RoomHeight();

	float x = float(roomWidth * cellWidth);		// pixel width of the room
	float xHalf = x / 2.f;
	float y = float(roomHeight * cellHeight);	// pixel height of the room
	float yHalf = y / 2.f;
	// "Magic number" fix, the room background files had a slight gap in them when placed, this shifts everything 
	// over to avoid said gap
	--x;
	--y;

	std::vector<GameObject*> enemies;
	std::vector<GameObject*> objects;
	std::vector<RoomNodeData> roomNodeDatas;

	for (short row = 0; row < m_rows; ++row) {
		for (short col = 0; col < m_cols; ++col) {
			if (m_roomNodes[row][col].m_type == RoomType::DEAD)	
				continue;

			Vector3D offset = Vector3D(col * x + xHalf, -(row * y + yHalf), 0);
			// Set the position of each important node
			m_roomNodes[row][col].m_position = offset;

			json* j;
			switch (m_roomNodes[row][col].m_type) {
				case RoomType::SPAWN: {
					j = _GetRoomJsonForSpawn(m_roomNodes[row][col].m_ConnectionType);
					break;
				}
				case RoomType::BOSS: {
					j = _GetRoomJsonForBoss(m_roomNodes[row][col].m_ConnectionType);
					break;
				}
				case RoomType::SHOP: {
					j = _GetRoomJsonForShop(m_roomNodes[row][col].m_ConnectionType);
					break;
				}
				default: {
					j = _GetRoomJsonForDifficulty(m_roomNodes[row][col].m_ConnectionType);
					break;
				}
			}

			if (!j) {
				std::cout << "ABORTING! NO LEVEL FILE FOUND FOR ROOM TYPE: " << m_roomNodes[row][col].m_ConnectionType << std::endl;
				return;
			}

			// Move all game objects in this room to their designated location
			std::vector<GameObject*> createdGameObjects = TETRA_LEVELS.LoadRoomFile(*j);
			for (GameObject* pGO : createdGameObjects) {
				Transform* pTransform = pGO->GetComponent<Transform>(ComponentType::C_Transform);
				if (pTransform)	pTransform->Move(offset);

				if (m_roomNodes[row][col].m_type != RoomType::SPAWN && m_roomNodes[row][col].m_type != RoomType::SHOP) {
					if (_IsGOAViableObject(pGO))		objects.push_back(pGO);
					else if (_IsGOAViableEnemy(pGO))	enemies.push_back(pGO);
				}
				if (pGO->TagIs(GameObjectTag::T_Background)) {
					RoomNodeData roomNodeData;
					roomNodeData.col = col;
					roomNodeData.row = row;
					roomNodeData.spriteComponent = pGO->GetComponent<Sprite>(C_Sprite);
					roomNodeDatas.push_back(roomNodeData);
				}
			}
		}
	}

	GameObject* pGO = TETRA_GAME_OBJECTS.FindObjectWithTag(GameObjectTag::T_Player);
	if (pGO) {
		Transform* pTransform = pGO->GetComponent<Transform>(ComponentType::C_Transform);
		if (pTransform)
			pTransform->SetPosition(m_spawnNode->m_position);
	}


	// TODO: Do this better in general
	int idx = 0;
	if (enemies.size() > 0) {
		idx = RandomInt(0, enemies.size());
		enemies[idx]->GetComponent<SpawnOnHealthZero>(C_SpawnOnHealthZero)->AddSpawnObject("P_EggPickUp"); // TODO: DO NOT HARD CODE THIS STRING
	}

	if (objects.size() > 0) {
		idx = RandomInt(0, objects.size());
		objects[idx]->GetComponent<SpawnOnHealthZero>(C_SpawnOnHealthZero)->AddSpawnObject("P_EggPickUp"); // TODO: DO NOT HARD CODE THIS STRING
		int prevIdx = idx;
		idx = RandomInt(0, objects.size());
		while (idx == prevIdx)
			idx = RandomInt(0, objects.size());

		objects[idx]->GetComponent<SpawnOnHealthZero>(C_SpawnOnHealthZero)->AddSpawnObject("P_EggPickUp"); // TODO: DO NOT HARD CODE THIS STRING
	}

	TETRA_POST_PROCESSING.CreateMiniMapTexture(roomNodeDatas, m_rows, m_cols, FloorWidthPixels(), FloorHeightPixels());
}

RoomConnections FloorPlanGenerator::GetRoomConnectionType(const std::string connectionType)
{
	return ROOM_CONNECTION_STRINGS_TO_TYPES[connectionType];
}

void FloorPlanGenerator::LoadRoomFiles()
{
	std::string path = TETRA_GAME_CONFIG.RoomFilesDir();
	for (const std::experimental::filesystem::v1::directory_entry &room : std::experimental::filesystem::directory_iterator(path)) {
		if (room.path().has_extension()) {
			json* j = new json();
			*j = OpenJsonFile(room.path().string());

			std::string name = (*j)[ROOM_CONNECTION_TYPE];
			int difficulty = ValueExists((*j), ROOM_DIFFICULTY) ? (*j)[ROOM_DIFFICULTY] : 1;

			BossAndShop bossAndShop = static_cast<BossAndShop>(((*j)[ROOM_BOSS_LEVEL] ? 1 : 0) + ((*j)[ROOM_SHOP_LEVEL] ? 2 : 0));

			switch (bossAndShop) {
				case BossAndShop::BOSS_ONLY: {
					m_roomFiles[GetRoomConnectionType(name)][difficulty].bossRooms.push_back(j);
					break;
				}
				case BossAndShop::SHOP_ONLY: {
					m_roomFiles[GetRoomConnectionType(name)][difficulty].shopRooms.push_back(j);
					break;
				}
				case BossAndShop::BOSS_SHOP: {
					m_roomFiles[GetRoomConnectionType(name)][difficulty].bossRooms.push_back(j);
					m_roomFiles[GetRoomConnectionType(name)][difficulty].shopRooms.push_back(j);
					break;
				}
				default: {
					m_roomFiles[GetRoomConnectionType(name)][difficulty].normalRooms.push_back(j);
					break;
				}
			}
		}
	}

	// Shrink m_roomFiles vectors
}

unsigned int FloorPlanGenerator::FloorWidthPixels() const
{
	return int(TETRA_GAME_CONFIG.RoomWidthPixels() * m_cols);
}

unsigned int FloorPlanGenerator::FloorHeightPixels() const
{
	return int(TETRA_GAME_CONFIG.RoomHeightPixels() * m_rows);
}

#pragma endregion

std::ostream& operator<<(std::ostream& out, const RoomConnections& rc) {
	out << ROOM_CONNECTION_STRINGS[rc-1];
	return out;
}

std::ostream& operator<<(std::ostream& out, const RoomType& rt) {
	switch (rt) {
	case RoomType::BOSS:
		out << " G ";
		break;
	case RoomType::INTERESTING:
		out << " I ";
		break;
	case RoomType::SPAWN:
		out << " S ";
		break;
	case RoomType::DEAD:
		out << " X ";
		break;
	case RoomType::ALIVE:
		out << " A ";
		break;
	case RoomType::SHOP:
		out << " P ";
		break;
	}
	return out;
}

bool RoomNode::operator()(const RoomNode & lhs, const RoomNode & rhs)
{
	return lhs.m_totalCost < rhs.m_totalCost;
}


#undef ROOM_CONNECTION_TYPE
#undef ROOM_DIFFICULTY
#undef ROOM_BOSS_LEVEL
#undef ROOM_SHOP_LEVEL