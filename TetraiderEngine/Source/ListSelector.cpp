#include "Stdafx.h"
#include "ListSelector.h"


ListSelector::ListSelector():Component(ComponentType::C_ListSelector)
{
	
}


ListSelector::~ListSelector()
{
}

void ListSelector::Update(float dt)
{
	if (m_pText) {
		m_pText->SetText(m_Choice[m_currentChoice]);
		m_pText->SetOffset(m_pListTransform->GetPosition());
	}
}

void ListSelector::Serialize(const json & j)
{
	m_spawnOffset = ParseFloat(j,"spawnOffset");
	m_isResolution = ParseBool(j,"isResolution");
	m_leftSelectorPrefab = ParseString(j, "leftSelectorPrefab");
	m_rightSelecetorPrefab = ParseString(j, "rightSelectorPrefab");
	m_defaultChoice = ParseInt(j, "defaultChoice");
	m_currentChoice = m_defaultChoice;

	int numberOfElements = j["Choice"].size();
	for (int i = 0; i<numberOfElements; ++i)
	{
		GameObject *pObject;
		m_Choice.push_back( ParseString(j["Choice"][i], "text"));		
	}
}

void ListSelector::HandleEvent(Event * pEvent)
{
}

void ListSelector::LateInitialize()
{
	if (m_pListTransform == nullptr) {
		if (pGO != nullptr)
			m_pListTransform = pGO->GetComponent<Transform>(ComponentType::C_Transform);
		else {
			printf("No Game Object found. ListSelector component failed to operate.\n");
			return;
		}

		if (m_pListTransform == nullptr) {
			printf("No Transform component found. Transform component failed to operate.\n");
			assert(m_pListTransform);
			return;
		}
	}

	if (m_pText == nullptr) {
		if (pGO != nullptr)
			m_pText = pGO->GetComponent<Text>(ComponentType::C_Text);
		else {
			printf("No Game Object found. ListSelector component failed to operate.\n");
			return;
		}

		if (m_pText == nullptr) {
			printf("No Text component found. Text component failed to operate.\n");
			assert(m_pText);
			return;
		}
	}

	Vector3D pos(m_pListTransform->GetPosition());
	GameObject* left = TETRA_GAME_OBJECTS.CreateGameObject(m_leftSelectorPrefab, true, Vector3D(pos.x - m_spawnOffset, pos.y, pos.z ));
	TETRA_UI.AddGameObjectToCanvas(CanvasType::CANVAS_VIDEO, left);
	SelectableButton* pLeft = left->GetComponent<SelectableButton>(ComponentType::C_SelectableButton);
	pLeft->SetSelectableOwner(this->pGO);

	GameObject* right = TETRA_GAME_OBJECTS.CreateGameObject(m_rightSelecetorPrefab, true, Vector3D(pos.x + m_spawnOffset, pos.y, pos.z ));
	TETRA_UI.AddGameObjectToCanvas(CanvasType::CANVAS_VIDEO, right);
	SelectableButton* pRight = right->GetComponent<SelectableButton>(ComponentType::C_SelectableButton);
	pRight->SetSelectableOwner(this->pGO);
}
