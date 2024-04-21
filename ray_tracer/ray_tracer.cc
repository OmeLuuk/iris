#include "ray_tracer.h"

RayTracer::RayTracer(xcb_connection_t *conn, xcb_screen_t *scr) : Window(conn, scr) {}

void RayTracer::initialize()
{
    log(LL::INFO, "Initializing ray tracer...");
}

void RayTracer::handleEvent(xcb_generic_event_t *event)
{
    Window::handleEvent(event); // Base class handling for common events
}

void RayTracer::draw()
{
    clearWindow();
    for (int i = 0; i < 500; ++i)
    {
        drawPixel(rand() % width, rand() % height, rand() % 0xFFFFFF);
    }
    xcb_flush(connection);
}

void RayTracer::drawPixel(uint16_t x, uint16_t y, uint32_t color)
{
    xcb_change_gc(connection, gc, XCB_GC_FOREGROUND, &color);
    xcb_point_t point = {x, y};
    xcb_poly_point(connection, XCB_COORD_MODE_ORIGIN, window, gc, 1, &point);
}