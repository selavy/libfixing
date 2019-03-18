cmake_minimum_required(VERSION 3.5)

find_package(Boost REQUIRED)

# TODO: is there a better way to do this?
# Flag Targets
add_library(WarningFlags INTERFACE)
target_compile_options(WarningFlags INTERFACE
    -Wall
    -Werror
    -Wpedantic
    )

add_library(PerformanceFlags INTERFACE)
target_compile_options(PerformanceFlags INTERFACE
    -march=native
    )

add_library(StandardFlags INTERFACE)
target_compile_options(StandardFlags INTERFACE
    -std=gnu++1y)

add_library(DefaultFlags INTERFACE)
target_link_libraries(DefaultFlags INTERFACE
    WarningFlags
    PerformanceFlags
    StandardFlags
    )

add_subdirectory(third_party)
add_subdirectory(include)
add_subdirectory(tests)
add_subdirectory(benchs)
