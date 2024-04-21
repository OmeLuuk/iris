#pragma once

struct WindowConfig
{
    int width = 0;
    int height = 0;
    int x;
    int y;
};

#include <cmath> // For std::sqrt

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

class Rectangle
{
public:
    Rectangle(const Vector3 &p0, const Vector3 &p1, const Vector3 &p2, const Vector3 &p3)
        : p0(p0), p1(p1), p2(p2), p3(p3)
    {
        width = (p1 - p0).length();
        height = (p2 - p0).length();
    }

    Vector3 p0, p1, p2, p3;
    double width = 0, height = 0;
};