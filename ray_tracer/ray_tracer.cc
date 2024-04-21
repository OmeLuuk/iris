#include "ray_tracer.h"

RayTracer::RayTracer(xcb_connection_t *conn, xcb_screen_t *scr) : window(0), gc(0), running(false) {}

void RayTracer::initialize()
{
    log(LL::INFO, "Initializing ray tracer...");
    connection = xcb_connect(NULL, NULL); // Connect to the X server
    xcb_screen_iterator_t iter = xcb_setup_roots_iterator(xcb_get_setup(connection));
    screen = iter.data;
    createWindow();
    setupGraphicsContext(); // Set up the reusable graphic context
    clearWindow();          // Initial clear
}

void RayTracer::update()
{
    if (!running)
    {
        running = true;
        mainEventLoop();
        running = false;
    }
}

void RayTracer::createWindow()
{
    window = xcb_generate_id(connection);
    uint32_t mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
    uint32_t values[2] = {screen->black_pixel, XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_KEY_PRESS};
    xcb_create_window(connection, XCB_COPY_FROM_PARENT, window, screen->root,
                      0, 0, width, height, 0, XCB_WINDOW_CLASS_INPUT_OUTPUT,
                      screen->root_visual, mask, values);
    xcb_map_window(connection, window);
    xcb_flush(connection);
}

void RayTracer::setupGraphicsContext()
{
    gc = createGraphicsContext(screen->white_pixel);
}

void RayTracer::mainEventLoop()
{
    xcb_generic_event_t *event;
    while ((event = xcb_poll_for_event(connection)))
    {
        switch (event->response_type & ~0x80)
        {
        case XCB_EXPOSE:
            clearWindow();
            break;
        case XCB_KEY_PRESS:
            xcb_free_gc(connection, gc); // Free the graphic context when done
            xcb_disconnect(connection);
            return; // Exit on key press
        }
        free(event);
    }
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

xcb_gcontext_t RayTracer::createGraphicsContext(uint32_t color)
{
    xcb_gcontext_t gc = xcb_generate_id(connection);
    uint32_t mask = XCB_GC_FOREGROUND;
    uint32_t values[1] = {color};
    xcb_create_gc(connection, gc, window, mask, values);
    return gc;
}

void RayTracer::clearWindow()
{
    xcb_rectangle_t rect = {0, 0, width, height};
    xcb_poly_fill_rectangle(connection, window, gc, 1, &rect);
}
