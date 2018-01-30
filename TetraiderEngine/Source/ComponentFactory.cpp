#include "ComponentFactory.h"
#include "Transform.h"
#include "Sprite.h"
#include "Animation.h"
#include "Controller.h"
#include "Body.h"
#include "Camera.h"
#include "CamFollow.h"

ComponentFactory::ComponentFactory() { }

Component* ComponentFactory::CreateComponent(std::string component) {
	if (component == "Transform") return new Transform();
	else if (component == "Sprite") return new Sprite();
	else if (component == "Animation") return new Animation();
	else if (component == "Controller") return new Controller();
	else if (component == "Body") return new Body();
	else if (component == "Camera") return new Camera();
	else if (component == "CamFollow") return new CamFollow();

	return 0;
}