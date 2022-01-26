/*

IslandTool
Copyright (c) 2022 Mason Lee Back

File name: OmniTypes.h

*/

#ifndef _OMNITYPES_H_
#define _OMNITYPES_H_

struct Vector3 {
    double X;
    double Y;
    double Z;

    bool operator==(const Vector3& vec) {
        return X == vec.X && Y == vec.Y && Z == vec.Z;
    }

    bool operator!=(const Vector3& vec) {
        if (X != vec.X || Y != vec.Y || Z != vec.Z) {
            return true;
        }
        return false;
    }
};

#endif // _OMNITYPES_H_
