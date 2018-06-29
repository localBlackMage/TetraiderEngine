/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

void Mesh::_LoadMeshToGraphicsCard()
{
	int vertexBufferSize = sizeof(Vector3D) * vertexCount();

#pragma region Vertex Buffer
	glGenBuffers(1, &m_vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER,
		vertexBufferSize,
		vertexArray(), GL_STATIC_DRAW);
#pragma endregion

#pragma region Normal Buffer
	glGenBuffers(1, &m_normalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_normalBuffer);
	glBufferData(GL_ARRAY_BUFFER,
		vertexBufferSize,
		normalArray(), GL_STATIC_DRAW);
#pragma endregion

#pragma region Tangent Buffer
	glGenBuffers(1, &m_tangentBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_tangentBuffer);
	glBufferData(GL_ARRAY_BUFFER,
		vertexBufferSize,
		tangentArray(), GL_STATIC_DRAW);
#pragma endregion

#pragma region Bitangent Buffer
	glGenBuffers(1, &m_bitangentBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_bitangentBuffer);
	glBufferData(GL_ARRAY_BUFFER,
		vertexBufferSize,
		bitangentArray(), GL_STATIC_DRAW);
#pragma endregion

#pragma region Face Buffer
	glGenBuffers(1, &m_faceBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_faceBuffer);
	int faceBufferSize = sizeof(Face)*faceCount();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		faceBufferSize,
		faceArray(), GL_STATIC_DRAW);
#pragma endregion

#pragma region Texture Coordinate Buffer
	glGenBuffers(1, &m_textCoordBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_textCoordBuffer);
	int texCoordBufferSize = 2 * sizeof(GLfloat) * vertexCount();
	glBufferData(GL_ARRAY_BUFFER,
		texCoordBufferSize,
		texCoordArray(), GL_STATIC_DRAW);
#pragma endregion

}

Mesh::Mesh() {}

Mesh::Mesh(const aiMesh * mesh)
{
	m_vertices.reserve(mesh->mNumVertices);
	for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
		m_vertices.push_back(Vector3D(mesh->mVertices[i]));
	}

	m_normals.reserve(mesh->mNumVertices);
	for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
		m_normals.push_back(Vector3D(mesh->mNormals[i]));
	}

	m_tangents.reserve(mesh->mNumVertices);
	for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
		m_tangents.push_back(Vector3D(mesh->mTangents[i]));
	}

	m_bitangents.reserve(mesh->mNumVertices);
	for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
		m_bitangents.push_back(Vector3D(mesh->mBitangents[i]));
	}

	m_faces.reserve(mesh->mNumFaces);
	for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
		m_faces.push_back(Face(mesh->mFaces[i]));
	}

	m_texCoords.reserve(mesh->mNumVertices * 2);
	for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
		m_texCoords.push_back(mesh->mTextureCoords[0][i].x);
		m_texCoords.push_back(mesh->mTextureCoords[0][i].y);
	}

	_LoadMeshToGraphicsCard();
}

Mesh::~Mesh()
{
	// delete the vertex, normal, texture coordinate, and face buffers
	glDeleteBuffers(1, &m_vertexBuffer);
	glDeleteBuffers(1, &m_normalBuffer);
	glDeleteBuffers(1, &m_tangentBuffer);
	glDeleteBuffers(1, &m_bitangentBuffer);
	glDeleteBuffers(1, &m_faceBuffer);
	glDeleteBuffers(1, &m_textCoordBuffer);

	m_vertices.clear();
	m_normals.clear();
	m_tangents.clear();
	m_bitangents.clear();
	m_faces.clear();
}

#pragma region Add Triangle

void Mesh::AddTriangle(Vector3D p1, Vector3D p2, Vector3D p3)
{
	unsigned int idx0 = m_vertices.size(), 
		idx1 = idx0 + 1, 
		idx2 = idx0 + 2;
	m_vertices.push_back(p1);
	m_vertices.push_back(p2);
	m_vertices.push_back(p3);

	m_faces.push_back(Face(idx0, idx1, idx2));

	Vector3D normal = Vector3D(Vector3D::Normalize(Vector3D::Cross(p2 - p1, p3 - p1)), 0.f);
	Vector3D tangent = Vector3D(Vector3D::Normalize(p2 - p1), 0.f);
	Vector3D bitangent = Vector3D(Vector3D::Normalize(Vector3D::Cross(normal, tangent)), 0.f);

	m_normals.push_back(normal);
	m_tangents.push_back(tangent);
	m_bitangents.push_back(bitangent);
}

void Mesh::AddTriangle(float p1x, float p1y, float p1z, float p2x, float p2y, float p2z, float p3x, float p3y, float p3z)
{
	AddTriangle(
		Vector3D(p1x, p1y, p1z),
		Vector3D(p2x, p2y, p2z),
		Vector3D(p3x, p3y, p3z)
	);
}

void Mesh::AddTriangle(Vector3D p1, TexCoords uv1, Vector3D p2, TexCoords uv2, Vector3D p3, TexCoords uv3)
{
	AddTriangle(p1, p2, p3);

	m_texCoords.push_back(uv1.u);
	m_texCoords.push_back(uv1.v);

	m_texCoords.push_back(uv2.u);
	m_texCoords.push_back(uv2.v);

	m_texCoords.push_back(uv3.u);
	m_texCoords.push_back(uv3.v);
}

void Mesh::AddTriangle(Vector3D p1, TexCoords uv1, unsigned long c1, Vector3D p2, TexCoords uv2, unsigned long c2, Vector3D p3, TexCoords uv3, unsigned long c3)
{
	AddTriangle(p1, uv1, p2, uv2, p3, uv3);

	m_vertColors.push_back(c1);
	m_vertColors.push_back(c2);
	m_vertColors.push_back(c3);
}

