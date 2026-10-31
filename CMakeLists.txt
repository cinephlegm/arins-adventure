cmake_minimum_required(VERSION 3.24)

project(ArinsAdventure VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# raylib 5.0 predates CMake 4's removal of compatibility with policy versions < 3.5.
# This keeps the pinned dependency reproducible on current CMake releases.
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

include(FetchContent)

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG ae50bfa2cc569c0f8d5bc4315d39db64005b1b08
    GIT_SHALLOW FALSE
)

FetchContent_MakeAvailable(raylib)

add_executable(arins-adventure
    src/main.cpp
)

target_include_directories(arins-adventure PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party
)

target_link_libraries(arins-adventure PRIVATE raylib)

if(MSVC)
    target_compile_options(arins-adventure PRIVATE /W3 /permissive-)
else()
    target_compile_options(arins-adventure PRIVATE -Wall -Wextra -Wpedantic)
endif()

set_target_properties(arins-adventure PROPERTIES
    OUTPUT_NAME "ArinsAdventure"
)

foreach(asset_dir audio map sprite)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${asset_dir}")
        add_custom_command(
            TARGET arins-adventure POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
                    "${CMAKE_CURRENT_SOURCE_DIR}/${asset_dir}"
                    "$<TARGET_FILE_DIR:arins-adventure>/${asset_dir}"
            COMMENT "Copying ${asset_dir} assets"
        )
    endif()
endforeach()
