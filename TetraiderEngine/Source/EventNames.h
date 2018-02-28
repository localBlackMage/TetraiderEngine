/*
All event types are listed here. These are event names that will be 
turned into entries within the EventType enum
*/
REGISTER_EVENT_NAME(INVALID_EVENT) // Special case - this should always be first
// Input events start
// DO NOT CHANGE ORDER OF INPUT EVENTS
REGISTER_EVENT_NAME(EVENT_INPUT_MOVE)
REGISTER_EVENT_NAME(EVENT_INPUT_AIM)
REGISTER_EVENT_NAME(EVENT_INPUT_FLY)
REGISTER_EVENT_NAME(EVENT_INPUT_MELEE)
REGISTER_EVENT_NAME(EVENT_INPUT_RANGE)
REGISTER_EVENT_NAME(EVENT_INPUT_RESTART)
REGISTER_EVENT_NAME(EVENT_INPUT_PAUSEMUSIC)
REGISTER_EVENT_NAME(EVENT_INPUT_TOGGLEJOYSTICK)
REGISTER_EVENT_NAME(EVENT_INPUT_PAUSEGAME)

//Input events complete

REGISTER_EVENT_NAME(WINDOW_CLOSED)
REGISTER_EVENT_NAME(RESTART_LEVEL)
REGISTER_EVENT_NAME(EVENT_FPS_UPDATE)
REGISTER_EVENT_NAME(KEY_PRESSED)
REGISTER_EVENT_NAME(EVENT_OnCollide)
REGISTER_EVENT_NAME(EVENT_StaticsLoaded)
REGISTER_EVENT_NAME(EVENT_OnLevelInitialized)
REGISTER_EVENT_NAME(EVENT_OnEnemyHealthZero)
REGISTER_EVENT_NAME(EVENT_OnPlayerHealthZero)
REGISTER_EVENT_NAME(EVENT_OnHealthZero)
REGISTER_EVENT_NAME(EVENT_OnTakeDamage)
REGISTER_EVENT_NAME(EVENT_OnPlayerHeal)
REGISTER_EVENT_NAME(EVENT_FlipScaleX)
REGISTER_EVENT_NAME(EVENT_FlipScaleY)
REGISTER_EVENT_NAME(EVENT_StaminaUse)
REGISTER_EVENT_NAME(EVENT_StaminaRecharge)
REGISTER_EVENT_NAME(EVENT_EggCollected)
REGISTER_EVENT_NAME(EVENT_HealthCollected)
REGISTER_EVENT_NAME(EVENT_EnemySpawned)
REGISTER_EVENT_NAME(EVENT_LevelComplete)
REGISTER_EVENT_NAME(EVENT_LevelInComplete)
REGISTER_EVENT_NAME(EVENT_EggCounterInitialization)
REGISTER_EVENT_NAME(EVENT_ForceKnockBack)