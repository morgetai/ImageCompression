cmake_minimum_required(VERSION 3.20)

project(BMPCompression)

set(CMAKE_CXX_STANDARD 17)

option(BUILD_TEST "Build tests" OFF)
set(VCPKG_APPLOCAL_DEPS OFF)
set(CMAKE_AUTOMOC ON)

add_subdirectory(BMPCompression)
add_subdirectory(BMPCopressionApp)

if(BUILD_TEST)
    enable_testing()
    add_subdirectory(BMPCompression/test)
endif()
