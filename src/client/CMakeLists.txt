# Add the main executable
add_executable(iris_client
    main.cc
    iris_client.h
    iris_client.cc
)

# Link the main executable against the common library
target_link_libraries(iris_client
    common
)
