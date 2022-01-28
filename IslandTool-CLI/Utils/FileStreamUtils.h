/*

IslandTool
Copyright (c) 2022 Mason Lee Back

File name: FileStreamUtils.h

*/

#ifndef _FILESTREAMUTILS_H_
#define _FILESTREAMUTILS_H_

char* GetNextString(std::ifstream* stream);
void FillCharacter(std::ofstream* outStream, uint8_t byte, size_t size);
void StoreName(std::ofstream* outStream, char* name);
void FixPadding(std::ifstream* stream);

#endif // _FILESTREAMUTILS_H_
