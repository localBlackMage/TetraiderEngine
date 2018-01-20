#include "ComponentFactory.h"
#include "Transform.h"
#include "Sprite.h"
#include "Animation.h"
#include "Controller.h"

ComponentFactory::ComponentFactory() { }

Component* ComponentFactory::CreateComponent(std::string component) {
	if (component == "Transform") return new Transform();
	else if (component == "Sprite") return new Sprite();
	else if (component == "Animation") return new Animation();
	else if (component == "Controller") return new Controller();

	return 0;
}