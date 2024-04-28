#include "debug_view.h"
#include <iostream>
#include <logging.h>
#include <stopwatch.h>

DebugView::DebugView(xcb_connection_t *conn, xcb_screen_t *scr, const WindowConfig &cfg, const Scene &scene) : Window(conn, scr, cfg, scene) {}

void DebugView::initialize()
{
    log(LL::INFO, "Initializing Debug View for Ray Tracer");
}

void DebugView::handleEvent(xcb_generic_event_t *event)
{
    Window::handleEvent(event); // Base class handling for common events
}

static double a = 0;
void DebugView::renderScreen()
{
    clearWindow();

    for (int y = 0; y < config.height; y++)
    {
        for (int x = 0; x < config.width; x++)
        {
            drawPixel(x, y,
                      (uint8_t)((x + a) * 255 / config.width),
                      (uint8_t)((y + a) * 255 / config.height),
                      0);
        }
    }
    a += 0.01; // Update 'a' to move points more visibly
}

void DebugView::drawLine(int x1, int y1, int x2, int y2, uint8_t R, uint8_t G, uint8_t B)
{
    // TODO: clip to window

    if (std::abs(x2 - x1) > std::abs(y2 - y1))
    {
        double coefficient = (y2 - y1) / (x2 - x1);
    }
}