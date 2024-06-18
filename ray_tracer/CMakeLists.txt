cmake_minimum_required(VERSION 3.10)
project(RayTracer)

# Use pkg-config to find the XCB package
find_package(PkgConfig REQUIRED)
pkg_check_modules(XCB REQUIRED xcb xcb-icccm xcb-image)

# Assuming `common` is either a target already available or a directory
# with a CMakeLists.txt that you include or add_subdirectory on

# Include directories found by pkg-config
include_directories(${XCB_INCLUDE_DIRS})
# Optionally include common directory if it's not handled by its own CMakeLists.txt
include_directories(${PROJECT_SOURCE_DIR}/../common) # Adjust path as necessary

# Link directories found by pkg-config
link_directories(${XCB_LIBRARY_DIRS})

# Add the main executable
add_executable(ray_tracer
    main.cc
    ray_tracer.cc
    application.cc
    debug_view.cc
    window.cc
    scene.cc
    geometry.cc
)

# Add the linker flags
target_link_libraries(ray_tracer
    common # Assuming `common` is a target or a find_package() handled library
    ${XCB_LIBRARIES}
)

# Optionally, set the C++ standard to use
set_target_properties(ray_tracer PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

# Add a test executable
add_executable(test
    test.cc # Your test entry file
)

# Link the test executable similarly
target_link_libraries(test
    common # Same linkage as the main app
    ${XCB_LIBRARIES}
)

# Set C++ standard for the test executable
set_target_properties(test PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)
