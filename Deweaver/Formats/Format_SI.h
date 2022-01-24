/*

Deweaver
Copyright (c) 2022 Mason Lee Back

File name: Format_SI.h

*/

#ifndef _FORMAT_SI_H_
#define _FORMAT_SI_H_

#define SI_VERSION_MAJOR 2
#define SI_VERSION_MINOR 2

struct RiffHeader {
	uint32_t   Signature; // Should always be "RIFF"
	uint32_t   Size;
	uint32_t   Subtype; // In the case of any Interleaf files, this will be "OMNI"
};

struct MxHeader {
	uint32_t   Signature; // Should be "MxHd"
	uint32_t   ChunkSize;
	uint16_t   MajorVersion;
	uint16_t   MinorVersion;
	uint32_t   BufferSize; // Should be in bytes, .SS files translate from Kibibytes
	uint32_t   BufferCount;
};

struct MxOffsets {
	uint32_t   Signature; // Should be "MxOf"
	uint32_t   ChunkSize;
	uint32_t   Unknown;
};

struct MxStream {
	uint32_t   Signature; // Should be "MxSt"
	uint32_t   StreamSize;
};

struct MxChunk {
	uint32_t   Signature; // Should be "MxCh"
	uint32_t   ChunkSize;
	uint16_t   Flags;
	uint32_t   Identifier;
};

enum class ObjectDef : uint16_t {
	Unknown = 0,
	Anim = 3,
	Sound = 4,
	EntryPoint = 6,
	Parallel = 7,
	Bitmap = 10,
	Object = 11,
};

struct MxObject {
	uint32_t  Signature;
	uint32_t  ObjectSize;
	ObjectDef ObjectType;
};

void Deweave_SI(std::ifstream* stream);

#endif // _FORMAT_SI_H_
