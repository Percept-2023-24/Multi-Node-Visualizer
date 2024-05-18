cmake_minimum_required(VERSION 3.9)
project(Percept__Multi_node_UI)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

#find_library(OpenCV)
find_package(OpenCV)
include_directories(${OpenCV_INCLUDE_DIRS})

#find_library(Qt)
find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(Percept__Multi_node_UI main.cpp)
target_link_libraries(Percept__Multi_node_UI Qt5::Core Qt5::Gui Qt5::Widgets)
target_link_libraries(Percept__Multi_node_UI ${OpenCV_LIBS})
