#pragma once

#include "types.h"
#include "logging.h"

namespace
{
    constexpr int ACOS_TABLE_PRECISION = 1000;                // Number of entries in the acos table
    std::vector<float> acos_table(ACOS_TABLE_PRECISION + 1);  // Lookup table for acos
}

inline void initialize_acos_table() // candidate to move to geometry.cc
{
    for (int i = 0; i <= ACOS_TABLE_PRECISION; ++i)
    {
        float dot_product = i * (1.0 / ACOS_TABLE_PRECISION);
        acos_table[i] = std::acos(dot_product);
    }

    for (int i = 0; i < 500; i++)
        log(LL::DEBUG, "--> " + std::to_string(i) + ", " + std::to_string(acos_table.at(i)));
}

inline float lookup_acos(float dot_product)
{
    int index = static_cast<int>(dot_product * ACOS_TABLE_PRECISION);
    for (int i = 0; i < 500; i++)
        log(LL::DEBUG, "--> " + std::to_string(i) + ", " + std::to_string(acos_table.at(i)));
    return acos_table[index];
}

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
log(LL::DEBUG, std::to_string(dotProduct*inverseSquareRoot));
return std::acos(dotProduct*inverseSquareRoot); 
    return lookup_acos(dotProduct * inverseSquareRoot);
}

inline bool solveRaySphereIntersection(const Vector3 &ray, const Sphere &sphere, Vector3 &intersection)
{
    float a = ray.x * ray.x + ray.y * ray.y + ray.z * ray.z;
    float b = -2 * sphere.center.x * ray.x - 2 * sphere.center.y * ray.y - 2 * sphere.center.z * ray.z;
    float D = b * b - 4 * a * sphere.c;
    if (D <= 0)
        return false;

    float l1 = (-b + sqrt(D)) / (2 * a);
    float l2 = (-b - sqrt(D)) / (2 * a);

    if (l1 < l2)
        intersection = {l1 * ray.x, l1 * ray.y, l1 * ray.z};
    else
        intersection = {l2 * ray.x, l2 * ray.y, l2 * ray.z};

    return true;
}