/*

IslandTool
Copyright (c) 2022 Mason Lee Back

File name: IslandToolSerializable.cpp

*/

#include <stdafx.h>

void IslandToolSerializable::PreUpdate(std::ofstream* stream)
{
    // If we have a previous handle, seek to the position
    // and store the old one.
    if (position != -1) {
        updatePosition = stream->tellp();
        stream->seekp(position, std::ios::beg);
    }
}

void IslandToolSerializable::PostUpdate(std::ofstream* stream)
{
    if (updatePosition != -1) {
        stream->seekp(updatePosition, std::ios::beg);
        updatePosition = -1;
    }
}
