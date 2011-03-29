# Makefile for field demo
#
CXX=g++
CXXFLAGS= -Wall -g -I./ -I../../include -I/usr/include/GL -I/usr/include/bullet -DM3D_MATH_USE_GENERIC
LDFLAGS=-L/usr/lib -L/usr/X11R6/lib -L/usr/X11R6/lib64 -L/usr/local/lib -L../../ -l3DMagic -lX11 -lglut -lGLEW -lGL -lGLU -lBulletDynamics -lBulletCollision -l3ds -lm

SRC=$(wildcard ./*.cpp)
HEADS=$(wildcard ./*.h)

EXE=./field
OBJ=$(subst .cpp,.o,$(SRC)) 

# default target
all: Field

# list all files under makefile's supervison
list:
	@echo "######## Source #########"
	@echo $(SRC) | sed -e "s/\s\+/\n/g"
	@echo "######## Headers ########"
	@echo $(HEADS) | sed -e "s/\s\+/\n/g"

Field: $(OBJ)
	$(CXX) -g $(OBJ) -o $(EXE) $(LDFLAGS) 

# implicit compiling rule for .cc file
%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@


.PHONY: clean
clean:
	rm $(OBJ)
	rm ${EXE}

.PHONY: love
love:
	@echo "not war?"
