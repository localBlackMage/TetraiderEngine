#include <Stdafx.h>

#define SHAPE_STR "SHAPE"

#undef SHAPE_STR

#pragma endregion

ParticleEmitterResizer::ParticleEmitterResizer() :
	Component(ComponentType::C_ParticleEmitterResizer)
{
}

ParticleEmitterResizer::~ParticleEmitterResizer()
{
}

void ParticleEmitterResizer::Deactivate()
{
}

void ParticleEmitterResizer::Override(const json & j)
{
	if (m_pParticleEmitter->m_pSpawnShape) {
		if (!m_pTransform) {
			m_pTransform = pGO->GetComponent<Transform>(ComponentType::C_Transform);
			assert(m_pTransform && "No Transform component found. ParticleEmitterResizer component failed to operate.");
		}

		float xScale = abs(m_pTransform->GetScaleX());
		float yScale = abs(m_pTransform->GetScaleY());
		float particleScale = m_pParticleEmitter->m_scale.amplitude;
		particleScale *= 2.f;

		switch (m_pParticleEmitter->m_pSpawnShape->type) {
		case ShapeType::ST_AABB: {
			AABB* aabb = static_cast<AABB*>(m_pParticleEmitter->m_pSpawnShape);
			aabb->width = xScale - particleScale;
			aabb->halfWidth = aabb->width / 2.f;
			aabb->height = yScale - particleScale;
			aabb->halfHeight = aabb->height / 2.f;
			break;
		}
		case ShapeType::ST_Circle: {
			Circle* circle = static_cast<Circle*>(m_pParticleEmitter->m_pSpawnShape);
			circle->radius = min(xScale, yScale) - particleScale * 2.f;
			break;
		}
		}
	}
}

void ParticleEmitterResizer::LateInitialize()
{
	assert(pGO && "No Game Object found. ParticleEmitterResizer component failed to operate.");
	if (!m_pParticleEmitter) {
		m_pParticleEmitter = pGO->GetComponent<ParticleEmitter>(ComponentType::C_ParticleEmitter);
		assert(m_pParticleEmitter && "No ParticleEmitter component found. ParticleEmitterResizer component failed to operate.");
	}
}

