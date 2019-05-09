add_executable(test_utils test_utils.cpp ../../src/utils.cpp)
target_link_libraries(test_utils fmt)
add_test(NAME test_utils COMMAND $<TARGET_FILE:test_utils>)
