#pragma once
#include <xcb/xcb.h>
#include <xcb/xcb_image.h>
#include "window.h"

class DebugView : public Window
{
public:
    DebugView(xcb_connection_t *conn, xcb_screen_t *scr, const WindowConfig &cfg, const Scene &scene);
    void initialize() override;
    void handleEvent(xcb_generic_event_t *event) override;
    void draw() override;

private:
    void drawLine();
    xcb_image_t *image;
};
