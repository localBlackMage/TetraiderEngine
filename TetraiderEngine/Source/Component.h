#ifndef COMPONENT_H
#define COMPONENT_H

#include "JsonReader.h"
#include "GameObject.h"
#include "Subscriber.h"

using json = nlohmann::json;
using namespace JsonReader;

enum class ComponentType {
	Transform,
	Sprite,
	Animation,
	Controller,
	Body,
	Camera,
	CamFollow,
	Health,
	FlashOnTakeDamage,

	NONE
};

class Component : 
	public Subscriber
{
public:
	Component(ComponentType _type) : type(_type) {};
	virtual ~Component() {};
	virtual void LateInitialize() {};
	virtual void Update(double dt) = 0;
	virtual void LateUpdate(double dt) {};
	virtual void Serialize(json j) = 0;
	virtual void HandleEvent(Event* pEvent) {}

	GameObject *pGO;
	ComponentType type;
};


#endif
