#include "scene.h"

Scene::Scene()
    : camera(Vector3(0, 0, 0)),                    // Position the camera at the origin
      canvas(Rectangle(Vector3(-500, -400, 1000),  // p0
                       Vector3(500, -400, 1000),   // p1
                       Vector3(500, 400, 1000),    // p2
                       Vector3(-500, 400, 1000))) // p3
{
}