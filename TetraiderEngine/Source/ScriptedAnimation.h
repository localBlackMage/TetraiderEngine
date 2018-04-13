#pragma once
#ifndef SCRIPTED_ANIMATION_H
#define SCRIPTED_ANIMATION_H

struct ScriptedAnimationData {
	ScriptedAnimationData(const Vector3D& finalPos, float translationSpeed, float finalZRotation, float rotationSpeed, const Vector3D& finalScale, float scaleSpeed, float finalFade, float fadeSpeed, bool isTranslate, bool isScale, bool isRotate, bool isFade, bool isEaseIn, bool isEaseOut) :
		m_finalPos(finalPos),
		m_translationSpeed(translationSpeed),
		m_finalZRotation(finalZRotation),
		m_rotationSpeed(rotationSpeed),
		m_finalScale(finalScale),
		m_scaleSpeed(scaleSpeed),
		m_finalFade(finalFade),
		m_fadeSpeed(fadeSpeed),
		m_isTranslate(isTranslate),
		m_isScale(isScale),
		m_isRotate(isRotate),
		m_isFade(isFade),
		m_isEaseIn(isEaseIn),
		m_isEaseOut(isEaseOut)
	{}
	Vector3D m_finalPos;
	float m_translationSpeed;
	float m_finalZRotation;
	float m_rotationSpeed;
	float m_finalFade;
	float m_fadeSpeed;
	Vector3D m_finalScale;
	float m_scaleSpeed;
	bool m_isTranslate;
	bool m_isScale;
	bool m_isRotate;
	bool m_isFade;
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
	void PlayAnimation(bool isReverse = false);
	void SetInitialPos(const Vector3D& pos);
	void SetInitialScale(const Vector3D& scale);
	void SetInitialFade(float fade);
	void SetInitialRot(float rot);
	void StopPlaying();
	bool IsPlaying() { return m_isPlayAnimation; }
private:
	std::vector<ScriptedAnimationData> m_scriptedAnimation;
	float m_translationT;
	float m_scaleT;
	float m_rotationT;
	float m_fadeT;
	Vector3D m_initialPos;
	Vector3D m_initialScale;
	float m_initialRotation;
	float m_initialFade;

	bool m_isPlayAnimation;
	bool m_isPlayOnAwake;

	Vector3D m_startPos;
	Vector3D m_startScale;
	float m_startRot;
	float m_startFade;

	Transform* m_pTransform;
	Sprite* m_pSprite;
	int m_currentAnimationIndex;
	void NextAnim();
	bool m_isReverse;
	bool m_isRelative;

	bool m_isPlayOnPause;
};

#endif

