cmake_minimum_required(VERSION 3.21)
project(fourier_transform)

set(CMAKE_CXX_STANDARD 14)

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

SET(EXTERNAL_INCLUDES ${Boost_INCLUDES})
SET(EXTERNAL_LIBRARIES ${Boost_LIBRARIES})

add_executable(fourier_transform main.cpp utils.cpp utils.h)
target_link_libraries( fourier_transform ${OpenCV_LIBS} )