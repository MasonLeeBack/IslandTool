/*

Deweaver
Copyright (c) 2022 Mason Lee Back

File name: OmniTypes.h

*/

#ifndef _OMNITYPES_H_
#define _OMNITYPES_H_

struct OmniVertex {
	float       X;
	float       Y;
	float       Z;
};

struct OmniColor {
	uint8_t     Red;
	uint8_t     Green;
	uint8_t     Blue;
};

struct OmniMesh {
	OmniColor   Color;
	uint8_t     Shading;
	uint32_t    NumVertices;
	OmniVertex* Vertices;
	uint32_t    NumFaces;
	uint32_t*   Faces;
};

#endif // _OMNITYPES_H_
