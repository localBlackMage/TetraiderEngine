/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

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
	void SetActive(bool active);
};

#endif

