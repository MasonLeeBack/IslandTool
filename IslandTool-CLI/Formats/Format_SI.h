/*

IslandTool
Copyright (c) 2022 Mason Lee Back

File name: Format_SI.h

*/

#ifndef _FORMAT_SI_H_
#define _FORMAT_SI_H_

#define SI_VERSION_MAJOR 2
#define SI_VERSION_MINOR 2

// MxChunk flags
#define MX_FLAG_CHUNK_ENDING 2
#define MX_FLAG_CHUNK_SPLIT  16

// MxObject flags
#define MX_FLAG_TRANSPARENT    0b00001000
#define MX_FLAG_LOOP_NONE      0b00000010
#define MX_FLAG_LOOP_CACHE     0b00000001
#define MX_FLAG_LOOP_STREAM    0b00000100
#define MX_FLAG_UNKNOWN        0b00100000

#define MX_FLAG_DEFAULT        MX_FLAG_UNKNOWN | MX_FLAG_LOOP_NONE



// Subchunk tags
#define MXHEADER_TAG 'dHxM'
#define MXOFFSETS_TAG 'fOxM'
#define MXSTREAM_TAG 'tSxM'
#define MXCHUNK_TAG 'hCxM'
#define MXOBJECT_TAG 'bOxM'
#define MXLIST_TAG 'TSIL'

struct SubchunkHeader {
    uint32_t  Signature;
    uint32_t  ChunkSize;
};

struct MxHeader {
    uint32_t   Signature;       // "MxHd"
    uint32_t   ChunkSize;
    uint16_t   MajorVersion;
    uint16_t   MinorVersion;
    uint32_t   BufferSize;      // Buffer size in bytes. This is where chunks are "split".
    uint32_t   BufferCount;     // Number of buffers? Not sure how this is applied.
};

struct MxOffsets {
    uint32_t   Signature;       // "MxOf"
    uint32_t   ChunkSize;
    uint32_t   NumOfObjects;    // Total number of objects defined in the .SI file
};

struct MxStream {
    uint32_t   Signature;       // "MxSt"
    uint32_t   StreamSize;
};

#include <pshpack1.h>
struct MxChunk {
    uint32_t   Signature;       // "MxCh"
    uint32_t   ChunkSize;
    uint16_t   Flags;
    uint32_t   StreamID;
    int32_t    Milliseconds;
    uint32_t   ChunkDataSize;
};
#include <poppack.h>

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

static const char* ObjectDefToString(ObjectDef object)
{
    switch (object) {
    case ObjectDef::Anim:
        return "Anim";
        break;
    case ObjectDef::Sound:
        return "Sound";
        break;
    case ObjectDef::Linear:
        return "Linear";
        break;
    case ObjectDef::Parallel:
        return "Parallel";
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

static ObjectDef StringToObjectDef(const char* name)
{
    if (name == "Anim") {
        return ObjectDef::Anim;
    }
    if (name == "Sound") {
        return ObjectDef::Sound;
    }
    if (name == "Linear") {
        return ObjectDef::Linear;
    }
    if (name == "Parallel") {
        return ObjectDef::Parallel;
    }
    if (name == "Event") {
        return ObjectDef::Event;
    }
    if (name == "Bitmap") {
        return ObjectDef::Bitmap;
    }
    if (name == "Object") {
        return ObjectDef::Object;
    }
    return ObjectDef::Unknown;
}

#define MX_DURATION_INDEFINITE 0xFFFFFFFF

// MxObject is very flexible size wise.
struct MxObject {
    uint32_t  Signature;        // "MxOb"
    uint32_t  ObjectSize;       // When this is Linear or Parallel, it also accounts for children's sizes.
    ObjectDef ObjectType;       // See ObjectDef
    char*     HandlerClass;     // Can be NULL if not specified
    char*     Name;             // Name of the object
    uint32_t  StreamID;         // Object's stream ID, used to differentiate between chunks.
    uint8_t   Flags;            // See MxObject flags above
    uint8_t   Padding[3];
    uint32_t  Unknown;          // This may be a variation of Duration, I've only seen it used on Lego3DWavePresenter classes.
    uint32_t  Duration;         // Duration in milliseconds * Loops
    uint32_t  Loops;            // Defaults to 1
    Vector3   Position;         // Defaults to Vector3(0, 0, 0)
    Vector3   Direction;        // Defaults to Vector3(0, 0, 1)
    Vector3   Up;               // Defaults to Vector3(0, 1, 0)
    uint16_t  ExtraData_Size;   // If size is 0, ExtraData is NULL.
    char*     ExtraData;
    char*     FileName;         // Will be NULL if ObjectType is Parallel or Linear

    // File attributes
    uint32_t  Unknown2[3];
    uint32_t  FileFormat;
    uint32_t  Unknown3[2];

    uint32_t  ExtraData_2;      // Extra data for WAV files specifically

    // Not part of MxObject structure!!
    int       Offset;           // Offset in file
    bool      Weaved = false;   // If this is a root object, this is TRUE.

    uint32_t  ChildrenCount = 0;
    MxObject* Children;
};

// The chunk size REALLY likes to be divisible by 2.
// So much so, that it adds random bytes to the end of 
// other structures.
struct MxList {
    uint32_t  Signature; // "LIST"
    uint32_t  ChunkSize;
    uint32_t  ChunkSignature;
    uint32_t  NumberOfObjects;
};

void Deweave_SI(std::ifstream* stream, char* outDestination);
void Weave_SI(std::ofstream* outStream, char* inputFile);

#endif // _FORMAT_SI_H_
