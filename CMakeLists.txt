cmake_minimum_required(VERSION 3.14)
project(tile_wfc VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set project directories
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(RAYLIB_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/raylib)
set(RAYLIB_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib)

# Collect sources
file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS "${SOURCE_DIR}/*.cpp")

# Create executable
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

# Include directories (stb and raylib)
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIR} ${RAYLIB_INCLUDE_DIR})

# Platform and toolchain detection for linking
if(MINGW)  # MinGW-w64 on Windows
    # Prefer static linking if available; fallback to shared
    if(EXISTS "${RAYLIB_LIB_DIR}/libraylib.a")
        target_link_libraries(${PROJECT_NAME} PRIVATE "${RAYLIB_LIB_DIR}/libraylib.a")
        message(STATUS "Linking statically with libraylib.a")
    else()
        target_link_libraries(${PROJECT_NAME} PRIVATE "${RAYLIB_LIB_DIR}/libraylibdll.a")
        # Copy DLL to output directory for runtime
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${RAYLIB_LIB_DIR}/raylib.dll" $<TARGET_FILE_DIR:${PROJECT_NAME}>)
        message(STATUS "Linking dynamically with libraylibdll.a and copying raylib.dll")
    endif()
    # MinGW-specific system libraries
    target_link_libraries(${PROJECT_NAME} PRIVATE gdi32 winmm user32 shell32 opengl32 mingw32)
elseif(WIN32)  # MSVC fallback (if .lib files were present)
    find_package(raylib QUIET)
    if(raylib_FOUND)
        target_link_libraries(${PROJECT_NAME} PRIVATE raylib)
    else()
        message(FATAL_ERROR "MSVC requires raylib.lib; consider rebuilding raylib for MSVC.")
    endif()
    target_link_libraries(${PROJECT_NAME} PRIVATE gdi32 winmm user32 shell32)
elseif(UNIX)
    target_link_libraries(${PROJECT_NAME} PRIVATE "${RAYLIB_LIB_DIR}/libraylib.a")
    target_link_libraries(${PROJECT_NAME} PRIVATE GL m pthread dl X11)
endif()

# Output directory for executables
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)