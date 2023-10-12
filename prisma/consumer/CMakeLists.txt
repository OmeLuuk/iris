# Add the main executable
add_executable(prisma_consumer
    main.cc
    prisma_consumer.cc
)

# Link the main executable against the common library
target_link_libraries(prisma_consumer
    common
    iris_client_lib
)
