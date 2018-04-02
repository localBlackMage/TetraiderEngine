#include <Stdafx.h>

#define SHAPE_STR "SHAPE"

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

Vector3D ParticleEmitter::_GetSpawnPositionWithinShape()
{
	if (!m_pSpawnShape)
		return Vector3D();

	switch (m_pSpawnShape->type) {
		case ShapeType::ST_AABB: {
			AABB* aabb = static_cast<AABB*>(m_pSpawnShape);
			float x = RandomFloat(-aabb->halfWidth, aabb->halfWidth);
			float y = RandomFloat(-aabb->halfHeight, aabb->halfHeight);
			return Vector3D(x, y, 0.f);
		}
		case ShapeType::ST_Circle: {
			Circle* circle = static_cast<Circle*>(m_pSpawnShape);
			float d = RandomFloat(0.f, circle->radius);
			float rot = RandomFloat(-180.f, 180.f);
			return Matrix4x4::Rotate(rot, ZAXIS) * Vector3D( 0.f, 1.f * d, 0.f);
		}
		default:
			return Vector3D();
	}
}

void ParticleEmitter::_SpawnParticle()
{
	int idx = _FindUnusedParticle();
	if (idx > -1) {
		m_particles[idx].m_life = m_lifeTime;
		m_particles[idx].m_pos = m_pTransform->GetPosition() + _GetSpawnPositionWithinShape();
		float HI = 250.f;
		float LO = -HI;
		float x = LO + static_cast<float>(rand()) / static_cast<float>(RAND_MAX/(HI-LO));

		float angleOffset = (m_rotateToParentOnSpawn ? m_pTransform->GetAngleZ() : 0.f ) +
			RandomFloat(-m_angleVariation, m_angleVariation);

		m_particles[idx].m_angleOffset = angleOffset;


		switch (m_textureSelection) {
			case P_TextureSelection::RANDOM:
				m_particles[idx].m_texCoords.u = float(RandomInt(0, (int)(m_cols + 1))) * m_frameWidth;
				m_particles[idx].m_texCoords.v = float(RandomInt(0, (int)(m_rows + 1))) * m_frameHeight;
				break;
			case P_TextureSelection::CYCLE:
			case P_TextureSelection::SINGLE:
			default:
				m_particles[idx].m_texCoords.u = 0.f;
				m_particles[idx].m_texCoords.v = 0.f;
		}
	}
}

