# cmake_minimum_required(VERSION 3.10)

# # Required version of cpp
# set(CMAKE_CXX_STANDARD 14)
# set(CMAKE_CXX_STANDARD_REQUIRED True)

# # set the project name
# project("Graphics_Labs" VERSION 0.1 LANGUAGES CXX C)

# message(STATUS "The build type is set as ${CMAKE_BUILD_TYPE}.")
# # set build type as debug
# if (NOT DEFINED CMAKE_BUILD_TYPE)
#     set(CMAKE_BUILD_TYPE Debug)
# endif()

# # create a list to keep source files in a place
# # set(SRCS "srcs/Lab 1/Lab 1.cpp")

# # set(SRCS "srcs/Lab 2/Lab 2_Bresenham.cpp")
# # set(SRCS "srcs/Lab 2/Lab 2_DDA.cpp")
# # set(SRCS "srcs/Lab 2/Lab 2_Histogram.cpp")

# # set(SRCS "srcs/Lab 3/Lab 3_Midpoint.cpp")
# # set(SRCS "srcs/Lab 3/Lab 3_Ellipse.cpp")
# # set(SRCS "srcs/Lab 3/Lab 3_Polar.cpp")

# # set(SRCS "srcs/Lab 4/transformation.cpp")

# # set(SRCS "srcs/Lab 5/Lab 5_line_clipping.cpp")
# # set(SRCS "srcs/Lab 5/Lab 5_polygon_clipping.cpp")

# set(SRCS "srcs/Lab 6/Lab 6_transformation.cpp")

# # set(SRCS "srcs/Projects/square.cpp")
# # set(SRCS "src/Projects/square copy 4 working.cpp")


# add_executable(${PROJECT_NAME} ${SRCS})

# find_package(OpenGL REQUIRED)

# include_directories( ${OPENGL_INCLUDE_DIRS})

# target_include_directories("${PROJECT_NAME}" PUBLIC "${PROJECT_BINARY_DIR}" PUBLIC include)

# add_subdirectory("./lib")

# target_link_libraries("${PROJECT_NAME}" ${OPENGL_LIBRARIES} 
#                 OpenGL::GL
#                 glfw 
#                 glad
#                 glm::glm )


# set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 11)



cmake_minimum_required(VERSION 3.10)

# Required version of cpp
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# set the project name
project("OpenGL_Lab" VERSION 0.1 LANGUAGES CXX C)

message(STATUS "The build type is set as ${CMAKE_BUILD_TYPE}.")
# set build type as debug
if (NOT DEFINED CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# create a list to keep source files in a place
# set(SRCS "srcs/Lab 6/Lab 6_transformation.cpp")
set(SRCS "srcs/Lab 6/Lab 6_projection.cpp")
# set(SRCS "srcs/Lab 6/Lab 6_transformation copy 3.cpp")
# list(APPEND SRCS "srcs/Lab 6/lab6.cpp")


if (APPLE)
    # create bundle on apple compiles
    add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${SRCS} )
else()
    add_executable(${PROJECT_NAME} ${SRCS})
endif()

set(OpenGL_GL_PREFERENCE "LEGACY")

find_package(OpenGL)

include_directories(${OPENGL_INCLUDE_DIRS})

target_include_directories("${PROJECT_NAME}" PUBLIC "${PROJECT_BINARY_DIR}" PUBLIC include)

add_subdirectory("./lib")

target_link_libraries("${PROJECT_NAME}" ${OPENGL_LIBRARIES} glfw glad glm::glm)


