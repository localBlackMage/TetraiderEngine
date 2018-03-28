#pragma once
#include "Component.h"
#include "GameObject.h"

struct NodeInfo {
	NodeInfo(GameObject* _pGo, int _nodeToGo, int _currentNode) : m_pGO(_pGo), m_nodeToGo(_nodeToGo), m_currentNode(_currentNode) {}
	GameObject* m_pGO;
	int m_nodeToGo;
	int m_currentNode;
};

class HillNodes:public Component
{
private:
	std::vector<NodeInfo> m_Nodes;
	bool m_isMove;
public:
	HillNodes();
	~HillNodes();
	static Component* CreateInstance() { return new HillNodes(); }

	// Inherited via Component
	 void Update(float dt);
	 void Serialize(const json & j);
	 void LateInitialize();
	 void HandleEvent(Event* pEvent);
};

