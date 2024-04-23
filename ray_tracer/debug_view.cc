#include "debug_view.h"
#include <iostream>
#include <logging.h>

DebugView::DebugView(xcb_connection_t *conn, xcb_screen_t *scr, const WindowConfig &cfg, const Scene &scene) : Window(conn, scr, cfg, scene) {}

void DebugView::initialize()
{
    log(LL::INFO, "Initializing Debug View for Ray Tracer");
    uint8_t depth = 32; // Full color depth with alpha
    uint8_t bpp = 32;   // Bits per pixel
    image = xcb_image_create(config.width, config.height,
                                          XCB_IMAGE_FORMAT_Z_PIXMAP,
                                          screen->root_depth, // this might also need to be 32 if screen->root_depth is not 32
                                          depth,
                                          bpp,
                                          32, // scanline pad, should match bpp
                                          XCB_IMAGE_ORDER_LSB_FIRST,
                                          XCB_IMAGE_ORDER_LSB_FIRST,
                                          NULL, // Base: NULL for data allocated by xcb_image_create
                                          config.width * config.height * (bpp / 8),
                                          new uint8_t[config.width * config.height * (bpp / 8)]);

    // Create pixmap for double buffering
    pixmap = xcb_generate_id(connection);
    xcb_create_pixmap(connection, screen->root_depth, pixmap, window, config.width, config.height);

    // Setup graphics context if needed
    gc = xcb_generate_id(connection);
    uint32_t values[2] = {screen->black_pixel, screen->white_pixel};
    xcb_create_gc(connection, gc, pixmap, XCB_GC_FOREGROUND | XCB_GC_BACKGROUND, values);
}

void DebugView::handleEvent(xcb_generic_event_t *event)
{
    Window::handleEvent(event); // Base class handling for common events
}

void DebugView::draw()
{
    static double a = 0.0;
    if (image != nullptr && image->data != nullptr)
    {
        for (int y = 0; y < config.height; y++)
        {
            for (int x = 0; x < config.width; x++)
            {
                int idx = (y * config.width + x) * 4;
                if (idx >= 0 && idx + 3 < config.width * config.height * 4)
                {                                                                    // Ensure idx+3 is within bounds
                    image->data[idx] = (uint8_t)((x + a) * 255 / config.width);      // Blue
                    image->data[idx + 1] = (uint8_t)((y + a) * 255 / config.height); // Green
                    image->data[idx + 2] = 0;                                        // Red
                    image->data[idx + 3] = 255;                                      // Alpha (fully opaque)
                }
            }
        }
        a += 0.01; // Update 'a' to move points more visibly
    }
    log(LL::INFO, "test");
    // Render the image to the pixmap
    xcb_image_put(connection, pixmap, gc, image, 0, 0, 0);

    // Copy the pixmap to the window
    xcb_copy_area(connection, pixmap, window, gc, 0, 0, 0, 0, config.width, config.height);
    xcb_flush(connection);
}
