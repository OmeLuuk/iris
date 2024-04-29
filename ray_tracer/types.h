#pragma once

#include <cmath>

struct WindowConfig
{
    int width = 0;
    int height = 0;
    int x;
    int y;
};

class Vector3
{
public:
    double x, y, z;

    Vector3() : x(0), y(0), z(0) {}
    Vector3(double x, double y, double z) : x(x), y(y), z(z) {}
    Vector3(const Vector3 &other) = default;
    Vector3 &operator=(const Vector3 &other) = default;

    Vector3 operator+(const Vector3 &other) const
    {
        return Vector3(x + other.x, y + other.y, z + other.z);
    }

    Vector3 operator-(const Vector3 &other) const
    {
        return Vector3(x - other.x, y - other.y, z - other.z);
    }

    double length() const
    {
        return std::sqrt(x * x + y * y + z * z);
    }
};

struct Ray
{
    Vector3 origin;
    Vector3 direction;
};

class Rectangle
{
public:
    Rectangle(const Vector3 &p0, const Vector3 &p1, const Vector3 &p2, const Vector3 &p3)
        : p0(p0), p1(p1), p2(p2), p3(p3)
    {
        width = (p1 - p0).length();
        height = (p2 - p0).length();
    }

    Vector3 p0, p1, p2, p3; // top left, top right, bottom left, bottom right
    double width = 0, height = 0;
};