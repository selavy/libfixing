cmake_minimum_required(VERSION 3.5)

find_package(Boost REQUIRED)

# TODO: make a FindCatch so this just looks like find_package(Catch)
# TODO: make an INTERFACE target for Catch
set(CATCH_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/third_party/catch")

# TODO: make an INTERFACE target for libfixing
# add_subdirectory(include)
set(LIBFIXING_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")

add_subdirectory(tests)
