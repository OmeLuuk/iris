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
    void initialize() override;
    void handleEvent(xcb_generic_event_t *event) override;
    void draw() override;

private:
    void drawPixel(uint16_t x, uint16_t y, uint32_t color);
};
