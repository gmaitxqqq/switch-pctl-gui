cmake_minimum_required(VERSION 3.15)

#===============================================================================
# Borealis cmake modules — MUST be included before project()
# These define PLATFORM_SWITCH etc. and set up the toolchain
#===============================================================================
set(BOREALIS_LIBRARY ${CMAKE_CURRENT_SOURCE_DIR}/externals/borealis/library)

if(NOT EXISTS "${BOREALIS_LIBRARY}/cmake/commonOption.cmake")
    message(FATAL_ERROR
        "Borealis not found. Please checkout borealis (wiliwili branch) to externals/borealis.\n"
        "  actions/checkout@v4 with: repository=xfangfang/borealis ref=wiliwili path=externals/borealis submodules=recursive"
    )
endif()

include(${BOREALIS_LIBRARY}/cmake/commonOption.cmake)
include(${BOREALIS_LIBRARY}/cmake/toolchain.cmake)

#===============================================================================
# Project
#===============================================================================
project(switch-pctl-gui LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#===============================================================================
# Project metadata (for NRO generation)
#===============================================================================
set(VERSION_MAJOR "1")
set(VERSION_MINOR "0")
set(VERSION_ALTER "0")
set(PROJECT_AUTHOR "switch-pctl-gui")
set(PSN_TITLE_ID  "010000000000BD23")
set(PROJECT_ICON  ${CMAKE_SOURCE_DIR}/icon.jpg)
set(PROJECT_RESOURCES ${CMAKE_SOURCE_DIR}/romfs)

#===============================================================================
# Borealis library
#===============================================================================
add_subdirectory(${BOREALIS_LIBRARY})

find_package(Threads REQUIRED)
list(APPEND APP_PLATFORM_LIB ${CMAKE_THREAD_LIBS_INIT})

#===============================================================================
# Our source files
#===============================================================================
file(GLOB_RECURSE SOURCES "source/*.cpp")

# Switch-specific wrapper (required by borealis on Switch)
if(PLATFORM_SWITCH)
    list(APPEND SOURCES ${BOREALIS_LIBRARY}/lib/platforms/switch/switch_wrapper.c)
endif()

#===============================================================================
# Executable — use borealis's program_target()
#===============================================================================
program_target(${PROJECT_NAME} "${SOURCES}")
target_include_directories(${PROJECT_NAME} PRIVATE include)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17)
target_link_libraries(${PROJECT_NAME} PRIVATE borealis ${APP_PLATFORM_LIB})

#===============================================================================
# NRO generation (Switch only) — ALL keyword makes it part of default build
#===============================================================================
set(APP_VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_ALTER}")

if(PLATFORM_SWITCH)
    add_custom_target(${PROJECT_NAME}.nro ALL
        DEPENDS ${PROJECT_NAME}
        COMMAND ${NX_NACPTOOL_EXE} --create ${PROJECT_NAME} ${PROJECT_AUTHOR}
            ${APP_VERSION} ${PROJECT_NAME}.nacp
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${PROJECT_RESOURCES} ${CMAKE_BINARY_DIR}/resources
        COMMAND ${NX_ELF2NRO_EXE} ${PROJECT_NAME}.elf ${PROJECT_NAME}.nro
            --icon=${PROJECT_ICON}
            --nacp=${PROJECT_NAME}.nacp
            --romfsdir=${CMAKE_BINARY_DIR}/resources
    )
endif()
