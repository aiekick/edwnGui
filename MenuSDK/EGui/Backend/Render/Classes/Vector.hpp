#pragma once

struct Vec {
    float x, y, z;

    Vec() { x = y = z = 0; }
    Vec(float _x, float _y, float _z)
        : x(_x), y(_y), z(_z) {}

    Vec& operator+=(const Vec& other) {
        x += other.x;
        y += other.x;
        z += other.x;
        return *this;
    }

    Vec& operator-=(const Vec& other) {
        x -= other.x;
        y -= other.x;
        z -= other.x;
        return *this;
    }

    Vec operator+(const Vec& other) {
        return Vec(x + other.x, y + other.y, z + other.z);
    }

    Vec operator-(const Vec& other) {
        return Vec(x - other.x, y - other.y, z - other.z);
    }

    bool operator==(const Vec& other) const {
        return x == other.x && y == other.y && z == other.z;
    }

    void clear() {
        x, y, z = NULL;
    }
};