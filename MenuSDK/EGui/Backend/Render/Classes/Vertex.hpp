#pragma once
#pragma once

struct vertex
{
    float x, y, z, rhw;
    DWORD color;

    vertex() {}
    vertex(float x_, float y_, float z_, float rhw_, DWORD color_)
        : x(x_), y(y_), z(z_), rhw(rhw_), color(color_)
    {}
};