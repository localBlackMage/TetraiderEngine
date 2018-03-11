#pragma once
#ifndef SCRIPTED_ANIMATION_H
#define SCRIPTED_ANIMATION_H

struct ScriptedAnimationData {
	ScriptedAnimationData(const Vector3D& finalPos, float translationSpeed, float finalZRotation, float rotationSpeed, const Vector3D& finalScale, float scaleSpeed, bool isTranslate, bool isScale, bool isRotate, bool isEaseIn, bool isEaseOut) :
		m_finalPos(finalPos),
		m_translationSpeed(translationSpeed),
		m_finalZRotation(finalZRotation),
		m_rotationSpeed(rotationSpeed),
		m_finalScale(finalScale),
		m_scaleSpeed(scaleSpeed),
		m_isTranslate(isTranslate),
		m_isScale(isScale),
		m_isRotate(isRotate),
		m_isEaseIn(isEaseIn),
		m_isEaseOut(isEaseOut)
	{}
	Vector3D m_finalPos;
	float m_translationSpeed;
	float m_finalZRotation;
	float m_rotationSpeed;
	Vector3D m_finalScale;
	float m_scaleSpeed;
	bool m_isTranslate;
	bool m_isScale;
	bool m_isRotate;
	bool m_isEaseIn;
	bool m_isEaseOut;
};

class ScriptedAnimation : public Component {
public:
	ScriptedAnimation();
	~ScriptedAnimation();
	static Component* CreateInstance() { return new ScriptedAnimation(); }
	virtual void DeActivate();
	virtual void Update(float dt);
	virtual void Serialize(const json& j);
	virtual void LateInitialize();
	virtual void LateUpdate(float dt) {}
	virtual void HandleEvent(Event* pEvent);
	void PlayAnimation();
	bool IsPlaying() { return m_isPlayAnimation; }
private:
	std::vector<ScriptedAnimationData> m_scriptedAnimation;
	Vector3D m_initialPos;
	Vector3D m_initialScale;
	float m_initialRotation;
	bool m_isPlayAnimation;
	Transform* m_pTransform;
	int m_currentAnimationIndex;
	float m_translationT;
	float m_scaleT;
	float m_rotationT;
	Vector3D m_startPos;
	Vector3D m_startScale;
	bool m_isPlayOnAwake;
	float m_startRot;
	void NextAnim();
};

#endif

