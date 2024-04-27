#include "ray_tracer.h"

RayTracer::RayTracer(xcb_connection_t *conn, xcb_screen_t *scr, const WindowConfig &cfg, const Scene &scene) : Window(conn, scr, cfg, scene) {}

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
    clearWindow();
    for (int i = 0; i < 500; ++i)
    {
        // drawPixel(rand() % config.width, rand() % config.height, rand() % 0xFFFFFF);
    }
    flush();
}