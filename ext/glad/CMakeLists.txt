cmake_minimum_required(VERSION 3.2)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

project(glad C)
set(glad_sources
    glad.c
)
list(TRANSFORM glad_sources PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/)

add_library(glad ${glad_sources})

target_include_directories(glad PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
