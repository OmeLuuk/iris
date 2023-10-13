# Add the main executable
add_executable(iris_chat
    main.cc
    iris_chat.cc
)

# Link the main executable against the common library
target_link_libraries(iris_chat
    common
)
