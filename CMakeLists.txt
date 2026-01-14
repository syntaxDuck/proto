cmake_minimum_required(VERSION 3.15)

project(proto VERSION 0.1.0 LANGUAGES C)

# Set C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Export compile commands for IDEs
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Compiler flags
include(cmake/CompilerFlags.cmake)

# Collect source files from all modules
file(GLOB_RECURSE SRC_FILES 
    "src/*.c"
)

# Create executable
add_executable(${PROJECT_NAME} ${SRC_FILES})

# Include directories
target_include_directories(${PROJECT_NAME}
    PRIVATE
        include
)

# Installation
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)

# Optional testing
if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()