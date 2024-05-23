#include "ray_tracer.h"
#include "geometry.h"
#include <unistd.h>

namespace
{
    constexpr int NUMBER_OF_DEBUG_LINES = 10;
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
            Color color = castRay(direction);
            drawPixel(x + scene.canvas.halfWidth, y + scene.canvas.halfHeight, color.r, color.g, color.b, color.a);
        }
    }
}

Color RayTracer::castRay(const Vector3 &direction)
{
    std::optional<Vector3> closestIntersection;
    std::optional<Sphere> closestHitSphere;
    for (const Sphere& sphere : scene.spheres)
    {
        const auto intersection = solveRaySphereIntersection(direction, sphere);

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

    float angleMultiplier = 0;
    for (const Light& light : scene.lights)
        angleMultiplier += angleIntensityMultiplier(closestHitSphere->getNormalAtPoint(*closestIntersection), light.position - *closestIntersection);
    return closestHitSphere->color * std::clamp(angleMultiplier, 0.0f, 1.0f);
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