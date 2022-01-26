/*

IslandTool
Copyright (c) 2022 Mason Lee Back

File name: LegoLocomotionAnimPresenter.h

*/

#ifndef _LEGOLOCOMOTIONANIMPRESENTER_H_
#define _LEGOLOCOMOTIONANIMPRESENTER_H_

class LegoLocomotionAnimPresenter : public MxSerializable {
public:
    static const char* MyClassName() { return "LegoLocomotionAnimPresenter"; }
    virtual const char* GetClassName() { return MyClassName(); }
};

#endif // _LEGOLOCOMOTIONANIMPRESENTER_H_
