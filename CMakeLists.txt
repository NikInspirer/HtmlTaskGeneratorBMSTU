cmake_minimum_required (VERSION 3.14)
project(HtmlTaskGeneratorBMSTU)

set(CMAKE_AUTOMOC ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} Qt5::Widgets)
