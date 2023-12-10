# Create a library for the common utilities
add_library(common STATIC
    config.cc
    logging.cc
    engine.cc
    base_connection_manager.cc
    server_connection_manager.cc
    client_connection_manager.cc
    types.cc
)

# Ensure the headers can be found
target_include_directories(common PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)