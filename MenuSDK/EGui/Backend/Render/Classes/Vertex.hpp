#pragma once
#include <windows.h>

struct vertex
{
    float x, y, z, rhw;
    DWORD color;

    vertex()
        : x(0), y(0), z(0), rhw(0), color(0)
    {}

    vertex(float x_, float y_, float z_, float rhw_, DWORD color_)
        : x(x_), y(y_), z(z_), rhw(rhw_), color(color_)
    {}
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