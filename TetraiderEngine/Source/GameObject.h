#pragma once

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <vector>
#include "Subscriber.h"
#include "GameObjectTags.h"
#include "ComponentTypes.h"

// Forward declaration
class Event;
class Component;

class GameObject :
 public Subscriber {
public:
	GameObject(unsigned int id);
	~GameObject();

	bool operator==(const GameObject& rhs) const;

	void Update(float dt);
	void LateUpdate(float dt);
	void AddComponent(Component* pComponent);
	void LateInitialize();
	void Destroy();

	template <typename C>
	C* GetComponent(ComponentType type) {
		for (auto &comp : mComponents) {
			if (comp->Type() == type)
				return static_cast<C*>(comp);
		}

		return nullptr;
	}

	template <typename C>
	const C * GetComponent(ComponentType type) const
	{
		for (auto &comp : mComponents) {
			if (comp->Type() == type)
				return static_cast<C*>(comp);
		}

		return nullptr;
	}

	bool HasComponent(ComponentType type) const;
	
	bool m_isActive;
	bool m_isRender;
	bool m_isCollisionDisabled;
	bool m_isDestroy;
	GameObjectTag m_tag;

	virtual void HandleEvent(Event* pEvent);
private:
	std::vector<Component*> mComponents;
	unsigned int m_id;
};

#endif 

