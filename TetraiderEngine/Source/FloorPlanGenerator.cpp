#include "FloorPlanGenerator.h"
#include <cstdlib>
#include <time.h>
#include <vector>
#include <unordered_map>

// small helper enum to remember directions easier
static enum NEIGHBOR {
	LEFT = 0, UP, RIGHT, DOWN
};

static float _Heuristic(RoomNode& a, RoomNode& b) {
	short x = a.m_row - b.m_row;
	short y = a.m_col - b.m_col;
	return sqrtf(float(x * x + y * y));
}

static ReconRetValue _ReconstructPath(std::unordered_map<short, RoomNode&>& cameFrom, RoomNode& current, RoomNode& start, ReconRetValue& reconRetValue) {
	if (current == start) {
		reconRetValue.path->push(current);
		return reconRetValue;
	}
	RoomNode* cameFromNode = &cameFrom[current.m_id];
	if (cameFromNode) {
		ReconRetValue reconstructedPath = _ReconstructPath(cameFrom, *cameFromNode, start, reconRetValue);
		reconRetValue.path->push(current);
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
	MinHeap<RoomNode&> openSet;
	std::unordered_map<short, RoomNode&> cameFrom;
	std::unordered_map<short, float> gScore = { {start.m_id, 0 } };
	openSet.push(start);
	start.m_distance = _Heuristic(start, goal);
	while (!openSet.empty()) {
		RoomNode& current = openSet.top();
		openSet.pop();
		// Found the end
		if (start == goal) {
			_ReconstructPath(cameFrom, goal, start, reconRetValue);
			return;
		}
		closedSet[current.m_id] = true;

		float t_gScore = gScore[current.m_id] + current.m_distance;
		for (int idx = 0; idx < 4; ++idx) {
			if (!current.m_Neighbors[idx])	return;

			RoomNode& neighbor = *current.m_Neighbors[idx];
			float neighborGScore = _Heuristic(neighbor, start);
			if (t_gScore > neighborGScore && closedSet[neighbor.m_id]) continue;
			// DERIVE QUEUE for nodeInHeap
			if (t_gScore < neighborGScore || !openSet.contains(neighbor)) {
				cameFrom[neighbor.m_id] = current;
				gScore[neighbor.m_id] = t_gScore;
				neighbor.m_distance = gScore[neighbor.m_id] + _Heuristic(neighbor, goal);
				openSet.push(neighbor);
			}
		}
	}
	// Didn't find path
	return;
}

void FloorPlanGenerator::_ResetNodeDistances()
{
	for (short row = 0; row < 5; ++row)
		for (short col = 0; col < 5; ++col)
			if (m_roomNodes[row][col])
				m_roomNodes[row][col]->m_distance = MAX_DISTANCE;
}

void FloorPlanGenerator::_ConnectNeighbors()
{

	//Top Left
	m_roomNodes[0][0]->m_Neighbors[NEIGHBOR::RIGHT] = m_roomNodes[0][1];
	m_roomNodes[0][0]->m_Neighbors[NEIGHBOR::DOWN] = m_roomNodes[1][0];
	//Top Right
	m_roomNodes[0][MAX_COLS]->m_Neighbors[NEIGHBOR::LEFT] = m_roomNodes[0][MAX_COLS - 1];
	m_roomNodes[0][MAX_COLS]->m_Neighbors[NEIGHBOR::DOWN] = m_roomNodes[1][MAX_COLS];
	//Bottom Left
	m_roomNodes[MAX_ROWS][0]->m_Neighbors[NEIGHBOR::UP] = m_roomNodes[MAX_ROWS - 1][0];
	m_roomNodes[MAX_ROWS][0]->m_Neighbors[NEIGHBOR::RIGHT] = m_roomNodes[MAX_ROWS][1];
	//Bottom Right
	m_roomNodes[MAX_ROWS][MAX_COLS]->m_Neighbors[NEIGHBOR::LEFT] = m_roomNodes[MAX_ROWS][MAX_COLS - 1];
	m_roomNodes[MAX_ROWS][MAX_COLS]->m_Neighbors[NEIGHBOR::UP] = m_roomNodes[MAX_ROWS - 1][MAX_COLS];

	for (short row = 1; row < MAX_ROWS - 1; ++row) {
		for (short col = 1; col < MAX_COLS - 1; ++col) {
			m_roomNodes[row][col]->m_Neighbors[NEIGHBOR::LEFT] = m_roomNodes[row][col - 1];
			m_roomNodes[row][col]->m_Neighbors[NEIGHBOR::UP] = m_roomNodes[row - 1][col];
			m_roomNodes[row][col]->m_Neighbors[NEIGHBOR::RIGHT] = m_roomNodes[row][col + 1];
			m_roomNodes[row][col]->m_Neighbors[NEIGHBOR::DOWN] = m_roomNodes[row + 1][col];
		}
	}

	for (short col = 1; col < MAX_COLS - 1; ++col) {
		//Top
		m_roomNodes[0][col]->m_Neighbors[NEIGHBOR::LEFT] = m_roomNodes[0][col - 1];
		m_roomNodes[0][col]->m_Neighbors[NEIGHBOR::DOWN] = m_roomNodes[1][col];
		m_roomNodes[0][col]->m_Neighbors[NEIGHBOR::RIGHT] = m_roomNodes[0][col + 1];
		//Bottom
		m_roomNodes[MAX_ROWS][col]->m_Neighbors[NEIGHBOR::LEFT] = m_roomNodes[MAX_ROWS][col - 1];
		m_roomNodes[MAX_ROWS][col]->m_Neighbors[NEIGHBOR::UP] = m_roomNodes[MAX_ROWS - 1][col];
		m_roomNodes[MAX_ROWS][col]->m_Neighbors[NEIGHBOR::RIGHT] = m_roomNodes[MAX_ROWS][col + 1];
	}

	for (short row = 1; row < MAX_ROWS - 1; ++row) {
		//Left
		m_roomNodes[row][0]->m_Neighbors[NEIGHBOR::UP] = m_roomNodes[row - 1][0];
		m_roomNodes[row][0]->m_Neighbors[NEIGHBOR::RIGHT] = m_roomNodes[row][1];
		m_roomNodes[row][0]->m_Neighbors[NEIGHBOR::DOWN] = m_roomNodes[row + 1][0];
		//Right
		m_roomNodes[row][MAX_COLS]->m_Neighbors[NEIGHBOR::UP] = m_roomNodes[row - 1][MAX_COLS];
		m_roomNodes[row][MAX_COLS]->m_Neighbors[NEIGHBOR::LEFT] = m_roomNodes[row][MAX_COLS - 1];
		m_roomNodes[row][MAX_COLS]->m_Neighbors[NEIGHBOR::DOWN] = m_roomNodes[row + 1][MAX_COLS];
	}
}

void FloorPlanGenerator::_SelectNodes()
{
	std::vector<RoomType> types;
	types.push_back(RoomType::GOAL);
	types.push_back(RoomType::SPAWN);
	short numTypes = rand() % 2;
	for (short idx = 0; idx < numTypes; ++idx) types.push_back(RoomType::INTERESTING);

	std::vector< std::pair<short, short> > chosenCoords;
	while (!types.empty()) {
		std::pair<short, short> coords = std::pair<short, short>(rand() % MAX_ROWS, rand() % MAX_COLS);
		for (short pairIdx = 0; pairIdx < chosenCoords.size(); ++pairIdx)
			if (chosenCoords[pairIdx] == coords)	continue;

		m_roomNodes[coords.first][coords.second]->m_type = (*types.end());
		m_selectedNodes.push_back(m_roomNodes[coords.first][coords.second]);
		types.pop_back();
	}
}

void FloorPlanGenerator::_ConnectSelectedNodes()
{
	RoomNode* goalNode = (*m_selectedNodes.end());
	m_selectedNodes.pop_back();
	RoomNode* startNode = (*m_selectedNodes.end());
	m_selectedNodes.pop_back();

	while (!m_selectedNodes.empty()) {
		ReconRetValue path;
		_A_Star(*startNode, *goalNode, path);
		while (!path.path->empty()) {
			RoomNode& node = path.path->top();
			path.path->pop();

			if (node.m_type != RoomType::GOAL && node.m_type != RoomType::SPAWN && node.m_type != RoomType::INTERESTING)
				node.m_type = RoomType::ALIVE;
		}
		m_selectedNodes.pop_back();
	}
}

FloorPlanGenerator::~FloorPlanGenerator()
{
	for (short row = 0; row < 5; ++row) {
		for (short col = 0; col < 5; ++col) {
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
		srand(time(nullptr));
	else
		srand(seed);

	short id = 0;
	for (short row = 0; row < MAX_ROWS; ++row)
		for (short col = 0; col < MAX_COLS; ++col)
			m_roomNodes[row][col] = new RoomNode(RoomType::DEAD, ++id, row, col);
	_ConnectNeighbors();
	_SelectNodes();
	_ConnectSelectedNodes();
}
