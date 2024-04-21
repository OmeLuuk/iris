#pragma once
#include "ray_tracer.h"
#include "debug_view.h"

class Application
{
public:
    Application(bool debugMode);
    void run();

private:
    RayTracer rayTracer;
    DebugView *debugView;
    bool isDebugMode;

    void initialize();
    void mainEventLoop();
};
