# CMakeList.txt : CMake project for comp280-a2-cpp, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (comp280-a2-cpp
	"main.cpp"
	"commands.cpp"
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET comp280-a2-cpp PROPERTY CXX_STANDARD 20)
endif()

add_subdirectory(game)
add_subdirectory(cli)
