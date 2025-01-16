#include "ray_tracer.h"
#include "geometry.h"
#include <unistd.h>

namespace
{
    constexpr int NUMBER_OF_DEBUG_LINES = 10;
    constexpr int MAX_DEPTH = 5;

    bool PRINT_TO_SCREEN = false;

    int frame = 0;
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

    log(LL::DEBUG, "frame: " + std::to_string(frame++));

    for (int x = scene.canvas.p0.x; x < scene.canvas.p1.x; x++)
    {
        for (int y = scene.canvas.p0.y; y < scene.canvas.p2.y; y++)
        {
            PRINT_TO_SCREEN = x == -100 && y == 0;
            
            Vector3 direction = {x, y, scene.canvas.p0.z};
            Color color = Color(castPrimaryRay(direction));//if(PRINT_TO_SCREEN)color = Color(255, 255, 255, 0);
            drawPixel(x + scene.canvas.halfWidth, y + scene.canvas.halfHeight, color.r, color.g, color.b, color.a);
        }
    }
}

HDColor RayTracer::castPrimaryRay(const Vector3 &direction)
{
    std::optional<Vector3> closestIntersection;
    Sphere *closestHitSphere = nullptr;
    for (Sphere &sphere : scene.spheres)
    {
        auto intersection = solveRaySphereIntersection(direction, sphere);
        if (intersection && (!closestIntersection || intersection->z < closestIntersection->z))
        {
            closestIntersection = intersection;
            closestHitSphere = &sphere;
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
    HDColor diffuseColor = closestHitSphere->material.color * std::clamp(angleMultiplier, 0.0f, 1.0f);

    // Calculate reflected component
    Vector3 reflectionDirection = reflectedVector(direction, normal);
    Ray secondaryRay = {*closestIntersection, reflectionDirection};

    std::vector<Sphere*> hits = {closestHitSphere};
    HDColor reflectedColor = castSecondaryRay(secondaryRay, MAX_DEPTH, hits);

    // Combine based on material properties
    float reflectivity = closestHitSphere->material.reflexivity;

    if (PRINT_TO_SCREEN)
    {
        log(LL::DEBUG, "refl: " + reflectedColor.toString());
        log(LL::DEBUG, "diff: " + diffuseColor.toString());
    }
    
    return diffuseColor * (1 - reflectivity) + reflectedColor * reflectivity;
}

HDColor RayTracer::castSecondaryRay(const Ray &ray, int depth, std::vector<Sphere*> &hitSpheres)
{
    if (depth <= 0)
        return {0, 0, 0, 0};
    
    std::optional<Vector3> closestIntersection;
    Sphere* closestHitSphere;
    for (Sphere &sphere : scene.spheres)
    {
        if (std::find(hitSpheres.begin(), hitSpheres.end(), &sphere) != hitSpheres.end())
            continue;

        const auto intersection = solveRaySphereIntersection(ray, sphere);

        bool betterIntersection = intersection &&
                                  (!closestIntersection || intersection->z < closestIntersection->z);

        if (betterIntersection)
        {
            closestIntersection = intersection;
            closestHitSphere = &sphere;
        }
    }

    if (!closestIntersection)
        return {0, 0, 0, 0};
if (PRINT_TO_SCREEN) log(LL::DEBUG, "inters: " + closestIntersection->toString());
    Vector3 notNormalizedNormal = closestHitSphere->getNotNormalizedNormalAtPoint(*closestIntersection);

    float angleMultiplier = 0;
    for (const Light &light : scene.lights)
        angleMultiplier += angleIntensityMultiplier(notNormalizedNormal, light.position - *closestIntersection);

    HDColor diffuseColor = closestHitSphere->material.color * std::clamp(angleMultiplier, 0.0f, 1.0f);

    Ray secondaryRay = {*closestIntersection, reflectedVector(ray.direction, notNormalizedNormal)};
    
    hitSpheres.push_back(closestHitSphere);
    HDColor reflectedColor = castSecondaryRay(secondaryRay, depth - 1, hitSpheres);

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