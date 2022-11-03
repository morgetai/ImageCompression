cmake_minimum_required(VERSION 3.20)

project(BMPCompressionAlgorithm
    VERSION 1.0.0 
    LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Core)

set (PROJECT_INCLUDES
    inc/CompressedBitmap.h)

set (PROJECT_SOURCES 
    src/CompressedBitmap.cpp)

set(PROJECT_FILES ${PROJECT_INCLUDES} ${PROJECT_SOURCES})

add_library(${PROJECT_NAME} STATIC ${PROJECT_FILES})

target_include_directories(${PROJECT_NAME} 
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/inc
    PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/src)
