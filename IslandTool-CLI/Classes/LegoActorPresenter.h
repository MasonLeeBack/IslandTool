/*

IslandTool
Copyright (c) 2022 Mason Lee Back

File name: LegoActorPresenter.h

*/

#ifndef _LEGOACTORPRESENTER_H_
#define _LEGOACTORPRESENTER_H_

class LegoActorPresenter : public MxSerializable {
public:
    static const char* MyClassName() { return "LegoActorPresenter"; }
    virtual const char* GetClassName() { return MyClassName(); }
};

#endif // _LEGOACTORPRESENTER_H_
