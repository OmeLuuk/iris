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
    std::vector<Sphere> spheres = {{ { 0, 0, 2000 }, {255, 0, 0, 255}, 1800 }};
    std::vector<Light> lights = {{{-300, -300, 100}, 1}};
};