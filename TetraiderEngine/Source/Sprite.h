#ifndef SPRITE_H
#define SPRITE_H

#include "Component.h"

// Forward declaration
class Vector3D;

class Sprite :public Component {
public:
	Sprite();
	~Sprite();
	virtual void Update(float dt);
	virtual void Serialize(json j);
	virtual void LateInitialize() {}
	virtual void LateUpdate(float dt) {}
	std::string texturePath;
	float m_xTiling;
	float m_yTiling;
	float m_uOffset;
	float m_vOffset;
	Vector3D* m_pColor;
	bool m_hasAlpha;
private:
};

#endif

