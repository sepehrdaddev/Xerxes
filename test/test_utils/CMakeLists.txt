add_executable(test_utils test_utils.cpp ../../src/utils.h)

add_test(NAME test_utils COMMAND  $<TARGET_FILE:test_utils>)