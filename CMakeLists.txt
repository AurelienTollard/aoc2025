cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_COMPILER clang++)

project(aoc2025
    DESCRIPTION "Advent of Code 2025"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

set(COMMON_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/common
)
add_library(aoc2025_lib STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/common/empty.cpp
)
target_include_directories(aoc2025_lib PUBLIC ${COMMON_FILES})
link_libraries(aoc2025_lib)
add_link_options(LINKER:/DEBUG,/PDB:myapp.pdb)

add_executable(${CURRENT_DAY}
    ${CURRENT_DAY}/main.cpp
)
