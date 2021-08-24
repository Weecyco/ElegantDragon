# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

# set project and version
project (ElegantDragon VERSION 0.0)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

configure_file(${PROJECT_SOURCE_DIR}/include/cmakeConfig.h.in ${PROJECT_SOURCE_DIR}/include/cmakeConfig.h)

# Include sub-projects.
add_subdirectory ("src")
