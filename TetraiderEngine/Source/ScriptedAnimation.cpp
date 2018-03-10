#include <Stdafx.h>

ScriptedAnimation::ScriptedAnimation(): Component(ComponentType::C_ScriptedAnimation), m_isPlayAnimation(false), m_currentAnimationIndex(0) {}
ScriptedAnimation::~ScriptedAnimation() {}

void ScriptedAnimation::DeActivate() {
	pGO = nullptr;
}

void ScriptedAnimation::Update(float dt) {
	if (TETRA_GAME_STATE.IsGamePaused()) return;

	/*if (TETRA_INPUT.IsKeyTriggered(SDL_SCANCODE_Y)) {
		PlayAnimation();
	}*/

	if (m_isPlayAnimation) {
		if (m_scriptedAnimation[m_currentAnimationIndex].m_isTranslate) {
			m_translationT += dt*m_scriptedAnimation[m_currentAnimationIndex].m_translationSpeed;
			if (m_translationT > 1) m_translationT = 1;

			m_pTransform->SetPosition(Lerp(m_startPos, m_scriptedAnimation[m_currentAnimationIndex].m_finalPos, m_translationT, m_scriptedAnimation[m_currentAnimationIndex].m_isEaseIn, m_scriptedAnimation[m_currentAnimationIndex].m_isEaseOut));
		}
		else
			m_translationT = 1;

		if (m_scriptedAnimation[m_currentAnimationIndex].m_isRotate) {
			m_rotationT += dt*m_scriptedAnimation[m_currentAnimationIndex].m_rotationSpeed;
			if (m_rotationT > 1) m_rotationT = 1;

			m_pTransform->SetAngleZ(Lerp(m_startRot, m_scriptedAnimation[m_currentAnimationIndex].m_finalZRotation, m_rotationT, m_scriptedAnimation[m_currentAnimationIndex].m_isEaseIn, m_scriptedAnimation[m_currentAnimationIndex].m_isEaseOut));
		}
		else
			m_rotationT = 1;

		if (m_scriptedAnimation[m_currentAnimationIndex].m_isScale) {
			m_scaleT += dt*m_scriptedAnimation[m_currentAnimationIndex].m_scaleSpeed;
			if (m_scaleT > 1) m_scaleT = 1;

			m_pTransform->SetScale(Lerp(m_startScale, m_scriptedAnimation[m_currentAnimationIndex].m_finalScale, m_scaleT, m_scriptedAnimation[m_currentAnimationIndex].m_isEaseIn, m_scriptedAnimation[m_currentAnimationIndex].m_isEaseOut));
		}
		else
			m_scaleT = 1;

		if (m_translationT >= 1 && m_scaleT >= 1 && m_rotationT >= 1) {
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
			ParseBool(j["ScriptedAnimation"][i], "isTranslate"),
			ParseBool(j["ScriptedAnimation"][i], "isScale"),
			ParseBool(j["ScriptedAnimation"][i], "isRotate"),
			ParseBool(j["ScriptedAnimation"][i], "isEaseIn"),
			ParseBool(j["ScriptedAnimation"][i], "isEaseOut")
		));
	}

	m_initialPos = ParseVector3D(j, "initialPos");
	m_initialScale = ParseVector3D(j, "initialScale");
	m_initialRotation = ParseFloat(j, "initialRotation");
	m_isPlayOnAwake = ParseBool(j, "isPlayOnAwake");
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

	if (m_isPlayOnAwake)
		PlayAnimation();
}

void ScriptedAnimation::PlayAnimation() {
	if (!m_isPlayAnimation) {
		m_isPlayAnimation = true;
		m_startPos = m_initialPos;
		m_startScale = m_initialScale;
		m_startRot = m_initialRotation;
		m_currentAnimationIndex = 0;
		m_rotationT = 0;
		m_scaleT = 0;
		m_translationT = 0;
		m_pTransform->SetPosition(m_startPos);
		m_pTransform->SetScale(m_startScale);
		m_pTransform->SetAngleZ(m_startRot);
	}
}

void ScriptedAnimation::HandleEvent(Event* pEvent) {
	
}

void ScriptedAnimation::NextAnim() {
	++m_currentAnimationIndex;
	if (m_currentAnimationIndex >= (int)m_scriptedAnimation.size()) {
		m_isPlayAnimation = false;
		m_currentAnimationIndex = 0;
	}
	else {
		m_startPos = m_pTransform->GetPosition();
		m_startScale = m_pTransform->GetScaleVector();
		m_startRot = m_pTransform->GetAngleZ();
		m_rotationT = 0;
		m_scaleT = 0;
		m_translationT = 0;
	}
}