# benchs/CMakeLists.txt

add_executable(bench-example example.cpp)
target_link_libraries(bench-example PUBLIC
    DefaultFlags
    libfixing
    benchmark
    )

add_executable(bench-fix_parsing fix_parsing.cpp)
target_link_libraries(bench-fix_parsing PUBLIC
    DefaultFlags
    libfixing
    benchmark
    )
