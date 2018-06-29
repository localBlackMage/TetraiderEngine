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
	std::string meshName = ParseString(j, "Mesh");
	if (meshName == "quad")
		m_pMesh = TETRA_RESOURCES.GetInternalMesh(meshName);
	else
		m_pMesh = TETRA_RESOURCES.GetMesh(meshName);
	m_shader = ValueExists(j, "shader") ? JsonReader::ParseStringUnsafe(j, "shader") : "default";
}

void MeshComponent::Override(const json& j) 
{

}
