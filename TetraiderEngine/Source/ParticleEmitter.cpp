#include "TetraiderAPI.h"
#include "ParticleEmitter.h"
#include "GameObject.h"
#include <iostream>
#include <cstdlib>

#pragma region Private Methods

static Color Lerp(const Color& colorA, const Color& colorB, float t)
{
	if (t > 1)
		t = 1;
	else if (t < 0)
		t = 0;
	float tMinus = 1.f - t;

	return Color(
		int(colorA.r * tMinus + t * colorB.r),
		int(colorA.g * tMinus + t * colorB.g),
		int(colorA.b * tMinus + t * colorB.b),
		int(colorA.a * tMinus + t * colorB.a)
	);
}

int ParticleEmitter::_FindUnusedParticle()
{
	for (int i = m_lastUsedParticle; i<m_maxParticles; i++) {
		if (m_particles[i].m_life <= 0.f) {
			m_lastUsedParticle = i;
			return i;
		}
	}
	// Cycle around to the start of the array
	for (int i = 0; i<m_lastUsedParticle; i++) {
		if (m_particles[i].m_life <= 0.f) {
			m_lastUsedParticle = i;
			return i;
		}
	}

	return -1; // All particles are taken
}

void ParticleEmitter::_SpawnParticle()
{
	int idx = _FindUnusedParticle();
	if (idx > -1) {
		m_particles[idx].m_life = m_lifeTime;
		m_particles[idx].m_pos = m_pTransform->GetPosition();			// TODO: Update these
		float HI = 250.f;
		float LO = -HI;
		float x = LO + static_cast<float>(rand()) / static_cast<float>(RAND_MAX/(HI-LO));
		m_particles[idx].m_velocity = m_pTransform->LookAt() * m_speed;		// TODO: Update these
	}
}

void ParticleEmitter::_UpdateParticles(float deltaTime)
{
	const Vector3D Gravity = Vector3D(0, -98.f * m_gravityMod, 0);
	m_liveParticleCount = 0;
	for (int i = 0; i<m_maxParticles; i++) {

		Particle& p = m_particles[i];

		if (p.m_life > 0.0f) {

			// Decrease life
			p.m_life -= deltaTime;
			if (p.m_life > 0.0f) {
				p.m_velocity += Gravity * deltaTime;
				p.m_pos += p.m_velocity * deltaTime;
				p.m_pos.z = 0.f;

				p.m_scale = m_size;

				// TODO: decide on a better way to get a camera
				p.m_cameraDistance = Vector3D::SquareDistance(p.m_pos, TETRA_GAME_OBJECTS.GetCamera(0)->GetComponent<Transform>(ComponentType::C_Transform)->GetPosition());

				Color color = Lerp(m_colorStart, m_colorEnd, m_lifeTime - p.m_life);
				p.m_color.r = color.r;
				p.m_color.g = color.g;
				p.m_color.b = color.b;
				p.m_color.a = color.a;

				// Fill the GPU buffer
				m_positionsScales[4 * m_liveParticleCount + 0] = p.m_pos.x;
				m_positionsScales[4 * m_liveParticleCount + 1] = p.m_pos.y;
				m_positionsScales[4 * m_liveParticleCount + 2] = p.m_pos.z;

				m_positionsScales[4 * m_liveParticleCount + 3] = p.m_scale;

				m_colors[4 * m_liveParticleCount + 0] = p.m_color.r;
				m_colors[4 * m_liveParticleCount + 1] = p.m_color.g;
				m_colors[4 * m_liveParticleCount + 2] = p.m_color.b;
				m_colors[4 * m_liveParticleCount + 3] = p.m_color.a;

			}
			else {
				// Particles that just died will be put at the end of the buffer in SortParticles();
				p.m_cameraDistance = -1.0f;
			}
			++m_liveParticleCount;
		}
	}
}

