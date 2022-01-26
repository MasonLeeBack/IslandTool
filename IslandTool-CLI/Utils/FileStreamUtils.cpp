/*

IslandTool
Copyright (c) 2022 Mason Lee Back

File name: FileStreamUtils.cpp

*/

#include <stdafx.h>

// This function gets the next string found in the file stream.
// It is used since .SI files do not specify string size and
// offsets text by a seemingly random value. (Investigate this!)
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

    return stringToGet;
}

void FixPadding(std::ifstream* stream)
{
    uint8_t character;
    stream->read((char*)&character, sizeof(uint8_t));

    if (character != 0x0) {
        stream->seekg(-1, std::ios_base::cur);
        return;
    }

    while (character == 0x0) {
        stream->read((char*)&character, sizeof(uint8_t));
    }

    stream->seekg(-1, std::ios_base::cur);
}
