/*

Deweaver
Copyright (c) 2022 Mason Lee Back

File name: LegoEntityPresenter.h

*/

#ifndef _LEGOENTITYPRESENTER_H_
#define _LEGOENTITYPRESENTER_H_

class LegoEntityPresenter : public MxSerializable {
public:
	static const char* MyClassName() { return "LegoEntityPresenter"; }
	virtual const char* GetClassName() { return MyClassName(); }
};

#endif // _LEGOENTITYPRESENTER_H_
