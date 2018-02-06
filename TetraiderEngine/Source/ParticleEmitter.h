#pragma once

#ifndef PARTICLE_EMITTER_H
#define PARTICLE_EMITTER_H

#include "Component.h"
#include "Transform.h"
#include "Math\MathLibs.h"

class ParticleEmitter : public Component {
protected:
	float m_loopDuration;		// Time it takes for the Emitter to complete a cycle in seconds
	bool m_looping;				// Whether or not the Emitter should loop the emission process
	bool m_prewarmed;			// Stars the Emitter as though it had completed a cycle of emission
	float m_startDelay;			// How long the emitter will wait to emit in seconds
	float m_lifeTime;			// How long a particle will live in seconds
	float m_speed;				// Speed of a particle in the starting direction
	float m_size;				// X and Y scale of a particle
	float m_rotation;			// Z-Axis rotation of a particle at start
	Vector3D m_color;			// Color of a particle at start
	float m_gravityMod;			// Scales gravity applied to particles
	int m_maxParticles;			// Max number of particles allowed at once - emission will halt temporarily if this number is met
	float m_rotationOverTime;	// Speed at which a particle will rotate during it's lifetime

	Transform* m_pTransform;	// Parent GO is required to have a Transform for an emitter


public:
	ParticleEmitter();
	~ParticleEmitter();

	virtual void LateInitialize();
	virtual void Update(float dt);
	virtual void LateUpdate(float dt);
	virtual void Serialize(json j);

	virtual void HandleEvent(Event * p_event);
};

#endif