void ParticleEmitter::_UpdateParticles(float deltaTime)
{
	const Vector3D Gravity = Vector3D(0, -98.f * m_gravityMod, 0);
	m_liveParticleCount = 0;
	float t = 0.f;
	for (int i = 0; i<m_maxParticles; i++) {

		Particle& p = m_particles[i];

		if (p.m_life > 0.0f) {

			// Decrease life
			p.m_life -= deltaTime;
			if (p.m_life > 0.0f) {
				t = 1.f - (p.m_life / m_lifeTime);
				Vector3D velocityOffset = Matrix4x4::Rotate(p.m_angleOffset, ZAXIS) * 
					Vector3D(
						BezierInterpolation(m_velocityX.points, t) * m_velocityX.amplitude,
						BezierInterpolation(m_velocityY.points, t) * m_velocityY.amplitude,
						0.f,
						0.f
					);
				
				p.m_velocity = (velocityOffset * (m_speed * m_directionMod)) + Gravity;
				p.m_pos += p.m_velocity * deltaTime;
				if (m_particlesFollowParent) {
					p.m_pos += m_pTransform->GetMovement();
				}
				p.m_pos.z = 0.f;

				p.m_scale = BezierInterpolation(m_scale.points, t) * m_scale.amplitude;

				// TODO: decide on a better way to get a camera
				p.m_cameraDistance = Vector3D::SquareDistance(p.m_pos, TETRA_GAME_OBJECTS.GetPrimaryCamera()->GetComponent<Transform>(ComponentType::C_Transform)->GetPosition());

				Color color = Lerp(m_colorStart, m_colorEnd, t);
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

				m_textureCoords[2 * m_liveParticleCount + 0] = p.m_texCoords[0];
				m_textureCoords[2 * m_liveParticleCount + 1] = p.m_texCoords[1];
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
	m_textureCoords = (GLfloat*)TETRA_MEMORY.Alloc(sizeof(GLfloat) * m_maxParticles * 2);	//(GLfloat*)malloc(sizeof(GLfloat) * m_maxParticles * 2);

	for (int i = 0; i < m_maxParticles; ++i) {
		m_particles[i].m_life = -1.f;
		m_particles[i].m_angleOffset = 0.f;

		m_particles[i].m_texCoords.u = m_frameWidth / m_texture->surface->w;
		m_particles[i].m_texCoords.v = m_frameHeight / m_texture->surface->h;

		int i4 = i * 4;
		m_positionsScales[i4] = 0.f;
		m_positionsScales[i4 + 1] = 0.f;
		m_positionsScales[i4 + 2] = 0.f;
		m_positionsScales[i4 + 3] = 0.f;

		m_colors[i4] = 0;
		m_colors[i4 + 1] = 0;
		m_colors[i4 + 2] = 0;
		m_colors[i4 + 3] = 0;

		m_textureCoords[i * 2 + 0] = 0.f;
		m_textureCoords[i * 2 + 1] = 0.f;
	}
}

void ParticleEmitter::_DeallocateParticleArrays()
{
	if (m_particles)		TETRA_MEMORY.Free(m_particles);
	if (m_positionsScales)	TETRA_MEMORY.Free(m_positionsScales);
	if (m_colors)			TETRA_MEMORY.Free(m_colors);
	if (m_textureCoords)	TETRA_MEMORY.Free(m_textureCoords);
}

void ParticleEmitter::_AllocateVBOs()
{
	m_positionsScalesBuffer = TETRA_RENDERER.GenerateStreamingVBO(m_maxParticles * 4 * sizeof(GLfloat));
	m_colorsBuffer = TETRA_RENDERER.GenerateStreamingVBO(m_maxParticles * 4 * sizeof(GLubyte));
	m_textureCoordsBuffer = TETRA_RENDERER.GenerateStreamingVBO(m_maxParticles * 2 * sizeof(GLfloat));
}

void ParticleEmitter::_WarmSystem()
{
	const float step = 0.1f;
	for (float timer = 0.f; timer < m_loopDuration; timer += step) {
		m_emissionTimer += step;

		while (m_emissionTimer > m_emissionTime) {
			_SpawnParticle();
			m_emissionTimer -= m_emissionTime;
		}

		_UpdateParticles(step);
	}
}

#pragma endregion

ParticleEmitter::ParticleEmitter() :
	Component(ComponentType::C_ParticleEmitter),
	m_active(true),
	m_currentTime(0.f),
	m_emissionTime(0.f),
	m_emissionTimer(0.f),
	m_liveParticleCount(0),
	m_mesh(*TETRA_RESOURCES.LoadMesh(QUAD_MESH)),
	m_pSpawnShape(nullptr),
	m_directionMod(1.f),
	m_brightness(1.f)
{
	TETRA_EVENTS.Subscribe(EVENT_OnLevelInitialized, this);
}

ParticleEmitter::~ParticleEmitter() 
{
	_DeallocateParticleArrays();
	if (m_pSpawnShape)
		delete m_pSpawnShape;
}

#pragma region Component Methods

void ParticleEmitter::LateInitialize()
{
	if (!m_pTransform) {
		assert(pGO && "No Game Object found. ParticleEmitter component failed to operate.");
		m_pTransform = pGO->GetComponent<Transform>(ComponentType::C_Transform);
		assert(m_pTransform && "No Transform component found. ParticleEmitter component failed to operate.");
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

		if (m_active) {
			while (m_emissionTimer > m_emissionTime) {
				_SpawnParticle();
				m_emissionTimer -= m_emissionTime;
			}
		}

		if (m_looping && m_currentTime >= m_loopDuration)
			m_currentTime = 0.f;
	}
}

void ParticleEmitter::LateUpdate(float dt)
{
	_UpdateParticles(dt);
	//_SortParticles();	// Not used in our game on purpose
}

void ParticleEmitter::Deactivate()
{
	pGO = nullptr;
	m_pTransform = nullptr;

	_DeallocateParticleArrays();
}

void ParticleEmitter::Serialize(const json & j)
{
	m_active = ValueExists(j, "active") ? ParseBool(j, "active") : m_active;
	m_loopDuration = ParseFloat(j, "loopDuration");
	m_looping = ParseBool(j, "looping");
	m_prewarmed = ParseBool(j, "prewarmed");
	m_startDelay = ParseFloat(j, "startDelay");
	m_lifeTime = ParseFloat(j, "lifeTime");
	m_animationSpeed = ParseFloat(j, "animationSpeed");
	m_speed = ParseFloat(j, "speed");

	SerializeInterpolationItem(m_velocityX, j, "velocityX");
	SerializeInterpolationItem(m_velocityY, j, "velocityY");
	SerializeInterpolationItem(m_scale, j, "scale");

	m_rotation = ParseFloat(j, "rotation");

	Vector3D colorStart = ParseColor(j, "color", "start");
	m_colorStart.r = int(colorStart.x * 255.f);
	m_colorStart.g = int(colorStart.y * 255.f);
	m_colorStart.b = int(colorStart.z * 255.f);
	m_colorStart.a = int(colorStart.w * 255.f);
	Vector3D colorEnd = ParseColor(j, "color", "end");
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
	m_rows = ParseFloat(j, "rows");
	m_cols = ParseFloat(j, "cols");
	m_frameHeight = ParseFloat(j, "frameHeight");
	m_frameWidth = ParseFloat(j, "frameWidth");
	if (m_rows != 0.f && m_frameHeight == 0.f)	m_frameHeight = (m_texture->surface->h / m_rows) / m_texture->surface->h;
	else										m_frameHeight = 1.f;
	if (m_cols != 0.f && m_frameWidth == 0.f)	m_frameWidth = (m_texture->surface->w / m_cols) / m_texture->surface->w;
	else										m_frameWidth = 1.f;

	std::string textureSelection = ParseString(j, "textureSelection");
	if (textureSelection == "sequence")			m_textureSelection = P_TextureSelection::CYCLE;
	else if (textureSelection == "random")		m_textureSelection = P_TextureSelection::RANDOM;
	else										m_textureSelection = P_TextureSelection::SINGLE;


	if (ValueExists(j, SHAPE_STR)) {
		std::string shape = ParseString(j[SHAPE_STR], "type");

		if (!m_pSpawnShape) {
			if (shape == "AABB")
				m_pSpawnShape = new AABB(ParseFloat(j[SHAPE_STR], "width"), ParseFloat(j[SHAPE_STR], "height"));
			else if (shape == "circle")
				m_pSpawnShape = new Circle(ParseFloat(j[SHAPE_STR], "radius"));
			// Particle Emitter does not support polygonal spawn shapes currently
		}
	}

	m_angleVariation = ParseFloat(j, "angleVariation");

	m_emissionTime = m_loopDuration / float(m_emissionRate);

	m_shader = ValueExists(j, "shader") ? j["shader"] : "particle";
	m_active = ValueExists(j, "isActive") ? j["isActive"] : true;


	m_particlesFollowParent = ParseBool(j, "followParentPosition");
	m_rotateToParentOnSpawn = ParseBool(j, "matchParentRotation");

	m_renderedOnTop = ParseBool(j, "renderOnTop");

	m_brightness = ValueExists(j, "brightness") ? j["brightness"] : m_brightness;

	_AllocateParticleArrays();
	_AllocateVBOs();
	m_liveParticleCount = 0;
}

void ParticleEmitter::Override(const json & j)
{
}

void ParticleEmitter::HandleEvent(Event * p_event)
{
	switch (p_event->Type()) {
		case EventType::EVENT_FlipScaleX: {
			m_directionMod *= -1;
			break;
		}
		case EventType::EVENT_OnLevelInitialized: {
			if (m_prewarmed)
				_WarmSystem();
			break;
		}
	}
}

void ParticleEmitter::BindBufferDatas() const
{
	TETRA_RENDERER.BindBufferData(m_positionsScalesBuffer, m_positionsScales, m_liveParticleCount * sizeof(GLfloat) * 4);
	TETRA_RENDERER.BindBufferData(m_colorsBuffer, m_colors, m_liveParticleCount * sizeof(GLubyte) * 4);
	TETRA_RENDERER.BindBufferData(m_textureCoordsBuffer, m_textureCoords, m_liveParticleCount * sizeof(GLfloat) * 2);
}

#undef SHAPE_STR

#pragma endregion
