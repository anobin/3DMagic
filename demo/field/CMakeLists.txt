cmake_minimum_required(VERSION 2.6)

# set the executable and project name
SET(PROJECT Field)
SET(EXE Field)

# set source files
SET(SOURCES Field.cpp)

# Project name and language
PROJECT(${PROJECT} CXX)

# look for libraries needed specifically by the demo
FIND_PACKAGE(GLUT REQUIRED)

# set include directories
INCLUDE_DIRECTORIES(include ${OPENGL_INCLUDE_DIR} ${BULLET_INCLUDE_DIRS} 
    ${GLUT_INCLUDE_DIR} ${GLEW_INCLUDE_DIR} ${LIB3DS_INCLUDE_DIR}
    ${PNG_INCLUDE_DIR})

# add executable to make and files to make it from
ADD_EXECUTABLE(${EXE} ${SOURCES})

# set compile and link flags
SET_SOURCE_FILES_PROPERTIES(${SOURCES} PROPERTIES COMPILE_FLAGS ${COMPILE_FLAGS})
IF(${LINK_FLAGS})
    SET_TARGET_PROPERTIES(${EXE} PROPERTIES LINK_FLAGS ${LINK_FLAGS})
ENDIF(${LINK_FLAGS})

# add libraries to link
LINK_DIRECTORIES(${GLEW_LIBRARY_DIR} ${PNG_LIBRARY})
TARGET_LINK_LIBRARIES(${EXE} 3DMagic ${GLUT_LIBRARIES} ${GLEW_LIBRARY} 
    ${OPENGL_LIBRARIES} ${BULLET_LIBRARIES} ${LIB3DS_LIBRARY} ${PNG_LIBRARIES} m)

# add dependency to 3dmagic library
ADD_DEPENDENCIES(${EXE} 3DMagic)
















