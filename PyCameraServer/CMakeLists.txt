cmake_minimum_required(VERSION 3.18)
project(PyCameraServer)

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})

project(PyCameraTest)

add_executable(${PROJECT_NAME} test.cpp)
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})