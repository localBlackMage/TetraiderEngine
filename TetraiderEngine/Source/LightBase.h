#pragma once
#ifndef LIGHT_BASE_H
#define LIGHT_BASE_H

class LightBase : public Component
{
protected:
	Transform * m_pTransform;
	Vector3D m_offset;
	Color m_color;
	float m_distance;	// How far before the light falls off entirely
	float m_a, m_b;		// Light fall off  (1 / (1 + a * d + b^2 * d) )

	bool m_layers[RENDER_LAYER::L_NUM_LAYERS];

public:
	LightBase(ComponentType cType);
	~LightBase() {};

	virtual void DeActivate();
	virtual void LateInitialize();
	virtual void Update(float dt) {};
	virtual void LateUpdate(float dt) {};
	virtual void Serialize(const json& j);
	virtual void Override(const json& j);
	virtual void HandleEvent(Event* pEvent) {};

	Vector3D GetPosition() const;
	GLubyte Red() const { return m_color[0]; }
	GLubyte Green() const { return m_color[1]; }
	GLubyte Blue() const { return m_color[2]; }
	GLubyte Alpha() const { return m_color[3]; }
	Color GetColor() const { return m_color; }
	void SetColor(const Vector3D& color);
	float Distance() const { return m_distance; }
	float LightA() const { return m_a; }
	float LightB() const { return m_b; }
	void SetFallOff(float a, float b) { m_a = a; m_b = b; }
	Vector3D GetOffset() { return m_offset; }
	void SetOffset(const Vector3D& offset) { m_offset = offset; }

	bool GetLayer(int layer) const { return m_layers[layer]; }
};

#endif

