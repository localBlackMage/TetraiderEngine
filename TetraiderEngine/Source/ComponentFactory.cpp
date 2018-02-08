#include "ComponentFactory.h"
#include "Component.h"
#include "Transform.h"
#include "Sprite.h"
#include "Animation.h"
#include "Controller.h"
#include "Body.h"
#include "Camera.h"
#include "CamFollow.h"
#include "Health.h"
#include "FlashOnTakeDamage.h"
#include "Projectile.h"
#include "Weapon.h"
#include "Audio.h"
#include "NPCController.h"
#include "DestroyOnHealthZero.h"
#include "DealDamageOnCollision.h"
#include "ProjectileSpawner.h"
#include "PointLight.h"
#include "ScaleByHPStamina.h"
#include "Stamina.h"
#include "ParticleEmitter.h"

ComponentFactory::ComponentFactory() {
	m_creationFunctions["Transform"] = Transform::CreateInstance;
	m_creationFunctions["Sprite"] = Sprite::CreateInstance;
	m_creationFunctions["Animation"] = Animation::CreateInstance;
	m_creationFunctions["Controller"] = Controller::CreateInstance;
	m_creationFunctions["Body"] = Body::CreateInstance;
	m_creationFunctions["Camera"] = Camera::CreateInstance;
	m_creationFunctions["CamFollow"] = CamFollow::CreateInstance;
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
	m_creationFunctions["ParticleEmitter"] = ParticleEmitter::CreateInstance;
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