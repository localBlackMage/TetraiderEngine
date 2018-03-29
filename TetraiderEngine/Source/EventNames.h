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

REGISTER_EVENT_NAME(EVENT_LIGHT_A_DOWN)
REGISTER_EVENT_NAME(EVENT_LIGHT_A_UP)
REGISTER_EVENT_NAME(EVENT_LIGHT_B_DOWN)
REGISTER_EVENT_NAME(EVENT_LIGHT_B_UP)

REGISTER_EVENT_NAME(EVENT_TOGGLE_LIGHTS)
REGISTER_EVENT_NAME(EVENT_TOGGLE_CURSOR)
REGISTER_EVENT_NAME(EVENT_TOGGLE_POST_PROCESSING)

REGISTER_EVENT_NAME(EVENT_INPUT_EXITLEVEL)
REGISTER_EVENT_NAME(EVENT_INPUT_OPENSHOP)
REGISTER_EVENT_NAME(EVENT_INPUT_GOLDENFEATHERCHEAT)

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
REGISTER_EVENT_NAME(EVENT_AmmoUpdate)
REGISTER_EVENT_NAME(EVENT_UIAmmoUpdate)
REGISTER_EVENT_NAME(EVENT_Explode)
REGISTER_EVENT_NAME(EVENT_DisableCamFollow)
REGISTER_EVENT_NAME(EVENT_EnableCamFollow)
REGISTER_EVENT_NAME(EVENT_ChangeBGMVol)
REGISTER_EVENT_NAME(EVENT_ChangeSFXVol)
REGISTER_EVENT_NAME(EVENT_ChangeMasterVol)
REGISTER_EVENT_NAME(EVENT_ExitLevel)
REGISTER_EVENT_NAME(EVENT_GoldenFeatherCollected)
REGISTER_EVENT_NAME(EVENT_UIGoldenFeatherUpdate)
REGISTER_EVENT_NAME(EVENT_OnPauseGame)
REGISTER_EVENT_NAME(EVENT_OnGameResume)
REGISTER_EVENT_NAME(EVENT_ShopOpened)
REGISTER_EVENT_NAME(EVENT_ShopClosed)
REGISTER_EVENT_NAME(EVENT_PowerUpPurchased)
REGISTER_EVENT_NAME(EVENT_NotEnoughGoldenFeathers)
REGISTER_EVENT_NAME(EVENT_EnoughGoldenFeathers)
REGISTER_EVENT_NAME(EVENT_CloseGate)
REGISTER_EVENT_NAME(EVENT_OpenGate)
REGISTER_EVENT_NAME(EVENT_OnEnterBoss)
REGISTER_EVENT_NAME(EVENT_OnExitBoss)
REGISTER_EVENT_NAME(EVENT_OnBossDefeated)
REGISTER_EVENT_NAME(EVENT_OnCanvasActivated)
REGISTER_EVENT_NAME(EVENT_OnCanvasDeactivated)
REGISTER_EVENT_NAME(EVENT_OnLoadNextLevel)