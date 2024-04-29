#include "application.h"
#include <iostream>
#include <stopwatch.h>

namespace
{
    // The window gets moved into the area not used for Ubuntu headers
    // Ie, as the side header is 70px wide, all x coordinates below 70 are mapped
    // to the same value, 0 as seen from the end of the side header
    // To spawn a window with an offset of 500 from the side, you need x = 570
    const WindowConfig rayTracerWindowConfig(800, 600, 0, 20);
    const WindowConfig debugViewWindowConfig(500, 500, 870, 20);
    constexpr int START_DISTANCE = 100;
}

Application::Application(bool isDebugMode)
{
    initialize(isDebugMode);
}

Application::~Application()
{
    Stopwatch::report();
}

void Application::initialize(bool isDebugMode)
{
    connection = xcb_connect(NULL, NULL); // Connect to the X server
    xcb_screen_iterator_t iter = xcb_setup_roots_iterator(xcb_get_setup(connection));
    screen = iter.data;

    scene = std::make_unique<Scene>();
    scene->canvas = Rectangle(
                        Vector3(-rayTracerWindowConfig.width / 2, -rayTracerWindowConfig.height / 2, START_DISTANCE),
                        Vector3(rayTracerWindowConfig.width / 2, -rayTracerWindowConfig.height / 2, START_DISTANCE),
                        Vector3(-rayTracerWindowConfig.width / 2, rayTracerWindowConfig.height / 2, START_DISTANCE),
                        Vector3(rayTracerWindowConfig.width / 2, rayTracerWindowConfig.height / 2, START_DISTANCE)
                        );

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
    bool running = true;
    int s = Stopwatch::start("Application.TotalDuration");

    while (running)
    {
        while ((event = xcb_poll_for_event(connection)))
        {
            for (auto& window : windows)
                window->handleEvent(event);

            if (event->response_type == XCB_KEY_PRESS)
                running = false;

            free(event);
        }

        for (auto& window : windows)
            window->draw();
    }

    for (auto &window : windows)
        window.reset();

    Stopwatch::stop("Application.TotalDuration", s);
}