void Mesh::AddTriangle(float p1x, float p1y, float p1z, float uv1u, float uv1v, float p2x, float p2y, float p2z, float uv2u, float uv2v, float p3x, float p3y, float p3z, float uv3u, float uv3v)
{
	AddTriangle(
		Vector3D(p1x, p1y, p1z), TexCoords(uv1u, uv1v),
		Vector3D(p2x, p2y, p2z), TexCoords(uv2u, uv2v),
		Vector3D(p3x, p3y, p3z), TexCoords(uv3u, uv3v)
	);
}

void Mesh::AddTriangle(float p1x, float p1y, float p1z, float uv1u, float uv1v, unsigned long c1, float p2x, float p2y, float p2z, float uv2u, float uv2v, unsigned long c2, float p3x, float p3y, float p3z, float uv3u, float uv3v, unsigned long c3)
{
	AddTriangle(
		Vector3D(p1x, p1y, p1z), TexCoords(uv1u, uv1v), c1,
		Vector3D(p2x, p2y, p2z), TexCoords(uv2u, uv2v), c2,
		Vector3D(p3x, p3y, p3z), TexCoords(uv3u, uv3v), c3
	);
}

#pragma endregion

void Mesh::AddVertex(float px, float py, float pz)
{
	m_vertices.push_back(Vector3D(px, py, pz));
}

void Mesh::AddVertex(float px, float py, float pz, const TexCoords& uv)
{
	m_vertices.push_back(Vector3D(px, py, pz));
	m_texCoords.push_back(uv.u);
	m_texCoords.push_back(uv.v);
}

void Mesh::AddFace(unsigned int a, unsigned int b, unsigned int c)
{
	m_faces.push_back(Face(a, b, c));
}

void Mesh::FinishMesh()
{
	CalcNormals();
	m_faces.shrink_to_fit();
	m_vertices.shrink_to_fit();
	m_normals.shrink_to_fit();
	m_tangents.shrink_to_fit();
	m_bitangents.shrink_to_fit();
	_LoadMeshToGraphicsCard();
}

void Mesh::CalcNormals()
{
	m_faces.shrink_to_fit();
	m_vertices.shrink_to_fit();
	unsigned int numVerts = m_vertices.size();
	unsigned int numFaces = m_faces.size();
	m_normals.reserve(numVerts);
	m_tangents.reserve(numVerts);
	m_bitangents.reserve(numVerts);

	std::unordered_map<unsigned int, Vector3D> normals;

	// Calculate normal per face, add it to all vertices that make up that tri/face
	for (unsigned int i = 0; i < numFaces; ++i)
	{
		Vector3D& v1 = m_vertices[m_faces[i][0]];
		Vector3D& v2 = m_vertices[m_faces[i][1]];
		Vector3D& v3 = m_vertices[m_faces[i][2]];
		Vector3D norm = Vector3D::Normalize(Vector3D::Cross(v2 - v1, v3 - v1));

		normals[m_faces[i][0]] += norm;
		normals[m_faces[i][1]] += norm;
		normals[m_faces[i][2]] += norm;
	}

	for (unsigned int i = 0; i < numVerts; ++i)
	{
		Vector3D normal = Vector3D::Normalize(normals[i]);
		normal.w = 0.0f;
		m_normals.push_back(normal);
		// NOTE: This is a garbage implementation, replace it later
		Vector3D tangent = Matrix4x4::Rotate(90.f, YAXIS) * normal;

		m_tangents.push_back(tangent);
		m_bitangents.push_back(Vector3D::Normalize(Vector3D::Cross(normal, tangent)));
	}

}

int Mesh::vertexCount() const
{
	return m_vertices.size();
}

Vector3D * Mesh::vertexArray()
{
	return &(m_vertices[0]);
}

const GLuint & Mesh::GetVertexBuffer() const
{
	return m_vertexBuffer;
}

Vector3D * Mesh::normalArray()
{
	return &(m_normals[0]);
}

const GLuint & Mesh::GetNormalBuffer() const
{
	return m_normalBuffer;
}

Vector3D * Mesh::tangentArray()
{
	return &(m_tangents[0]);
}

const GLuint & Mesh::GetTangentBuffer() const
{
	return m_tangentBuffer;
}

Vector3D * Mesh::bitangentArray()
{
	return &(m_bitangents[0]);
}

const GLuint & Mesh::GetBitangentBuffer() const
{
	return m_bitangentBuffer;
}

int Mesh::faceCount() const
{
	return m_faces.size();
}

Face * Mesh::faceArray()
{
	return  &(m_faces[0]);
}

const GLuint & Mesh::GetFaceBuffer() const
{
	return m_faceBuffer;
}

const GLuint & Mesh::GetTextCoordBuffer() const
{
	return m_textCoordBuffer;
}

GLfloat * Mesh::texCoordArray()
{
	return  &(m_texCoords[0]);
}

Vector3D ParseVec3(std::string& line) {
	size_t end = line.find(' ');
	float first = std::stof(line, &end);

	line = line.substr(end);
	end = line.find(' ');
	float second = std::stof(line, &end);

	line = line.substr(end);
	float third = std::stof(line);

	return Vector3D(first, second, third);
}

MeshScene::MeshScene(unsigned short numMeshes) : 
	m_numMeshes(numMeshes)
{
	m_meshes = (std::shared_ptr<Mesh>*)malloc(sizeof(std::shared_ptr<Mesh>) * m_numMeshes);
}

MeshScene::~MeshScene()
{
	
}

std::shared_ptr<Mesh> MeshScene::operator[](const int idx) const
{
	return std::shared_ptr<Mesh>();
}

std::shared_ptr<Mesh>& MeshScene::operator[](const int idx)
{
	// TODO: insert return statement here
}
