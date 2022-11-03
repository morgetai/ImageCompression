cmake_minimum_required(VERSION 3.20)

project(BMPCompression)

set(CMAKE_CXX_STANDARD 17)
#add_compile_options(-fsanitize=address)

option(BUILD_TEST "Build tests" ON)
set(VCPKG_APPLOCAL_DEPS OFF)
set(CMAKE_AUTOMOC ON)
add_subdirectory(BMPCompression)
add_subdirectory(BMPCompression/test)
add_subdirectory(BMPCopressionApp)
if(BUILD_TEST)
    #enable_testing()
    
endif()
