#pragma once
#include <xcb/xcb.h>

class DebugView
{
public:
    DebugView(xcb_connection_t *conn, xcb_screen_t *scr);
    void initialize();
    void drawDebugInfo(); // Implement specific debug drawing methods

private:
    xcb_connection_t *connection;
    xcb_screen_t *screen;
    xcb_window_t window;
    xcb_gcontext_t gc;

    static const int width = 800;
    static const int height = 600;

    void createWindow();
    void clearWindow();
    void setupGraphicsContext();
    xcb_gcontext_t createGraphicsContext(uint32_t color);
};
