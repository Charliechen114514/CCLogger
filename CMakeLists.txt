cmake_minimum_required(VERSION 3.10.0)

# set as debug
set(CMAKE_BUILD_TYPE Debug)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(LoggerSystem VERSION 0.1.0 LANGUAGES C CXX)

set(QueueSrc cached_queue/logger_queue.cpp cached_queue/logger_queue.h)
set(CoreSrc core/logger_tools.cpp core/logger_tools.h)
set(FormatSrc format/logger_format.cpp format/logger_format.h)
set(IOSrc IO/io.h IO/fileio.h IO/stdio.h)
set(LoggerSrc logger/logger.cpp logger/logger.h)
add_library(cclogger STATIC ${QueueSrc} ${FormatSrc} ${CoreSrc} ${IOSrc} ${LoggerSrc})
target_include_directories(cclogger PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_BINARY_DIR})

add_executable(fast_main main.cpp)
target_link_libraries(fast_main PRIVATE cclogger)

add_subdirectory(test)
add_subdirectory(example)
