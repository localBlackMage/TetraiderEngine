#include <Stdafx.h>

ScriptedAnimation::ScriptedAnimation(): Component(ComponentType::C_ScriptedAnimation), m_isPlayAnimation(false), m_currentAnimationIndex(0), m_isReverse(false), m_isRelative(false) {}
ScriptedAnimation::~ScriptedAnimation() {}

void ScriptedAnimation::DeActivate() {
	pGO = nullptr;
}

void ScriptedAnimation::Update(float dt) {
	if (TETRA_GAME_STATE.IsGamePaused()) return;

	int index = m_currentAnimationIndex;
	if (m_isReverse)
		index -= 1;

	if (m_isPlayAnimation) {
		if (m_scriptedAnimation[m_currentAnimationIndex].m_isTranslate) {
			m_translationT += dt*m_scriptedAnimation[m_currentAnimationIndex].m_translationSpeed;
			if (m_translationT > 1) m_translationT = 1;

			if(index >= 0 && !m_isRelative)
				m_pTransform->SetPosition(Lerp(m_startPos, m_scriptedAnimation[index].m_finalPos, m_translationT, m_scriptedAnimation[m_currentAnimationIndex].m_isEaseIn, m_scriptedAnimation[m_currentAnimationIndex].m_isEaseOut));
			else if(index >=0 && m_isRelative)
				m_pTransform->SetPosition(Lerp(m_startPos, m_startPos + m_scriptedAnimation[index].m_finalPos, m_translationT, m_scriptedAnimation[m_currentAnimationIndex].m_isEaseIn, m_scriptedAnimation[m_currentAnimationIndex].m_isEaseOut));
			else
				m_pTransform->SetPosition(Lerp(m_startPos, m_initialPos, m_translationT, m_scriptedAnimation[m_currentAnimationIndex].m_isEaseIn, m_scriptedAnimation[m_currentAnimationIndex].m_isEaseOut));
		}
		else
			m_translationT = 1;

		if (m_scriptedAnimation[m_currentAnimationIndex].m_isRotate) {
			m_rotationT += dt*m_scriptedAnimation[m_currentAnimationIndex].m_rotationSpeed;
			if (m_rotationT > 1) m_rotationT = 1;

			if (index >= 0 && !m_isRelative)
				m_pTransform->SetAngleZ(Lerp(m_startRot, m_scriptedAnimation[index].m_finalZRotation, m_rotationT, m_scriptedAnimation[m_currentAnimationIndex].m_isEaseIn, m_scriptedAnimation[m_currentAnimationIndex].m_isEaseOut));
			else if (index >= 0 && m_isRelative)
				m_pTransform->SetAngleZ(Lerp(m_startRot, m_startRot + m_scriptedAnimation[index].m_finalZRotation, m_rotationT, m_scriptedAnimation[m_currentAnimationIndex].m_isEaseIn, m_scriptedAnimation[m_currentAnimationIndex].m_isEaseOut));
			else
				m_pTransform->SetAngleZ(Lerp(m_startRot, m_initialRotation, m_rotationT, m_scriptedAnimation[m_currentAnimationIndex].m_isEaseIn, m_scriptedAnimation[m_currentAnimationIndex].m_isEaseOut));
		}
		else
			m_rotationT = 1;

		if (m_scriptedAnimation[m_currentAnimationIndex].m_isScale) {
			m_scaleT += dt*m_scriptedAnimation[m_currentAnimationIndex].m_scaleSpeed;
			if (m_scaleT > 1) m_scaleT = 1;

			if (index >= 0 && !m_isRelative)
				m_pTransform->SetScale(Lerp(m_startScale, m_scriptedAnimation[index].m_finalScale, m_scaleT, m_scriptedAnimation[m_currentAnimationIndex].m_isEaseIn, m_scriptedAnimation[m_currentAnimationIndex].m_isEaseOut));
			else if (index >= 0 && m_isRelative)
				m_pTransform->SetScale(Lerp(m_startScale, m_startScale + m_scriptedAnimation[index].m_finalScale, m_scaleT, m_scriptedAnimation[m_currentAnimationIndex].m_isEaseIn, m_scriptedAnimation[m_currentAnimationIndex].m_isEaseOut));
			else
				m_pTransform->SetScale(Lerp(m_startScale, m_initialScale, m_scaleT, m_scriptedAnimation[m_currentAnimationIndex].m_isEaseIn, m_scriptedAnimation[m_currentAnimationIndex].m_isEaseOut));
		}
		else
			m_scaleT = 1;

		if (m_scriptedAnimation[m_currentAnimationIndex].m_isFade && m_pSprite) {
			m_fadeT += dt*m_scriptedAnimation[m_currentAnimationIndex].m_fadeSpeed;
			if (m_fadeT > 1) m_fadeT = 1;

			if (index >= 0 && !m_isRelative)
				m_pSprite->SetAlpha(Lerp(m_startFade, m_scriptedAnimation[index].m_finalFade, m_fadeT, m_scriptedAnimation[m_currentAnimationIndex].m_isEaseIn, m_scriptedAnimation[m_currentAnimationIndex].m_isEaseOut));
			else if (index >= 0 && m_isRelative)
				m_pSprite->SetAlpha(Lerp(m_startFade, m_startFade + m_scriptedAnimation[index].m_finalFade, m_fadeT, m_scriptedAnimation[m_currentAnimationIndex].m_isEaseIn, m_scriptedAnimation[m_currentAnimationIndex].m_isEaseOut));
			else
				m_pSprite->SetAlpha(Lerp(m_startFade, m_initialFade, m_fadeT, m_scriptedAnimation[m_currentAnimationIndex].m_isEaseIn, m_scriptedAnimation[m_currentAnimationIndex].m_isEaseOut));
		}
		else
			m_fadeT = 1;

		if (m_translationT >= 1 && m_scaleT >= 1 && m_rotationT >= 1 && m_fadeT >= 1) {
			NextAnim();
		}
	}
}

