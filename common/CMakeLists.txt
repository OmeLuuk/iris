# Create a library for the common utilities
add_library(common STATIC
    config.cc
    logging.cc
    connection_handler.cc
)

# Ensure the headers can be found
target_include_directories(common PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
