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

# allow for user to select if source should be compiled for profiling with gprof
OPTION(GPROF_COMPILE "Whether to compile for profiling using gprof" OFF)

# allow the user to set the math implementation to use
OPTION(MATH_USE_GENERIC "Use Generic Math implementation" OFF)
OPTION(MATH_USE_INTEL   "Use Intel Math implementation"   OFF)

# allow the user to disable building and/or enable running tests
OPTION(BUILD_TESTS "Build the test suite" ON)
OPTION(RUN_TESTS "Run the Test Suite during build" OFF)

# allow user to set the build without vertex arrays
OPTION(USE_VERTEX_ARRAYS "Enable/Disable Vertex Array use" ON)
IF(USE_VERTEX_ARRAYS)
ELSE(USE_VERTEX_ARRAYS)
    SET(CXX_FLAGS "${CXX_FLAGS} -DMAGIC3D_NO_VERTEX_ARRAYS")
ENDIF(USE_VERTEX_ARRAYS)


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
    SET(CXX_FLAGS "${CXX_FLAGS} -DM3D_MATH_USE_INTEL -masm=intel")
    MESSAGE(STATUS "MATH: using Intel implementation")
ELSEIF(MATH_USE_GENERIC)
    SET(CXX_FLAGS "${CXX_FLAGS} -DM3D_MATH_USE_GENERIC")
    MESSAGE(STATUS "MATH: using Generic implementation")
ELSE(MATH_USE_INTEL)
    MESSAGE(SEND_ERROR "MATH: implementation not selected")
ENDIF(MATH_USE_INTEL)

# if compiling for profiling, add options
IF(GPROF_COMPILE)
    SET(CXX_FLAGS "${CXX_FLAGS} -pg")
ENDIF(GPROF_COMPILE)

# set compile flags on sources
SET_SOURCE_FILES_PROPERTIES(${SOURCES} PROPERTIES COMPILE_FLAGS ${CXX_FLAGS})

# add the tests build configuration
IF(BUILD_TESTS)
    ADD_SUBDIRECTORY(Test)
ENDIF(BUILD_TESTS)













































