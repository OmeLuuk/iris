#pragma once

#include <cmath>
#include <cstdint>

struct WindowConfig
{
    int width = 0;
    int height = 0;
    int x;
    int y;
};

struct Color
{
    uint8_t r, g, b, a;
    Color operator*(const float other) const
    {
        return Color(other * r, other * g, other * b, a);
    }

    Color operator+(const Color &other) const
    {
        return Color(r + other.r, g + other.g, b + other.b, a + other.a);
    }
};

struct Material
{
    Color color;
    float reflexivity = 0.0f;
};

class Vector3
{
public:
    Vector3() : x(.0f), y(.0f), z(.0f),
                length(std::sqrt(.0f)),
                lengthSquared(.0f)
    {
    }
    Vector3(float x, float y, float z) : x(x), y(y), z(z),
                                         length(std::sqrt(x * x + y * y + z * z)),
                                         lengthSquared(x * x + y * y + z * z)
    {
    }
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

    Vector3 operator*(const float other) const
    {
        return Vector3(other * x, other * y, other * z);
    }

    bool operator==(const Vector3 &other) const
    {
        return std::abs(x - other.x) < 0.001 &&
               std::abs(y - other.y) < 0.001 &&
               std::abs(z - other.z) < 0.001;
    }

    inline float dot(const Vector3 &other) const
    {
        return x * other.x + y * other.y + z * other.z;
    }

    float x = .0f, y = .0f, z = .0f;
    float length = .0f, lengthSquared = .0f;
};

struct Hit
{
    const Vector3 &intersection;
    const Material &material;
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
        width = (p1 - p0).length;
        height = (p2 - p0).length;
        halfWidth = width / 2;
        halfHeight = height / 2;
    }

    Vector3 p0, p1, p2, p3; // top left, top right, bottom left, bottom right
    int width = 0, height = 0;
    int halfWidth = 0, halfHeight = 0;
};

class Sphere
{
public:
    Sphere(const Vector3 &center, const Material &material, const double r) : center(center), material(material), r(r)
    {
        c = center.x * center.x + center.y * center.y + center.z * center.z - r * r;
    }

    inline Vector3 getNotNormalizedNormalAtPoint(const Vector3 &point) const
    {
        return {point.x - center.x, point.y - center.y, point.z - center.z};
    }

    Vector3 center;
    Material material;
    float r = .0f;
    float c = .0f; // we can store the C for the ABC formula in the sphere as it doesn't depend on the ray
};

class Light
{
public:
    Light(Vector3 position, const float intensity) : position(position), intensity(intensity)
    {
    }

    Vector3 position;
    const float intensity = .0f;
};