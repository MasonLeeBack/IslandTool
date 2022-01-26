/*

IslandTool
Copyright (c) 2022 Mason Lee Back

File name: LegoActionControlPresenter.h

*/

#ifndef _LEGOACTIONCONTROLPRESENTER_H_
#define _LEGOACTIONCONTROLPRESENTER_H_

class LegoActionControlPresenter : public MxSerializable {
public:
    static const char* MyClassName() { return "LegoActionControlPresenter"; }
    virtual const char* GetClassName() { return MyClassName(); }
};

#endif // _LEGOACTIONCONTROLPRESENTER_H_
