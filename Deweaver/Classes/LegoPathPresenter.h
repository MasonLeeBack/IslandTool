/*

Deweaver
Copyright (c) 2022 Mason Lee Back

File name: LegoPathPresenter.h

*/

#ifndef _LEGOPATHPRESENTER_H_
#define _LEGOPATHPRESENTER_H_

class LegoPathPresenter : public MxSerializable {
public:
	static const char* MyClassName() { return "LegoPathPresenter"; }
	virtual const char* GetClassName() { return MyClassName(); }
};

#endif // _LEGOPATHPRESENTER_H_
