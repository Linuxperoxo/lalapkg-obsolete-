CXX = g++
CXXFLAGS = -Wall -O3 -march=native -Wextra -lconfig++ -std=c++17

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

SOURCES = $(wildcard $(SRC_DIR)/*.cc)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cc=$(OBJ_DIR)/%.o)

EXECUTABLE = $(BIN_DIR)/lalapkg

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJECTS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cc
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: all clean
