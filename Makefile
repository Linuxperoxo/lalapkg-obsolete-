CXX = g++ -std=c++17 -v
CXXFLAGS = -Wall -O3 -march=native -pipe -pedantic -Wextra
LDFLAGS= -lconfig++

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

SOURCES = $(wildcard $(SRC_DIR)/*.cc)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cc=$(OBJ_DIR)/%.o)

EXECUTABLE = $(BIN_DIR)/lalapkg

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJECTS) $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cc
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: all clean
