#pragma once

#include "types.h"
#include <vector>

class Scene
{
public:
    Scene();
    
    Rectangle canvas;
    Vector3 camera;

    std::vector<Ray> debugRays;
    std::vector<Sphere> spheres = {{ { 0, 0, 300 }, {255, 0, 0, 255}, 100 }};
    std::vector<Light> lights = {{{-400, -400, 100}, 1}};
};