/*

IslandTool
Copyright (c) 2022 Mason Lee Back

File name: LegoMeterPresenter.h

*/

#ifndef _LEGOMETERPRESENTER_H_
#define _LEGOMETERPRESENTER_H_

class LegoMeterPresenter : public MxSerializable {
public:
    static const char* MyClassName() { return "LegoMeterPresenter"; }
    virtual const char* GetClassName() { return MyClassName(); }
};

#endif // _LEGOMETERPRESENTER_H_
