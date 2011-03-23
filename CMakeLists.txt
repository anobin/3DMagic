# Cmake file for 3DMagic
# 3DMagic needs:
# - cmake (obviously)
# - openGL >=3.1
# - GLEW
# - bullet
# - openAL

# set the project and exe name
SET(PROJECT 3DMagic)
SET(EXE 3DMagic)

# set the library include directories
SET(OPENGL_INCLUDE /usr/include/GL)
SET(BULLET_INCLUDE /usr/include/bullet)

# set the global compile flags
SET(CXX_FLAGS "-g -Wall") # debugging and all warnings

# make cmake stop complaining by giving it a min version number
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

# Project name and language
PROJECT(${PROJECT} CXX)

# set the include directories
INCLUDE_DIRECTORIES(include ${OPENGL_INCLUDE} ${BULLET_INCLUDE})


# add source and headers
# just glob all files for now, as source files will soon change
set(SRC src)
SET(INC include)
FILE(GLOB SOURCES ${SRC}/*.cpp ${SRC}/*/*.cpp ${SRC}/*/*/*.cpp)
FILE(GLOB HEADERS ${INC}/*.h ${INC}/*/*.h ${INC}/*/*/*.h)


# add library using sources and headers identified
ADD_LIBRARY(${EXE} STATIC ${SOURCES} ${HEADERS})


# set compile flags on sources
SET_SOURCE_FILES_PROPERTIES(${SOURCES} PROPERTIES COMPILE_FLAGS ${CXX_FLAGS})












