#include "FloorPlanGenerator.h"
#include <cstdlib>
#include <time.h>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <algorithm>

// small helper enum to remember directions easier
enum NEIGHBOR {
	N_LEFT = 0, N_UP, N_RIGHT, N_DOWN
};

static float _Heuristic(RoomNode& a, RoomNode& b) {
	short x = abs(a.m_row - b.m_row);
	short y = abs(a.m_col - b.m_col);
	return float(std::max(x, y));
}

static ReconRetValue _ReconstructPath(std::unordered_map<short, RoomNode*>& cameFrom, RoomNode& current, RoomNode& start, ReconRetValue& reconRetValue) {
	if (current == start) {
		reconRetValue.path.push(&current);
		return reconRetValue;
	}
	RoomNode* cameFromNode = cameFrom[current.m_id];
	if (cameFromNode) {
		ReconRetValue reconstructedPath = _ReconstructPath(cameFrom, *cameFromNode, start, reconRetValue);
		reconRetValue.path.push(&current);
	}
	else
		reconRetValue.node = &current;

	return reconRetValue;
};

void FloorPlanGenerator::_A_Star(RoomNode& start, RoomNode& goal, ReconRetValue& reconRetValue)
{
	if (start == goal)	return;
	_ResetNodeDistances();
	start.m_distance = 0;
	std::unordered_map<short, bool> closedSet;

	MinHeap<RoomNode*> openSet;
	//RoomNode* openSet[MAX_COLS * MAX_ROWS];
	//unsigned int lastUsed = 0;

	std::unordered_map<short, RoomNode*> cameFrom;
	std::unordered_map<short, float> gScore = { {start.m_id, 0.f } };
	openSet.push(&start);
	//openSet[lastUsed] = &start;
	//++lastUsed;

	start.m_distance = _Heuristic(start, goal);
	while (!openSet.empty()) {
		// Get the top of the openset
		RoomNode* current = openSet.top();
		openSet.pop();
		// Found the end
		if (*current == goal) {
			_ReconstructPath(cameFrom, goal, start, reconRetValue);
			return;
		}
		closedSet[current->m_id] = true;

		float t_gScore = gScore[current->m_id] + current->m_distance;
		for (int idx = 0; idx < 4; ++idx) {
			RoomNode* neighbor = current->m_Neighbors[idx];
			if (!neighbor)
				continue;

			float neighborGScore = _Heuristic(*neighbor, start);
			if (t_gScore >= neighborGScore && closedSet[neighbor->m_id]) continue;
			// DERIVE QUEUE for nodeInHeap
			if (t_gScore < neighborGScore || !openSet.contains(neighbor)) {
				cameFrom[neighbor->m_id] = current;
				gScore[neighbor->m_id] = t_gScore;
				neighbor->m_distance = gScore[neighbor->m_id] + _Heuristic(*neighbor, goal);
				openSet.push(neighbor);
			}
		}
	}
	// Didn't find path
	return;
}

void FloorPlanGenerator::_ResetNodeDistances()
{
	for (short row = 0; row < MAX_ROWS; ++row)
		for (short col = 0; col < MAX_COLS; ++col)
			if (m_roomNodes[row][col])
				m_roomNodes[row][col]->m_distance = MAX_DISTANCE;
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

void FloorPlanGenerator::_SelectNodes()
{
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
		m_selectedNodes.push_back(m_roomNodes[coords.first][coords.second]);
		types.pop_back();
	}
}

void FloorPlanGenerator::_ConnectSelectedNodes()
{
	RoomNode* startNode = m_selectedNodes.back();
	m_selectedNodes.pop_back();

	while (!m_selectedNodes.empty()) {
		ReconRetValue reconRetVal;
		RoomNode* node = m_selectedNodes.back();
		m_selectedNodes.pop_back();
		_A_Star(*startNode, *node, reconRetVal);
		while (!reconRetVal.path.empty()) {
			RoomNode* node = reconRetVal.path.top();
			reconRetVal.path.pop();

			if (node->m_type != RoomType::GOAL && node->m_type != RoomType::SPAWN && node->m_type != RoomType::INTERESTING)
				node->m_type = RoomType::ALIVE;
		}
	}
}

void FloorPlanGenerator::_SetRoomConnectionTypes()
{
	for (short row = 0; row < MAX_ROWS; ++row) {
		for (short col = 0; col < MAX_COLS; ++col) {
			RoomNode* node = m_roomNodes[row][col];
			int connection = node->m_Neighbors[NEIGHBOR::N_LEFT] ? 1 : 0 + 
				node->m_Neighbors[NEIGHBOR::N_UP] ? 2 : 0 +
				node->m_Neighbors[NEIGHBOR::N_RIGHT] ? 4 : 0 +
				node->m_Neighbors[NEIGHBOR::N_DOWN] ? 8 : 0;

			node->m_ConnectionType = static_cast<RoomConnections>(connection);
		}
	}
}

FloorPlanGenerator::~FloorPlanGenerator()
{
	for (short row = 0; row < MAX_ROWS; ++row) {
		for (short col = 0; col < MAX_COLS; ++col) {
			if (m_roomNodes[row][col]) {
				UnsetNodeNeigbors(*m_roomNodes[row][col]);
				delete m_roomNodes[row][col];
			}
		}
	}
}

void FloorPlanGenerator::UnsetNodeNeigbors(RoomNode& node)
{
	node.m_Neighbors[0] = nullptr;
	node.m_Neighbors[1] = nullptr;
	node.m_Neighbors[2] = nullptr;
	node.m_Neighbors[3] = nullptr;
}

void FloorPlanGenerator::GenerateFloorPlan(int seed)
{
	if (seed == -1)
		srand(int(time(nullptr)));
	else
		srand(seed);

	short id = 0;
	for (short row = 0; row < MAX_ROWS; ++row) {
		for (short col = 0; col < MAX_COLS; ++col) {
			m_roomNodes[row][col] = new RoomNode(RoomType::DEAD, ++id, row, col);
			UnsetNodeNeigbors(*m_roomNodes[row][col]);
		}
	}
	_ConnectNeighbors();
	_SelectNodes();
	_ConnectSelectedNodes();
	_SetRoomConnectionTypes();
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
