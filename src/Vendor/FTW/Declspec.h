/*
*********************************************************************
* File          : Declspec.h
* Project		: FTW
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#pragma once

#if defined DLL_EXPORT
#	define DECLSPEC __declspec(dllimport)
#else
#	define DECLSPEC __declspec(dllexport)
#endif
