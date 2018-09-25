/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Moodie Ghaddar>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

ComponentFactory::ComponentFactory() {
	m_creationFunctions["Transform"] = Transform::CreateInstance;
	m_creationFunctions["Mesh"] = MeshComponent::CreateInstance;
	m_creationFunctions["HeightMap"] = HeightMap::CreateInstance;
	m_creationFunctions["FBOSprite"] = FBOSprite::CreateInstance;
	m_creationFunctions["Sprite"] = Sprite::CreateInstance;
	m_creationFunctions["ParticleEmitterResizer"] = ParticleEmitterResizer::CreateInstance;
	m_creationFunctions["ParticleEmitter"] = ParticleEmitter::CreateInstance;
	m_creationFunctions["Text"] = Text::CreateInstance;
	m_creationFunctions["Animation"] = Animation::CreateInstance;
	m_creationFunctions["LetterBox"] = LetterBox::CreateInstance;
	m_creationFunctions["Controller"] = Controller::CreateInstance;
	m_creationFunctions["Body"] = Body::CreateInstance;
	m_creationFunctions["Camera"] = Camera::CreateInstance;
	m_creationFunctions["CamFollow"] = CamFollow::CreateInstance;
	m_creationFunctions["ScreenWrap"] = ScreenWrap::CreateInstance;
	m_creationFunctions["SizeToScreen"] = SizeToScreen::CreateInstance;
	m_creationFunctions["FollowCamera"] = FollowCamera::CreateInstance;
	m_creationFunctions["SnapToCamera"] = SnapToCamera::CreateInstance;
	m_creationFunctions["Health"] = Health::CreateInstance;
	m_creationFunctions["FlashOnTakeDamage"] = FlashOnTakeDamage::CreateInstance;
	m_creationFunctions["Projectile"] = Projectile::CreateInstance;
	m_creationFunctions["Weapon"] = Weapon::CreateInstance;
	m_creationFunctions["Audio"] = Audio::CreateInstance;
	m_creationFunctions["NPCController"] = NPCController::CreateInstance;
	m_creationFunctions["DestroyOnHealthZero"] = DestroyOnHealthZero::CreateInstance;
	m_creationFunctions["DealDamageOnCollision"] = DealDamageOnCollision::CreateInstance;
	m_creationFunctions["ProjectileSpawner"] = ProjectileSpawner::CreateInstance;
	m_creationFunctions["PointLight"] = PointLight::CreateInstance;
	m_creationFunctions["ScaleByHPStamina"] = ScaleByHPStamina::CreateInstance;
	m_creationFunctions["Stamina"] = Stamina::CreateInstance;
	m_creationFunctions["Collectible"] = Collectible::CreateInstance;
	m_creationFunctions["WaveMovement"] = WaveMovement::CreateInstance;
	m_creationFunctions["StraightLineMovement"] = StraightLineMovement::CreateInstance;
	m_creationFunctions["SpawnOnHealthZero"] = SpawnOnHealthZero::CreateInstance;
	m_creationFunctions["FollowCursor"] = FollowCursor::CreateInstance;
	m_creationFunctions["Button"] = Button::CreateInstance;
	m_creationFunctions["LevelStatus"] = LevelStatus::CreateInstance;
	m_creationFunctions["EggCounter"] = EggCounter::CreateInstance;
	m_creationFunctions["Microphone"] = Microphone::CreateInstance;
	m_creationFunctions["UICounter"] = UICounter::CreateInstance;
	m_creationFunctions["ScriptedAnimation"] = ScriptedAnimation::CreateInstance;
	m_creationFunctions["WinMessage"] = WinMessage::CreateInstance;
	m_creationFunctions["Canvas"] = Canvas::CreateInstance;
	m_creationFunctions["Slider"] = Slider::CreateInstance;
	m_creationFunctions["Endpoints"] = Endpoints::CreateInstance;
	m_creationFunctions["PowerUpButton"] = PowerUpButton::CreateInstance;
	m_creationFunctions["Slider"] = Slider::CreateInstance;
	m_creationFunctions["Endpoints"] = Endpoints::CreateInstance;
	m_creationFunctions["Shop"] = Shop::CreateInstance;
	m_creationFunctions["NotEnoughGoldFeathers"] = NotEnoughGoldFeathers::CreateInstance;
	m_creationFunctions["Gate"] = Gate::CreateInstance;
	m_creationFunctions["GateTriggerBox"] = GateTriggerBox::CreateInstance;
	m_creationFunctions["CutScene"] = CutScene::CreateInstance;
	m_creationFunctions["HillNodes"] = HillNodes::CreateInstance;
	m_creationFunctions["Spawner"] = Spawner::CreateInstance;
	m_creationFunctions["TimeToLive"] = TimeToLive::CreateInstance;
	m_creationFunctions["ApplyTintOnDifficultyUp"] = ApplyTintOnDifficultyUp::CreateInstance;
	m_creationFunctions["SplashScreen"] = SplashScreen::CreateInstance;
	m_creationFunctions["SelectableButton"] = SelectableButton::CreateInstance;
	m_creationFunctions["ListSelector"] = ListSelector::CreateInstance;
	m_creationFunctions["InGameCredits"] = InGameCredits::CreateInstance;
	m_creationFunctions["Spinner"] = Spinner::CreateInstance;
}

Component* ComponentFactory::CreateComponent(std::string component) {
	// check if there's empty component stored in cache
	Component* cachedComp = TETRA_MEMORY.GetNewComponent(component);
	if (cachedComp) {
		return cachedComp;
	}
	// no empty component available, return create new comp
	CreationFunction* Create = m_creationFunctions[component];
	if (Create)
		return Create();

	return nullptr;
}