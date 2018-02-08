#include "ParticleEmitter.h"
#include "GameObject.h"
#include <iostream>

#pragma region Private Methods

void ParticleEmitter::_SpawnParticle()
{

}

#pragma endregion

ParticleEmitter::ParticleEmitter() :
	Component(ComponentType::C_ParticleEmitter),
	m_currentTime(0.f),
	m_emissionTime(0.f),
	m_emissionTimer(0.f)
{}

ParticleEmitter::~ParticleEmitter() {}

#pragma region Component Methods

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
			std::cout << "No Transform component found. ParticleEmitter component failed to operate." << std::endl;
			assert(m_pTransform);
			return;
		}
	}
}

void ParticleEmitter::Update(float dt)
{
	if (m_startDelay > 0.f) {
		m_startDelay -= dt;
		return;
	}

	if (m_currentTime < m_loopDuration) {
		m_currentTime += dt;
		m_emissionTimer += dt;

		while (m_emissionTimer > m_emissionTime) {
			_SpawnParticle();
			m_emissionTimer -= m_emissionTime;
		}

		// Update Particles

		if (m_looping && m_currentTime >= m_loopDuration)
			m_currentTime = 0.f;
	}
}

void ParticleEmitter::Serialize(const json & j)
{
	m_loopDuration = ParseFloat(j, "loopDuration");
	m_looping = ParseBool(j, "looping");
	m_prewarmed = ParseBool(j, "prewarmed");
	m_startDelay = ParseFloat(j, "startDelay");
	m_lifeTime = ParseFloat(j, "lifeTime");
	m_speed = ParseFloat(j, "speed");
	m_size = ParseFloat(j, "size");
	m_rotation = ParseFloat(j, "rotation");
	m_color = ParseColor(j, "color");
	m_gravityMod = ParseFloat(j, "gravity");
	m_emissionRate = ParseInt(j, "emissionRate");
	m_maxParticles = ParseInt(j, "max");
	m_rotationOverTime = ParseFloat(j, "rotationOverTime");

	m_emissionTime = m_loopDuration / float(m_emissionRate);
}

void ParticleEmitter::LateUpdate(float dt)
{
}

void ParticleEmitter::HandleEvent(Event * p_event)
{
}

#pragma endregion
