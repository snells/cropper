cmake_minimum_required(VERSION 2.8.0)

project(cropper)

#set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
#find_package(SDL2 REQUIRED)

set (CMAKE_C_FLAGS "-std=c11 -ftrapv -Wall -Wextra -Wshadow -Wconversion -g")
include_directories(include) #${SDL2_INCLude_DIR})
file (GLOB C_FILES src/*.c)
file (GLOB H_FILES src/*.h)
set (SOURCE_FILES ${C_FILES} ${H_FILES})

add_executable(cropper ${SOURCE_FILES})
target_link_libraries(cropper allegro allegro_image allegro_font allegro_ttf allegro_primitives ${CMAKE_THREAD_LIBS_INIT})
#${SDL2_LIBRARY}
