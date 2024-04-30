#include "ray_tracer.h"

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
}

void RayTracer::refreshDebugLines()
{
    scene.debugRays.clear();
    double jumpSize = config.width / NUMBER_OF_DEBUG_LINES;

    for (double x = scene.canvas.p0.x; x <= scene.canvas.p1.x; x += jumpSize)
    {
        Vector3 destination = {x, (scene.canvas.p2.y + scene.canvas.p0.y), scene.canvas.p0.z};
        scene.debugRays.push_back({scene.camera, destination - scene.camera});
    }
}