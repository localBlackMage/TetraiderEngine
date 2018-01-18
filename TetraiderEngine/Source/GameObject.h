#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <vector>

// Forward declaration
class Component;
enum class ComponentType;

enum class GameObjectTag {
	Player,

	NONE
};

class GameObject {
public:
	GameObject();
	~GameObject();

	void Update(float dt);
	void AddComponent(Component* pComponent);
	void LateInitialize();
	void Destroy();
	Component* GetComponent(ComponentType type);
	
	bool m_isActive;
	bool m_isRender;
	bool m_isCollisionDisabled;
	bool m_isDestroy;
	GameObjectTag m_tag;
	// TODO 
	// void HandleEvent(Event* pEvent);
	// void AddEventSubscription(EventType type);
private:
	std::vector<Component*> mComponents;
};

#endif 

