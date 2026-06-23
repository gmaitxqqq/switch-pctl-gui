cmake_minimum_required(VERSION 3.15)
project(switch-pctl-gui LANGUAGES C CXX)

#===============================================================================
# devkitPro Toolchain
#   - 优先使用 CMAKE_TOOLCHAIN_FILE（由命令行 -D 传入）
#   - 回退到 DEVKITPRO 环境变量
#===============================================================================
if(CMAKE_TOOLCHAIN_FILE)
    message(STATUS "Using toolchain: ${CMAKE_TOOLCHAIN_FILE}")
elseif(DEFINED ENV{DEVKITPRO})
    set(DEVKITPRO_ENV $ENV{DEVKITPRO})
    # 尝试常见文件名
    foreach(_fn DevkitA64Libnx.cmake devkitA64.cmake)
        set(_path "${DEVKITPRO_ENV}/cmake/${_fn}")
        if(EXISTS "${_path}")
            set(CMAKE_TOOLCHAIN_FILE "${_path}")
            break()
        endif()
    endforeach()
    if(NOT CMAKE_TOOLCHAIN_FILE)
        message(FATAL_ERROR "Cannot find devkitPro toolchain in ${DEVKITPRO_ENV}/cmake/")
    endif()
    message(STATUS "Auto-detected toolchain: ${CMAKE_TOOLCHAIN_FILE}")
    include(${CMAKE_TOOLCHAIN_FILE})
else()
    message(FATAL_ERROR "Please set DEVKITPRO env var or pass -DCMAKE_TOOLCHAIN_FILE=...")
endif()

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
if(EXISTS "${CMAKE_SOURCE_DIR}/externals/borealis/include")
    include_directories(externals/borealis/include)
endif()

#===============================================================================
# Source Files
#===============================================================================
file(GLOB_RECURSE SOURCES "source/*.cpp")

#===============================================================================
# Borealis Submodule
#===============================================================================
if(NOT EXISTS "${CMAKE_SOURCE_DIR}/externals/borealis/CMakeLists.txt")
    message(FATAL_ERROR
      "Borealis not found at externals/borealis. "
      "Run: actions/checkout@v4 with repository=xfangfang/borealis ref=wiliwili path=externals/borealis"
    )
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
    set(ROMFS_TARGET romfs)
endif()

#===============================================================================
# Executable
#===============================================================================
add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} borealis)

if(ROMFS_TARGET)
    add_dependencies(${PROJECT_NAME} ${ROMFS_TARGET})
    target_link_options(${PROJECT_NAME} PRIVATE ${CMAKE_BINARY_DIR}/romfs.o)
endif()

#===============================================================================
# Post-build: strip + generate NRO/NPDM
#===============================================================================
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_STRIP} $<TARGET_FILE:${PROJECT_NAME}>
    COMMAND ${DEVKITPRO}/tools/elf2nro
        $<TARGET_FILE:${PROJECT_NAME}>
        ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.nro
        --icon=${CMAKE_SOURCE_DIR}/icon.jpg
        --name=${PROJECT_NAME}
    COMMENT "Generating NRO"
)