void ScriptedAnimation::Serialize(const json& j) {
	int size = j["ScriptedAnimation"].size();
	for (int i = 0; i < size; ++i) {
		m_scriptedAnimation.push_back(ScriptedAnimationData
		(
			ParseVector3D(j["ScriptedAnimation"][i], "finalPos"),
			ParseFloat(j["ScriptedAnimation"][i], "translationSpeed"),
			ParseFloat(j["ScriptedAnimation"][i], "finalZRotation"),
			ParseFloat(j["ScriptedAnimation"][i], "rotationSpeed"),
			ParseVector3D(j["ScriptedAnimation"][i], "finalScale"),
			ParseFloat(j["ScriptedAnimation"][i], "scaleSpeed"),
			ParseFloat(j["ScriptedAnimation"][i], "finalFade"),
			ParseFloat(j["ScriptedAnimation"][i], "fadeSpeed"),
			ParseBool(j["ScriptedAnimation"][i], "isTranslate"),
			ParseBool(j["ScriptedAnimation"][i], "isScale"),
			ParseBool(j["ScriptedAnimation"][i], "isRotate"),
			ParseBool(j["ScriptedAnimation"][i], "isFade"),
			ParseBool(j["ScriptedAnimation"][i], "isEaseIn"),
			ParseBool(j["ScriptedAnimation"][i], "isEaseOut")
		));
	}

	m_initialPos = ParseVector3D(j, "initialPos");
	m_initialScale = ParseVector3D(j, "initialScale");
	m_initialRotation = ParseFloat(j, "initialRotation");
	m_initialFade = ParseFloat(j, "initialFade");
	m_isPlayOnAwake = ParseBool(j, "isPlayOnAwake");
	m_isRelative = ParseBool(j, "isRelative");
}

