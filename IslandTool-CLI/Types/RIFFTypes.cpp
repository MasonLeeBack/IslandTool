/*

IslandTool
Copyright (c) 2022 Mason Lee Back

File name: RIFFTypes.cpp

*/

#include <stdafx.h>

RIFF::RIFF() : Identifier('FFIR'), ChunkSize(0xFFFFFFFF), Format('LLUN') {}

void RIFF::Serialize(std::ofstream* stream) {
    PreUpdate(stream);
    position = stream->tellp();

    stream->write((char*)&Identifier, sizeof(uint32_t));
    stream->write((char*)&ChunkSize, sizeof(uint32_t));
    stream->write((char*)&Format, sizeof(uint32_t));

    PostUpdate(stream);
}

void RIFF::Deserialize(std::ifstream* stream) {
    stream->read((char*)&Identifier, sizeof(uint32_t));
    stream->read((char*)&ChunkSize, sizeof(uint32_t));
    stream->read((char*)&Format, sizeof(uint32_t));
}

Subchunk::Subchunk() : Identifier('LLUN'), SubchunkSize(0xFFFFFFFF) {}

void Subchunk::Serialize(std::ofstream* stream)
{
    PreUpdate(stream);
    position = stream->tellp();

    stream->write((char*)&Identifier, sizeof(uint32_t));
    stream->write((char*)&SubchunkSize, sizeof(uint32_t));

    PostUpdate(stream);
}

void Subchunk::Deserialize(std::ifstream* stream)
{
    stream->read((char*)&Identifier, sizeof(uint32_t));
    stream->read((char*)&SubchunkSize, sizeof(uint32_t));
}
