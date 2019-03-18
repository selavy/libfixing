# tests/CMakeLists.txt

add_executable(test_tag2idx test_tag2idx.cpp)
target_link_libraries(test_tag2idx PUBLIC DefaultFlags)
target_link_libraries(test_tag2idx PUBLIC libfixing)
target_link_libraries(test_tag2idx PUBLIC Catch2)

add_executable(test_fix_parser test_fix_parser.cpp)
target_link_libraries(test_fix_parser PUBLIC DefaultFlags)
target_link_libraries(test_fix_parser PUBLIC libfixing)
target_link_libraries(test_fix_parser PUBLIC Catch2)

add_executable(test_gen_tag test_gen_tag.cpp)
target_link_libraries(test_gen_tag PUBLIC DefaultFlags)
target_link_libraries(test_gen_tag PUBLIC Catch2)
