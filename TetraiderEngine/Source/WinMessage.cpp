#include <Stdafx.h>


WinMessage::WinMessage():Component(ComponentType::C_WinMessage), 
m_isMessageOn(false),
m_isExitingLevel(false),
m_isRightTransitionSpawned(false), 
m_timeToSpawnRightTransition(0.75f), 
m_timeToLoadNextLevel(3.0f),
m_timer(0.0f),
m_levelToLoad(3)
{}

WinMessage::~WinMessage(){}


void WinMessage::Update(float dt)
{
	if (TETRA_GAME_STATE.IsGamePaused() && TETRA_GAME_STATE.IsShopOpen()) return;

	if (TETRA_INPUT.IsKeyTriggered(SDL_SCANCODE_F5) && !m_isMessageOn) {
		pGO->m_isRender = true;
		m_pScriptedAnim->PlayAnimation();
		m_isMessageOn = true;
	}

	// Check if player is alive
	if (m_isMessageOn) {
		if (!m_pWaveMovement->IsActive() && !m_pScriptedAnim->IsPlaying()) {
			m_pWaveMovement->SetActive(true);
			m_pWaveMovement->SetInitialPos(pGO->GetComponent<Transform>(C_Transform)->GetPosition());
		}
	}

	if (m_isExitingLevel) {
		m_timer += dt;

		if (m_timer > m_timeToSpawnRightTransition && !m_isRightTransitionSpawned) {
			TETRA_GAME_OBJECTS.CreateGameObject(m_rightTransitionPrefab);
			m_isRightTransitionSpawned = true;
		}
		
		if (m_timer > m_timeToLoadNextLevel) {
			TETRA_LEVELS.ActivateRandomGeneration(false);

			if(TETRA_LEVELS.IsFinalLevel())
				TETRA_LEVELS.ChangeLevel(m_outroLevel);
			else
				TETRA_LEVELS.ChangeLevel(m_levelToLoad);
		}
	}
}

void WinMessage::Serialize(const json & j)
{
	m_timeToSpawnRightTransition = ParseFloat(j, "timeToSpawnRightTransition");
	m_timeToLoadNextLevel = ParseFloat(j, "timeToLoadNextLevel");
	m_rightTransitionPrefab = ParseString(j, "rightTransitionPrefab");
	m_levelToLoad = ParseInt(j, "levelToLoad");
	m_outroLevel = ParseInt(j, "outroLevel");
}

void WinMessage::HandleEvent(Event * pEvent)
{
	if (pEvent->Type() == EVENT_LevelComplete)
	{
		pGO->m_isRender = true;
		m_pScriptedAnim->PlayAnimation();
		m_isMessageOn = true;
		Audio* pAudio = pGO->GetComponent<Audio>(C_Audio);
		if (pAudio)
			pAudio->Play();
	}
	else if (m_isMessageOn && !m_isExitingLevel && pEvent->Type() == EVENT_INPUT_EXITLEVEL && !TETRA_GAME_STATE.IsGamePaused()) {
		m_isExitingLevel = true;
		TETRA_EVENTS.BroadcastEventToSubscribers(&Event(EVENT_ExitLevel));
	}
}

void WinMessage::LateInitialize()
{
	if (!m_pWaveMovement) {
		if (pGO)
			m_pWaveMovement = pGO->GetComponent<WaveMovement>(ComponentType::C_WaveMovement);
		else {
			printf("No Game Object found. Win message component failed to operate.\n");
			return;
		}

		if (!m_pWaveMovement) {
			printf("No Wave movement component found.  Win message component failed to operate.\n");
			assert(m_pWaveMovement);
			return;
		}
		else {
			m_pWaveMovement->SetActive(false);
		}
	}

	if (!m_pScriptedAnim) {
		if (pGO)
			m_pScriptedAnim = pGO->GetComponent<ScriptedAnimation>(ComponentType::C_ScriptedAnimation);
		else {
			printf("No Game Object found. Win message component failed to operate.\n");
			return;
		}

		if (!m_pScriptedAnim) {
			printf("No Wave movement found.  Win message component failed to operate.\n");
			assert(m_pScriptedAnim);
			return;
		}
	}
	
	pGO->m_isRender = false;
	TETRA_EVENTS.Subscribe(EVENT_LevelComplete , this);
	TETRA_EVENTS.Subscribe(EVENT_INPUT_EXITLEVEL, this);
}

