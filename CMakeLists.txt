cmake_minimum_required(VERSION 3.10)

project(Iris)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Include sub-directories
add_subdirectory(common)
add_subdirectory(src/server)
add_subdirectory(src/client)

# If you include other libraries in the future, link them here.
# For example, for Boost.Asio:
# find_package(Boost 1.70 REQUIRED)
# target_link_libraries(iris Boost::Boost)
