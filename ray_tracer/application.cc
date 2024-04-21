#include "application.h"
#include <iostream>

Application::Application(bool debugMode) : isDebugMode(debugMode), debugView(nullptr)
{
    initialize();
}

void Application::initialize()
{
    rayTracer.initialize();
    if (isDebugMode)
    {
        debugView = new DebugView(rayTracer.getConnection(), rayTracer.getScreen());
        debugView->initialize();
    }
}

void Application::run()
{
    mainEventLoop();
}

void Application::mainEventLoop()
{
    while (true)
    {
        // Process events, update both views
        rayTracer.update();
        if (isDebugMode)
        {
            debugView->drawDebugInfo();
        }
    }
}
