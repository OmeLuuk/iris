#include "application.h"
#include <iostream>

namespace
{
    // The window gets moved into the area not used for Ubuntu headers
    // Ie, as the side header is 70px wide, all x coordinates below 70 are mapped
    // to the same value, 0 as seen from the end of the side header
    // To spawn a window with an offset of 500 from the side, you need x = 570
    WindowConfig rayTracerWindowConfig(800, 600, 0, 20);
    WindowConfig debugViewWindowConfig(500, 500, 870, 20);
}

Application::Application(bool isDebugMode)
{
    initialize(isDebugMode);
}

void Application::initialize(bool isDebugMode)
{
    connection = xcb_connect(NULL, NULL); // Connect to the X server
    xcb_screen_iterator_t iter = xcb_setup_roots_iterator(xcb_get_setup(connection));
    screen = iter.data;

    scene = std::make_unique<Scene>();

    windows.emplace_back(std::make_unique<RayTracer>(connection, screen, rayTracerWindowConfig, *scene));
    if (isDebugMode) windows.emplace_back(std::make_unique<DebugView>(connection, screen, debugViewWindowConfig, *scene));
    
    for (auto& window : windows)
        window->initialize();
}

void Application::run()
{
    mainEventLoop();
}

void Application::mainEventLoop()
{
    xcb_generic_event_t *event;
    while (true)
    {
        while ((event = xcb_poll_for_event(connection)))
        {
            for (auto& window : windows)
                window->handleEvent(event);
            free(event);
        }

        for (auto& window : windows)
            window->draw();
    }
}
