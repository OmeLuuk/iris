#include "debug_view.h"
#include <iostream>
#include <logging.h>
#include <stopwatch.h>

namespace
{
    constexpr double CANVAS_WIDTH_PROPORTION = 0.1; // proportion of the screen between camera_x and canvas_p0_x
    constexpr double CANVAS_HEIGHT_PROPORTION = 0.2; // proportion of the screen between camera_z and canvas_p0_z

    // Liang-Barsky function by Daniel White @ http://www.skytopia.com/project/articles/compsci/clipping.html
    // This function inputs 8 numbers, and outputs 4 new numbers (plus a boolean value to say whether the clipped line is drawn at all).
    //
    bool LiangBarsky(double edgeLeft, double edgeRight, double edgeBottom, double edgeTop, // Define the x/y clipping values for the border.
                     double x0src, double y0src, double x1src, double y1src,               // Define the start and end points of the line.
                     double &x0clip, double &y0clip, double &x1clip, double &y1clip)       // The output values, so declare these outside.
    {

        double t0 = 0.0;
        double t1 = 1.0;
        double xdelta = x1src - x0src;
        double ydelta = y1src - y0src;
        double p, q, r;

        for (int edge = 0; edge < 4; edge++)
        { // Traverse through left, right, bottom, top edges.
            if (edge == 0)
            {
                p = -xdelta;
                q = -(edgeLeft - x0src);
            }
            if (edge == 1)
            {
                p = xdelta;
                q = (edgeRight - x0src);
            }
            if (edge == 2)
            {
                p = -ydelta;
                q = -(edgeBottom - y0src);
            }
            if (edge == 3)
            {
                p = ydelta;
                q = (edgeTop - y0src);
            }
            r = q / p;
            if (p == 0 && q < 0)
                return false; // Don't draw line at all. (parallel line outside)

            if (p < 0)
            {
                if (r > t1)
                    return false; // Don't draw line at all.
                else if (r > t0)
                    t0 = r; // Line is clipped!
            }
            else if (p > 0)
            {
                if (r < t0)
                    return false; // Don't draw line at all.
                else if (r < t1)
                    t1 = r; // Line is clipped!
            }
        }

        x0clip = x0src + t0 * xdelta;
        y0clip = y0src + t0 * ydelta;
        x1clip = x0src + t1 * xdelta;
        y1clip = y0src + t1 * ydelta;

        return true; // (clipped) line is drawn
    }
}

DebugView::DebugView(xcb_connection_t *conn, xcb_screen_t *scr, const WindowConfig &cfg, Scene &scene) : Window(conn, scr, cfg, scene) {}

void DebugView::initialize()
{
    log(LL::INFO, "Initializing Debug View for Ray Tracer");
}

void DebugView::handleEvent(xcb_generic_event_t *event)
{
    Window::handleEvent(event); // Base class handling for common events
}

void DebugView::renderScreen()
{
    clearWindow();

    // scale things on screen such that the canvas fits well
    double scalex = CANVAS_WIDTH_PROPORTION * config.width / scene.canvas.p0.x;
    double scaley = CANVAS_HEIGHT_PROPORTION * config.height / scene.canvas.p0.z;
    const auto fx = [scalex, this](int x) -> int { return scalex * x + 0.5 * config.width; };
    const auto gz = [scaley, this](int z) -> int { return -scaley * z + config.height; };

    // draw camera
    drawPixel(fx(scene.camera.x), gz(scene.camera.z)-1, 255, 0, 0);

    // draw canvas
    drawLine(fx(scene.canvas.p0.x), gz(scene.canvas.p0.z), fx(scene.canvas.p1.x), gz(scene.canvas.p1.z), 255, 255, 255);
}

void DebugView::drawLine(int rawx1, int rawy1, int rawx2, int rawy2, uint8_t R, uint8_t G, uint8_t B)
{
    double x1 = 0, x2 = 0, y1 = 0, y2 = 0;
    bool lineNotOnScreen = !LiangBarsky(0, 0, config.width, config.height, rawx1, rawy1, rawx2, rawy2, x1, y1, x2, y2);

    if (lineNotOnScreen)
        return;
    log(LL::DEBUG, "sssss");
    if (std::abs(x2 - x1) > std::abs(y2 - y1))
    {
        double coefficient = (y2 - y1) / (x2 - x1); // we dont need to check for division by zero because in this flow |Dx| > |Dy|

        int xmin = std::min(x1, x2); // we need to walk the x distance from small x to big x
        int xmax = std::max(x1, x2);

        for (int x = xmin; x <= xmax; x++)
        {
            int y = static_cast<int>(y1 + (x - x1) * coefficient);
            drawPixel(x, y, R, G, B);
        }
    }
    else
    {
        double coefficient = (x2 - x1) / (y2 - y1);

        int ymin = std::min(y1, y2);
        int ymax = std::max(y1, y2);

        for (int y = ymin; y <= ymax; y++)
        {
            int x = static_cast<int>(x1 + (y - y1) * coefficient);
            drawPixel(x, y, R, G, B);
        }
    }
}