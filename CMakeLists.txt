cmake_minimum_required(VERSION 3.5)

find_package(Boost REQUIRED)

add_library(WarningFlags INTERFACE)
target_compile_options(WarningFlags INTERFACE
    -Wall
    -Werror
    -Wpedantic
    )

add_library(PerformanceFlags INTERFACE)
target_compile_options(PerformanceFlags INTERFACE
    -march=native
    # TODO: remove me
    -std=gnu++1y
    )

add_library(DefaultFlags INTERFACE)
target_link_libraries(DefaultFlags INTERFACE
    WarningFlags
    PerformanceFlags
    )

# TODO: make a FindCatch so this just looks like find_package(Catch)
# TODO: make an INTERFACE target for Catch
set(CATCH_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/third_party/catch")

add_subdirectory(include)
add_subdirectory(tests)
