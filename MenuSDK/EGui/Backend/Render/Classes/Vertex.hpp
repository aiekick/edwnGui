#pragma once
#include <windows.h>

struct vertex
{
    FLOAT x, y, z, rhw;
    DWORD color;
};

struct D3DTLVERTEX
{
	float fX;
	float fY;
	float fZ;
	float fRHW;
	D3DCOLOR Color;
	float fU;
	float fV;
};