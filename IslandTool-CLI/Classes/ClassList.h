/*

IslandTool
Copyright (c) 2022 Mason Lee Back

File name: ClassList.h

*/

#ifndef _CLASSLIST_H_
#define _CLASSLIST_H_

// MxSerializable should always be on top
#include "MxSerializable.h"

#include "Lego3DWavePresenter.h"
#include "LegoActionControlPresenter.h"
#include "LegoActorPresenter.h"
#include "LegoAnimMMPresenter.h"
#include "LegoAnimPresenter.h"
#include "LegoEntityPresenter.h"
#include "LegoLoadCacheSoundPresenter.h"
#include "LegoLocomotionAnimPresenter.h"
#include "LegoLoopingAnimPresenter.h"
#include "LegoMeterPresenter.h"
#include "LegoModelPresenter.h"
#include "LegoPathPresenter.h"
#include "LegoPhonemePresenter.h"
#include "LegoTexturePresenter.h"
#include "LegoWorldPresenter.h"

#include "MxCompositeMediaPresenter.h"
#include "MxControlPresenter.h"

#define ClassListSize 17
extern MxSerializable* classesList[];

#endif // _CLASSLIST_H_
