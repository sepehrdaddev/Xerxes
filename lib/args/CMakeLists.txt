add_executable(test_args test.cxx)

add_test(NAME test_args COMMAND  $<TARGET_FILE:test_args>)