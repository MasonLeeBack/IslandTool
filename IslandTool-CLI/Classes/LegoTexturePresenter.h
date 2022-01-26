/*

IslandTool
Copyright (c) 2022 Mason Lee Back

File name: LegoTexturePresenter.h

*/

#ifndef _LEGOTEXTUREPRESENTER_H_
#define _LEGOTEXTUREPRESENTER_H_

class LegoTexturePresenter : public MxSerializable {
public:
    static const char* MyClassName() { return "LegoTexturePresenter"; }
    virtual const char* GetClassName() { return MyClassName(); }
};

#endif // _LEGOTEXTUREPRESENTER_H_
