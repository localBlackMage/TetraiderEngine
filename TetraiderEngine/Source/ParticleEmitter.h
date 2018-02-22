#pragma once

#ifndef PARTICLE_EMITTER_H
#define PARTICLE_EMITTER_H

#include "Component.h"
#include "Transform.h"
#include "SurfaceTextureBuffer.h"
#include "Mesh.h"
#include "Math\MathLibs.h"
#include <glew.h>
#include <string>

class GameObject;

struct Color {
	explicit Color() : r(0), g(0), b(0), a(0) {}
	GLubyte r, g, b, a;
};

struct Particle {
	Vector3D m_pos, m_velocity;
	Color m_color;
	float m_scale, m_weight;
	float m_life;
	float m_cameraDistance;

	explicit Particle() :
		m_pos(Vector3D()),
		m_velocity(Vector3D()),
		m_color(Color()),
		m_scale(0.f),
		m_weight(0.f),
		m_life(0.f),
		m_cameraDistance(-1.f)
	{}

	bool operator<(Particle& that) {
		// Sort in reverse order : far particles drawn first.
		return this->m_cameraDistance > that.m_cameraDistance;
	}
};

class ParticleEmitter : public Component {
protected:
	// Emitter Configuration Properties
	float m_loopDuration;		// Time it takes for the Emitter to complete a cycle in seconds
	bool m_looping;				// Whether or not the Emitter should loop the emission process
	bool m_prewarmed;			// Starts the Emitter as though it had completed a cycle of emission
	float m_startDelay;			// How long the emitter will wait to emit in seconds
	float m_lifeTime;			// How long a particle will live in seconds
	float m_speed;				// Speed of a particle in the starting direction
	float m_size;				// X and Y scale of a particle
	float m_rotation;			// Z-Axis rotation of a particle at start
	Color m_color;			// Color of a particle at start
	float m_gravityMod;			// Scales gravity applied to particles
	int m_emissionRate;			// How many particles to emit over the duration of a single loop
	int m_maxParticles;			// Max number of particles allowed at once - emission will halt temporarily if this number is met
	float m_rotationOverTime;	// Speed at which a particle will rotate during it's lifetime


	// Emitter Run Properties
	Transform* m_pTransform;	// Parent GO is required to have a Transform for an emitter
	float m_currentTime;		// Current time (t) within this loop
	float m_emissionTime;		// How long to wait to spawn the next particle
	float m_emissionTimer;		// How long since the last particle was spawned

	// Particle Configuration Properties
	SurfaceTextureBuffer * m_texture;

	// Particle Manager Properties
	Particle* m_particles;
	Mesh& m_mesh;
	GLuint m_positionsScalesBuffer;			// OpenGL Buffer where positions and sizes are to be streamed
	GLuint m_colorsBuffer;					// OpenGL Buffer where colors are to be streamed

	GLfloat* m_positionsScales;				// Array of positions and sizes (xyz pos, w is uniform scale)
	GLubyte* m_colors;						// Array of Colors split up into it's components
	int m_liveParticleCount;				// Indicates how many particles are currently alive, set each Update loop
	int m_lastUsedParticle;					// An index into m_particles, represents the index of the last used particle

	int _FindUnusedParticle();
	void _SpawnParticle();
	void _SortParticles() { std::sort(&m_particles[0], &m_particles[m_maxParticles]); }
	void _UpdateParticles(float deltaTime);
	void _AllocateParticleArrays();
	void _AllocateVBOs();
public:
	ParticleEmitter();
	~ParticleEmitter();

	static Component* CreateInstance() { return new ParticleEmitter(); }

	virtual void Update(float dt);
	virtual void LateUpdate(float dt);
	virtual void Deactivate();
	virtual void Serialize(const json& j);
	virtual void Override(const json& j);
	virtual void LateInitialize();
	virtual void HandleEvent(Event* pEvent);

	const Mesh& GetMesh() const { return m_mesh; };
	void SetMesh(Mesh& mesh) { m_mesh = mesh; }

	GLuint GetTextureBuffer() const { return m_texture->bufferId; }
	int GetAlphaMode() const { return m_texture->alphaMode; }
	void BindBufferDatas() const;
	GLuint GetPositions() const { return m_positionsScalesBuffer; }
	GLuint GetColors() const { return m_colorsBuffer; }

	int LiveParticles() const { return m_liveParticleCount; }
};

#endif

