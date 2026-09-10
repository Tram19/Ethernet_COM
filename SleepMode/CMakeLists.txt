cmake_minimum_required(VERSION 3.22)

#
# This file is generated only once,
# and is not re-generated if converter is called multiple times.
#
# User is free to modify the file as much as necessary
#

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Set the project name
set(CMAKE_PROJECT_NAME SleepMode)

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Core project settings
project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

# Enable CMake support for ASM, C and C++ languages
enable_language(C ASM CXX)

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME})

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)

# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined library search paths
)

file(GLOB_RECURSE GREMSY_LIB_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/../Libraries/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../Libraries/*.cpp
)
file(GLOB_RECURSE USER_APP_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/User/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/User/*.cpp
)
# --- Exclude from build (tương đương "Exclude from Build" trong CubeIDE) ---

foreach(excluded_file ${EXCLUDED_SOURCES})
    list(REMOVE_ITEM GREMSY_LIB_SOURCES ${excluded_file})
endforeach()

# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    ${GREMSY_LIB_SOURCES}
    ${USER_APP_SOURCES}
)

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # --- Thư viện dùng chung, tương ứng linked resources trong .project ---
    # GremsyLibraries -> PARENT-3-PROJECT_LOC/Libraries
    ${CMAKE_CURRENT_SOURCE_DIR}/../Libraries
    # DLibraries -> PARENT-3-PROJECT_LOC/DLibraries
    ${CMAKE_CURRENT_SOURCE_DIR}/../Submodule/SG14/SG14
    ${CMAKE_CURRENT_SOURCE_DIR}/../Submodule/etl/include
    ${CMAKE_CURRENT_SOURCE_DIR}/User
    # Add user defined include paths
)
target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE
    $<$<COMPILE_LANGUAGE:CXX>:-fexceptions>
    $<$<CONFIG:Debug>:-Og>
    $<$<NOT:$<CONFIG:Debug>>:-Os>
)
# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined symbols
)

# Remove wrong libob.a library dependency when using cpp files
list(REMOVE_ITEM CMAKE_C_IMPLICIT_LINK_LIBRARIES ob)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    stm32cubemx

    # Add user defined libraries
)

# Tắt cảnh báo unused cho cả C và C++ compiler của ARM GCC
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-unused")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused")

# Convert output to hex and bin files after build
add_custom_command(TARGET ${CMAKE_PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${CMAKE_PROJECT_NAME}> $<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>/${CMAKE_PROJECT_NAME}.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${CMAKE_PROJECT_NAME}> $<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>/${CMAKE_PROJECT_NAME}.bin
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${CMAKE_PROJECT_NAME}>
    BYPRODUCTS ${CMAKE_PROJECT_NAME}.hex ${CMAKE_PROJECT_NAME}.bin
    COMMENT "Generating hex and bin files"
)