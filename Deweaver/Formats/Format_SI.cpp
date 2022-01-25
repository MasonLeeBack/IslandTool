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
	case ObjectDef::Linear:
		return "Linear Operation";
		break;
	case ObjectDef::Parallel:
		return "Parallel Operation (interweaved)";
		break;
	case ObjectDef::Event:
		return "Event";
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

// messy, but works.
char* GetNextString(std::ifstream* stream) {
	int firstPos = 0;
	int lastPos = 0;

	uint8_t currentByte;
	stream->read((char*)&currentByte, sizeof(uint8_t));

	while (currentByte == '\0') {
		stream->read((char*)&currentByte, sizeof(uint8_t));
	}

	firstPos = (int)stream->tellg();

	while (currentByte != '\0') {
		stream->read((char*)&currentByte, sizeof(uint8_t));
	}

	lastPos = (int)stream->tellg();

	firstPos--;
	lastPos--;

	char* stringToGet = new char[(lastPos - firstPos) + 1];

	stream->seekg(firstPos);
	stream->read(stringToGet, (lastPos - firstPos) + 1);

	//stringToGet[lastPos - firstPos] = '\0';

	return stringToGet;
}

void DeweaveObject(std::ifstream* stream)
{
	MxObject mxObject = {};

	printf("    0x%X:\n", (int)stream->tellg());

	stream->read((char*)&mxObject.Signature, sizeof(uint32_t));
	stream->read((char*)&mxObject.ObjectSize, sizeof(uint32_t));
	stream->read((char*)&mxObject.ObjectType, sizeof(uint16_t));

	if (mxObject.Signature != 'bOxM') {
		printf("Mx Object header does not match!\n");
		return;
	}

	char* tempString = GetNextString(stream);

	// HandlerClass always comes before the actual name,
	// complicating matters. There might be a flag for this?
	// but as far as I can tell, there's not.
	for (int i = 0; i < ClassListSize; i++) {
		if (!strcmp(tempString, classesList[i]->GetClassName()))
		{
			mxObject.HandlerClass = tempString;
		}
	}

	if (mxObject.HandlerClass == NULL) {
		mxObject.Name = tempString;
	}
	else {
		mxObject.Name = GetNextString(stream);
	}

	printf("        Type: %s\n", ObjectDefToString(mxObject.ObjectType));
	printf("        Name: %s\n", mxObject.Name);

	if (mxObject.HandlerClass != NULL) {
		printf("        Handler class: %s\n", mxObject.HandlerClass);
	}

	stream->read((char*)&mxObject.StreamID, sizeof(uint32_t));

	printf("        Stream ID: %lu\n", mxObject.StreamID);

	stream->read((char*)&mxObject.Flags, sizeof(uint32_t));

	if (mxObject.Flags & MX_FLAG_LOOP_STREAM) {
		printf("        LoopMethod: STREAM\n");
	}

	if (mxObject.Flags & MX_FLAG_TRANSPARENT) {
		printf("        Transparency: TRUE\n");
	}

	stream->read((char*)&mxObject.Unknown, sizeof(uint32_t));

	stream->read((char*)&mxObject.Duration, sizeof(uint32_t));
	stream->read((char*)&mxObject.Loops, sizeof(uint32_t));

	if (mxObject.Duration == MX_DURATION_INDEFINITE) {
		printf("        Duration: INDEFINITE\n");
	}

	if (mxObject.Loops != 1) {
		printf("        Loops: %lu\n", mxObject.Loops);
	}

	// Coordinate Data
	stream->read((char*)&mxObject.Position, sizeof(Vector3));
	stream->read((char*)&mxObject.Direction, sizeof(Vector3));
	stream->read((char*)&mxObject.Up, sizeof(Vector3));

	// Display position, direction, and Up value if not their default values
	if (mxObject.Position != Vector3{ 0,0,0 }) {
		printf("        Position: { %f, %f, %f }\n", mxObject.Position.X, mxObject.Position.Y, mxObject.Position.Z);
	}

	if (mxObject.Direction != Vector3{ 0,0,1 }) {
		printf("        Direction: { %f, %f, %f }\n", mxObject.Direction.X, mxObject.Direction.Y, mxObject.Direction.Z);
	}

	if (mxObject.Up != Vector3{ 0,1,0 }) {
		printf("        Up: { %f, %f, %f }\n", mxObject.Up.X, mxObject.Up.Y, mxObject.Up.Z);
	}

	stream->read((char*)&mxObject.ExtraData_Size, sizeof(uint16_t));

	// ExtraData is defined when the size is not NULL.
	if (mxObject.ExtraData_Size != 0) {
		mxObject.ExtraData = GetNextString(stream);

		printf("        Extra data: %s\n", mxObject.ExtraData);
	}

	// Object types of Parallel and Linear do not support filenames,
	// and any objects that don't reference files directly use the "inline"
	// identifier, with the actual entry referenced in the extra data.
	if (mxObject.ObjectType != ObjectDef::Parallel && mxObject.ObjectType != ObjectDef::Linear) {
		mxObject.FileName = GetNextString(stream);
		printf("        File name: %s\n", mxObject.FileName);
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
	for (unsigned int i = 0; i < OffsetsCount; i++) {
		if (offsetList[i] != 0) {
			MxStream mxStream = {};
			stream->seekg(offsetList[i], std::ios_base::beg);
			stream->read((char*)&mxStream, sizeof(MxStream));

			if (mxStream.Signature != 'tSxM') {
				printf("Mx Stream header does not match!\n");
				return;
			}

			DeweaveObject(stream);
		}
	}

}
