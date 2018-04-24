/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

DebugLineMesh::DebugLineMesh(float p1x, float p1y, float p1z, float p2x, float p2y, float p2z) :
	m_vertices(new Vector3D[2]),
	m_vertColors(new unsigned long[2])
{
#pragma region Vertex Buffer
	m_vertices[0] = Vector3D(p1x, p1y, p1z);
	m_vertices[1] = Vector3D(p2x, p2y, p2z);
	int vertexBufferSize = sizeof(Vector3D) * 2;

	glGenBuffers(1, &m_vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER,
		vertexBufferSize,
		vertexArray(), GL_STATIC_DRAW);
#pragma endregion

	//SetColors(c1, c2);
}

DebugLineMesh::~DebugLineMesh()
{
	// delete the vertex, normal, texture coordinate, and face buffers
	glDeleteBuffers(1, &m_vertexBuffer);
	glDeleteBuffers(1, &m_colorBuffer);

	delete m_vertices;
	delete m_vertColors;
}

int DebugLineMesh::vertexCount() const
{
	return 2;
}

Vector3D * DebugLineMesh::vertexArray()
{
	return &(m_vertices[0]);
}

const GLuint & DebugLineMesh::GetVertexBuffer() const
{
	return m_vertexBuffer;
}

int DebugLineMesh::colorCount() const
{
	return 2;
}

unsigned long * DebugLineMesh::colorArray()
{
	return &(m_vertColors[0]);
}

const GLuint & DebugLineMesh::GetColorBuffer() const
{
	return m_colorBuffer;
}

void DebugLineMesh::SetColor0(unsigned long c0)
{
	SetColors(c0, m_vertColors[1]);
}

void DebugLineMesh::SetColor1(unsigned long c1)
{
	SetColors(m_vertColors[0], c1);
}

void DebugLineMesh::SetColor(unsigned long color)
{
	SetColors(color, color);
}

void DebugLineMesh::SetColors(unsigned long c0, unsigned long c1)
{
	m_vertColors[0] = c0;
	m_vertColors[1] = c1;

	int colorBufferSize = sizeof(unsigned long) * 2;

	glGenBuffers(1, &m_colorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer);
	glBufferData(GL_ARRAY_BUFFER,
		colorBufferSize,
		colorArray(), GL_STATIC_DRAW);
}

