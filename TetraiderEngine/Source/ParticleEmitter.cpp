#include "ParticleEmitter.h"
#include "GameObject.h"
#include <iostream>

ParticleEmitter::ParticleEmitter() : Component(ComponentType::C_ParticleEmitter) {}

ParticleEmitter::~ParticleEmitter() {}

void ParticleEmitter::LateInitialize()
{
	if (!m_pTransform) {
		if (pGO)
			m_pTransform = pGO->GetComponent<Transform>(ComponentType::C_Transform);
		else {
			std::cout << "No Game Object found. ParticleEmitter component failed to operate." << std::endl;
			return;
		}

		if (!m_pTransform) {
			std::cout << "No Sprite component found. ParticleEmitter component failed to operate." << std::endl;
			return;
		}
	}
}

void ParticleEmitter::Update(float dt)
{
}

void ParticleEmitter::LateUpdate(float dt)
{
}

void ParticleEmitter::Serialize(json j)
{
	m_loopDuration = ParseFloat(j, "loopDuration");
	m_looping = ParseBool(j, "looping");
}

void ParticleEmitter::HandleEvent(Event * p_event)
{
}


