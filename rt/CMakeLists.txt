find_package(Threads REQUIRED)
find_package(TBB REQUIRED)
find_package(OpenCV REQUIRED)

file(GLOB_RECURSE sources src/*.cpp)

add_library(rt ${sources})
target_include_directories(rt
	PUBLIC include/
	PRIVATE src/)
if(DEFINED LOGLEVEL)
	target_compile_options(rt PRIVATE -DLOGLEVEL=${LOGLEVEL})
endif()
target_link_libraries(rt
	PUBLIC ${OpenCV_LIBS}
	PUBLIC Threads::Threads
	PUBLIC tbb)
