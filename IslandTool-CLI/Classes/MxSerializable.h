/*

IslandTool
Copyright (c) 2022 Mason Lee Back

File name: MxSerializable.h

*/

#ifndef _MXSERIALIZABLE_H_
#define _MXSERIALIZABLE_H_

// This is the base class for any "serialized" classes
class MxSerializable {
public:
    static const char* MyClassName() { return "MxSerializable"; }
    virtual const char* GetClassName() { return MyClassName(); }
};

#endif // _MXSERIALIZABLE_H_
