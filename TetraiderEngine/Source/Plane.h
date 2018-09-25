#pragma once

#ifndef PLANE_H
#define PLANE_H

#define PLANE_MESH_20 20
#define PLANE_MESH_NAME_20 "plane_20"

#define PLANE_MESH_50 50
#define PLANE_MESH_NAME_50 "plane_50"

#define PLANE_MESH_100 100
#define PLANE_MESH_NAME_100 "plane_100"

class Plane : public Mesh {
public:
	Plane(unsigned int size) : Mesh() {
		float d = 2.0f / size;

		m_vertices.reserve((size + 1)*(size + 1));
		for (int j = 0; j <= size; ++j) {
			float y = j*d - 1.0f;
			for (int i = 0; i <= size; ++i) {
				float x = i*d - 1.0f;
				int index = (size + 1)*j + i;
				m_vertices.push_back(Vector3D(x, y, 0, 1));
			}
		}
		int numVerts = m_vertices.size();

		m_normals.reserve(numVerts);
		m_tangents.reserve(numVerts);
		m_bitangents.reserve(numVerts);
		for (int n = 0; n < numVerts; ++n) {
			m_normals.push_back(Vector3D(0, 0, 1, 0));
			m_tangents.push_back(Vector3D(1, 0, 0, 0));
			m_bitangents.push_back(Vector3D(0, 1, 0, 0));
		}

		m_faces.reserve(size * size * 2);
		for (int  j = 0; j < size; ++j) {
			for (int i = 0; i < size; ++i) {
				m_faces.push_back(Face((size + 1)*j + i, (size + 1)*j + i + 1, (size + 1)*(j + 1) + i + 1));
				m_faces.push_back(Face((size + 1)*j + i, (size + 1)*(j + 1) + i + 1, (size + 1)*(j + 1) + i));
			}
		}

		m_texCoords.reserve(numVerts * 2);
		Matrix4x4 Std2Unit = Matrix4x4::Scale(0.5f, 0.5f, 1)
			* Matrix4x4::Translate(Vector3D(1, 1, 0, 0));
		for (unsigned int i = 0; i < numVerts; ++i) {
			Vector3D uv = Std2Unit * m_vertices[i];
			m_texCoords.push_back(uv[0]);
			m_texCoords.push_back(uv[1]);
		}
		//_LoadMeshToGraphicsCard();
		_LoadMeshToGraphicsCard_OLD();
	};

	~Plane() {};
};

#endif