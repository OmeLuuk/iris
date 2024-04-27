#pragma once

#include <xcb/xcb.h>
#include <xcb/xcb_image.h>
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
    virtual void draw();
    xcb_window_t getWindow() const { return window; }

protected:
    virtual void renderScreen() {};
    inline void drawPixel(int x, int y, uint8_t R, uint8_t G, uint8_t B, uint8_t A);
    void flush();
    void refresh();

    xcb_connection_t *connection;
    xcb_screen_t *screen;
    xcb_window_t window;
    xcb_gcontext_t gc;
    xcb_pixmap_t pixmap;
    xcb_image_t *image;
    const WindowConfig &config;
    const Scene &scene;

private:
    void createPixmap();
};

inline void Window::drawPixel(int x, int y, uint8_t R, uint8_t G, uint8_t B, uint8_t A)
{
    int idx = (y * config.width + x) * 4;
    if (idx >= 0 && idx + 3 < config.width * config.height * 4)
    {   // Ensure idx+3 is within bounds
        image->data[idx] = B;     // Blue
        image->data[idx + 1] = G; // Green
        image->data[idx + 2] = R; // Red
        image->data[idx + 3] = A; // Alpha
    }
}