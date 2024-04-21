#include "application.h"
#include <iostream>

int main()
{
    log(LL::INFO, "Starting Ray Tracer...");
    Application app(true);
    app.run();
    return 0;
}
