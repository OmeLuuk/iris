#pragma once
#include "ray_tracer.h"
#include "debug_view.h"

#include <vector>

class Application
{
public:
    Application(bool debugMode);
    void run();

private:
    std::vector<std::unique_ptr<Window>> windows;
    xcb_connection_t *connection;
    xcb_screen_t *screen;

    void initialize(bool isDebugMode);
    void mainEventLoop();
};
