add_library(libfixing INTERFACE)

target_include_directories(libfixing INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    )
