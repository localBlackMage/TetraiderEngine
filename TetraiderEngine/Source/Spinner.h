/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Moodie Ghaddar>
- End Header --------------------------------------------------------*/

#pragma once

class Spinner : public Component {
protected:
	float m_speed;
	Transform* m_pTransform;

public:
	Spinner();
	~Spinner();

	static Component* CreateInstance() { return new Spinner(); }

	virtual void Update(float dt);
	virtual void LateUpdate(float dt);
	virtual void Deactivate();
	virtual void Serialize(const json& j);
	virtual void Override(const json& j);
	virtual void LateInitialize();
	virtual void HandleEvent(Event* pEvent);
};

