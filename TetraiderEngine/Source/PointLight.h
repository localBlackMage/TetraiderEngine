#pragma once
#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

class PointLight : public LightBase
{
public:
	PointLight();
	~PointLight() {};

	static Component* CreateInstance() { return new PointLight(); }
	virtual void HandleEvent(Event* pEvent) {};
};

#endif

