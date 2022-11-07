cmake_minimum_required(VERSION 3.20)

project(BMPCompressionTest
    VERSION 1.0.0 
    LANGUAGES CXX)

set (PROJECT_SOURCES 
    test.cpp)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

add_test(NAME "${PROJECT_NAME}" COMMAND "${PROJECT_NAME}")

target_link_libraries(${PROJECT_NAME} PRIVATE BMPCompressionAlgorithm )
