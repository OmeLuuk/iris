# Add the iris_client library
add_library(iris_client_lib STATIC
    iris_client.cc
    iris_client.h
)

target_include_directories(iris_client_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Link the iris_client library against the common library
target_link_libraries(iris_client_lib
    common
)

# Add the main executable
add_executable(test_iris_client
    main.cc
)

# Link the main executable against the iris_client library
target_link_libraries(test_iris_client
    iris_client_lib
)
