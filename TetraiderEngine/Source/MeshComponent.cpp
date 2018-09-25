/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

MeshComponent::MeshComponent() :
	RenderableComponent(ComponentType::C_Mesh),
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
		//m_pMesh = TETRA_RESOURCES.GetMesh(meshName);
		m_pScene = TETRA_RESOURCES.GetScene(meshName);
	std::string shaderProgramName = ValueExists(j, "shader") ? JsonReader::ParseStringUnsafe(j, "shader") : "default";
	m_shaderProgramID = TETRA_RENDERER.GetShaderIDByName(shaderProgramName);

	std::string texture = ParseString(j, "Texture1");
	if (texture != "")
		m_pTexture = TETRA_RESOURCES.GetTexture(texture);
	
	texture = ParseString(j, "Normal");
	if (texture != "")
		m_pNormal = TETRA_RESOURCES.GetTexture(texture);

	texture = ParseString(j, "Bump");
	if (texture != "")
		m_pBump = TETRA_RESOURCES.GetTexture(texture);
}

void MeshComponent::Override(const json& j) 
{

}
