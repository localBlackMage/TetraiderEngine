#pragma once
#include "Component.h"


class ListSelector;

class SelectableButton :
	public Component
{
private:
	bool m_isRightSelectable;
	bool m_isLeftSelectable;
	
	//pointer of the owner with text
	GameObject* m_pOwner;
	ListSelector* m_pList;

public:
	SelectableButton();
	~SelectableButton();
	static Component* CreateInstance() { return new SelectableButton(); }
	
	// Inherited via Component
	 void Update(float dt);
	 void Serialize(const json & j);
	 void HandleEvent(Event* pEvent);
	 void LateInitialize();
	 void SetSelectableOwner(GameObject*);
};

