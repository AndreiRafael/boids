cmake_minimum_required(VERSION 3.0)
project(hf_lib C)

set(hf_lib_sources
    hf_algorithm.c
    hf_line.c
    hf_mat.c
    hf_memory.c
    hf_path.c
    hf_shape.c
    hf_string.c
    hf_transform.c
    hf_ui.c
    hf_vec.c
)
list(TRANSFORM hf_lib_sources PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/src/)
add_library(hf_lib ${hf_lib_sources})

if(${CMAKE_C_COMPILER_ID} EQUAL MSVC)
    target_compile_options(hf_lib PRIVATE /D_CRT_SECURE_NO_WARNINGS)
else()
	target_compile_options(hf_lib PRIVATE -Wstrict-prototypes -Wconversion -Wall -Wextra -Wpedantic -pedantic -Werror)
endif()

if(CMAKE_TESTING_ENABLED)
    add_executable(hf_test_string  ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_string.c)
    target_link_libraries(hf_test_string PUBLIC hf_lib)
    add_test(NAME hf_string COMMAND hf_test_string)

    add_executable(hf_test_path  ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_path.c)
    target_link_libraries(hf_test_path PUBLIC hf_lib)
    add_test(NAME hf_path COMMAND hf_test_path)

    add_executable(hf_test_algorithm  ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_algorithm.c)
    target_link_libraries(hf_test_algorithm PUBLIC hf_lib)
    add_test(NAME hf_algorithm COMMAND hf_test_algorithm)

    add_executable(hf_test_transform  ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_transform.c)
    target_link_libraries(hf_test_transform PUBLIC hf_lib)
    add_test(NAME hf_transform COMMAND hf_test_transform)
endif()
