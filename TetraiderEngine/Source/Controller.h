#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "Component.h"

// Forward declaration
class Transform;
class Body;

class Controller : public Component {
public:
	Controller();
	~Controller();
	virtual void Update(float dt);
	virtual void LateInitialize();
	virtual void Serialize(json j);
	virtual void Override(json j) {};
private:
	float m_speed;
	Transform* m_pTransform;
	Body* m_pBody;
};

#endif

