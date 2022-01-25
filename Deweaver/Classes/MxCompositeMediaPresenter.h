/*

Deweaver
Copyright (c) 2022 Mason Lee Back

File name: MxCompositeMediaPresenter.h

*/

#ifndef _MXCOMPOSITEMEDIAPRESENTER_H_
#define _MXCOMPOSITEMEDIAPRESENTER_H_

class MxCompositeMediaPresenter : public MxSerializable {
public:
	static const char* MyClassName() { return "MxCompositeMediaPresenter"; }
	virtual const char* GetClassName() { return MyClassName(); }
};

#endif // _MXCOMPOSITEMEDIAPRESENTER_H_
