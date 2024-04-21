cmake_minimum_required(VERSION 3.10)
project(RayTracer)

# Use pkg-config to find the XCB package
find_package(PkgConfig REQUIRED)
pkg_check_modules(XCB REQUIRED xcb)

# Add the main executable
add_executable(ray_tracer
    main.cc
    ray_tracer.cc
    application.cc
    debug_view.cc
    window.cc
)

# Include directories found by pkg-config
include_directories(${XCB_INCLUDE_DIRS})

# Link directories found by pkg-config
link_directories(${XCB_LIBRARY_DIRS})

# Add the linker flags
target_link_libraries(ray_tracer
    common
    ${XCB_LIBRARIES}
)

# Optionally, set the C++ standard to use
set_target_properties(ray_tracer PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)
