# Add the main executable
add_executable(iris
    main.cc
)

# Link the main executable against the common library
target_link_libraries(iris
    common
)
