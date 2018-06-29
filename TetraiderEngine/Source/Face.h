#pragma once

#ifndef  FACE_H
#define FACE_H 

struct Face {
	unsigned int index[3];
	Face(unsigned int v1, unsigned int v2, unsigned int v3) {
		index[0] = v1; index[1] = v2; index[2] = v3;
	}
	Face(const aiFace& face) {
		index[0] = face.mIndices[0];
		index[1] = face.mIndices[1];
		index[2] = face.mIndices[2];
	}
	unsigned int operator[](int i) const { return index[i]; }
	unsigned int& operator[](int i) { return index[i]; }
};

#endif // ! FACE_H
