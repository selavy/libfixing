cmake_minimum_required(VERSION 3.5)

find_package(Boost REQUIRED)

# TODO: make a FindCatch so this just looks like find_package(Catch)
# really should be a target
set(CATCH_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/third_party/catch")

add_subdirectory(tests)
