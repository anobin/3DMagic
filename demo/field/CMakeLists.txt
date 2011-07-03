cmake_minimum_required(VERSION 2.6)

# set the executable and project name
SET(PROJECT Field)
SET(EXE Field)

# set source files
SET(SOURCES Field.cpp)

# Project name and language
PROJECT(${PROJECT} CXX)

# set include directories
INCLUDE_DIRECTORIES(include ${OPENGL_INCLUDE} ${BULLET_INCLUDE})

# set locations to look for libraries


# add executable to make and files to make it from
ADD_EXECUTABLE(${EXE} ${SOURCES})

# set compile and link flags
SET_SOURCE_FILES_PROPERTIES(${SOURCES} PROPERTIES COMPILE_FLAGS ${COMPILE_FLAGS})
SET_TARGET_PROPERTIES(${EXE} PROPERTIES LINK_FLAGS ${LINK_FLAGS})

# add libraries to link
TARGET_LINK_LIBRARIES(${EXE} 3DMagic X11 glut GLEW GLU BulletDynamics BulletCollision 3ds m)

# add dependency to 3dmagic library
ADD_DEPENDENCIES(${EXE} 3DMagic)
















