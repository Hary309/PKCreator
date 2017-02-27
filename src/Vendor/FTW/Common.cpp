/*
*********************************************************************
* File          : Common.cpp
* Project		: FTW
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#include "Common.h"
#include <DxErr.h>
#include <stdio.h>

int (WINAPIV * __vsnprintf)(char *, size_t, const char*, va_list) = _vsnprintf;

const char * FTW::GetDXErrorMsg(HRESULT result)
{
	const unsigned bufferSize = 256;

	char localError[bufferSize];
	char *error = new char[bufferSize];

	sprintf_s(localError, "%s - %s", DXGetErrorString(result), DXGetErrorDescription(result));

	memcpy(error, localError, bufferSize);

	return error;
}
