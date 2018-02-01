#pragma once
#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "Agent.h"

class Controller : public Agent {
public:
	Controller();
	~Controller();
	virtual void Update(float dt);
	virtual void LateInitialize();
	virtual void Serialize(json j);
	virtual void HandleEvent(Event* pEvent);
protected:
};

#endif

