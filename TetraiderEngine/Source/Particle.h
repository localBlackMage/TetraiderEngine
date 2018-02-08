#pragma once

#ifndef PARTICLE_H
#define PARTICLE_H

#include "Component.h"
#include "Transform.h"
#include "Math\MathLibs.h"

class Particle : public Component {
protected:
	Transform* m_pTransform;
public:
	Particle();
	~Particle();

	static Component* CreateInstance() { return new Particle(); }

	virtual void Update(float dt);
	virtual void Serialize(const json& j);
	virtual void LateInitialize();
	virtual void LateUpdate(float dt);
	virtual void HandleEvent(Event* pEvent);
};

#endif

