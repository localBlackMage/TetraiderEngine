#include "FloorPlanGenerator.h"
#include "TetraiderAPI.h"

#include "GameObject.h"
#include "Transform.h"

#include <cstdlib>
#include <time.h>
#include <iostream>
#include <algorithm>
#include <filesystem>

static std::unordered_map<std::string, RoomConnections> ROOM_CONN_STRINGS = {
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
	_ResetNodeDistances();
	_ResetNodeParents();
	std::unordered_map<short, bool> closedSet; // node id -> true if this node is on the closedSet
	MinHeap<RoomNode*> openSet;
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

void FloorPlanGenerator::_GenerateRoomNodes()
{
	short id = 0;
	for (short row = 0; row < MAX_ROWS; ++row) {
		for (short col = 0; col < MAX_COLS; ++col) {
			m_roomNodes[row][col] = new RoomNode(RoomType::DEAD, ++id, row, col);
			_UnsetNodeNeigbors(*m_roomNodes[row][col]);
		}
	}
}

void FloorPlanGenerator::_ResetNodeDistances()
{
	for (short row = 0; row < MAX_ROWS; ++row)
		for (short col = 0; col < MAX_COLS; ++col)
			if (m_roomNodes[row][col])
				m_roomNodes[row][col]->m_cost = MAX_DISTANCE;
}

void FloorPlanGenerator::_ResetNodeParents()
{
	for (short row = 0; row < MAX_ROWS; ++row)
		for (short col = 0; col < MAX_COLS; ++col)
			if (m_roomNodes[row][col])
				m_roomNodes[row][col]->m_parent = nullptr;
}

void FloorPlanGenerator::_ConnectNeighbors()
{

	//Top Left
	m_roomNodes[0][0]->m_Neighbors[NEIGHBOR::N_RIGHT] = m_roomNodes[0][1];
	m_roomNodes[0][0]->m_Neighbors[NEIGHBOR::N_DOWN] = m_roomNodes[1][0];
	//Top Right
	m_roomNodes[0][MAX_COLS_IDX]->m_Neighbors[NEIGHBOR::N_LEFT] = m_roomNodes[0][MAX_COLS_IDX - 1];
	m_roomNodes[0][MAX_COLS_IDX]->m_Neighbors[NEIGHBOR::N_DOWN] = m_roomNodes[1][MAX_COLS_IDX];
	//Bottom Left
	m_roomNodes[MAX_ROWS_IDX][0]->m_Neighbors[NEIGHBOR::N_UP] = m_roomNodes[MAX_ROWS_IDX - 1][0];
	m_roomNodes[MAX_ROWS_IDX][0]->m_Neighbors[NEIGHBOR::N_RIGHT] = m_roomNodes[MAX_ROWS_IDX][1];
	//Bottom Right
	m_roomNodes[MAX_ROWS_IDX][MAX_COLS_IDX]->m_Neighbors[NEIGHBOR::N_LEFT] = m_roomNodes[MAX_ROWS_IDX][MAX_COLS_IDX - 1];
	m_roomNodes[MAX_ROWS_IDX][MAX_COLS_IDX]->m_Neighbors[NEIGHBOR::N_UP] = m_roomNodes[MAX_ROWS_IDX - 1][MAX_COLS_IDX];

	// Connect all inner nodes (avoid all edges)
	for (short row = 1; row < MAX_ROWS_IDX; ++row) {
		for (short col = 1; col < MAX_COLS_IDX; ++col) {
			m_roomNodes[row][col]->m_Neighbors[NEIGHBOR::N_LEFT] = m_roomNodes[row][col - 1];
			m_roomNodes[row][col]->m_Neighbors[NEIGHBOR::N_UP] = m_roomNodes[row - 1][col];
			m_roomNodes[row][col]->m_Neighbors[NEIGHBOR::N_RIGHT] = m_roomNodes[row][col + 1];
			m_roomNodes[row][col]->m_Neighbors[NEIGHBOR::N_DOWN] = m_roomNodes[row + 1][col];
		}
	}

	// Top and bottom edges
	for (short col = 1; col < MAX_COLS_IDX; ++col) {
		//Top
		m_roomNodes[0][col]->m_Neighbors[NEIGHBOR::N_LEFT] = m_roomNodes[0][col - 1];
		m_roomNodes[0][col]->m_Neighbors[NEIGHBOR::N_DOWN] = m_roomNodes[1][col];
		m_roomNodes[0][col]->m_Neighbors[NEIGHBOR::N_RIGHT] = m_roomNodes[0][col + 1];
		//Bottom
		m_roomNodes[MAX_ROWS_IDX][col]->m_Neighbors[NEIGHBOR::N_LEFT] = m_roomNodes[MAX_ROWS_IDX][col - 1];
		m_roomNodes[MAX_ROWS_IDX][col]->m_Neighbors[NEIGHBOR::N_UP] = m_roomNodes[MAX_ROWS_IDX - 1][col];
		m_roomNodes[MAX_ROWS_IDX][col]->m_Neighbors[NEIGHBOR::N_RIGHT] = m_roomNodes[MAX_ROWS_IDX][col + 1];
	}

	// Left and right edges
	for (short row = 1; row < MAX_ROWS_IDX; ++row) {
		//Left
		m_roomNodes[row][0]->m_Neighbors[NEIGHBOR::N_UP] = m_roomNodes[row - 1][0];
		m_roomNodes[row][0]->m_Neighbors[NEIGHBOR::N_RIGHT] = m_roomNodes[row][1];
		m_roomNodes[row][0]->m_Neighbors[NEIGHBOR::N_DOWN] = m_roomNodes[row + 1][0];
		//Right
		m_roomNodes[row][MAX_COLS_IDX]->m_Neighbors[NEIGHBOR::N_UP] = m_roomNodes[row - 1][MAX_COLS_IDX];
		m_roomNodes[row][MAX_COLS_IDX]->m_Neighbors[NEIGHBOR::N_LEFT] = m_roomNodes[row][MAX_COLS_IDX - 1];
		m_roomNodes[row][MAX_COLS_IDX]->m_Neighbors[NEIGHBOR::N_DOWN] = m_roomNodes[row + 1][MAX_COLS_IDX];
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
		std::pair<short, short> coords = std::pair<short, short>(short(rand() % MAX_ROWS), short(rand() % MAX_COLS));
		for (unsigned int pairIdx = 0; pairIdx < chosenCoords.size(); ++pairIdx)
			if (chosenCoords[pairIdx] == coords)	continue;

		m_roomNodes[coords.first][coords.second]->m_type = types.back();
		selectedNodes.push_back(m_roomNodes[coords.first][coords.second]);
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

void FloorPlanGenerator::_SelectivelyUnsetNodeNeighbors()
{
	RoomNode* node;
	for (short row = 0; row < MAX_ROWS; ++row) {
		for (short col = 0; col < MAX_COLS; ++col) {
			node = m_roomNodes[row][col];
			if (node->m_type == RoomType::DEAD)
				_UnsetNodeNeigbors(*node);
			else {
				node->m_Neighbors[0] = !node->m_Neighbors[0] || node->m_Neighbors[0]->m_type == RoomType::DEAD ? nullptr : node->m_Neighbors[0];
				node->m_Neighbors[1] = !node->m_Neighbors[1] || node->m_Neighbors[1]->m_type == RoomType::DEAD ? nullptr : node->m_Neighbors[1];;
				node->m_Neighbors[2] = !node->m_Neighbors[2] || node->m_Neighbors[2]->m_type == RoomType::DEAD ? nullptr : node->m_Neighbors[2];;
				node->m_Neighbors[3] = !node->m_Neighbors[3] || node->m_Neighbors[3]->m_type == RoomType::DEAD ? nullptr : node->m_Neighbors[3];;
			}
		}
	}
}

void FloorPlanGenerator::_SetRoomConnectionTypes()
{
	RoomNode* node;
	for (short row = 0; row < MAX_ROWS; ++row) {
		for (short col = 0; col < MAX_COLS; ++col) {
			node = m_roomNodes[row][col];
			if (node->m_type == RoomType::DEAD)	continue;

			int connection = (node->m_Neighbors[NEIGHBOR::N_LEFT] ? 1 : 0) + 
				(node->m_Neighbors[NEIGHBOR::N_UP] ? 2 : 0) +
				(node->m_Neighbors[NEIGHBOR::N_RIGHT] ? 4 : 0) +
				(node->m_Neighbors[NEIGHBOR::N_DOWN] ? 8 : 0);

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
#pragma endregion

#pragma region FloorPlanGenerator Methods

FloorPlanGenerator::FloorPlanGenerator()
{
	_GenerateRoomNodes();
}

FloorPlanGenerator::~FloorPlanGenerator()
{
	for (short row = 0; row < MAX_ROWS; ++row) {
		for (short col = 0; col < MAX_COLS; ++col) {
			if (m_roomNodes[row][col]) {
				_UnsetNodeNeigbors(*m_roomNodes[row][col]);
				delete m_roomNodes[row][col];
			}
		}
	}
}

void FloorPlanGenerator::GenerateFloorPlan(int seed)
{
	ResetAllNodes();
	_ConnectNeighbors();
	if (seed == -1)
		srand(int(time(nullptr)));
	else
		srand(seed);
	
	_ConnectSelectedNodes(_SelectNodes());
	_SelectivelyUnsetNodeNeighbors();
	_SetRoomConnectionTypes();
}

void FloorPlanGenerator::ResetAllNodes()
{
	for (short row = 0; row < MAX_ROWS; ++row)
		for (short col = 0; col < MAX_COLS; ++col)
			m_roomNodes[row][col]->m_type = RoomType::DEAD;
	m_spawnNode = nullptr;
}

void FloorPlanGenerator::PrintFloorPlan()
{
	for (short row = 0; row < MAX_ROWS; ++row) {
		for (short col = 0; col < MAX_COLS; ++col) {
			std::cout << m_roomNodes[row][col]->m_type;
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

	for (short row = 0; row < MAX_ROWS; ++row) {
		for (short col = 0; col < MAX_COLS; ++col) {
			if (m_roomNodes[row][col]->m_type == RoomType::DEAD)	
				continue;

			Vector3D offset = Vector3D(col * x + xHalf, -(row * y + yHalf), 0);
			// Set the position of each important node
			m_roomNodes[row][col]->m_position = offset;

			// TODO: Add some logic to select a room file, for now just select the first
			json* j = m_roomFiles[m_roomNodes[row][col]->m_ConnectionType][0];

			// Move all game objects in this room to their designated location
			std::vector<GameObject*> createdGameObjects = TETRA_LEVELS.LoadRoomFile(*j);
			for (GameObject* pGO : createdGameObjects) {
				Transform* pTransform = pGO->GetComponent<Transform>(ComponentType::C_Transform);
				if (pTransform)	pTransform->Move(offset);
			}
		}
	}

	GameObject* pGO = TETRA_GAME_OBJECTS.FindObjectWithTag(GameObjectTag::T_Player);
	if (pGO) {
		Transform* pTransform = pGO->GetComponent<Transform>(ComponentType::C_Transform);

		if (pTransform)
			pTransform->SetPosition(m_spawnNode->m_position);
	}
}

RoomConnections FloorPlanGenerator::GetRoomConnectionType(const std::string connectionType)
{
	return ROOM_CONN_STRINGS[connectionType];
}

void FloorPlanGenerator::LoadRoomFiles()
{
	const std::string ROOM_CONNECTION_TYPE = "ROOM_CONNECTION_TYPE";
	std::string path = TETRA_GAME_CONFIG.RoomFilesDir();
	for (auto &room : std::experimental::filesystem::directory_iterator(path)) {
		if (room.path().has_extension()) {
			json* j = new json();
			*j = OpenJsonFile(room.path().string());

			std::string name = (*j)[ROOM_CONNECTION_TYPE];
			m_roomFiles[GetRoomConnectionType(name)].push_back(j);
		}
	}
}

#pragma endregion

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
		out << " ";
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
