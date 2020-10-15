cmake_minimum_required (VERSION 3.14)
project(HtmlTaskGeneratorBMSTU)

set(CMAKE_AUTOMOC ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt5 COMPONENTS Xml REQUIRED)

set(HEADER_FILES
    "src/core/TaskManager.h")

set(SOURCE_FILES
    "src/core/TaskManager.cpp"
    "src/main.cpp")

add_executable(${PROJECT_NAME}
    ${HEADER_FILES}
    ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME}
    PUBLIC "src")

target_link_libraries(${PROJECT_NAME}
    Qt5::Widgets
    Qt5::Xml)
