/*

Deweaver
Copyright (c) 2022 Mason Lee Back

File name: MxControlPresenter.h

*/

#ifndef _MXCONTROLPRESENTER_H_
#define _MXCONTROLPRESENTER_H_

class MxControlPresenter : public MxSerializable {
public:
	static const char* MyClassName() { return "MxControlPresenter"; }
	virtual const char* GetClassName() { return MyClassName(); }
};

#endif // _MXCONTROLPRESENTER_H_
