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

inline std::optional<Vector3> solveRaySphereIntersection(const Vector3 &direction, const Sphere &sphere)
{
    float a = direction.x * direction.x + direction.y * direction.y + direction.z * direction.z;
    float b = -2 * (sphere.center.x * direction.x + sphere.center.y * direction.y + sphere.center.z * direction.z);
    float D = b * b - 4 * a * sphere.c;
    if (D <= 0)
        return std::nullopt;

    float l1 = (-b + std::sqrt(D)) / (2 * a);
    float l2 = (-b - std::sqrt(D)) / (2 * a);

    if (l1 < l2)
        return std::optional<Vector3>({l1 * direction.x, l1 * direction.y, l1 * direction.z});
    else
        return std::optional<Vector3>({l2 * direction.x, l2 * direction.y, l2 * direction.z});
}

inline std::optional<Vector3> solveRaySphereIntersection(const Ray &ray, const Sphere &sphere)
{
    Vector3 oc = ray.origin - sphere.center;

    float a = ray.direction.dot(ray.direction);
    float b = 2.0f * oc.dot(ray.direction);
    float c = oc.dot(oc) - sphere.r * sphere.r;

    float discriminant = b * b - 4 * a * c;
    if (discriminant <= 0)
        return std::nullopt;

    float sqrtD = std::sqrt(discriminant);
    float t1 = (-b - sqrtD) / (2 * a);
    float t2 = (-b + sqrtD) / (2 * a);

    float t = t1 < t2 ? t1 : t2;
    if (t < 0)
        t = t1 > t2 ? t1 : t2;
    if (t < 0)
        return std::nullopt;

    return std::optional<Vector3>({ray.origin.x + t * ray.direction.x,
                                   ray.origin.y + t * ray.direction.y,
                                   ray.origin.z + t * ray.direction.z});
}

inline Vector3 reflectedVector(const Vector3 &ray, const Vector3 &surfaceNormal)
{
    //r = d−2(d⋅n) n
    Vector3 normalizedSurfaceNormal = surfaceNormal * Q_rsqrt(surfaceNormal.lengthSquared);
    return ray - normalizedSurfaceNormal * 2 * ray.dot(normalizedSurfaceNormal);
}