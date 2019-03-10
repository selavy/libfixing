add_library(libfixing INTERFACE)

target_include_directories(libfixing INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    )

# TODO: change this to target_compile_features
# TODO: make WarningFlags as target to link with
# target_compile_options(libfixing INTERFACE
#     -march=native
#     -std=gnu++1y
#     # -Wall
#     # -Werror
#     # -Wpedantic
#     )
