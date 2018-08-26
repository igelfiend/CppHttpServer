cmake_minimum_required(VERSION 3.1)

project(HTTPServer)
add_executable(${PROJECT_NAME}
    "main.cpp"
    "httpserver.h"
    "httpserver.cpp"
    "httpserverprivate.h"
    "httpserverprivate.cpp"
    "utilities.h")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

if(WIN32)
  target_link_libraries(${PROJECT_NAME} wsock32 ws2_32)
endif()
