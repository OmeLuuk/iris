#pragma once

#include <xcb/xcb.h>
#include <iostream>
#include <stdlib.h>
#include "logging.h"
#include "window.h"

class RayTracer : public Window
{
public:
    RayTracer(xcb_connection_t *conn, xcb_screen_t *scr);
    void initialize();
    void update();

private:
    void mainEventLoop();
    void drawPixel(uint16_t x, uint16_t y, uint32_t color);
    xcb_gcontext_t createGraphicsContext(uint32_t color);

    bool running;
};
