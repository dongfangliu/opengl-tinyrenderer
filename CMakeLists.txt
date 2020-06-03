cmake_minimum_required(VERSION 3.0.0)
project(opengl-basic VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 11)
configure_file(${CMAKE_SOURCE_DIR}/tinyRenderer/configuration/root_directory.h.in ${CMAKE_SOURCE_DIR}/tinyRenderer/configuration/root_directory.h)

include_directories($${CMAKE_SOURCE_DIR}/tinyRenderer/configuration)
# find the required packages
find_package(glm REQUIRED)
message(STATUS "GLM included at ${GLM_INCLUDE_DIRS}")
find_package(glfw3 REQUIRED)
message(STATUS "Found GLFW3 in ${GLFW_INCLUDE_DIR}")
find_package(assimp REQUIRED)
message(STATUS "Found ASSIMP in ${ASSIMP_INCLUDE_DIRS}")
set(OpenGL_GL_PREFERENCE LEGACY)
find_package(OpenGL REQUIRED)
add_definitions(${OPENGL_DEFINITIONS})
find_package(X11 REQUIRED)
set(GLLIBS glfw X11 Xrandr Xinerama Xi Xxf86vm Xcursor GL dl pthread ${ASSIMP_LIBRARIES} ${OPENGL_glu_LIBRARY} ${OPENGL_LIBRARIES})
# find Eigen
find_package(Eigen3)
include_directories(${EIGEN3_INCLUDE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/tinyRenderer/includes)
# first create relevant static libraries requried for other projects
add_library(STB_IMAGE ${CMAKE_SOURCE_DIR}/tinyRenderer/src/stb_image.cpp)
set(GLLIBS ${GLLIBS} STB_IMAGE)
add_library(GLAD ${CMAKE_SOURCE_DIR}/tinyRenderer/src/glad.c)
set(GLLIBS ${GLLIBS} GLAD)

# find source codes
file(GLOB_RECURSE SOURCE_FILES 
${CMAKE_SOURCE_DIR}/tinyRenderer/src/*.c
${CMAKE_SOURCE_DIR}/tinyRenderer/src/*.cpp)
add_executable(opengl-basic ${SOURCE_FILES})
target_link_libraries(opengl-basic ${GLLIBS})

