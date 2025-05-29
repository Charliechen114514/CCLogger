function(add_test_executable test_name source_file)
    add_executable(${test_name} ${source_file})
    target_link_libraries(${test_name} PRIVATE cclogger)
    add_test(NAME Run${test_name}Tests COMMAND ${test_name})
endfunction()

enable_testing()

add_test_executable(test_queue test_queue.cpp)
add_test_executable(test_format test_format.cpp)
add_test_executable(test_logger test_logger.cpp)