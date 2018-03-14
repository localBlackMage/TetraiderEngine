#include <Stdafx.h>

PostProcessing::PostProcessing() : 
	m_gbVShader(""), m_gbHShader(""),
	m_mesh(*TETRA_RESOURCES.LoadMesh("quad"))
{
}

void PostProcessing::InitFBOs()
{
}
