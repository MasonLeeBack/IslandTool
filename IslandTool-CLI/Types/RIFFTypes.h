/*

IslandTool
Copyright (c) 2022 Mason Lee Back

File name: RIFFTTypes.h

*/

#ifndef _RIFFTYPES_H_
#define _RIFFTYPES_H_

#define RIFF_IDENTIFIER 'FFIR'

class RIFF : public IslandToolSerializable {
public:
    uint32_t    Identifier;
    uint32_t    ChunkSize;
    uint32_t    Format;

    RIFF();

    void Serialize(std::ofstream* stream);
    void Deserialize(std::ifstream* stream);
};

class Subchunk : public IslandToolSerializable {
public:
    uint32_t    Identifier;
    uint32_t    SubchunkSize;

    Subchunk();

    void Serialize(std::ofstream* stream);
    void Deserialize(std::ifstream* stream);
};

#endif // _RIFFTTYPES_H_
