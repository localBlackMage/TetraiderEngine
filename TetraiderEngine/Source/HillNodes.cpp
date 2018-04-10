#include "Stdafx.h"
#include "HillNodes.h"

HillNodes::HillNodes():Component(ComponentType::C_HillNodes), m_isPlayAnim(true)
{
	m_Move=false;
	TETRA_EVENTS.Subscribe(EVENT_OnLevelInitialized, this);
	TETRA_EVENTS.Subscribe(EVENT_OnCanvasDeactivated, this);
	m_t = 0.0f;
	m_changeLevel = false;
}

HillNodes::~HillNodes()
{
}

void HillNodes::Update(float dt)
{

	if (m_t<=1 && m_Move && m_isPlayAnim)
	{
		m_t += dt * m_speed;
		m_pTransPlayerUI->SetPosition(Lerp(m_startPos, m_finalPos, m_t, false, true));
		m_playerUI->GetComponent<Animation>(ComponentType::C_Animation)->Play(0,false);
		m_changeLevel = true;
	}
	else
	{
		m_playerUI->GetComponent<Animation>(ComponentType::C_Animation)->Play(1, false);
		m_Move = false;
		if (m_changeLevel)
		{
			Event *pEvent = new Event(EVENT_OnLoadNextLevel, 0.5f);
			TETRA_EVENTS.AddDelayedEvent(pEvent);
			m_t = 0.0f;
		}
	}

	if (m_pText) {
		m_pText->SetText("Level"+std::to_string(m_currentLevel));
		m_pText->SetOffset(Vector3D(0, 300, 0));
	}
}

void HillNodes::Serialize(const json & j)
{
	std::string prefabName;
	m_speed = ParseFloat(j, "speed");
	int numberOfElements = j["Nodes"].size();
	m_offset = ParseVector3D(j, "offset");
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
	if (!m_pText) {
		if (pGO)
			m_pText = pGO->GetComponent<Text>(ComponentType::C_Text);
		else {
			printf("No Game Object found. Hill Node component failed to operate.\n");
			return;
		}
		if (!m_pText) {
			printf("No Text component found. Hill Node component failed to operate.\n");
			assert(m_pText);
			return;
		}
	}
}

void HillNodes::HandleEvent(Event * pEvent)
{
	if (pEvent->Type() == EVENT_OnLevelInitialized)
	{
		std::cout << "level init called" << std::endl;

		m_currentLevel = TETRA_LEVELS.GetLevelsCompleted();
		m_playerUI = TETRA_GAME_OBJECTS.FindObjectWithTag(T_PlayerUI);
		m_pTransPlayerUI = m_playerUI->GetComponent<Transform>(ComponentType::C_Transform);
		if (!m_pTransPlayerUI)
		{
			printf(" No PlayerUI Transform found.\n");
		}
		if (m_currentLevel > (int)m_Nodes.size())
		{
			m_pTransPlayerUI->SetPosition(m_Nodes[m_Nodes.size() - 1].m_pGO->GetComponent<Transform>(ComponentType::C_Transform)->GetPosition() + m_offset);
			m_startPos = m_pTransPlayerUI->GetPosition();
			return;
		}
		m_pTransPlayerUI->SetPosition(m_Nodes[m_currentLevel-1].m_pGO->GetComponent<Transform>(ComponentType::C_Transform)->GetPosition() + m_offset);
		m_startPos = m_pTransPlayerUI->GetPosition();
	}

	else if (pEvent->Type() == EVENT_OnCanvasDeactivated)
	{
		//std::cout << "canvas deaactivated called" << std::endl;
		++m_currentLevel;

		if (m_currentLevel > (int)m_Nodes.size())
		{
			m_isPlayAnim = false;
			m_changeLevel = true;
			return;
		}
		m_finalPos = m_Nodes[m_currentLevel - 1].m_pGO->GetComponent<Transform>(ComponentType::C_Transform)->GetPosition() + m_offset;
		m_Move = true;
	}
	
}
