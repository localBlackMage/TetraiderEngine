#pragma once
#ifndef COMPONENT_FACTORY_H
#define COMPONENT_FACTORY_H

#include <string>
#include <unordered_map>

// Forward declaration
class Component;

class ComponentFactory {
public:
	ComponentFactory();
	~ComponentFactory() {};

	Component* CreateComponent(std::string component);
private:
	typedef Component* CreationFunction();
	std::unordered_map <std::string, CreationFunction*> m_componentFunctions;
};

#endif 