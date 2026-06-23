cmake_minimum_required(VERSION 3.15)
project(switch-pctl-gui LANGUAGES C CXX)

#===============================================================================
# devkitPro Toolchain
#===============================================================================
if(NOT DEFINED ENV{DEVKITPRO})
    message(FATAL_ERROR "Please set DEVKITPRO in your environment variables")
endif()

include($ENV{DEVKITPRO}/cmake/DevkitA64Libnx.cmake)

#===============================================================================
# C++ Standard
#===============================================================================
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#===============================================================================
# Build Type
#===============================================================================
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build" FORCE)
endif()

#===============================================================================
# Global Compiler Flags
#===============================================================================
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

#===============================================================================
# Include Directories
#===============================================================================
include_directories(include)
include_directories(externals/borealis/include)

#===============================================================================
# Source Files
#===============================================================================
file(GLOB_RECURSE SOURCES "source/*.cpp")

#===============================================================================
# Borealis Submodule
#===============================================================================
if(NOT EXISTS "${CMAKE_SOURCE_DIR}/externals/borealis/CMakeLists.txt")
    message(WARNING "Borealis submodule not found. Please run: git submodule update --init --recursive")
endif()

add_subdirectory(externals/borealis)

#===============================================================================
# ROMFS
#===============================================================================
if(EXISTS "${CMAKE_SOURCE_DIR}/romfs")
    add_custom_target(
        romfs ALL
        COMMAND ${DEVKITPRO}/tools/romfs-tool -c ${CMAKE_SOURCE_DIR}/romfs -o ${CMAKE_BINARY_DIR}/romfs.o
        DEPENDS ${CMAKE_SOURCE_DIR}/romfs
        COMMENT "Packing ROMFS"
    )
    set(ROMFS_OBJECT "${CMAKE_BINARY_DIR}/romfs.o")
endif()

#===============================================================================
# Executable (NRO)
#===============================================================================
add_executable(${PROJECT_NAME}.nro ${SOURCES})

if(ROMFS_OBJECT)
    add_dependencies(${PROJECT_NAME}.nro romfs)
    target_link_libraries(${PROJECT_NAME}.nro ${ROMFS_OBJECT})
endif()

#===============================================================================
# Link Libraries
#===============================================================================
target_link_libraries(${PROJECT_NAME}.nro borealis libnx)

#===============================================================================
# Output Name
#===============================================================================
set_target_properties(
    ${PROJECT_NAME}.nro
    PROPERTIES
    OUTPUT_NAME "switch-pctl-gui"
)

#===============================================================================
# Install Target (for `make install`)
#===============================================================================
if(DEFINED ENV{DEVKITPRO})
    install(FILES ${CMAKE_BINARY_DIR}/switch-pctl-gui.nro
        DESTINATION ${DEVKITPRO}/switch/switch-pctl-gui
        RENAME switch-pctl-gui.nro
    )
endif()
