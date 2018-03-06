#pragma once

#ifndef PARTICLE_EMITTER_H
#define PARTICLE_EMITTER_H

//#include "Component.h"
//#include "Transform.h"
//#include "SurfaceTextureBuffer.h"
//#include "Mesh.h"
//#include "Color.h"
//#include "Math\MathLibs.h"
//#include <glew.h>
//#include <string>

class GameObject;

struct Particle {
	Vector3D m_pos, m_velocity;
	Color m_color;
	float m_scale, m_weight;
	float m_life;
	float m_animationTime;
	float m_cameraDistance;
	TexCoords m_texCoords;

	explicit Particle() :
		m_pos(Vector3D()),
		m_velocity(Vector3D()),
		m_color(Color()),
		m_scale(0.f),
		m_weight(0.f),
		m_life(0.f),
		m_animationTime(0.f),
		m_cameraDistance(-1.f),
		m_texCoords(TexCoords(0.f, 0.f))
	{}

	bool operator<(Particle& that) {
		// Sort in reverse order : far particles drawn first.
		return this->m_cameraDistance > that.m_cameraDistance;
	}
};

enum class P_TextureSelection {
	CYCLE, RANDOM, SINGLE
};

struct InterpolationItem {			
	ControlPoints points;	// Control Points for bezier curve interpolation between start and end
	float amplitude;		// How much of an offset to give the result from the curve

	void Serialize(const json& j, const std::string& item) {
		points = ParsePoints(j, item, "points");
		amplitude = ParseFloat(j, item, "amplitude");
	}
};

class ParticleEmitter : public Component {
protected:
	// Emitter Configuration Properties
	float m_loopDuration;						// Time it takes for the Emitter to complete a cycle in seconds
	bool m_looping;								// Whether or not the Emitter should loop the emission process
	bool m_prewarmed;							// Starts the Emitter as though it had completed a cycle of emission
	float m_startDelay;							// How long the emitter will wait to emit in seconds
	float m_lifeTime;							// How long a particle will live in seconds
	float m_animationSpeed;						// How long until a particle cycles to the next frame
	float m_speed;								// Speed of a particle in the starting direction
	InterpolationItem m_velocityX;				// Velocity x offsets of a particle at the start of it's lifetime
	InterpolationItem m_velocityY;				// Velocity y offsets of a particle at the start of it's lifetime
	InterpolationItem m_scale;					// X & Y scale of a particle at start and end, points to interpolate between
	float m_rotation;							// Z-Axis rotation of a particle at start
	Color m_colorStart;							// Color of a particle at start 
	Color m_colorEnd;							// Color of a particle at end
	float m_gravityMod;							// Scales gravity applied to particles
	int m_emissionRate;							// How many particles to emit over the duration of a single loop
	int m_maxParticles;							// Max number of particles allowed at once - emission will halt temporarily if this number is met
	float m_rotationOverTime;					// Speed at which a particle will rotate during it's lifetime
	bool m_randomEmission;						// Whether or not the Emitter should "randomly" emit new particles
	P_TextureSelection m_textureSelection;		// Whether or not particles should cycle through the available particle frames or choose one somehow


	// Emitter Run Properties
	bool m_active;								// Determines whether or not this emitter will run
	Transform* m_pTransform;					// Parent GO is required to have a Transform for an emitter
	float m_currentTime;						// Current time (t) within this loop
	float m_emissionTime;						// How long to wait to spawn the next particle
	float m_emissionTimer;						// How long since the last particle was spawned

	// Particle Configuration Properties
	SurfaceTextureBuffer * m_texture;			// Texture for all particles in this emitter
	float m_rows, m_cols;						// Number of rows & columns of sprite frames in the texture
	float m_frameHeight, m_frameWidth;			// Size of each frame in the sprite texture

	// Particle Manager Properties
	Particle* m_particles;						// Array of all existing Particles managed by this Emitter
	Mesh& m_mesh;
	GLuint m_positionsScalesBuffer;				// OpenGL Buffer where positions and sizes are to be streamed
	GLuint m_colorsBuffer;						// OpenGL Buffer where colors are to be streamed
	GLuint m_textureCoordsBuffer;				// OpenGL Buffer where texture coords are to be streamed

	GLfloat* m_positionsScales;					// Array of positions and sizes (xyz pos, w is uniform scale)
	GLubyte* m_colors;							// Array of Colors split up into it's components
	GLfloat* m_textureCoords;					// Array of texture coords
	int m_liveParticleCount;					// Indicates how many particles are currently alive, set each Update loop
	int m_lastUsedParticle;						// An index into m_particles, represents the index of the last used particle

	std::string m_shader;

	int _FindUnusedParticle();
	void _SpawnParticle();
	void _SortParticles() { std::sort(&m_particles[0], &m_particles[m_maxParticles]); }
	void _UpdateParticles(float deltaTime);
	void _AllocateParticleArrays();
	void _DeallocateParticleArrays();
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
	GLuint GetTextureCoords() const { return m_textureCoordsBuffer; }

	int LiveParticles() const { return m_liveParticleCount; }

	void ActivateParticles() { m_active = true; }
	void DeactivateParticles() { m_active = false; }

	float FrameWidth() const { return m_frameWidth; }
	float FrameHeight() const { return m_frameHeight; }

	std::string Shader() const { return m_shader; }
};

#endif

