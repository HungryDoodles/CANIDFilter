cmake_minimum_required(VERSION 3.1...3.26)

project(
	CANIDFilter 
	VERSION 0.0
	LANGUAGES CXX)

# Let's ensure -std=c++xx instead of -std=g++xx
set(CMAKE_CXX_EXTENSIONS OFF)

# Little hack
file(GLOB all_src_files CONFIGURE_DEPENDS "*.h" "*.cpp")

add_executable(CANFilter ${all_src_files})

# set_target_properties(CANFilter PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")
set_property(TARGET CANFilter PROPERTY CXX_STANDARD 11)