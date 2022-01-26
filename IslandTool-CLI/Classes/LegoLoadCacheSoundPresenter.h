/*

IslandTool
Copyright (c) 2022 Mason Lee Back

File name: LegoLoadCacheSoundPresenter.h

*/

#ifndef _LEGOLOADCACHESOUNDPRESENTER_H_
#define _LEGOLOADCACHESOUNDPRESENTER_H_

class LegoLoadCacheSoundPresenter : public MxSerializable {
public:
    static const char* MyClassName() { return "LegoLoadCacheSoundPresenter"; }
    virtual const char* GetClassName() { return MyClassName(); }
};

#endif // _LEGOLOADCACHESOUNDPRESENTER_H_
