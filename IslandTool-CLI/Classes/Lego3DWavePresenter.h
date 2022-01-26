/*

IslandTool
Copyright (c) 2022 Mason Lee Back

File name: Lego3DWavePresenter.h

*/

#ifndef _LEGO3DWAVEPRESENTER_H_
#define _LEGO3DWAVEPRESENTER_H_

class Lego3DWavePresenter : public MxSerializable {
public:
    static const char* MyClassName() { return "Lego3DWavePresenter"; }
    virtual const char* GetClassName() { return MyClassName(); }
};

#endif // _LEGO3DWAVEPRESENTER_H_
