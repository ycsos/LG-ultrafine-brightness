cmake_minimum_required(VERSION 3.13)
project(LG_ultrafine_brightness)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS -g)

set(libusb_1_FIND_REQUIRED TRUE)
include(${PROJECT_SOURCE_DIR}/cmake/Findlibusb-1.0.cmake)
include_directories(${LIBUSB_1_INCLUDE_DIRS})

add_executable(LG_ultrafine_brightness main.cpp UltrafineDisplay.cpp UltrafineDisplay.h Brightness.cpp Brightness.h)
target_link_libraries(LG_ultrafine_brightness ${LIBUSB_1_LIBRARIES} ncurses)