#include "Particle.h"
#include "GameObject.h"
#include "TetraiderAPI.h"
#include <iostream>

#pragma region Private Methods

#pragma endregion

Particle::Particle() :
	Component(ComponentType::C_Particle),
	m_mesh(*TETRA_RESOURCES.LoadMesh("quad"))
{}

Particle::~Particle() {}

#pragma region Component Methods

void Particle::LateInitialize()
{
	//if (!m_pTransform) {
	//	if (pGO)
	//		m_pTransform = pGO->GetComponent<Transform>(ComponentType::C_Transform);
	//	else {
	//		std::cout << "No Game Object found. Particle component failed to operate." << std::endl;
	//		return;
	//	}

	//	if (!m_pTransform) {
	//		std::cout << "No Transform component found. Particle component failed to operate." << std::endl;
	//		assert(m_pTransform);
	//		return;
	//	}
	//}
}

void Particle::Update(float dt)
{
	
}

void Particle::Serialize(const json & j)
{
}

void Particle::LateUpdate(float dt)
{
}

void Particle::HandleEvent(Event * p_event)
{
}

#pragma endregion
