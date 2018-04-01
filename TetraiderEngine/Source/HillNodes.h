#pragma once
#include "Component.h"
#include "GameObject.h"
#include "Text.h"

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
	bool m_Move;
	bool m_changeLevel;
	bool m_isPlayAnim;
	Text* m_pText;
	float m_speed;
	int m_currentLevel;
	GameObject* m_playerUI;
	Transform* m_pTransPlayerUI;
	Vector3D m_startPos, m_finalPos;
	float m_t;
	Vector3D m_offset;
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

