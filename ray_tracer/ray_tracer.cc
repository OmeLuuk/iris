#include "ray_tracer.h"
#include <xcb/xcb.h>
#include <iostream>
#include <stdlib.h> // for memory allocation

void RayTracer::initialize()
{
    std::cout << "Initializing ray tracer..." << std::endl;
    // Initialization code here
}

void RayTracer::run()
{
    std::cout << "Running ray tracer..." << std::endl;

    /* Open the connection to the X server */
    xcb_connection_t *connection = xcb_connect(NULL, NULL);

    /* Get the first screen */
    const xcb_setup_t *setup = xcb_get_setup(connection);
    xcb_screen_iterator_t iter = xcb_setup_roots_iterator(setup);
    xcb_screen_t *screen = iter.data;

    /* Create the window */
    xcb_window_t window = xcb_generate_id(connection);
    uint32_t mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
    uint32_t values[2] = {screen->white_pixel, XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_KEY_PRESS};
    xcb_create_window(connection, XCB_COPY_FROM_PARENT, window, screen->root,
                      0, 0, 800, 600, 10, XCB_WINDOW_CLASS_INPUT_OUTPUT,
                      screen->root_visual, mask, values);

    /* Map the window on the screen */
    xcb_map_window(connection, window);
    xcb_flush(connection);

    /* Main event loop */
    xcb_generic_event_t *event;
    bool running = true;
    while ((event = xcb_wait_for_event(connection)) && running)
    {
        switch (event->response_type & ~0x80)
        {
        case XCB_EXPOSE:
        {
            // Draw a pixel at (10,10)
            xcb_gcontext_t foreground = xcb_generate_id(connection);
            uint32_t gc_values[2] = {screen->black_pixel, 0};
            xcb_create_gc(connection, foreground, window, XCB_GC_FOREGROUND, gc_values);
            xcb_point_t point = {10, 10};
            xcb_poly_point(connection, XCB_COORD_MODE_ORIGIN, window, foreground, 1, &point);
            xcb_flush(connection);
            break;
        }
        case XCB_KEY_PRESS:
            running = false; // Exit on key press
            break;
        }
        free(event);
    }

    /* Clean up */
    xcb_disconnect(connection);
}