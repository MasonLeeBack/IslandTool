/*

IslandTool
Copyright (c) 2022 Mason Lee Back

File name: OmniTypes.cpp

*/

#include <stdafx.h>

Vector3::Vector3() : X(0.0), Y(0.0), Z(0.0) {}
Vector3::Vector3(double X, double Y) : X(X), Y(Y), Z(0.0) {}
Vector3::Vector3(double X, double Y, double Z) : X(X), Y(Y), Z(Z) {}

void Vector3::Serialize(std::ofstream* stream) {
    // Get the position if we ever need to update data later
    position = stream->tellp();
    
    // Write coordinates to working file
    stream->write((char*)&this->X, sizeof(double));
    stream->write((char*)&this->Y, sizeof(double));
    stream->write((char*)&this->Z, sizeof(double));
}

void Vector3::Serialize(tinyxml2::XMLElement* element)
{

}

void Vector3::Deserialize(std::ifstream* stream) {
    // Read coordinates from working file, this could be
    // simpler but that's okay
    stream->read((char*)&this->X, sizeof(double));
    stream->read((char*)&this->Y, sizeof(double));
    stream->read((char*)&this->Z, sizeof(double));
}

void Vector3::Deserialize(tinyxml2::XMLElement* element)
{

}