void ParticleEmitter::_AllocateParticleArrays()
{
	_DeallocateParticleArrays();

	m_particles = (Particle*)TETRA_MEMORY.Alloc(sizeof(Particle) * m_maxParticles);			//(Particle*)malloc(sizeof(Particle) * m_maxParticles);
	m_positionsScales = (GLfloat*)TETRA_MEMORY.Alloc(sizeof(GLfloat) * m_maxParticles * 4); //(GLfloat*)malloc(sizeof(GLfloat) * m_maxParticles * 4);
	m_colors = (GLubyte*)TETRA_MEMORY.Alloc(sizeof(GLubyte) * m_maxParticles * 4);			//(GLubyte*)malloc(sizeof(GLubyte) * m_maxParticles * 4);

	for (int i = 0; i < m_maxParticles; i += 4) {
		m_positionsScales[i] = 0.f;
		m_positionsScales[i + 1] = 0.f;
		m_positionsScales[i + 2] = 0.f;
		m_positionsScales[i + 3] = 0.f;

		m_colors[i] = 0;
		m_colors[i + 1] = 0;
		m_colors[i + 2] = 0;
		m_colors[i + 3] = 0;
	}
}

void ParticleEmitter::_DeallocateParticleArrays()
{
	if (m_particles)		TETRA_MEMORY.Free(m_particles);
	if (m_positionsScales)	TETRA_MEMORY.Free(m_positionsScales);
	if (m_colors)			TETRA_MEMORY.Free(m_colors);
}

void ParticleEmitter::_AllocateVBOs()
{
	m_positionsScalesBuffer = TETRA_RENDERER.GenerateStreamingVBO(m_maxParticles * 4 * sizeof(GLfloat));
	m_colorsBuffer = TETRA_RENDERER.GenerateStreamingVBO(m_maxParticles * 4 * sizeof(GLubyte));
}

#pragma endregion

ParticleEmitter::ParticleEmitter() :
	Component(ComponentType::C_ParticleEmitter),
	m_currentTime(0.f),
	m_emissionTime(0.f),
	m_emissionTimer(0.f),
	m_liveParticleCount(0),
	m_mesh(*TETRA_RESOURCES.LoadMesh("quad"))
{}

ParticleEmitter::~ParticleEmitter() 
{
	_DeallocateParticleArrays();
}

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

		_UpdateParticles(dt);

		if (m_looping && m_currentTime >= m_loopDuration)
			m_currentTime = 0.f;
	}
}

void ParticleEmitter::LateUpdate(float dt)
{
}

void ParticleEmitter::Deactivate()
{
	pGO = nullptr;
	m_pTransform = nullptr;

	_DeallocateParticleArrays();
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

	Vector3D colorStart = ParseColor(j, "colorStart");
	m_colorStart.r = int(colorStart.x * 255.f);
	m_colorStart.g = int(colorStart.y * 255.f);
	m_colorStart.b = int(colorStart.z * 255.f);
	m_colorStart.a = int(colorStart.w * 255.f);
	Vector3D colorEnd = ParseColor(j, "colorEnd");
	m_colorEnd.r = int(colorEnd.x * 255.f);
	m_colorEnd.g = int(colorEnd.y * 255.f);
	m_colorEnd.b = int(colorEnd.z * 255.f);
	m_colorEnd.a = int(colorEnd.w * 255.f);

	m_gravityMod = ParseFloat(j, "gravity");
	m_emissionRate = ParseInt(j, "emissionRate");
	m_maxParticles = ParseInt(j, "max");
	m_rotationOverTime = ParseFloat(j, "rotationOverTime");
	std::string textureName = ParseString(j, "particleTexture");

	m_texture = TETRA_RESOURCES.GetTexture(textureName);
	m_emissionTime = m_loopDuration / float(m_emissionRate);

	//_AllocateParticleArrays();
	//_AllocateVBOs();
	m_liveParticleCount = 0;
}

void ParticleEmitter::Override(const json & j)
{
	// TODO: Find way to release previous VBOs
	_AllocateParticleArrays();
	_AllocateVBOs();
}

void ParticleEmitter::HandleEvent(Event * p_event)
{
}

void ParticleEmitter::BindBufferDatas() const
{
	TETRA_RENDERER.BindBufferData(m_positionsScalesBuffer, m_positionsScales, m_liveParticleCount * sizeof(GLfloat) * 4);
	TETRA_RENDERER.BindBufferData(m_colorsBuffer, m_colors, m_liveParticleCount * sizeof(GLubyte) * 4);
}

#pragma endregion
