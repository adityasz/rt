cmake_minimum_required(VERSION 3.30)
project(raytracer)

set(FETCHCONTENT_BASE_DIR "${CMAKE_SOURCE_DIR}/.deps")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS "-Wall -Wpedantic -Wextra -march=native")
set(CMAKE_CXX_FLAGS_DEBUG "-g3")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -ffast-math")

add_subdirectory(rt)
add_subdirectory(scenes)
