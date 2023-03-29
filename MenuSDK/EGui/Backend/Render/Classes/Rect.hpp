#pragma once

struct Rect {
    float x, y, w, h;

    Rect() { x = y = w = 0; h = 0; }
    Rect(float _x, float _y, float _w, float _h)
        : x(_x), y(_y), w(_w), h(_h) {}

    Rect& operator+=(const Rect& other) {
        x += other.x;
        y += other.x;
        w += other.x;
        h += other.h;
        return *this;
    }

    Rect& operator-=(const Rect& other) {
        x -= other.x;
        y -= other.x;
        w -= other.x;
        h -= other.h;
        return *this;
    }

    Rect operator+(const Rect& other) {
        return Rect(x + other.x, y + other.y, w + other.w, other.h + other.h);
    }

    Rect operator-(const Rect& other) {
        return Rect(x - other.x, y - other.y, w - other.w, h - other.h);
    }

    bool operator==(const Rect& other) const {
        return x == other.x && y == other.y && w == other.w && h == other.h;
    }

    void clear() {
        x, y, w, h = NULL;
    }
};