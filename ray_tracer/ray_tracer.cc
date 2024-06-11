#include "ray_tracer.h"
#include "geometry.h"
#include <unistd.h>

namespace
{
    constexpr int NUMBER_OF_DEBUG_LINES = 10;
    constexpr int MAX_DEPTH = 5;
}

RayTracer::RayTracer(xcb_connection_t *conn, xcb_screen_t *scr, const WindowConfig &cfg, Scene &scene) : Window(conn, scr, cfg, scene) {}

void RayTracer::initialize()
{
    log(LL::INFO, "Initializing ray tracer...");
}

void RayTracer::handleEvent(xcb_generic_event_t *event)
{
    Window::handleEvent(event); // Base class handling for common events
}

void RayTracer::renderScreen()
{
    // add debug rays outside of normal loop to avoid extra checks in main pixel loop
    refreshDebugLines();

    for (int x = scene.canvas.p0.x; x < scene.canvas.p1.x; x++)
    {
        for (int y = scene.canvas.p0.y; y < scene.canvas.p2.y; y++)
        {
            Vector3 direction = {x, y, scene.canvas.p0.z};
            Color color = castPrimaryRay(direction);
            drawPixel(x + scene.canvas.halfWidth, y + scene.canvas.halfHeight, color.r, color.g, color.b, color.a);
        }
    }
}

Color RayTracer::castPrimaryRay(const Vector3 &direction)
{
    std::optional<Vector3> closestIntersection;
    std::optional<Sphere> closestHitSphere;
    for (const Sphere &sphere : scene.spheres)
    {
        auto intersection = solveRaySphereIntersection(direction, sphere);
        if (intersection && (!closestIntersection || intersection->z < closestIntersection->z))
        {
            closestIntersection = intersection;
            closestHitSphere = sphere;
        }
    }

    if (!closestIntersection)
    {
        return {0, 0, 0, 0}; // No intersection found
    }

    // Calculate the normal at the intersection point
    Vector3 normal = closestHitSphere->getNotNormalizedNormalAtPoint(*closestIntersection);

    // Calculate diffuse component
    float angleMultiplier = 0;
    for (const Light &light : scene.lights)
    {
        angleMultiplier += angleIntensityMultiplier(normal, light.position - *closestIntersection);
    }
    Color diffuseColor = closestHitSphere->material.color * std::clamp(angleMultiplier, 0.0f, 1.0f);

    // Calculate reflected component
    Vector3 reflectionDirection = reflectedVector(direction, normal);
    Ray secondaryRay = {*closestIntersection, reflectionDirection};
    Color reflectedColor = castSecondaryRay(secondaryRay, MAX_DEPTH);


    return reflectedColor * closestHitSphere->material.reflexivity;
    
    
    // Combine based on material properties
    float reflectivity = closestHitSphere->material.reflexivity;
    return diffuseColor * (1 - reflectivity) + reflectedColor * reflectivity;
}

Color RayTracer::castSecondaryRay(const Ray &ray, int depth)
{
    if (depth <= 0)
        return {0, 0, 0, 0};
    
    std::optional<Vector3> closestIntersection;
    std::optional<Sphere> closestHitSphere;
    for (const Sphere &sphere : scene.spheres)
    {
        const auto intersection = solveRaySphereIntersection(ray, sphere);

        bool betterIntersection = intersection &&
                                  (!closestIntersection || intersection->z < closestIntersection->z);

        if (betterIntersection)
        {
            closestIntersection = intersection;
            closestHitSphere = sphere;
        }
    }

    if (!closestIntersection)
        return {0, 0, 0, 0};

    Vector3 notNormalizedNormal = closestHitSphere->getNotNormalizedNormalAtPoint(*closestIntersection);

    float angleMultiplier = 0;
    for (const Light &light : scene.lights)
        angleMultiplier += angleIntensityMultiplier(notNormalizedNormal, light.position - *closestIntersection);

    Color diffuseColor = closestHitSphere->material.color * std::clamp(angleMultiplier, 0.0f, 1.0f);
return diffuseColor;
    Ray secondaryRay = {*closestIntersection, reflectedVector(ray.direction, notNormalizedNormal)};
    Color reflectedColor = castSecondaryRay(secondaryRay, depth - 1);

    float reflectivity = closestHitSphere->material.reflexivity;
    return diffuseColor * (1 - reflectivity) + reflectedColor * reflectivity;
}

void RayTracer::refreshDebugLines()
{
    scene.debugRays.clear();
    float jumpSize = config.width / NUMBER_OF_DEBUG_LINES;

    for (float x = scene.canvas.p0.x; x <= scene.canvas.p1.x; x += jumpSize)
    {
        Vector3 destination = {x, (scene.canvas.p2.y + scene.canvas.p0.y), scene.canvas.p0.z};
        scene.debugRays.push_back({scene.camera, destination - scene.camera});
    }
}