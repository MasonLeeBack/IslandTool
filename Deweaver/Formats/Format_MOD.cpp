/*

Deweaver
Copyright (c) 2022 Mason Lee Back

File name: Deweaver.cpp

*/

#include "stdafx.h"

void Deweave_MOD(std::ifstream* stream)
{
	ModelHeader modelHeader = {};

	// Get MOD Version
	stream->read((char*)&modelHeader.Version, sizeof(uint32_t));
	printf("MOD Version: %lu\n", modelHeader.Version);

	// Get number of ROIs
	stream->read((char*)&modelHeader.NumberOfROIs, sizeof(uint32_t));
	printf("Number of ROIs: %lu\n", modelHeader.NumberOfROIs);

	// Get Animation Duration
	stream->read((char*)&modelHeader.AnimationDuration, sizeof(uint32_t));
	printf("Animation Duration: %lu\n", modelHeader.AnimationDuration);

	// Model name
	stream->read((char*)&modelHeader.NameLength, sizeof(uint32_t));
	modelHeader.Name = new char[modelHeader.NameLength + 1];
	stream->read(modelHeader.Name, modelHeader.NameLength);
	modelHeader.Name[modelHeader.NameLength] = '\0';
	printf("Model Name: %s\n", modelHeader.Name);

	// Padding (this is technically just a NULL character, but it's not handled)
	uint8_t padding;
	stream->read((char*)&padding, sizeof(uint8_t));

	// Bound sphere center
	stream->read((char*)&modelHeader.BoundSphereCenter, sizeof(OmniVertex));
	printf("Bound Sphere Center: {XYZ: %f, %f, %f}\n", modelHeader.BoundSphereCenter.X, modelHeader.BoundSphereCenter.Y, modelHeader.BoundSphereCenter.Z);

	// Bound sphere radius
	stream->read((char*)&modelHeader.BoundSphereRadius, sizeof(uint32_t));
	printf("Bound Sphere Radius: %lu\n", modelHeader.BoundSphereRadius);

	stream->seekg(133);

	// ROI data
	ROIData* roi = new ROIData[modelHeader.NumberOfROIs];
	for (unsigned int i = 0; i <= modelHeader.NumberOfROIs - 1; i++) {
		printf("\nROI %i:\n", i);

		stream->read((char*)&roi[i].NumberOfLODs, sizeof(uint32_t));
		printf("    Number of LODs: %lu\n", roi[i].NumberOfLODs);

		stream->read((char*)&roi[i].NumVertices, sizeof(uint32_t));
		printf("    Vertice count: %lu\n", roi[i].NumVertices);

		stream->read((char*)&roi[i].AverageArea, sizeof(float));
		stream->read((char*)&roi[i].NumCosts, sizeof(uint32_t));

		stream->read((char*)&roi[i].NumMeshes, sizeof(uint32_t));
		printf("    Number of Meshes: %lu\n", roi[i].NumMeshes);

		OmniMesh* mesh = new OmniMesh[roi[i].NumMeshes];
		for (unsigned int j = 0; j <= roi[i].NumMeshes - 1; j++) {
			printf("    Mesh %i:\n", j);

			stream->read((char*)&mesh[j].Color, sizeof(OmniColor));
			printf("        Mesh color: {%u, %u, %u}\n", mesh[j].Color.Red, mesh[j].Color.Green, mesh[j].Color.Blue);
			
			stream->read((char*)&mesh[j].Shading, sizeof(uint8_t));
			printf("        Shading: %u\n", mesh[j].Shading);

			stream->read((char*)&mesh[j].NumVertices, sizeof(uint32_t));
			printf("        Vertice count: %lu\n", mesh[j].NumVertices);

			mesh[j].Vertices = new OmniVertex[mesh[j].NumVertices];
			printf("        Vertices:\n");
			for (unsigned int k = 0; k <= mesh[j].NumVertices - 1; k++) {
				stream->read((char*)&mesh[j].Vertices[k], sizeof(OmniVertex));
				printf("            V%i{%f, %f, %f}\n", k, mesh[j].Vertices[k].X, mesh[j].Vertices[k].Y, mesh[j].Vertices[k].Z);
			}

			stream->read((char*)&mesh[j].NumFaces, sizeof(uint32_t));
			printf("        Face count: %lu\n", mesh[j].NumFaces);
			printf("        Faces:\n");
			mesh[j].Faces = new uint32_t[mesh[j].NumFaces];
			for (unsigned int k = 0; k <= mesh[j].NumFaces - 1; k++) {
				stream->read((char*)&mesh[j].Faces[k], sizeof(uint32_t));
				printf("            F%i %lu\n", k, mesh[j].Faces[k]);
			}
		}
	}

}
