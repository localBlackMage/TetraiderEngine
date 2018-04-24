/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#pragma once
#ifndef SPAWNER_H
#define SPAWNER_H

class Spawner : public Component
{
private:
	Transform* m_pTransform;

	std::string m_prefab;
	Vector3D m_offset;

public:
	Spawner();
	~Spawner();
	static Component* CreateInstance() { return new Spawner(); }
	virtual void Deactivate();
	virtual void LateInitialize();
	virtual void Update(float dt);
	virtual void LateUpdate(float dt);
	virtual void Serialize(const json& j);

	void Spawn();
	void Spawn(const Vector3D& offset);
};

#endif

