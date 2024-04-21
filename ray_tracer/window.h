#pragma once

#include <xcb/xcb.h>

class Window
{
public:
    Window(xcb_connection_t *conn, xcb_screen_t *scr);
    ~Window();

    void createWindow();
    void setupGraphicsContext(uint32_t color);
    void clearWindow();
    virtual void initialize();
    virtual void handleEvent(xcb_generic_event_t *event);
    virtual void draw() {};
    xcb_window_t getWindow() const { return window; }
    xcb_gcontext_t getGraphicsContext() const { return gc; }

protected:
    void closeWindow();

    xcb_connection_t *connection;
    xcb_screen_t *screen;
    xcb_window_t window;
    xcb_gcontext_t gc;

    const int width = 800;
    const int height = 600;
};