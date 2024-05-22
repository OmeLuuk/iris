#pragma once

#include "types.h"
#include "logging.h"

#include <vector>
#include <optional>

class LookupTable
{
public:
    static void initialize();

    static inline float acos(float dot_product)
    {
        int index = static_cast<int>(dot_product * ACOS_TABLE_PRECISION);
        return acos_table[index];
    }

private:
    static constexpr int ACOS_TABLE_PRECISION = 1000;
    static std::vector<float> acos_table; // Declaration only.

    // Making constructors private to prevent instantiation
    LookupTable() = delete;
};

inline float Q_rsqrt(float number)
{
    long i;
    float x2, y;
    const float threehalfs = 1.5F;

    x2 = number * 0.5F;
    y = number;
    i = *(long *)&y;           // Evil floating point bit level hacking
    i = 0x5f3759df - (i >> 1); // What the...?
    y = *(float *)&i;
    y = y * (threehalfs - (x2 * y * y)); // 1st iteration

    return y;
}

inline float angleIntensityMultiplier(const Vector3 &normal, const Vector3 &lightDirection)
{
    float dotProduct = normal.dot(lightDirection);
    if (dotProduct < 0)
        return 0;

    float inverseSquareRoot = Q_rsqrt(normal.lengthSquared * lightDirection.lengthSquared);

    return LookupTable::acos(dotProduct * inverseSquareRoot);
}

inline std::optional<Vector3> solveRaySphereIntersection(const Vector3 &ray, const Sphere &sphere)
{
    float a = ray.x * ray.x + ray.y * ray.y + ray.z * ray.z;
    float b = -2 * sphere.center.x * ray.x - 2 * sphere.center.y * ray.y - 2 * sphere.center.z * ray.z;
    float D = b * b - 4 * a * sphere.c;
    if (D <= 0)
        return std::nullopt;

    float l1 = (-b + sqrt(D)) / (2 * a);
    float l2 = (-b - sqrt(D)) / (2 * a);

    if (l1 < l2)
        return std::optional<Vector3>({l1 * ray.x, l1 * ray.y, l1 * ray.z});
    else
        return std::optional<Vector3>({l2 * ray.x, l2 * ray.y, l2 * ray.z});
}