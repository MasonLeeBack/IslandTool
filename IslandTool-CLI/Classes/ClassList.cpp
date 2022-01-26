/*

IslandTool
Copyright (c) 2022 Mason Lee Back

File name: ClassList.cpp

*/

#include <stdafx.h>

MxSerializable* classesList[] = {
    new LegoActorPresenter(),
    new LegoAnimPresenter(),
    new LegoEntityPresenter(),
    new LegoModelPresenter(),
    new LegoPathPresenter(),
    new LegoWorldPresenter(),
    new LegoTexturePresenter(),
    new LegoLoopingAnimPresenter(),
    new LegoAnimMMPresenter(),
    new Lego3DWavePresenter(),
    new LegoPhonemePresenter(),
    new LegoMeterPresenter(),
    new LegoActionControlPresenter(),
    new LegoLocomotionAnimPresenter(),
    new LegoLoadCacheSoundPresenter(),
    new MxCompositeMediaPresenter(),
    new MxControlPresenter()
};
