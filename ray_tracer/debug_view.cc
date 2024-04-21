#include "debug_view.h"
#include <iostream>
#include <logging.h>

DebugView::DebugView(xcb_connection_t *conn, xcb_screen_t *scr, const WindowConfig &cfg) : Window(conn, scr, cfg) {}

void DebugView::initialize()
{
    log(LL::INFO, "Initializing Debug View for Ray Tracer");
}

void DebugView::handleEvent(xcb_generic_event_t *event)
{
    Window::handleEvent(event); // Base class handling for common events
}

void DebugView::draw()
{
    // Drawing debug info on the window
}