#include "ray_tracer.h"
#include <iostream>

int main()
{
    std::cout << "Starting Ray Tracer..." << std::endl;
    RayTracer rt;
    rt.initialize();
    rt.run();
    return 0;
}
