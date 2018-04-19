/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Moodie Ghaddar>
- End Header --------------------------------------------------------*/

#pragma once

class Shop :public Component
{
private:
	bool m_isShopOpen;
	bool m_isListenToInput;
	float m_radius;
	std::string m_controlsPrefab;
	GameObject* m_pControlsObject;
	Vector3D m_posOffset;
public:
	Shop();
	~Shop();
	static Component* CreateInstance() { return new Shop(); }
	void Update(float dt);
	void Serialize(const json& j);
	void LateInitialize();
	void HandleEvent(Event* pEvent);
};

