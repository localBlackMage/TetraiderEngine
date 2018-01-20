#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "Component.h"

// Forward declaration
class Transform;

class Controller : public Component {
public:
	Controller();
	~Controller();
	virtual void Update(float dt);
	virtual void Serialize(json j);
	virtual void LateInitialize();
	virtual void LateUpdate(float dt) {}
private:
	float m_speed;
	Transform* m_pTransform;
};

#endif

