#pragma once

#ifndef PLANE_H
#define PLANE_H

class Plane : public Mesh {
public:
	Plane(unsigned int size) {
		float d = 2.0f / size;

		for (int j = 0; j <= size; ++j) {
			float y = j*d - 1.0f;
			for (int i = 0; i <= size; ++i) {
				float x = i*d - 1.0f;
				int index = (size + 1)*j + i;
				vertices[index].x = x;
				vertices[index].y = y;
				vertices[index].z = 0;
				vertices[index].w = 1;
			}
		}
	};

	~Plane() {};
};
//
//Plane::Plane(int size)
//	: vertices((size + 1)*(size + 1)),
//	normals(vertices.size()),
//	faces(2 * size*size) {
//
//	float d = 2.0f / size;
//	for (int j = 0; j <= size; ++j) {
//		float y = j*d - 1.0f;
//		for (int i = 0; i <= size; ++i) {
//			float x = i*d - 1.0f;
//			int index = (size + 1)*j + i;
//			vertices[index].x = x;
//			vertices[index].y = y;
//			vertices[index].z = 0;
//			vertices[index].w = 1;
//		}
//	}
//
//	for (unsigned int n = 0; n < normals.size(); ++n)
//		normals[n] = Hcoord(0, 0, 1, 0);
//
//	for (int n = 0, j = 0; j < size; ++j) {
//		for (int i = 0; i < size; ++i) {
//			faces[n][0] = (size + 1)*j + i;
//			faces[n][1] = (size + 1)*j + i + 1;
//			faces[n][2] = (size + 1)*(j + 1) + i + 1;
//			++n;
//			faces[n][0] = (size + 1)*j + i;
//			faces[n][1] = (size + 1)*(j + 1) + i + 1;
//			faces[n][2] = (size + 1)*(j + 1) + i;
//			++n;
//		}
//	}
//}


#endif