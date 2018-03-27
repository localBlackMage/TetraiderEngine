#include "Stdafx.h"
#include "HillNodes.h"


HillNodes::HillNodes():Component(ComponentType::C_HillNodes)
{
}

HillNodes::~HillNodes()
{
}

void HillNodes::Update(float dt)
{
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
}
