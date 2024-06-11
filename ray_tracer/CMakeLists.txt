cmake_minimum_required(VERSION 3.10)
project(RayTracer)

# Use pkg-config to find the XCB package
find_package(PkgConfig REQUIRED)
pkg_check_modules(XCB REQUIRED xcb xcb-icccm xcb-image)

# Create a library from the common components
add_library(ray_tracer_lib STATIC
    ray_tracer.cc
    application.cc
    debug_view.cc
    window.cc
    scene.cc
    geometry.cc
)

# Ray tracer executable
add_executable(ray_tracer main.cc)
target_link_libraries(ray_tracer ray_tracer_lib common ${XCB_LIBRARIES})

# Test executable
add_executable(test test.cc)
target_link_libraries(test ray_tracer_lib common ${XCB_LIBRARIES})

# Include directories found by pkg-config
include_directories(${XCB_INCLUDE_DIRS})

# Link directories found by pkg-config
link_directories(${XCB_LIBRARY_DIRS})

# Optionally, set the C++ standard to use
set_target_properties(ray_tracer PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)
