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
    std::vector<Sphere> spheres = {{ { -1000, 0, 1300 }, {{255, 0, 0, 255}, 0.5f}, 1000 },
                                    {{ 1200, 0, 1200}, {{0, 255, 0, 255}, 0.0f}, 800}};
    std::vector<Light> lights = {{{-300, -300, 100}, 1}};
};