void ScriptedAnimation::LateInitialize() {
	if (!m_pTransform) {
		if (pGO)
			m_pTransform = pGO->GetComponent<Transform>(ComponentType::C_Transform);
		else {
			printf("No Game Object found. Scripted Animation component failed to operate.\n");
			return;
		}

		if (!m_pTransform) {
			printf("No Transform component found. Scripted Animation component failed to operate.\n");
			assert(m_pTransform);
			return;
		}
	}

	if (!m_pSprite) {
		if (pGO)
			m_pSprite = pGO->GetComponent<Sprite>(ComponentType::C_Sprite);
		else {
			printf("No Game Object found. Scripted Animation component failed to operate.\n");
			return;
		}

		if (!m_pTransform) {
			printf("No Sprite component found. Scripted Animation component failed to operate.\n");
			assert(m_pTransform);
			return;
		}
	}

	if (m_isPlayOnAwake)
		PlayAnimation();
}

void ScriptedAnimation::PlayAnimation(bool isReverse) {
	m_isReverse = isReverse;

	if (isReverse && m_isRelative) {
		std::cout << "Cannot play animation in reverse and be relative" << std::endl;
		return;
	}

	if (!m_isPlayAnimation) {
		m_isPlayAnimation = true;
		m_rotationT = 0;
		m_scaleT = 0;
		m_translationT = 0;
		m_fadeT = 0;
		if (!isReverse) {
			if (!m_isRelative) {
				m_startPos = m_initialPos;
				m_startScale = m_initialScale;
				m_startRot = m_initialRotation;
				m_startFade = m_initialFade;
			}
			else {
				m_startPos = m_pTransform->GetPosition();
				m_startScale = m_pTransform->GetScaleVector();
				m_startRot = m_pTransform->GetAngleZ();
				if (m_pSprite)
					m_startFade = m_pSprite->GetAlpha();
			}

			m_currentAnimationIndex = 0;
		}
		else {
			m_startPos = m_scriptedAnimation[m_scriptedAnimation.size() - 1].m_finalPos;
			m_startScale = m_scriptedAnimation[m_scriptedAnimation.size() - 1].m_finalScale;
			m_startRot = m_scriptedAnimation[m_scriptedAnimation.size() - 1].m_finalZRotation;
			m_startFade = m_scriptedAnimation[m_scriptedAnimation.size() - 1].m_finalFade;
			m_currentAnimationIndex = m_scriptedAnimation.size() - 1;
		}
	}
}

void ScriptedAnimation::HandleEvent(Event* pEvent) {
	
}

void ScriptedAnimation::NextAnim() {
	if (m_isReverse) {
		--m_currentAnimationIndex;
		if (m_currentAnimationIndex < 0) {
			m_isPlayAnimation = false;
			m_isReverse = false;
			m_currentAnimationIndex = 0;
		}
		else {
			m_startPos = m_pTransform->GetPosition();
			m_startScale = m_pTransform->GetScaleVector();
			m_startRot = m_pTransform->GetAngleZ();
			if (m_pSprite)
				m_startFade = m_pSprite->GetAlpha();

			m_rotationT = 0;
			m_scaleT = 0;
			m_translationT = 0;
			m_fadeT = 0;
		}
	}
	else {
		++m_currentAnimationIndex;
		if (m_currentAnimationIndex >= (int)m_scriptedAnimation.size()) {
			m_isReverse = false;
			m_isPlayAnimation = false;
			m_currentAnimationIndex = 0;
		}
		else {
			m_startPos = m_pTransform->GetPosition();
			m_startScale = m_pTransform->GetScaleVector();
			m_startRot = m_pTransform->GetAngleZ();
			if (m_pSprite)
				m_startFade = m_pSprite->GetAlpha();
			m_rotationT = 0;
			m_scaleT = 0;
			m_translationT = 0;
			m_fadeT = 0;
		}
	}
}

void ScriptedAnimation::SetInitialPos(const Vector3D& pos) {
	m_initialPos = pos;
}

void ScriptedAnimation::SetInitialScale(const Vector3D& scale) {
	m_initialScale = scale;
}

void ScriptedAnimation::SetInitialFade(float fade) {
	m_initialFade = fade;
}

void ScriptedAnimation::SetInitialRot(float rot) {
	m_initialRotation = rot;
}