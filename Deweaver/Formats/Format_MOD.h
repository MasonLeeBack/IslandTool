/*

Deweaver
Copyright (c) 2022 Mason Lee Back

File name: Format_MOD.h

*/

#ifndef _FORMAT_MOD_H_
#define _FORMAT_MOD_H_

#define ModelVersion_Current 3

struct ROIData {
	uint32_t   NumberOfLODs;
	uint32_t   NumVertices;
	float      AverageArea;
	uint32_t   NumCosts;
	uint32_t   NumMeshes;
	OmniMesh* Meshes;
};

struct ModelHeader {
	uint32_t   Version;
	uint32_t   NumberOfROIs;
	uint32_t   AnimationDuration;
	uint32_t   NameLength;
	char* Name;

	OmniVertex BoundSphereCenter;
	uint32_t   BoundSphereRadius;
};

void Deweave_MOD(std::ifstream* stream);

#endif // _FORMAT_MOD_H_