# Add the main executable
add_executable(prisma_producer
    main.cc
    prisma_producer.cc
)

# Link the main executable against the common library
target_link_libraries(prisma_producer
    common
    iris_client_lib
)
