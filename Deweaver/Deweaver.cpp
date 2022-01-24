/*

Deweaver
Copyright (c) 2022 Mason Lee Back

File name: Deweaver.cpp

*/

#include "stdafx.h"

void PrintUsage()
{
	printf("Deweaver\nCopyright (c) 2022 Mason Lee Back\nUsage: deweaver <.SI|.MOD>");
}

int main(int argc, char* argv[])
{
	if (argc == 1 || argc > 2) {
		PrintUsage();
		return 1;
	}

	std::ifstream is(argv[1], std::ifstream::binary);
	
	// Right now we'll assume .MOD files
	if (is) {
		//Deweave_MOD(&is);
		Deweave_SI(&is);
		is.close();
	}

	return 0;
}