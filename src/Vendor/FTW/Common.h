/*
*********************************************************************
* File          : Common.h
* Project		: FTW
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#pragma once

#define THROW_EXCEPTION
#define ALLOC_CONSOLE

#include <Declspec.h>
#include <Windows.h>

namespace FTW
{
	// remember to delete[]
	extern "C" DECLSPEC const char *GetDXErrorMsg(HRESULT result);
}

