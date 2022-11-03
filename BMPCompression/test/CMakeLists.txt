cmake_minimum_required(VERSION 3.20)

project(BMPCompressionTest
    VERSION 1.0.0 
    LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Core Quick Qml)

set (PROJECT_SOURCES 
    test.cpp)



add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

get_target_property(QT_TARGET_TYPE Qt6::Core TYPE)
if(${QT_TARGET_TYPE} STREQUAL "STATIC_LIBRARY")
    find_package(Qt6QmlImportScanner REQUIRED)
    qt6_import_qml_plugins(${PROJECT_NAME})
endif()

#add_test(NAME "${PROJECT_NAME}" COMMAND "${PROJECT_NAME}")



target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core Qt6::Quick BMPCompressionAlgorithm )
