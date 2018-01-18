#ifndef COMPONENT_H
#define COMPONENT_H

#include "JsonReader.h"
#include "GameObject.h"

using json = nlohmann::json;

enum class ComponentType {
	Transform,
	Sprite,

	NONE
};

class Component {
public:
	Component() {};
	virtual ~Component() {};
	virtual void LateInitialize() = 0;
	virtual void Update(float dt) = 0;
	virtual void Serialize(json j) = 0;
	// TODO
	// virtual void LateUpdate(float dt) = 0;
	// virtual void HandleEvent(Event* pEvent) {}

	GameObject *pGO;
	ComponentType type;
};


#endif
