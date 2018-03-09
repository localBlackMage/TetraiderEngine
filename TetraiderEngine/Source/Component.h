#pragma once

#ifndef COMPONENT_H
#define COMPONENT_H

class GameObject;

class Component : 
	public Subscriber
{
protected:
	ComponentType m_type;
public:
	Component(ComponentType _type) : m_type(_type) {};
	virtual ~Component() {};
	virtual void LateInitialize() {};
	virtual void Update(float dt) = 0;
	virtual void LateUpdate(float dt) {};
	virtual void Serialize(const json& j) = 0;
	virtual void Deactivate() {};
	virtual void Override(const json& j) {};
	virtual void HandleEvent(Event* pEvent) {}
	ComponentType Type() const { return m_type; }

	GameObject *pGO;
};


#endif
