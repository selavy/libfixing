# benchs/CMakeLists.txt

add_executable(bench-example example.cpp)
target_link_libraries(bench-example PUBLIC
    DefaultFlags
    libfixing
    benchmark
    )
