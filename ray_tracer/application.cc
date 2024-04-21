#include "application.h"
#include <iostream>

Application::Application(bool isDebugMode)
{
    initialize(isDebugMode);
}

void Application::initialize(bool isDebugMode)
{
    connection = xcb_connect(NULL, NULL); // Connect to the X server
    xcb_screen_iterator_t iter = xcb_setup_roots_iterator(xcb_get_setup(connection));
    screen = iter.data;

    windows.emplace_back(std::make_unique<RayTracer>(connection, screen));
    if (isDebugMode) windows.emplace_back(std::make_unique<DebugView>(connection, screen));
    
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
