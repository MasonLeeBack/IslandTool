/*

Deweaver
Copyright (c) 2022 Mason Lee Back

File name: LegoModelPresenter.h

*/

#ifndef _LEGOMODELPRESENTER_H_
#define _LEGOMODELPRESENTER_H_

class LegoModelPresenter : public MxSerializable {
public:
	static const char* MyClassName() { return "LegoModelPresenter"; }
	virtual const char* GetClassName() { return MyClassName(); }
};

#endif // _LEGOMODELPRESENTER_H_
