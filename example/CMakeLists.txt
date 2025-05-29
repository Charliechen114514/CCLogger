function(example_creator example_name source_file)
    add_executable(${example_name} ${source_file})
    target_link_libraries(${example_name} PRIVATE cclogger)
endfunction()

example_creator(format_set format_set.cpp)
example_creator(example_usage example_usage.cpp)