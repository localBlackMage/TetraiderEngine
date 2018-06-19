/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

MeshComponent::MeshComponent() :
	Component(ComponentType::C_Mesh),
	m_pMesh(nullptr)
{
}

MeshComponent::~MeshComponent() {}

void MeshComponent::Deactivate() 
{
	pGO = nullptr; 
}

void MeshComponent::Update(float dt) {}

void MeshComponent::Serialize(const json& j)
{
	m_pMesh = TETRA_RESOURCES.GetMesh(ParseString(j, "Mesh"));
	m_shader = ValueExists(j, "shader") ? j["shader"] : "default";
}

void MeshComponent::Override(const json& j) 
{

}
