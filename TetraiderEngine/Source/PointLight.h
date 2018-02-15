#pragma once
#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include "Component.h"
class Transform;
class GameObject;

class PointLight : public Component
{
private:
	Transform * m_pTransform;
	unsigned char m_color[4]; // r, g, b, a;
	unsigned short m_distance;


public:
	PointLight();
	~PointLight() {};

	static Component* CreateInstance() { return new PointLight(); }
	virtual void DeActivate();
	virtual void LateInitialize();
	virtual void Update(float dt) {};
	virtual void LateUpdate(float dt) {};
	virtual void Serialize(const json& j);
	virtual void Override(const json& j);
	virtual void HandleEvent(Event* pEvent) {};

	Vector3D GetPosition() const;
	unsigned char Red() const { return m_color[0]; }
	unsigned char Blue() const { return m_color[1]; }
	unsigned char Green() const { return m_color[2]; }
	unsigned char Alpha() const { return m_color[3]; }
	unsigned short Distance() const { return m_distance; }
};

#endif

