

CXX = g++
CXXFLAGS=-g -Wall -I./include -I/usr/include/GL -I/usr/include/bullet
PACKER=ar rcs

# collect all source in src folder
SRC=$(wildcard ./src/*.cpp ./src/*/*.cpp ./src/*/*/*.cpp)
OBJ=$(subst .cpp,.o,$(SRC))

# collect all headers
HEADS=$(wildcard ./include/*.h ./include/*/*.h ./include/*/*/*.h)

# output 
OUTPUT_DIR=./
PACKAGE=lib3DMagic.a



#default build, normal build
all : normal

# implicit compiling rule for .cc files
%.o: %.cc
	$(CXX) $(CXXFLAGS) -I $(<D) -c $< -o $@

# implicit compiling rule for .cpp files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -I $(<D) -c $< -o $@


########################
#### normal builds
########################
# list all files under makefile's supervison
list:
	@echo "######## Source #########"
	@echo $(SRC) | sed -e "s/\s\+/\n/g"
	@echo ""
	@echo "######## Headers ########"
	@echo $(HEADS) | sed -e "s/\s\+/\n/g"

normal: package

# make static library
package: $(OBJ) $(HEADS)
	${PACKER} $(OUTPUT_DIR)/${PACKAGE} $(OBJ)
	@echo "package placed at ${OUTPUT_DIR}/${PACKAGE}"



###################
### phony targets
###################
.PHONY: clean
clean:
	rm -f $(OBJ) 2> /dev/null > /dev/null
	rm -f $(OUTPUT_DIR)/${PACKAGE}
	

.PHONY: love
love:
	@echo "not war?"








