cmake_minimum_required(VERSION 4.2)
project(SGEL VERSION 1.0)

option(BUILD_SHARED_LIBS "Build SGEL as a shared library" OFF)

include(FetchContent)

find_package(SDL3)
if(NOT SDL3_FOUND)
    FetchContent_Declare(
            SDL3
            GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
            GIT_TAG release-3.1.3 # Usa siempre un tag estable de SDL3
            GIT_SHALLOW ON
            EXCLUDE_FROM_ALL
    )
    FetchContent_MakeAvailable(SDL3)
endif ()

set(SDLIMAGE_VENDORED OFF CACHE BOOL "" FORCE)
set(SDLIMAGE_AVIF OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
        SDL3_image
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
        GIT_TAG release-3.4.4
        GIT_SHALLOW ON
        GIT_SUBMODULES ""
        EXCLUDE_FROM_ALL
)

FetchContent_Declare(
        SDL3_gfx
        GIT_REPOSITORY https://github.com/sabdul-khabir/SDL3_gfx.git
        GIT_TAG v1.0.1
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
)
set(SDL3_LIB SDL3)

FetchContent_MakeAvailable(SDL3_image SDL3_gfx)

file(GLOB_RECURSE SRC_FILES "src/*.cpp")
file(GLOB_RECURSE HEADER_FILES "src/*.hpp")

add_library(${PROJECT_NAME}  ${SRC_FILES} ${HEADER_FILES})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_23)

target_include_directories(${PROJECT_NAME}
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        PRIVATE
        ${SDL3_gfx_SOURCE_DIR}
)

target_link_libraries(${PROJECT_NAME}
        PUBLIC
        SDL3::SDL3
        SDL3_image::SDL3_image
        PRIVATE
        SDL3_gfx_Static
)