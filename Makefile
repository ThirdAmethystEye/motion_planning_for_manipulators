FLAGS = -O3 -I include # -mavx -pthread -Wl,-rpath,'$$ORIGIN'
LIBS = -lmujoco -lglfw
CXX = g++
OBJ = obj
SRC = src
INC = include
TARGET = simulator

SOURCES = $(OBJ)/utils.o $(OBJ)/joint_state.o $(OBJ)/planner.o $(OBJ)/astar.o $(OBJ)/solution.o
INCLUDES = $(INC)/utils.h $(INC)/joint_state.h $(INC)/planner.h $(INC)/astar.h $(INC)/solution.h

.PHONY: all clean testing simulator

all: $(TARGET) tests/tests

clean:
	rm -rf $(OBJ)
	rm -f $(TARGET)

testing: tests/tests

simulator: $(TARGET)

$(TARGET): $(SOURCES) $(INCLUDES) $(OBJ)/main.o
	$(CXX) $(SOURCES) $(OBJ)/main.o $(LIBS) -o $(TARGET)

tests/tests: $(SOURCES) $(INCLUDES) $(OBJ)/catch_amalgamated.o tests/main.cpp
	$(CXX) $(FLAGS) $(SOURCES) $(OBJ)/catch_amalgamated.o tests/main.cpp $(LIBS) -o tests/tests

# compile commands
$(OBJ)/main.o: $(SRC)/main.cpp $(INCLUDES)
	mkdir -p $(OBJ)
	$(CXX) $(FLAGS) $(SRC)/main.cpp $(LIBS) -c -o $(OBJ)/main.o

$(OBJ)/joint_state.o: $(SRC)/joint_state.cpp $(INCLUDES)
	mkdir -p $(OBJ)
	$(CXX) $(FLAGS) $(SRC)/joint_state.cpp $(LIBS) -c -o $(OBJ)/joint_state.o

$(OBJ)/planner.o: $(SRC)/planner.cpp $(INCLUDES)
	mkdir -p $(OBJ)
	$(CXX) $(FLAGS) $(SRC)/planner.cpp $(LIBS) -c -o $(OBJ)/planner.o

$(OBJ)/astar.o: $(SRC)/astar.cpp $(INCLUDES)
	mkdir -p $(OBJ)
	$(CXX) $(FLAGS) $(SRC)/astar.cpp $(LIBS) -c -o $(OBJ)/astar.o

$(OBJ)/solution.o: $(SRC)/solution.cpp $(INCLUDES)
	mkdir -p $(OBJ)
	$(CXX) $(FLAGS) $(SRC)/solution.cpp $(LIBS) -c -o $(OBJ)/solution.o

$(OBJ)/utils.o: $(SRC)/utils.cpp $(INCLUDES)
	mkdir -p $(OBJ)
	$(CXX) $(FLAGS) $(SRC)/utils.cpp $(LIBS) -c -o $(OBJ)/utils.o

# test "lib" catch2
$(OBJ)/catch_amalgamated.o: tests/catch2/catch_amalgamated.cpp include/catch2/catch_amalgamated.hpp
	mkdir -p $(OBJ)
	$(CXX) $(FLAGS) tests/catch2/catch_amalgamated.cpp -c -o $(OBJ)/catch_amalgamated.o
