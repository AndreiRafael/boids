cmake_minimum_required(VERSION 3.0)
project(boids C)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/ext/hf_lib/)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/ext/glad/)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/ext/stb/)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/debug)
else()
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/release)
endif()

set(sources
    main
    hfe
    boids
)
list(TRANSFORM sources PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/src/)
list(TRANSFORM sources APPEND ".c")

add_executable(boids ${sources})

if(${CMAKE_C_COMPILER_ID} EQUAL MSVC)
    target_compile_options(boids PRIVATE /D_CRT_SECURE_NO_WARNINGS)
else()
	target_compile_options(boids PRIVATE -D_CRT_SECURE_NO_WARNINGS -Wstrict-prototypes -Wconversion -Wall -Wextra -Wpedantic -pedantic -Werror)
endif()

target_include_directories(boids PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/)

target_link_directories(boids PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/lib/sdl2/x64)
if(WIN32)
    target_link_libraries(boids SDL2main SDL2 hf_lib glad stb)
else()
    target_link_libraries(boids m SDL2main SDL2 hf_lib glad stb)
endif()

if(WIN32)
    file(COPY ${CMAKE_SOURCE_DIR}/lib/sdl2/x64/SDL2.dll DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endif()

#copy all resources
file(REMOVE_RECURSE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/res)
file(COPY ${CMAKE_SOURCE_DIR}/res DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
