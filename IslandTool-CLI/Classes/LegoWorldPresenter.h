/*

IslandTool
Copyright (c) 2022 Mason Lee Back

File name: LegoWorldPresenter.h

*/

#ifndef _LEGOWORLDPRESENTER_H_
#define _LEGOWORLDPRESENTER_H_

class LegoWorldPresenter : public MxSerializable {
public:
    static const char* MyClassName() { return "LegoWorldPresenter"; }
    virtual const char* GetClassName() { return MyClassName(); }
};

#endif // _LEGOWORLDPRESENTER_H_
