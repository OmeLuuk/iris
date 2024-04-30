#include "debug_view.h"
#include <iostream>
#include <logging.h>
#include <stopwatch.h>

namespace
{
    constexpr double CANVAS_WIDTH_PROPORTION = 0.1;  // proportion of the screen between camera_x and canvas_p0_x
    constexpr double CANVAS_HEIGHT_PROPORTION = 0.2; // proportion of the screen between camera_z and canvas_p0_z

    // Clipping function for lines (partly) outside of the screen https://www.geeksforgeeks.org/line-clipping-set-1-cohen-sutherland-algorithm/
    // Defining region codes
    const int INSIDE = 0; // 0000
    const int LEFT = 1;   // 0001
    const int RIGHT = 2;  // 0010
    const int BOTTOM = 4; // 0100
    const int TOP = 8;    // 1000

    // Function to compute region code for a point(x, y)
    int computeCode(const int x_min, const int y_min, const int x_max, const int y_max, double x, double y)
    {
        // initialized as being inside
        int code = INSIDE;

        if (x < x_min) // to the left of rectangle
            code |= LEFT;
        else if (x > x_max) // to the right of rectangle
            code |= RIGHT;
        if (y < y_min) // below the rectangle
            code |= BOTTOM;
        else if (y > y_max) // above the rectangle
            code |= TOP;

        return code;
    }

    // Implementing Cohen-Sutherland algorithm
    // Clipping a line from P1 = (x2, y2) to P2 = (x2, y2)
    bool cohenSutherlandClip(const int x_min, const int y_min,
                             const int x_max, const int y_max,
                             double x1, double y1,
                             double x2, double y2,
                             double &outx1, double &outy1,
                             double &outx2, double &outy2)
    {
        // Compute region codes for P1, P2
        int code1 = computeCode(x_min, y_min, x_max, y_max, x1, y1);
        int code2 = computeCode(x_min, y_min, x_max, y_max, x2, y2);

        // Initialize line as outside the rectangular window
        bool accept = false;

        while (true)
        {
            if ((code1 == 0) && (code2 == 0))
            {
                // If both endpoints lie within rectangle
                accept = true;
                break;
            }
            else if (code1 & code2)
            {
                // If both endpoints are outside rectangle,
                // in same region
                break;
            }
            else
            {
                // Some segment of line lies within the
                // rectangle
                int code_out;
                double x, y;

                // At least one endpoint is outside the
                // rectangle, pick it.
                if (code1 != 0)
                    code_out = code1;
                else
                    code_out = code2;

                // Find intersection point;
                // using formulas y = y1 + slope * (x - x1),
                // x = x1 + (1 / slope) * (y - y1)
                if (code_out & TOP)
                {
                    // point is above the clip rectangle
                    x = x1 + (x2 - x1) * (y_max - y1) / (y2 - y1);
                    y = y_max;
                }
                else if (code_out & BOTTOM)
                {
                    // point is below the rectangle
                    x = x1 + (x2 - x1) * (y_min - y1) / (y2 - y1);
                    y = y_min;
                }
                else if (code_out & RIGHT)
                {
                    // point is to the right of rectangle
                    y = y1 + (y2 - y1) * (x_max - x1) / (x2 - x1);
                    x = x_max;
                }
                else if (code_out & LEFT)
                {
                    // point is to the left of rectangle
                    y = y1 + (y2 - y1) * (x_min - x1) / (x2 - x1);
                    x = x_min;
                }

                // Now intersection point x, y is found
                // We replace point outside rectangle
                // by intersection point
                if (code_out == code1)
                {
                    x1 = x;
                    y1 = y;
                    code1 = computeCode(x_min, y_min, x_max, y_max, x1, y1);
                }
                else
                {
                    x2 = x;
                    y2 = y;
                    code2 = computeCode(x_min, y_min, x_max, y_max, x2, y2);
                }
            }
        }
        if (accept)
        {
            outx1 = x1;
            outy1 = y1;
            outx2 = x2;
            outy2 = y2;
        }

        return accept;
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
    const auto fx = [scalex, this](int x) -> int
    { return scalex * x + 0.5 * config.width; };
    const auto gz = [scaley, this](int z) -> int
    { return -scaley * z + config.height; };

    // draw debug rays
    double rayMultiplier = 2 / CANVAS_HEIGHT_PROPORTION;
    for (const Ray& ray : scene.debugRays)
        drawLine(fx(ray.origin.x), gz(ray.origin.z), fx(rayMultiplier * ray.direction.x), gz(rayMultiplier * ray.direction.z), 255, 255, 0);

    // draw camera
    drawPixel(fx(scene.camera.x), gz(scene.camera.z) - 1, 255, 0, 0);

    // draw canvas
    drawLine(fx(scene.canvas.p0.x), gz(scene.canvas.p0.z), fx(scene.canvas.p1.x), gz(scene.canvas.p1.z), 255, 255, 255);
}

void DebugView::drawLine(int rawx1, int rawy1, int rawx2, int rawy2, uint8_t R, uint8_t G, uint8_t B)
{
    double x1 = 0, x2 = 0, y1 = 0, y2 = 0;

    bool lineNotOnScreen = !cohenSutherlandClip(0, 0, config.width, config.height, rawx1, rawy1, rawx2, rawy2, x1, y1, x2, y2);

    if (lineNotOnScreen)
        return;

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