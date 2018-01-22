#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <vector>

// Forward declaration
class Component;
enum class ComponentType;

enum class GameObjectTag {
	Player,
	Camera,

	NONE
};

class GameObject {
public:
	GameObject(unsigned int id);
	~GameObject();

	bool operator==(const GameObject& rhs) const;

	void Update(float dt);
	void LateUpdate(float dt);
	void AddComponent(Component* pComponent);
	void LateInitialize();
	void Destroy();
	Component* GetComponent(ComponentType type);
	const Component* GetComponent(ComponentType type) const;
	bool HasComponent(ComponentType type);
	
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
	unsigned int m_id;
};

#endif 

