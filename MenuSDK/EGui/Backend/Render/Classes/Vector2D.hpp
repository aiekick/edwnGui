#pragma once

struct Vec2 {
    float x, y;

    Vec2() { x = y = 0; }
    Vec2(float _x, float _y)
        : x(_x), y(_y) {}

    Vec2& operator+=(const Vec2& other) {
        x += other.x;
        y += other.x;
        return *this;
    }
    Vec2& operator-=(const Vec2& other) {
        x -= other.x;
        y -= other.x;
        return *this;
    }
    Vec2 operator+(const Vec2& other) {
        return Vec2(x + other.x, y + other.y);
    }

    Vec2 operator-(const Vec2& other) {
        return Vec2(x - other.x, y - other.y);
    }

    bool operator==(const Vec2& other) const {
        return x == other.x && y == other.y;
    }

    void clear() {
        x, y = NULL;
    }
};