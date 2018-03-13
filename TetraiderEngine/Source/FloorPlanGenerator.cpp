#include <Stdafx.h>

#define ROOM_CONNECTION_TYPE "ROOM_CONNECTION_TYPE"
#define ROOM_DIFFICULTY "DIFFICULTY"

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

// Actual distance, no diagonals
static float _Distance(RoomNode& a, RoomNode& b) {
	short x = abs(a.m_row - b.m_row);
	short y = abs(a.m_col - b.m_col);
	return float(x + y);
}

#pragma region Private Methods

bool FloorPlanGenerator::_A_Star(RoomNode& start, RoomNode& goal)
{
	if (start == goal)	return true;
	_ResetNodeDistancesAndParents();
	std::unordered_map<short, bool> closedSet; // node id -> true if this node is on the closedSet
	Sorting::MinHeap<RoomNode*> openSet;
	openSet.push(&start);

	start.m_cost = _Heuristic(start, goal);

	while (!openSet.empty()) {
		RoomNode* current = openSet.top(); // parent node for all neighbors
		openSet.pop();

		if (*current == goal) 
			return true;	// Found the goal

		//Cycle through neighbors
		for (int i = 0; i < 4; ++i) {
			RoomNode* neighbor = current->m_Neighbors[i];	// short cut
			if (!neighbor) 
				continue;	// current node doesn't have a neighbor here, continue on
			
			float cost = _Distance(*neighbor, start) + _Heuristic(*neighbor, goal);
			// neighbor not on open or closed sets, push to open set
			if (!closedSet[neighbor->m_id] && openSet.contains(neighbor))
				openSet.push(neighbor);
			// If neighbor is on neither set and the new cost is cheaper than the current node's cost
			else if (cost < current->m_cost && cost < neighbor->m_cost) {
				neighbor->m_cost = cost;
				neighbor->m_parent = current;
				// Remove node from closedSet
				closedSet[current->m_id] = false;
				
				// If the openSet contains the neighbor, simply update it, else push it to
				if (!openSet.update(neighbor))
					openSet.push(neighbor);
			}
		}

		closedSet[current->m_id] = true;
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
			m_roomNodes[row][col].m_cost = MAX_DISTANCE;
			m_roomNodes[row][col].m_parent = nullptr;
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

std::vector<RoomNode*> FloorPlanGenerator::_SelectNodes()
{
	std::vector<RoomNode*> selectedNodes;
	std::vector<RoomType> types;
	types.push_back(RoomType::SPAWN);
	types.push_back(RoomType::GOAL);
	short numTypes = rand() % 3 + 2;
	for (short idx = 0; idx < numTypes; ++idx) types.push_back(RoomType::INTERESTING);

	std::vector< std::pair<short, short> > chosenCoords;
	while (!types.empty()) {
		std::pair<short, short> coords = std::pair<short, short>(short(rand() % m_rows), short(rand() % m_cols));
		for (unsigned int pairIdx = 0; pairIdx < chosenCoords.size(); ++pairIdx)
			if (chosenCoords[pairIdx] == coords)	continue;

		m_roomNodes[coords.first][coords.second].m_type = types.back();
		selectedNodes.push_back(&m_roomNodes[coords.first][coords.second]);
		types.pop_back();
	}
	// The Spawn node will always be the last in the list of selected nodes
	m_spawnNode = selectedNodes.back();
	return selectedNodes;
}

void FloorPlanGenerator::_ConnectSelectedNodes(std::vector<RoomNode*>& selectedNodes)
{
	if (selectedNodes.size() < 2)	return;

	RoomNode * startNode = selectedNodes.back();
	selectedNodes.pop_back();

	RoomNode * node, * curPathNode;

	while (!selectedNodes.empty()) {
		node = selectedNodes.back();
		selectedNodes.pop_back();

		if (_A_Star(*startNode, *node)) {
			curPathNode = node;
			while (*curPathNode != *startNode) {
				if (curPathNode->m_type != RoomType::GOAL && curPathNode->m_type != RoomType::SPAWN && curPathNode->m_type != RoomType::INTERESTING)
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

json * FloorPlanGenerator::_GetRoomJsonForDifficulty(short row, short col, RoomConnections connection)
{
	unsigned short difficulty = m_difficulty;
	while (m_roomFiles[connection][difficulty].size() == 0) {
		if (--difficulty < 1)
			return nullptr;
	}
	int idx = RandomInt(0, m_roomFiles[connection][difficulty].size());
	return m_roomFiles[connection][difficulty][idx];
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

void FloorPlanGenerator::GenerateFloorPlan(int seed)
{
	ResetAllNodes();
	_ConnectNeighbors();
	m_seed = seed == -1 ? int(time(nullptr)) : seed;
	std::cout << "Seed: " << m_seed << std::endl;
	srand(m_seed);

	_ConnectSelectedNodes(_SelectNodes());
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
		std::cout << std::endl;
	}
}

void FloorPlanGenerator::GenerateLevelFromFloorPlan()
{
	unsigned short cellWidth = TETRA_GAME_CONFIG.CellWidth();
	unsigned short cellHeight = TETRA_GAME_CONFIG.CellHeight();
	unsigned short roomWidth = TETRA_GAME_CONFIG.RoomWidth();
	unsigned short roomHeight = TETRA_GAME_CONFIG.RoomHeight();

	float x = float(roomWidth * cellWidth);
	float xHalf = x / 2.f;
	float y = float(roomHeight * cellHeight);
	float yHalf = y / 2.f;
	// "Magic number" fix, the room background files had a slight gap in them when placed, this shifts everything 
	// over to avoid said gap
	--x;
	--y;

	std::vector<GameObject*> enemies;
	std::vector<GameObject*> objects;

	for (short row = 0; row < m_rows; ++row) {
		for (short col = 0; col < m_cols; ++col) {
			if (m_roomNodes[row][col].m_type == RoomType::DEAD)	
				continue;

			Vector3D offset = Vector3D(col * x + xHalf, -(row * y + yHalf), 0);
			// Set the position of each important node
			m_roomNodes[row][col].m_position = offset;

			json* j = _GetRoomJsonForDifficulty(row, col, m_roomNodes[row][col].m_ConnectionType);
			if (!j) {
				std::cout << "ABORTING! NO LEVEL FILE FOUND FOR ROOM TYPE: " << m_roomNodes[row][col].m_ConnectionType << std::endl;
				return;
			}

			// Move all game objects in this room to their designated location
			std::vector<GameObject*> createdGameObjects = TETRA_LEVELS.LoadRoomFile(*j);
			for (GameObject* pGO : createdGameObjects) {
				Transform* pTransform = pGO->GetComponent<Transform>(ComponentType::C_Transform);
				if (pTransform)	pTransform->Move(offset);

				if (_IsGOAViableObject(pGO))		objects.push_back(pGO);
				else if (_IsGOAViableEnemy(pGO))	enemies.push_back(pGO);
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
		enemies[idx]->GetComponent<SpawnOnHealthZero>(C_SpawnOnHealthZero)->SetSpawnObject("P_EggPickUp"); // TODO: DO NOT HARD CODE THIS STRING
	}

	if (objects.size() > 0) {
		idx = RandomInt(0, objects.size());
		objects[idx]->GetComponent<SpawnOnHealthZero>(C_SpawnOnHealthZero)->SetSpawnObject("P_EggPickUp"); // TODO: DO NOT HARD CODE THIS STRING
		int prevIdx = idx;
		idx = RandomInt(0, objects.size());
		while (idx == prevIdx)
			idx = RandomInt(0, objects.size());

		objects[idx]->GetComponent<SpawnOnHealthZero>(C_SpawnOnHealthZero)->SetSpawnObject("P_EggPickUp"); // TODO: DO NOT HARD CODE THIS STRING
	}
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
			m_roomFiles[GetRoomConnectionType(name)][difficulty].push_back(j);
		}
	}
}

unsigned int FloorPlanGenerator::FloorWidthPixels() const
{
	return int(TETRA_GAME_CONFIG.RoomWidthPixels() * m_cols);
}

unsigned int FloorPlanGenerator::FloorHeightPixels() const
{
	return int(TETRA_GAME_CONFIG.RoomWidthPixels() * m_rows);
}

#pragma endregion

std::ostream& operator<<(std::ostream& out, const RoomConnections& rc) {
	out << ROOM_CONNECTION_STRINGS[rc-1];
	return out;
}

std::ostream& operator<<(std::ostream& out, const RoomType& rt) {
	switch (rt) {
	case RoomType::GOAL:
		out << "G";
		break;
	case RoomType::INTERESTING:
		out << "I";
		break;
	case RoomType::SPAWN:
		out << "S";
		break;
	case RoomType::DEAD:
		out << "X";
		break;
	case RoomType::ALIVE:
		out << "A";
		break;
	}
	return out;
}

bool RoomNode::operator()(const RoomNode & lhs, const RoomNode & rhs)
{
	return lhs.m_cost < rhs.m_cost;
}


#undef ROOM_CONNECTION_TYPE
#undef ROOM_DIFFICULTY