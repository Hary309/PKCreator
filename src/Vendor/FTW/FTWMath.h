/*
*********************************************************************
* File          : FTWMath.h
* Project		: FTW
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#pragma once

#include <Declspec.h>

#include <cmath>
#include <d3dx9math.h>

namespace FTW
{
	extern "C" DECLSPEC float PointDir(D3DXVECTOR2 pos1, D3DXVECTOR2 pos2);
	
	template <typename T>
	DECLSPEC bool InRange(T value, T min, T max)
	{
		return value >= min && value <= max;
	}

	DECLSPEC D3DXVECTOR2 LenDir(float len, float dir);

	extern "C" DECLSPEC float GetLen(D3DXVECTOR2 pos1, D3DXVECTOR2 pos2);
}