# Add the main executable
add_executable(iris_server
    main.cc
    iris_server.cc
)

# Link the main executable against the common library
target_link_libraries(iris_server
    common
)
