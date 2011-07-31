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

# make cmake stop complaining by giving it a min version number
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

# Project name and language
PROJECT(${PROJECT} CXX)

# find libraries needed specially for this project
FIND_PACKAGE(GTest REQUIRED)

# set the include directories
INCLUDE_DIRECTORIES(include ${OPENGL_INCLUDE_DIR} ${BULLET_INCLUDE_DIRS}
    ${GTEST_INCLUDE_DIRS})


# add source and headers
# just glob all files for now, as source files will soon change
FILE(GLOB SOURCES *.cpp */*.cpp)


# add library using sources and headers identified
ADD_EXECUTABLE(${EXE} ${SOURCES})

#add libraries needed
TARGET_LINK_LIBRARIES(${EXE} 3DMagic ${GTEST_MAIN_LIBRARIES} ${GTEST_LIBRARIES} 
    pthread)

# add dependency to 3dmagic library
ADD_DEPENDENCIES(${EXE} 3DMagic)

# set compile flags on sources
SET_SOURCE_FILES_PROPERTIES(${SOURCES} PROPERTIES COMPILE_FLAGS ${COMPILE_FLAGS})
















































