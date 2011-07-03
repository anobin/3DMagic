# Cmake file for 3DMagic tests
# 3DMagic tests needs:
# - cmake (obviously)
# - openGL >=3.1
# - GLEW
# - bullet
# - openAL
# - google test framework (gtest)

# set the project and exe name
SET(PROJECT 3DMagic_Tests)
SET(EXE 3DMagic_Tests)

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
FILE(GLOB SOURCES *.cpp */*.cpp)


# add library using sources and headers identified
ADD_EXECUTABLE(${EXE} ${SOURCES})

#add libraries needed
TARGET_LINK_LIBRARIES(${EXE} 3DMagic gtest_main gtest pthread)

# add dependency to 3dmagic library
ADD_DEPENDENCIES(${EXE} 3DMagic)

# evaluate Math interface choices
IF(MATH_USE_INTEL)
    SET(CXX_FLAGS "${CXX_FLAGS} -DM3D_MATH_USE_INTEL")
ELSEIF(MATH_USE_GENERIC)
    SET(CXX_FLAGS "${CXX_FLAGS} -DM3D_MATH_USE_GENERIC")
ELSE(MATH_USE_INTEL)
    MESSAGE(SEND_ERROR "MATH: implementation not selected")
ENDIF(MATH_USE_INTEL)

# set compile flags on sources
SET_SOURCE_FILES_PROPERTIES(${SOURCES} PROPERTIES COMPILE_FLAGS ${CXX_FLAGS})
















































