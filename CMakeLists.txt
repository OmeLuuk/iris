cmake_minimum_required(VERSION 3.10)

project(Iris)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add the source files
add_executable(iris src/main.cc)

# If you include other libraries in the future, link them here.
# For example, for Boost.Asio:
# find_package(Boost 1.70 REQUIRED)
# target_link_libraries(iris Boost::Boost)
