/*

Deweaver
Copyright (c) 2022 Mason Lee Back

File name: Deweaver.cpp

*/

#include "stdafx.h"

const char* ObjectDefToString(ObjectDef object)
{
	switch (object) {
	case ObjectDef::Anim:
		return "Animation (Video)";
		break;
	case ObjectDef::Sound:
		return "Sound";
		break;
	case ObjectDef::EntryPoint:
		return "SI Entry Point";
		break;
	case ObjectDef::Parallel:
		return "Parallel Operation (interweaved)";
		break;
	case ObjectDef::Bitmap:
		return "Bitmap";
		break;
	case ObjectDef::Object:
		return "Object";
		break;
	default:
		return "UNKNOWN!";
		break;
	}
}

void DecodeAnimObject(std::ifstream* stream)
{

}

void DecodeSoundObject(std::ifstream* stream)
{

}

void DecodeEntryPoint(std::ifstream* stream)
{

}

void DecodeParallelOperation(std::ifstream* stream)
{

}

void DecodeBitmapObject(std::ifstream* stream)
{

}

void DecodeStandardObject(std::ifstream* stream)
{

}

void DecodeObject(ObjectDef object, std::ifstream* stream)
{
	// Handle different object cases
	switch (object) {
	case ObjectDef::Anim:
		DecodeAnimObject(stream);
		break;
	case ObjectDef::Sound:
		DecodeSoundObject(stream);
		break;
	case ObjectDef::EntryPoint:
		DecodeEntryPoint(stream);
		break;
	case ObjectDef::Parallel:
		DecodeParallelOperation(stream);
		break;
	case ObjectDef::Bitmap:
		DecodeBitmapObject(stream);
		break;
	case ObjectDef::Object:
		DecodeStandardObject(stream);
		break;
	default:
		printf("Cannot decode- unknown object type!\n");
		break;
	}
}

void Deweave_SI(std::ifstream* stream)
{
	RiffHeader riffHeader = {};

	stream->read((char*)&riffHeader, sizeof(RiffHeader));

	// Verify RIFF header, and RIFF subtype
	if (riffHeader.Signature != 'FFIR') {
		printf("Not a RIFF file!\n");
		return;
	}

	printf("File size: %lu bytes\n", riffHeader.Size);

	if (riffHeader.Subtype != 'INMO') {
		printf("RIFF is not an OMNI subtype!\n");
		return;
	}

	MxHeader mxHeader = {};

	stream->read((char*)&mxHeader, sizeof(MxHeader));

	// Verify MX header
	if (mxHeader.Signature != 'dHxM') {
		printf("Mx header does not match!\n");
		return;
	}

	// Double check header size. ChunkSize doesn't account for the first 8 bytes
	if (mxHeader.ChunkSize + 8 != sizeof(MxHeader)) {
		printf("MX header size mismatch! Expected %lu, got %lu\n", sizeof(MxHeader), mxHeader.ChunkSize);
		return;
	}

	if (mxHeader.MajorVersion != SI_VERSION_MAJOR || mxHeader.MinorVersion != SI_VERSION_MINOR) {
		printf("SI version mismatch! Expected %u.%u, got %u.%u", SI_VERSION_MAJOR, SI_VERSION_MINOR, mxHeader.MajorVersion, mxHeader.MinorVersion);
		return;
	}

	printf("SI version %u.%u\n", mxHeader.MajorVersion, mxHeader.MinorVersion);

	printf("Buffer size: %lu bytes\n", mxHeader.BufferSize);
	printf("Number of buffers: %lu\n", mxHeader.BufferCount);

	// Offsets
	MxOffsets mxOffsets = {};

	stream->read((char*)&mxOffsets, sizeof(MxOffsets));

	// Verify MX header
	if (mxOffsets.Signature != 'fOxM') {
		printf("Mx Offsets header does not match!\n");
		return;
	}

	// Calculate number of chunk offsets, and store them
	unsigned int OffsetsCount = (mxOffsets.ChunkSize - 4) / 4;
	uint32_t* offsetList = new uint32_t[OffsetsCount];

	stream->read((char*)offsetList, sizeof(uint32_t) * OffsetsCount);

	// Stream offsets have blank spots in them for anything that is weaved
	printf("Stream offsets:\n");
	for (int i = 0; i < OffsetsCount; i++) {
		if (offsetList[i] != 0) {
			MxStream mxStream = {};
			stream->seekg(offsetList[i], std::ios_base::beg);
			stream->read((char*)&mxStream, sizeof(MxStream));

			if (mxStream.Signature != 'tSxM') {
				printf("Mx Stream header does not match!\n");
				return;
			}

			MxObject mxObject = {};
			stream->read((char*)&mxObject, sizeof(MxObject));

			if (mxObject.Signature != 'bOxM') {
				printf("Mx Object header does not match!\n");
				return;
			}

			printf("    0x%X, Type: %s\n", offsetList[i], ObjectDefToString(mxObject.ObjectType));

			DecodeObject(mxObject.ObjectType, stream);
		}
	}

}
