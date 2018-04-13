#pragma once

#ifndef PARTICLE_EMITTER_RESIZER_H
#define PARTICLE_EMITTER_RESIZER_H

class ParticleEmitterResizer : public Component {
protected:
	Transform* m_pTransform;
	ParticleEmitter* m_pParticleEmitter;

public:
	ParticleEmitterResizer();
	~ParticleEmitterResizer();

	static Component* CreateInstance() { return new ParticleEmitterResizer(); }

	virtual void Update(float dt) {};
	virtual void LateUpdate(float dt) {};
	virtual void Deactivate();
	virtual void Serialize(const json& j) {};
	virtual void Override(const json& j);
	virtual void LateInitialize();
	virtual void HandleEvent(Event* pEvent) {};
};

#endif

