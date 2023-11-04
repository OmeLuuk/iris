cmake_minimum_required(VERSION 3.10) # or whatever version you're using

project(IrisChat)

# This will make CMake automatically run moc, uic & rcc when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Add the main executable
add_executable(iris_chat
    main.cc
    iris_chat.cc
    iris_chat_gui.cc
)

# Find the necessary Qt5 modules
find_package(Qt5 COMPONENTS Core Widgets REQUIRED)

# Link the main executable against the common library and the Qt5 modules
target_link_libraries(iris_chat
    common
    Qt5::Widgets
    /lib/x86_64-linux-gnu/libpthread.so.0
)


# If you have any UI or resource files, add them to your executable
# qt5_add_resources(...) or qt5_wrap_ui(...)
