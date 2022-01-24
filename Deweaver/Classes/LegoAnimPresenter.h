/*

Deweaver
Copyright (c) 2022 Mason Lee Back

File name: LegoAnimPresenter.h

*/

#ifndef _LEGOANIMPRESENTER_H_
#define _LEGOANIMPRESENTER_H_

class LegoAnimPresenter : public MxSerializable {
public: 
	static const char* MyClassName() { return "LegoAnimPresenter"; }
	virtual const char* GetClassName() { return MyClassName(); }
};

#endif // _LEGOANIMPRESENTER_H_
