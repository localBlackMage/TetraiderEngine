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

struct TexCoords {
	GLfloat u, v;
	TexCoords(float _u, float _v) : u(_u), v(_v) {}
	TexCoords(const aiVector3D* uv) : u(uv->x), v(uv->y) {}
	GLfloat operator[](int idx) {
		return idx == 0 ? u : v;
	}
};

class Material {
protected:
	const aiMaterial * m_pMaterial;

public:
	Material(const aiMaterial * pAiMaterial) : m_pMaterial(pAiMaterial) {};
	~Material() {};
	inline const aiMaterial * GetMaterial() const { return m_pMaterial; }
	inline void SetMaterial(const aiMaterial * pAiMaterial) { m_pMaterial = pAiMaterial; }
};

class Mesh;
class Scene {
protected:
	unsigned short m_numMeshes;
	std::vector< std::shared_ptr<Mesh> > m_meshes;

public:
	Scene(unsigned short numMeshes);
	~Scene();

	unsigned short NumMeshes()	const { return m_numMeshes; }
	std::shared_ptr<Mesh>	operator[](const int idx) const { return m_meshes[idx]; };
	std::shared_ptr<Mesh>&	operator[](const int idx) { return m_meshes[idx]; };

	std::shared_ptr<Mesh>	operator[](const unsigned int idx) const { return m_meshes[idx]; };
	std::shared_ptr<Mesh>&	operator[](const unsigned int idx) { return m_meshes[idx]; };

	std::vector< std::shared_ptr<Mesh> >::iterator begin() { return std::begin(m_meshes); }
	std::vector< std::shared_ptr<Mesh> >::iterator end() { return std::end(m_meshes); }
};

class Mesh
{
protected:
	friend class Scene;

	enum VBO_TYPE {
		VBO_VERTICES = 0,
		VBO_NORMALS,
		VBO_TANGENTS,
		VBO_BITANGENTS,
		VBO_FACES,
		VBO_TEX_COORDS,

		NUM_VBO_TYPES
	};

	std::vector<Vector3D> m_vertices, m_normals, m_tangents, m_bitangents;
	std::vector<GLfloat> m_texCoords;
	std::vector<unsigned long> m_vertColors;
	std::vector<Face> m_faces;
	GLuint m_vertexBuffer;
	GLuint m_normalBuffer;
	GLuint m_tangentBuffer;
	GLuint m_bitangentBuffer;
	GLuint m_faceBuffer;
	GLuint m_textCoordBuffer;

	GLuint m_VAO, m_VBO[NUM_VBO_TYPES];

	void _LoadMeshToGraphicsCard();
public:
	Mesh();
	Mesh(const aiMesh* mesh);
	~Mesh();

	void AddTriangle(Vector3D p1, Vector3D p2, Vector3D p3);
	void AddTriangle(float p1x, float p1y, float p1z, float p2x, float p2y, float p2z, float p3x, float p3y, float p3z);
	void AddTriangle(Vector3D p1, TexCoords uv1, Vector3D p2, TexCoords uv2, Vector3D p3, TexCoords uv3);
	void AddTriangle(Vector3D p1, TexCoords uv1, unsigned long c1, Vector3D p2, TexCoords uv2, unsigned long c2, Vector3D p3, TexCoords uv3, unsigned long c3);
	void AddTriangle(float p1x, float p1y, float p1z, float uv1u, float uv1v, float p2x, float p2y, float p2z, float uv2u, float uv2v, float p3x, float p3y, float p3z, float uv3u, float uv3v);
	void AddTriangle(float p1x, float p1y, float p1z, float uv1u, float uv1v, unsigned long c1, float p2x, float p2y, float p2z, float uv2u, float uv2v, unsigned long c2, float p3x, float p3y, float p3z, float uv3u, float uv3v, unsigned long c3);

	void AddVertex(float px, float py, float pz);
	void AddVertex(float px, float py, float pz, const TexCoords& uv);
	void AddFace(unsigned int a, unsigned int b, unsigned int c);

	void FinishMesh();
	void CalcNormals();
	void CreateFromAiMesh(const aiMesh* mesh);

	int vertexCount() const;
	Vector3D* vertexArray();
	const GLuint& GetVertexBuffer() const;

	Vector3D* normalArray();
	const GLuint& GetNormalBuffer() const;

	Vector3D* tangentArray();
	const GLuint& GetTangentBuffer() const;

	Vector3D* bitangentArray();
	const GLuint& GetBitangentBuffer() const;

	int faceCount() const;
	Face* faceArray();
	const GLuint& GetFaceBuffer() const;
	
	const GLuint& GetTextCoordBuffer() const;
	GLfloat* texCoordArray();
};

#endif