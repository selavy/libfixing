# tests/CMakeLists.txt

add_executable(test_tag2idx test_tag2idx.cpp)
target_compile_options(test_tag2idx PRIVATE -march=native)
target_compile_options(test_tag2idx PRIVATE -std=c++1y)
target_include_directories(test_tag2idx PRIVATE ${CATCH_INCLUDE_DIR})
target_include_directories(test_tag2idx PRIVATE ${LIBFIXING_INCLUDE_DIR})

add_executable(test_fix_parser test_fix_parser.cpp)
target_compile_options(test_fix_parser PRIVATE -march=native)
target_compile_options(test_fix_parser PRIVATE -std=c++1y)
target_include_directories(test_fix_parser PRIVATE ${CATCH_INCLUDE_DIR})
target_include_directories(test_fix_parser PRIVATE ${LIBFIXING_INCLUDE_DIR})
