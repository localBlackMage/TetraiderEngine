#pragma once

#ifndef PARTICLE_H
#define PARTICLE_H

#include "Component.h"
#include "SurfaceTextureBuffer.h"
#include "Mesh.h"
#include "Math\MathLibs.h"

class Particle : public Component {
protected:
	std::string m_textureName;
	SurfaceTextureBuffer * m_texture;
	Mesh& m_mesh;
public:
	Particle();
	~Particle();

	static Component* CreateInstance() { return new Particle(); }

	virtual void Update(float dt);
	virtual void Serialize(const json& j);
	virtual void LateInitialize();
	virtual void LateUpdate(float dt);
	virtual void HandleEvent(Event* pEvent);
};

#endif

