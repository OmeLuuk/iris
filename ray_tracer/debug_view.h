#pragma once
#include <xcb/xcb.h>
#include "window.h"

class DebugView : public Window
{
public:
    DebugView(xcb_connection_t *conn, xcb_screen_t *scr, const WindowConfig &cfg);
    void initialize() override;
    void handleEvent(xcb_generic_event_t *event) override;
    void draw() override; // Implement specific debug drawing methods
};
