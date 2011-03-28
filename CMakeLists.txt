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

# allow the user to set the math implementation to use
OPTION(MATH_USE_GENERIC "Use Generic Math implementation" OFF)
OPTION(MATH_USE_INTEL   "USe Intel Math implementation"   OFF)


# add source and headers
# just glob all files for now, as source files will soon change
set(SRC src)
SET(INC include)
FILE(GLOB SOURCES ${SRC}/*.cpp ${SRC}/*/*.cpp ${SRC}/*/*/*.cpp)
FILE(GLOB HEADERS ${INC}/*.h ${INC}/*/*.h ${INC}/*/*/*.h)


# add library using sources and headers identified
ADD_LIBRARY(${EXE} STATIC ${SOURCES} ${HEADERS})

# evaluate Math interface choices
IF(MATH_USE_INTEL)
    SET(CXXFLAGS ${CXXFLAGS} -DM3D_MATH_USE_INTEL)
    MESSAGE(STATUS "MATH: using Intel implementation")
ELSEIF(MATH_USE_GENERIC)
    SET(CXXFLAGS ${CXXFLAGS} -DM3D_MATH_USE_GENERIC)
    MESSAGE(STATUS "MATH: using Generic implementation")
ELSE(MATH_USE_INTEL)
    MESSAGE(SEND_ERROR "MATH: implementation not selected")
ENDIF(MATH_USE_INTEL)

# set compile flags on sources
SET_SOURCE_FILES_PROPERTIES(${SOURCES} PROPERTIES COMPILE_FLAGS ${CXX_FLAGS})
















































