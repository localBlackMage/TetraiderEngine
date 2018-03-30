#pragma once

#ifndef TRANSFORM_H
#define TRANSFORM_H

//Forward declaration
class Body;

class Transform : public Component {
private:
	Vector3D m_position;
	Vector3D m_prevPosition;
	Vector3D m_worldPosition;
	Vector3D m_scale;
	Vector3D m_pivotOffset;
	Matrix4x4 m_transform;
	Vector3D m_lookAt;
	Transform* m_parent;
	float m_angleX, m_angleY, m_angleZ;
	bool m_is2d;

	void _UpdateLookAt();
	void _UpdateBodyComponent();
public:
	Transform();
	~Transform();

	static Component* CreateInstance() { return new Transform();  }
	virtual void Deactivate();
	virtual void Update(float dt);
	virtual void LateUpdate(float dt);
	virtual void Serialize(const json& j);
	virtual void Override(const json& j);

	virtual void HandleEvent(Event * p_event);

	bool operator<(const Transform& other) const;
	bool IsParented() { return m_parent; }

	void SetPosition(const Vector3D& pos);
	Vector3D GetPosition() const;
	Vector3D GetLocalPosition() const;
	void Move(const Vector3D& amount);
	Vector3D GetMovement() const { return m_worldPosition - m_prevPosition; }

	void SetAngles(float angleX, float angleY, float angleZ);
	float GetAngleX() const;
	void SetAngleX(float angle);
	float GetAngleY() const;
	void SetAngleY(float angle);
	float GetAngleZ() const;
	void SetAngleZ(float angle);
	float GetParentScaleX();
	float GetParentScaleY();

	void SetPivotOffset(float x, float y) {
		m_pivotOffset.x = x;
		m_pivotOffset.y = y;
	}

	float GetPivotOffsetX() { return m_pivotOffset.x; }
	float GetPivotOffsetY() { return m_pivotOffset.y; }

	void RotateX(float amount);
	void RotateY(float amount);
	void RotateZ(float amount);
	Vector3D GetRotVector() const;

	Vector3D Forward() const;
	Vector3D Right() const;
	Vector3D Up() const;

	Vector3D LookAt() const;

	float GetScaleX() const;
	void SetScaleX(float scaleX);
	void ScaleXby(float amount);

	float GetScaleY() const;
	void SetScaleY(float scaleY);
	void ScaleYby(float amount);

	float GetScaleZ() const;
	void SetScaleZ(float scaleZ);
	void ScaleZby(float amount);
	void SetParent(Transform* pParent) { m_parent = pParent; }

	void SetScaleUniform(float amount);
	void SetScale(float scaleX, float scaleY);
	void SetScale(float scaleX, float scaleY, float scaleZ);
	void SetScale(const Vector3D& scale);
	void ScaleUniform(float amount);
	Vector3D GetScaleVector() const;

	Matrix4x4 GetTransform() const;
	Matrix4x4 GetTransformAfterOffset(const Vector3D& offset) const;
	Matrix4x4 TransformWithOffsetAndScale(const Vector3D& offset, const float& scaleX, const float& scaleY) const;
	friend Body;
};

#endif

