/*

Deweaver
Copyright (c) 2022 Mason Lee Back

File name: LegoPhonemePresenter.h

*/

#ifndef _LEGOPHONEMEPRESENTER_H_
#define _LEGOPHONEMEPRESENTER_H_

class LegoPhonemePresenter : public MxSerializable {
public:
	static const char* MyClassName() { return "LegoPhonemePresenter"; }
	virtual const char* GetClassName() { return MyClassName(); }
};

#endif // _LEGOPHONEMEPRESENTER_H_
