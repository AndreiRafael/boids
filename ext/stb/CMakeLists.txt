cmake_minimum_required(VERSION 3.2)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

project(stb_image C)
set(stb_sources
    stb.c
)
list(TRANSFORM stb_sources PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/)

add_library(stb ${stb_sources})
