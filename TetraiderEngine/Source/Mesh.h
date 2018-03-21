/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Mesh.h
Purpose: Provides a collection of buffers that make up a Mesh
Language: C++
Project: GAM541
Author: Holden Profit
Creation date: 1/17/18
- End Header --------------------------------------------------------*/

#pragma once

#ifndef MESH_H
#define MESH_H

#define QUAD_MESH "quad"
#define SCREEN_QUAD_MESH "screenQuad"

struct Face {
	unsigned int index[3];
	explicit Face(unsigned int v1, unsigned int v2, unsigned int v3) { 
		index[0] = v1; index[1] = v2; index[2] = v3; 
	}
	unsigned int operator[](int i) const { return index[i]; }
	unsigned int& operator[](int i) { return index[i]; }
};

struct TexCoords {
	GLfloat u, v;
	explicit TexCoords(float _u, float _v) : u(_u), v(_v) {}
	GLfloat operator[](int idx) {
		return idx == 0 ? u : v;
	}
};

class Mesh
{
private:
	std::vector<Vector3D> m_vertices, m_normals;
	std::vector<GLfloat> m_texCoords;
	std::vector<unsigned long> m_vertColors;
	std::vector<Face> m_faces;
	GLuint m_vertexBuffer;
	GLuint m_normalBuffer;
	GLuint m_faceBuffer;
	GLuint m_textCoordBuffer;

public:
	Mesh();
	~Mesh();

	void AddTriangle(Vector3D p1, Vector3D p2, Vector3D p3);
	void AddTriangle(float p1x, float p1y, float p1z, float p2x, float p2y, float p2z, float p3x, float p3y, float p3z);
	void AddTriangle(Vector3D p1, TexCoords uv1, Vector3D p2, TexCoords uv2, Vector3D p3, TexCoords uv3);
	void AddTriangle(Vector3D p1, TexCoords uv1, unsigned long c1, Vector3D p2, TexCoords uv2, unsigned long c2, Vector3D p3, TexCoords uv3, unsigned long c3);
	void AddTriangle(float p1x, float p1y, float p1z, float uv1u, float uv1v, float p2x, float p2y, float p2z, float uv2u, float uv2v, float p3x, float p3y, float p3z, float uv3u, float uv3v);
	void AddTriangle(float p1x, float p1y, float p1z, float uv1u, float uv1v, unsigned long c1, float p2x, float p2y, float p2z, float uv2u, float uv2v, unsigned long c2, float p3x, float p3y, float p3z, float uv3u, float uv3v, unsigned long c3);

	void FinishMesh();

	int vertexCount() const;
	Vector3D* vertexArray();
	const GLuint& GetVertexBuffer() const;

	Vector3D* normalArray();
	const GLuint& GetNormalBuffer() const;

	int faceCount() const;
	Face* faceArray();
	const GLuint& GetFaceBuffer() const;
	
	const GLuint& GetTextCoordBuffer() const;
	GLfloat* texCoordArray();
};

#endif