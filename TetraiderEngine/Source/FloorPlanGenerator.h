#pragma once

#ifndef FLOOR_PLAN_GENERATOR_H
#define FLOOR_PLAN_GENERATOR_H

#include <queue>
#include <string>

template<class T, class Container = std::vector<T>, class Compare = std::less<typename Container::value_type> >
class MinHeap : public std::priority_queue<T, Container, Compare>
{
public:
	typedef typename
		std::priority_queue<T, Container, Compare>::container_type::const_iterator const_iterator;

	bool contains(const T&val) const
	{
		auto first = this->c.cbegin();
		auto last = this->c.cend();
		while (first != last) {
			if (*first == val) return true;
			++first;
		}
		return false;
	}
};


const float MAX_DISTANCE = 10000000.f;
const short MAX_ROWS = 5;
const short MAX_COLS = 5;
const short MAX_ROWS_IDX = MAX_ROWS - 1;
const short MAX_COLS_IDX = MAX_COLS - 1;

enum class RoomConnections {
	UP,
	DOWN, 
	LEFT, 
	RIGHT,

	UP_DOWN_LEFT, 
	UP_DOWN_RIGHT, 
	UP_LEFT_RIGHT, 
	DOWN_LEFT_RIGHT,

	UP_DOWN, 
	UP_LEFT, 
	UP_RIGHT,

	DOWN_LEFT, 
	DOWN_RIGHT,

	LEFT_RIGHT,
	ALL
};

enum class RoomType {
	GOAL, INTERESTING, SPAWN, DEAD, ALIVE
};

std::ostream& operator<<(std::ostream& out, const RoomType& rt);

struct RoomNode {
	explicit RoomNode(RoomType type, short id, short row, short col) : 
		m_type(type), m_id(id), m_row(row), m_col(col), m_distance(MAX_DISTANCE){}
	short m_id, m_row, m_col;
	RoomConnections m_ConnectionType;
	RoomNode* m_Neighbors[4]; // 0 = left, 1 = up, 2 = right, 3 = down
	RoomType m_type;
	float m_distance;

	bool operator==(const RoomNode& rhs) { return m_id == rhs.m_id; }
	bool operator() (const RoomNode& lhs, const RoomNode& rhs) {
		return lhs.m_distance < rhs.m_distance;
	}
};

struct ReconRetValue {
	ReconRetValue() : node(nullptr) {};
	ReconRetValue(const ReconRetValue& rhs) : node(rhs.node), path(rhs.path) {};
	RoomNode* node;
	MinHeap<RoomNode*> path;
};

class FloorPlanGenerator {
protected:
	RoomNode* m_roomNodes[MAX_ROWS][MAX_COLS];
	std::vector<RoomNode*> m_selectedNodes;

	void _A_Star(RoomNode& start, RoomNode& goal, ReconRetValue& reconRetValue);

	void _ResetNodeDistances();
	void _ConnectNeighbors();
	void _SelectNodes();
	void _ConnectSelectedNodes();
public:
	FloorPlanGenerator() {}
	~FloorPlanGenerator();
	
	void UnsetNodeNeigbors(RoomNode& node);
	void GenerateFloorPlan(int seed = -1);
	void PrintFloorPlan();
};

#endif

