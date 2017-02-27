/*
*********************************************************************
* File          : FTWMath.cpp
* Project		: FTW
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#include "FTWMath.h"

float FTW::PointDir(D3DXVECTOR2 pos1, D3DXVECTOR2 pos2)
{
	return float(atan2((pos1.y - pos2.y), (pos1.x - pos2.y)) * 180 / D3DX_PI + 180);
}

D3DXVECTOR2 FTW::LenDir(float len, float dir)
{
	D3DXVECTOR2 vec;

	vec.x = (float)cos(dir * D3DX_PI / 180) * len;
	vec.y = (float)-sin(dir * D3DX_PI / 180) * len;
	return vec;
}

float FTW::GetLen(D3DXVECTOR2 pos1, D3DXVECTOR2 pos2)
{
	float len = 0.f;

	float cx = 0.f, cy = 0.f;

	cx = pos2.x - pos1.x;
	cy = pos2.y - pos1.y;

	len = sqrt(cx*cx + cy*cy);

	return len;
}
