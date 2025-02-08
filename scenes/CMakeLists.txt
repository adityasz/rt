include(FetchContent)
FetchContent_Declare(
	argparse
	GIT_REPOSITORY https://github.com/p-ranav/argparse.git
)
FetchContent_MakeAvailable(argparse)

add_executable(one one.cpp)
target_link_libraries(one
	PRIVATE rt
	PRIVATE argparse)
