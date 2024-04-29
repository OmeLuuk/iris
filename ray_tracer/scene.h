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
};