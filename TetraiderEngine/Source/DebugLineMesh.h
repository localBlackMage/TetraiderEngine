#pragma once

#ifndef DEBUG_LINE_MESH_H
#define DEBUG_LINE_MESH_H

class DebugLineMesh
{
private:
	Vector3D* m_vertices;
	unsigned long* m_vertColors;
	GLuint m_vertexBuffer;
	GLuint m_colorBuffer;

public:
	DebugLineMesh(float p1x, float p1y, float p1z, float p2x, float p2y, float p2z);
	~DebugLineMesh();

	int vertexCount() const;
	Vector3D* vertexArray();
	const GLuint& GetVertexBuffer() const;


	int colorCount() const;
	unsigned long* colorArray();
	const GLuint& GetColorBuffer() const;

	void SetColor0(unsigned long c0);
	void SetColor1(unsigned long c1);
	void SetColor(unsigned long color);
	void SetColors(unsigned long c0, unsigned long c1);
};

#endif
