#pragma once

#include <xcb/xcb.h>
#include "types.h"
#include "scene.h"

class Window
{
public:
    Window(xcb_connection_t *conn, xcb_screen_t *scr, const WindowConfig &config, const Scene &scene);
    virtual ~Window();

    void createWindow();
    void setupGraphicsContext(uint32_t background, uint32_t foreground);
    void clearWindow();
    virtual void initialize() {};
    virtual void handleEvent(xcb_generic_event_t *event);
    virtual void draw() {};
    xcb_window_t getWindow() const { return window; }

protected:
    void closeWindow();
    void drawPixel(uint16_t x, uint16_t y, uint32_t color);
    void flush();
    void refresh();

    xcb_connection_t *connection;
    xcb_screen_t *screen;
    xcb_window_t window;
    xcb_gcontext_t gc;
    xcb_pixmap_t pixmap;
    const WindowConfig &config;
    const Scene &scene;

private:
    void createPixmap();
};