cmake_minimum_required(VERSION 3.20)

project(BMPCompressionAPP 
    VERSION 1.0.0 
    LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Core Quick Qml)

qt6_add_resources(APP_RESOURCES ${CMAKE_CURRENT_SOURCE_DIR}/qml/app.qrc)

set (PROJECT_SOURCES
    src/main.cpp
    src/BMPCompress.h
    src/BMPCompress.cpp
    src/BMPCompressionApp.h
    src/BMPCompressionApp.cpp
    src/FileExplorerModel.h
    src/FileExplorerModel.cpp)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${APP_RESOURCES})

get_target_property(QT_TARGET_TYPE Qt6::Core TYPE)
if(${QT_TARGET_TYPE} STREQUAL "STATIC_LIBRARY")
    find_package(Qt6QmlImportScanner REQUIRED)
    qt6_import_qml_plugins(${PROJECT_NAME})
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE BMPCompressionAlgorithm Qt6::Core Qt6::Quick Qt6::Qml)
