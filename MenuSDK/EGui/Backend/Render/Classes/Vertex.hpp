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

struct Vertex_t {
	Vec2 m_Position;
	Vec2 m_TexCoord;
	DWORD color;

	Vertex_t() { 
		m_Position = { 0, 0 }; m_TexCoord = { 0, 0 };
	}

	Vertex_t(const Vec2& pos, DWORD clr, const Vec2& coord = Vec2(0, 0)) {
		m_Position = pos;
		m_TexCoord = coord;
		color = clr;
	}
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