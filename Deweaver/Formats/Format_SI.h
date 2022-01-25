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

#define MX_FLAG_CHUNK_ENDING 2
#define MX_FLAG_CHUNK_SPLIT  16

struct MxChunk {
	uint32_t   Signature; // Should be "MxCh"
	uint32_t   ChunkSize;
	uint16_t   Flags;
	uint32_t   StreamID;
	uint32_t   Milliseconds;

};

enum class ObjectDef : uint16_t {
	Unknown = 0,
	Anim = 3,
	Sound = 4,
	Linear = 6,
	Parallel = 7,
	Event = 8,
	Bitmap = 10,
	Object = 11,
};

#define MX_FLAG_DEFAULT        0b00100010 // This probably isn't the "default" state. but so far no idea what else it'd be.
#define MX_FLAG_TRANSPARENT    0b00001000
#define MX_FLAG_NO_PALETTE  
#define MX_FLAG_LOOP_CACHE     0b00000010
#define MX_FLAG_LOOP_STREAM    0b00000100

#define MX_DURATION_INDEFINITE 0xFFFFFFFF

// This isn't 100% accurate byte-size wise.
// Due to the nature of the text objects, it won't be.
struct MxObject {
	uint32_t  Signature;
	uint32_t  ObjectSize;
	ObjectDef ObjectType;
	char*     HandlerClass; // Can be NULL if not specified
	char*     Name;
	uint32_t  StreamID;
	uint8_t   Flags; // This *may* be a uint32_t, but since it only ever takes up one byte, I'm doing uint8_t
	uint8_t   Padding[3];
	uint32_t  Unknown; // This may be a variation of Duration, i've only seen it used on Lego3DWavePresenter classes.
	uint32_t  Duration;
	uint32_t  Loops;
	Vector3   Position;
	Vector3   Direction;
	Vector3   Up;
	uint16_t  ExtraData_Size; // Can be 0, if 0, ExtraData is NULL.
	char*     ExtraData;
	char*     FileName; // Will be NULL if ObjectType is Parallel or Linear
};

void Deweave_SI(std::ifstream* stream);

#endif // _FORMAT_SI_H_
