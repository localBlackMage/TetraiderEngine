#pragma once

#ifndef RENDERABLE_COMPONENT_H
#define RENDERABLE_COMPONENT_H

class RenderableComponent : public Component
{
protected:
	GLint m_shaderProgramID;

public:
	RenderableComponent(ComponentType _type) : Component(_type) {};
	~RenderableComponent() {};

	inline GLint GetShaderProgramID() const { return m_shaderProgramID; }
};

#endif