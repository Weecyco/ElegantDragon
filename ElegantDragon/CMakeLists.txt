# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

# set project and version
project (ElegantDragon VERSION 0.0.3)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

configure_file(${PROJECT_SOURCE_DIR}/include/cmakeConfig.h.in ${PROJECT_SOURCE_DIR}/include/cmakeConfig.h)

# Include sub-projects.
# add_subdirectory ("src")

# Set sources locations
set(TEST_APP_SOURCES dragonApp.cpp)
# Higher level sources
set(DRAGON_SOURCES DragonDB.cpp GraphicObj.cpp MVPRenderBlock.cpp graphicsSetup.cpp)
# API level sources
set(API_LVL_SOURCES graphic_api_interface/graphicUtilities.cpp graphic_api_interface/IndexBuffer.cpp graphic_api_interface/Renderer.cpp 
graphic_api_interface/Shader.cpp graphic_api_interface/Textures.cpp graphic_api_interface/VertexArray.cpp graphic_api_interface/VertexBuffer.cpp)

file(GLOB_RECURSE DRAGON_SOURCES 
	${PROJECT_SOURCE_DIR}/src/*.c
	${PROJECT_SOURCE_DIR}/src/*.cpp)

file(GLOB_RECURSE DRAGON_HEADERS 
	${PROJECT_SOURCE_DIR}/src/*.h
	${PROJECT_SOURCE_DIR}/src/*.hpp)


#Set include locations
##TODO## format this area better?
set(LIBS opengl32 glfw3 glew32s)

set(LIB_INCLUDES ${PROJECT_SOURCE_DIR}/vendor/ ${PROJECT_SOURCE_DIR}/depend/glew-2.1.0/include ${PROJECT_SOURCE_DIR}/depend/GLFW/include)

set(LIB_BINS "${PROJECT_SOURCE_DIR}/depend/glew-2.1.0/lib/Release/x64/glew32s.lib" "${PROJECT_SOURCE_DIR}/depend/GLFW/lib-vc2019/glfw3.lib")

# Add source to this project's executable.
# may manually add "ElegantDragon.h" "version.h" but not necessary 
# ElegantDragon is just the test application the entry point
add_executable (ElegantDragon ${DRAGON_SOURCES} ${DRAGON_HEADERS})
target_include_directories(ElegantDragon PUBLIC ${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/include/graphic_api_interface ${LIB_INCLUDES})

message(STATUS "Will check for Libraries under ${LIB_BINS}")
set(CMAKE_MODULE_PATH ${LIB_BINS})

find_package(OpenGL REQUIRED)
#find_package(glfw3 REQUIRED)
#find_package(glew32s REQUIRED)

#ADD_LIBRARY(glfw3)
#ADD_LIBRARY(GLEW ${PROJECT_SOURCE_DIR}/depend/glew-2.1.0-src/src/glew.c)
add_library(GLEW STATIC IMPORTED)
set_target_properties(GLEW PROPERTIES
    IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/depend/glew-2.1.0/lib/Release/x64/glew32s.lib)

add_library(GLFW3 STATIC IMPORTED)
set_target_properties(GLFW3 PROPERTIES
    IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/depend/GLFW/lib-vc2019/glfw3.lib)


# ImGui library module setup
file(GLOB IMGUI_SOURCE_FILES 
	${PROJECT_SOURCE_DIR}/vendor/imgui/*.c
	${PROJECT_SOURCE_DIR}/vendor/imgui/*.cpp)
file(GLOB IMGUI_HEADER_FILES 
	${PROJECT_SOURCE_DIR}/vendor/imgui/*.h
	${PROJECT_SOURCE_DIR}/vendor/imgui/*.hpp)

add_library(IMGUI ${IMGUI_SOURCE_FILES} ${IMGUI_HEADER_FILES})
# since it's dependent on GLFW & GLEW
target_include_directories(IMGUI PUBLIC ${LIB_INCLUDES})

# stb_image library module setup
add_library(STBIMAGE ${PROJECT_SOURCE_DIR}/vendor/stb_image/stb_image.cpp ${PROJECT_SOURCE_DIR}/vendor/stb_image/stb_image.h)

# include independent libraries
target_link_libraries(ElegantDragon PUBLIC opengl32)

target_link_libraries(ElegantDragon PUBLIC GLEW)
target_compile_definitions(ElegantDragon PUBLIC GLEW_STATIC)

target_link_libraries(ElegantDragon PUBLIC GLFW3)

target_link_libraries(ElegantDragon PUBLIC IMGUI)

target_link_libraries(ElegantDragon PUBLIC STBIMAGE)
# include system library with find package
