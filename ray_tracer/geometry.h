#pragma once

#include "types.h"

inline bool solveRaySphereIntersection(const Vector3 &ray, const Sphere &sphere, Vector3 &intersection)
{
    double a = ray.x * ray.x + ray.y * ray.y + ray.z * ray.z;
    double b = -2 * sphere.center.x * ray.x - 2 * sphere.center.y * ray.y - 2 * sphere.center.z * ray.z;
    double D = b * b - 4 * a * sphere.c;
    if (D <= 0)
        return false;

    double l1 = (-b + sqrt(D)) / (2 * a);
    double l2 = (-b - sqrt(D)) / (2 * a);

    if (l1 < l2)
        intersection = {l1 * ray.x, l1 * ray.y, l1 * ray.z};
    else
        intersection = {l2 * ray.x, l2 * ray.y, l2 * ray.z};

    return true;
}