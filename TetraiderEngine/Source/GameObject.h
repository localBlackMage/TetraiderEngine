/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Moodie Ghaddar>
- End Header --------------------------------------------------------*/

#pragma once

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

class GameObject 
{
private:
	Component* mComponents[ComponentType::NUM_COMPONENTS];
	std::vector<ComponentType> m_activeComponents;
	unsigned int m_id;
	unsigned short m_layer;
	bool m_isSetToDestroy;
	float m_destroyTimer;
	float m_destroySetTimeStamp;
public:
	bool m_isActive;
	bool m_isRender;
	bool m_isCollisionDisabled;
	bool m_isStatic;
	bool m_isDestroy;
	GameObjectTag m_tag;

	GameObject(unsigned int id);
	~GameObject();

	bool operator==(const GameObject& rhs) const;

	virtual void HandleEvent(Event* pEvent);
	void Update(float dt);
	void LateUpdate(float dt);
	void UpdateForLevelEditor(float dt);
	void LateUpdateForLevelEditor(float dt);
	void LateInitialize();
	void Destroy();
	void DestroyIn(float time);
	void Deactivate();

	void OverrideComponents(const json & j);

	void SetLayer(RENDER_LAYER layer) { m_layer = layer; }
	unsigned short GetLayer() { return m_layer; }

	unsigned int ID() const { return m_id; }
	void SetID(unsigned int id) { m_id = id; }
	
	void SetParent(GameObject* pParent);
	void SetActive(bool active);
	bool IsParented();
	
	void SwitchTag(GameObjectTag t) { m_tag = t; }
	inline GameObjectTag Tag() const { return m_tag; }
	inline bool TagIs(GameObjectTag tag) const { return m_tag == tag; }

	void AddComponent(Component* pComponent);
	bool HasComponent(ComponentType type) const;

	template <typename C>
	C* GetComponent(ComponentType type) {
		return static_cast<C*>(mComponents[type]);
	}

	template <typename C>
	const C * GetComponent(ComponentType type) const
	{
		return static_cast<C*>(mComponents[type]);
	}
};

bool operator== (GameObject & lhs, GameObject & rhs)
{
	return lhs.ID() == rhs.ID();
}

#endif 

