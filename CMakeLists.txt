cmake_minimum_required(VERSION 3.10)

project(proto C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Collect all .c files in src/
file(GLOB SRC_FILES
    src/*.c
)

add_executable(${PROJECT_NAME} ${SRC_FILES})

# Include header directory
target_include_directories(${PROJECT_NAME}
    PRIVATE
        include
)

