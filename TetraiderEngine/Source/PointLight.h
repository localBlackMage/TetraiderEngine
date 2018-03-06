#pragma once
#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

//#include "Component.h"
//#include "Color.h"

//class Transform;
//class GameObject;

class PointLight : public Component
{
private:
	Transform * m_pTransform;
	Color m_color;
	float m_distance;


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
	GLubyte Red() const { return m_color[0]; }
	GLubyte Green() const { return m_color[1]; }
	GLubyte Blue() const { return m_color[2]; }
	GLubyte Alpha() const { return m_color[3]; }
	Color GetColor() const { return m_color; }
	float Distance() const { return m_distance; }
};

#endif

