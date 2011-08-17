# Cmake file for 3DMagic
# 3DMagic needs:
# - cmake (obviously)
# - openGL >=3.1
# - GLEW
# - bullet
# - openAL

# make cmake stop complaining by giving it a min version number
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

# Project name and language
PROJECT(${PROJECT} CXX)
SET(EXE 3DMagic)

# set path for local cmake modules
SET(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Find the libraries needed
FIND_PACKAGE(OpenGL REQUIRED)
FIND_PACKAGE(Bullet REQUIRED)
FIND_PACKAGE(GLEW REQUIRED)
FIND_PACKAGE(Lib3ds REQUIRED)
FIND_PACKAGE(PNG REQUIRED)

# set the global compile flags
SET(COMPILE_FLAGS "-g -Wall") # debugging and all warnings
SET(LINK_FLAGS  "" )

# set the include directories
INCLUDE_DIRECTORIES(include ${OPENGL_INCLUDE_DIR} ${BULLET_INCLUDE_DIRS}
    ${GLEW_INCLUDE_DIR} ${LIB3DS_INCLUDE_DIR} ${PNG_INCLUDE_DIR})

# allow for user to select if source should be compiled for profiling with gprof
OPTION(GPROF_COMPILE "Whether to compile for profiling using gprof" OFF)

# allow the user to set the math implementation to use
OPTION(MATH_USE_GENERIC "Use Generic Math implementation" OFF)
OPTION(MATH_USE_INTEL   "Use Intel Math implementation"   OFF)

# allow the user to disable building and/or enable running tests
OPTION(BUILD_TESTS "Build the test suite" ON)
OPTION(RUN_TESTS "Run the Test Suite during build" ON)

# allow the user to disable building demos
OPTION(BUILD_DEMOS "Build the demos" ON)

# allow user to set the build without vertex arrays
OPTION(USE_VERTEX_ARRAYS "Enable/Disable Vertex Array use" ON)
IF(USE_VERTEX_ARRAYS)
ELSE(USE_VERTEX_ARRAYS)
    SET(CXX_FLAGS "${COMPILE_FLAGS} -DMAGIC3D_NO_VERTEX_ARRAYS")
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
    SET(COMPILE_FLAGS "${COMPILE_FLAGS} -DM3D_MATH_USE_INTEL -masm=intel")
    MESSAGE(STATUS "MATH: using Intel implementation")
ELSEIF(MATH_USE_GENERIC)
    SET(COMPILE_FLAGS "${COMPILE_FLAGS} -DM3D_MATH_USE_GENERIC")
    MESSAGE(STATUS "MATH: using Generic implementation")
ELSE(MATH_USE_INTEL)
    MESSAGE(SEND_ERROR "MATH: implementation not selected")
ENDIF(MATH_USE_INTEL)

# if compiling for profiling, add options
IF(GPROF_COMPILE)
    SET(COMPILE_FLAGS "${COMPILE_FLAGS} -pg")
    SET(LINK_FLAGS    "${LINK_FLAGS} -pg" )
ENDIF(GPROF_COMPILE)

# set compile flags on sources
SET_SOURCE_FILES_PROPERTIES(${SOURCES} PROPERTIES COMPILE_FLAGS ${COMPILE_FLAGS})

# add the tests build configuration
IF(BUILD_TESTS)
    ADD_SUBDIRECTORY(Test)
ENDIF(BUILD_TESTS)

# add the demos build configuration
IF(BUILD_DEMOS)
    #ADD_SUBDIRECTORY(demo/field)
    ADD_SUBDIRECTORY(demo/sandbox)
ENDIF(BUILD_DEMOS)







































