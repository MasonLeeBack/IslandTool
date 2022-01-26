/*

IslandTool
Copyright (c) 2022 Mason Lee Back

File name: LegoAnimMMPresenter.h

*/

#ifndef _LEGOANIMMMPRESENTER_H_
#define _LEGOANIMMMPRESENTER_H_

class LegoAnimMMPresenter : public MxSerializable {
public:
    static const char* MyClassName() { return "LegoAnimMMPresenter"; }
    virtual const char* GetClassName() { return MyClassName(); }
};

#endif // _LEGOANIMMMPRESENTER_H_
