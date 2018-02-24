#include "PauseMenu.h"
#include "TetraiderAPI.h"
#include "Transform.h"

PauseMenu::PauseMenu():Component(ComponentType::C_PauseMenu)
{
	isActive = false;
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
		InputButtonData* pData = pEvent->Data<InputButtonData>();
		if (pData->m_isTrigger) {
			isActive = !isActive;
			for (auto gameObjects : m_objects)
			{
				gameObjects->SetActive(isActive);
			}
			pGO->m_isRender = isActive;
		}
		/*pObject = TETRA_GAME_OBJECTS.FindObjectWithTag(T_Pause);
		pObject->SetActive(!TETRA_GAME_STATE.IsGamePaused());*/
	}
}

void PauseMenu::LateInitialize()
{
	for (auto gameObjects : m_objects)
	{
		gameObjects->SetActive(isActive);
	}
	pGO->m_isRender = isActive;
	TETRA_EVENTS.Subscribe(EVENT_INPUT_PAUSEGAME, this);
}

