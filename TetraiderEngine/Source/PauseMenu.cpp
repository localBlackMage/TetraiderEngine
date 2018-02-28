#include "PauseMenu.h"
#include "TetraiderAPI.h"
#include "Transform.h"
#include "Text.h"

PauseMenu::PauseMenu():Component(ComponentType::C_PauseMenu)
{
	isActive = false;
	isLevelOver = false;
}

PauseMenu::~PauseMenu(){}


void PauseMenu::Update(float dt)
{
}

void PauseMenu::Serialize(const json & j)
{
	int numberOfElements = j["UIElements"].size();
	float x, y, z;
	for (int i=0;i<numberOfElements;++i)
	{
		GameObject *pObject;
		m_prefabName = ParseString(j["UIElements"][i], "Buttonprefab");
		pObject=TETRA_GAME_OBJECTS.CreateGameObject(m_prefabName);
		m_objects.push_back(pObject);

		x = ParseFloat(j["UIElements"][i]["position"],"x");
		y = ParseFloat(j["UIElements"][i]["position"], "y");
		z = ParseFloat(j["UIElements"][i]["position"], "z");
		Transform* m_pTransform = pObject->GetComponent<Transform>(C_Transform);
		m_pTransform->SetPosition(Vector3D(x,y,z));
	}
}


void PauseMenu::HandleEvent(Event * pEvent)
{
	if (pEvent->Type() == EVENT_INPUT_PAUSEGAME)
	{
		if (isLevelOver) return;

		InputButtonData* pData = pEvent->Data<InputButtonData>();
		if (pData->m_isTrigger) {
			isActive = !isActive;
			for (auto gameObjects : m_objects)
			{
				gameObjects->SetActive(isActive);
			}
			pGO->m_isRender = isActive;
		}

		if (m_pText) {
			m_pText->SetText("PAUSED");
			m_pText->SetOffset(Vector3D(-130, 200, 0));
		}
	}
	else if (pEvent->Type() == EVENT_LevelComplete) {
		isLevelOver = true;
		isActive = true;
		for (auto gameObjects : m_objects)
		{
			gameObjects->SetActive(isActive);
		}
		pGO->m_isRender = isActive;

		if (m_pText) {
			m_pText->SetText("YOU WIN");
			m_pText->SetOffset(Vector3D(-160, 200, 0));
		}
	}
	else if (pEvent->Type() == EVENT_LevelInComplete) {
		isLevelOver = true;
		isActive = true;

		for (auto gameObjects : m_objects) {
			gameObjects->SetActive(isActive);
		}

		pGO->m_isRender = isActive;

		if (m_pText) {
			m_pText->SetText("YOU LOSE");
			m_pText->SetOffset(Vector3D(-180, 200, 0));
		}
	}
}

void PauseMenu::LateInitialize()
{
	for (auto gameObjects : m_objects) {
		gameObjects->SetActive(isActive);
	}

	if (!m_pText) {
		if (pGO)
			m_pText = pGO->GetComponent<Text>(ComponentType::C_Text);
		else {
			//printf("No Game Object found. EggCounter component failed to operate.\n");
			return;
		}

		if (!m_pText) {
			//printf("No Text component found. EggCounter component failed to operate.\n");
			//assert(m_pText);
			return;
		}
	}

	pGO->m_isRender = false;

	TETRA_EVENTS.Subscribe(EVENT_INPUT_PAUSEGAME, this);
	TETRA_EVENTS.Subscribe(EVENT_LevelComplete , this);
	TETRA_EVENTS.Subscribe(EVENT_LevelInComplete, this);
}
