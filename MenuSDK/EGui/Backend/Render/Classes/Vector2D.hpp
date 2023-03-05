#pragma once

struct Vec2 {
    float x, y;

    Vec2() { x = y = 0; }
    Vec2(float _x, float _y) { x = _x; y = _y; }

    Vec2& operator+=(const Vec2& other)
    {
        x += other.x;
        y += other.x;
        return *this;
    }
    Vec2& operator-=(const Vec2& other)
    {
        x -= other.x;
        y -= other.x;
        return *this;
    }
    Vec2 operator+(const Vec2& other)
    {
        return Vec2(x + other.x, y + other.y);
    }

    Vec2 operator-(const Vec2& other)
    {
        return Vec2(x - other.x, y - other.y);
    }

    Vec2 operator*(const int& other) const {
        return Vec2(x * other, y * other);
    }

    Vec2 operator*(const float& other) const {
        return Vec2(x * other, y * other);
    }

    bool operator==(const Vec2& other) const {
        return x == other.x && y == other.y;
    }
};