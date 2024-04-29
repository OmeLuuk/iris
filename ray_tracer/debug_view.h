#pragma once
#include <xcb/xcb.h>
#include "window.h"

class DebugView : public Window
{
public:
    DebugView(xcb_connection_t *conn, xcb_screen_t *scr, const WindowConfig &cfg, Scene &scene);
    void initialize() override;
    void handleEvent(xcb_generic_event_t *event) override;

private:
    void renderScreen() override;
    void drawLine(int x1, int y1, int x2, int y2, uint8_t R, uint8_t G, uint8_t B);
};
