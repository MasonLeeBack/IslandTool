/*

IslandTool
Copyright (c) 2022 Mason Lee Back

File name: LegoLoopingAnimPresenter.h

*/

#ifndef _LEGOLOOPINGANIMPRESENTER_H_
#define _LEGOLOOPINGANIMPRESENTER_H_

class LegoLoopingAnimPresenter : public MxSerializable {
public:
    static const char* MyClassName() { return "LegoLoopingAnimPresenter"; }
    virtual const char* GetClassName() { return MyClassName(); }
};

#endif // _LEGOLOOPINGANIMPRESENTER_H_
