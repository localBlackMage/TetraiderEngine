#include "Stdafx.h"
#include "HillNodes.h"


HillNodes::HillNodes():Component(ComponentType::C_HillNodes)
{
	TETRA_EVENTS.Subscribe(EVENT_OnBlessingSelect, this);
	m_isMove=false;
}

HillNodes::~HillNodes()
{
}

void HillNodes::Update(float dt)
{
	/*if (m_isMove)
	{

	}*/
}

void HillNodes::Serialize(const json & j)
{
	std::string prefabName;
	int numberOfElements = j["Nodes"].size();
	for (int i = 0; i<numberOfElements; ++i)
	{
		prefabName = ParseString(j["Nodes"][i], "prefab");
		m_Nodes.push_back(NodeInfo(
			TETRA_GAME_OBJECTS.CreateGameObject(ParseString(j["Nodes"][i], "prefab"), true, ParseVector3D(j["Nodes"][i], "position")),
			ParseInt(j["Nodes"][i], "nodeToGo"),
			ParseInt(j["Nodes"][i], "currentNode")
		));
	}
}

void HillNodes::LateInitialize()
{
}

void HillNodes::HandleEvent(Event * pEvent)
{
	if (pEvent->Type() == EventType::EVENT_OnBlessingSelect)
	{
		//play animation, and transfer player from one node to other
		m_isMove = true;
	}
}
