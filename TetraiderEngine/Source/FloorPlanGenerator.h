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

	bool contains(const T& val) const
	{
		auto first = this->c.cbegin();
		auto last = this->c.cend();
		while (first != last) {
			if (*first == val) return true;
			++first;
		}
		return false;
	}

	// Returns true if updated successfully, false if the value wasn't in the heap
	bool update(T& val)
	{
		auto first = this->c.begin();
		auto last = this->c.end();
		while (first != last) {
			if (*first == val) {
				*first = val;
				return true;
			}
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

enum class RoomType {
	GOAL, INTERESTING, SPAWN, DEAD, ALIVE
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

	bool operator==(const RoomNode& rhs) { return m_id == rhs.m_id; }
	bool operator!=(const RoomNode& rhs) { return m_id != rhs.m_id; }
	bool operator() (const RoomNode& lhs, const RoomNode& rhs) { return lhs.m_cost < rhs.m_cost; }
};

class FloorPlanGenerator {
protected:
	RoomNode* m_roomNodes[MAX_ROWS][MAX_COLS];
	std::vector<RoomNode*> m_selectedNodes;

	bool _A_Star(RoomNode& start, RoomNode& goal);

	void _ResetNodeDistances();
	void _ResetNodeParents();
	void _ConnectNeighbors();
	void _SelectNodes();
	void _ConnectSelectedNodes();
	void _SetRoomConnectionTypes();
public:
	FloorPlanGenerator() {}
	~FloorPlanGenerator();
	
	void UnsetNodeNeigbors(RoomNode& node);
	void GenerateFloorPlan(int seed = -1);
	void PrintFloorPlan();
};

#endif

