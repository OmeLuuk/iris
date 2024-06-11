#pragma once

#include <xcb/xcb.h>
#include <iostream>
#include <stdlib.h>
#include <logging.h>
#include "window.h"

class RayTracer : public Window
{
public:
    RayTracer(xcb_connection_t *conn, xcb_screen_t *scr, const WindowConfig &cfg, Scene &scene);
    void initialize() override;
    void handleEvent(xcb_generic_event_t *event) override;
    void renderScreen() override;

private:
    void refreshDebugLines();
    Color castPrimaryRay(const Vector3 &direction);
    Color castSecondaryRay(const Ray &ray, int depth);
};
