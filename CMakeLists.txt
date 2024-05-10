cmake_minimum_required(VERSION 3.26)

project(raytracer VERSION 1.0
                  DESCRIPTION "A basic ray tracer"
                  HOMEPAGE_URL "github.com/adityasz/rt"
                  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SOURCE_DIR src)
set(INCLUDE_DIR include)
set(BINARY_DIR bin)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/${BINARY_DIR})

add_executable(rt ${SOURCE_DIR}/main.cpp)

find_package(Threads REQUIRED)
find_package(fmt CONFIG REQUIRED)
target_link_libraries(rt Threads::Threads fmt::fmt)

target_include_directories(rt
                           PUBLIC ${INCLUDE_DIR}/
                           PRIVATE ${SOURCE_DIR}/)

target_sources(rt
               PRIVATE ${SOURCE_DIR}/main.cpp)

add_compile_options(
	"-Ofast" "-Wall" "-Wpedantic" "-Wextra" "-fexceptions" "-march=native" "-mtune=native" "-Wunused"
	"$<$<CONFIG:DEBUG>:-g3;-ggdb3>"
)

set(DEBUG 0
    CACHE STRING "DEBUG level")
if (${DEBUG} GREATER 0)
	target_compile_definitions(rt PRIVATE "DEBUG=${DEBUG}")
endif ()
