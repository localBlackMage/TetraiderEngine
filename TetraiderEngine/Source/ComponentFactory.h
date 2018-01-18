#ifndef COMPONENT_FACTORY_H
#define COMPONENT_FACTORY_H

#include <string>

// Forward declaration
class Component;

class ComponentFactory {
public:
	ComponentFactory();
	Component* CreateComponent(std::string component);
};

#endif 