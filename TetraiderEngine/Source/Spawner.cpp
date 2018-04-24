/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

Spawner::Spawner() :
	Component(ComponentType::C_Spawner)
{

}

Spawner::~Spawner(){}

void Spawner::Deactivate() {
	pGO = nullptr;
	m_pTransform = nullptr; 
}

void Spawner::Serialize(const json& j)
{
	m_prefab = ParseString(j, "prefab");
	m_offset = ParseVector3D(j, "offset");
}

void Spawner::Spawn()
{
	Spawn(Vector3D(0,0,0,0));
}

void Spawner::Spawn(const Vector3D& offset)
{
	GameObject* pProjectileGO = TETRA_GAME_OBJECTS.CreateGameObject(m_prefab, true, m_pTransform->GetPosition() + m_offset + offset);
}

void Spawner::LateInitialize()
{
	if (!m_pTransform) {
		assert(pGO && "No Game Object found. Spawner component failed to operate.");
		m_pTransform = pGO->GetComponent<Transform>(ComponentType::C_Transform);
		assert(m_pTransform && "No Transform component found. Spawner component failed to operate.");
	}
}

void Spawner::Update(float dt)
{

}

void Spawner::LateUpdate(float dt)
{

}