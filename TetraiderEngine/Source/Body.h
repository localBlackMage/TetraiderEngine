#ifndef BODY_H
#define BODY_H

#include "Component.h"

// Forward declaration
class Transform;

class Body : public Component {
public:
	Body();
	~Body();
	virtual void Update(float dt);
	virtual void Serialize(json j);
	virtual void LateInitialize();
	virtual void LateUpdate(float dt) {}
private:
	Transform* m_pTransform;
};

#endif

