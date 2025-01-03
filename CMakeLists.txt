cmake_minimum_required(VERSION 3.30.1)

project(squares)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED TRUE)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

add_executable(${CMAKE_PROJECT_NAME}
    src/main.c
    src/square.c
    src/misc.c
)

target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE
    -Wall -Wextra -Wpedantic -Werror -Wconversion
)

add_custom_target(run
    COMMAND ${CMAKE_PROJECT_NAME}
    DEPENDS ${CMAKE_PROJECT_NAME}
    WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
)

find_package(SDL3 REQUIRED)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE SDL3::SDL3 PRIVATE SDL3::Headers)
