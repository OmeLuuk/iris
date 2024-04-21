#include "debug_view.h"
#include <iostream>

DebugView::DebugView(xcb_connection_t *conn, xcb_screen_t *scr) : connection(conn), screen(scr) {}

void DebugView::initialize()
{
    createWindow();
    setupGraphicsContext();
    clearWindow();
}

void DebugView::createWindow()
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

void DebugView::setupGraphicsContext()
{
    gc = createGraphicsContext(screen->white_pixel);
}

void DebugView::drawDebugInfo()
{
    // Drawing debug info on the window
}

void DebugView::clearWindow()
{
    // Clear debug window
}

xcb_gcontext_t DebugView::createGraphicsContext(uint32_t color)
{
    xcb_gcontext_t gc = xcb_generate_id(connection);
    uint32_t mask = XCB_GC_FOREGROUND;
    uint32_t values[1] = {color};
    xcb_create_gc(connection, gc, window, mask, values);
    return gc;
}