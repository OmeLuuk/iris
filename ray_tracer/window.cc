#include "window.h"
#include <iostream>

Window::Window(xcb_connection_t *conn, xcb_screen_t *scr) : connection(conn), screen(scr), window(0), gc(0)
{
    createWindow();
    setupGraphicsContext(screen->white_pixel);
    clearWindow();
}

Window::~Window()
{
    if (gc)
    {
        xcb_free_gc(connection, gc);
    }
    if (window)
    {
        xcb_destroy_window(connection, window);
    }
}

void Window::createWindow()
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

void Window::setupGraphicsContext(uint32_t color)
{
    gc = xcb_generate_id(connection);
    uint32_t mask = XCB_GC_FOREGROUND;
    uint32_t values[1] = {color};
    xcb_create_gc(connection, gc, window, mask, values);
}

void Window::clearWindow()
{
    xcb_rectangle_t rect = {0, 0, width, height};
    xcb_poly_fill_rectangle(connection, window, gc, 1, &rect);
}
