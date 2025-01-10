# Compiler
CC = clang

# Souce files
SRC_DIR = src
SRC = $(SRC_DIR)/main.c
RESOURCE_DIR = resources

# Output
OUT_DIR = bin
OUT = $(OUT_DIR)/dothack-game-workshop

# Library and framework paths
LIB_PATH = lib_mac/
LIBS = -L $(LIB_PATH) -lraylib
FRAMEWORKS = -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL

# Flags
FLAGS = -DDEBUG=1

# Check OS and adjust settings accordingly
ifeq ($(OS), Windows_NT)
	CC = gcc
	FRAMEWORKS = -lgdi32 -lwinmm
	LIB_PATH = lib_win/
endif


# Build the project
build:
	echo "Building project..."
	mkdir -p $(OUT_DIR)
	$(CC) $(FLAGS) $(SRC) $(LIBS) $(FRAMEWORKS) -o $(OUT)
	echo "Replacing resources..."
	rm -rf $(OUT_DIR)/$(RESOURCE_DIR)
	cp -r $(RESOURCE_DIR) $(OUT_DIR)

build-release:
	echo "Building project in release mode..."
	$(MAKE) FLAGS="-DDEBUG=0" build

clean:
	echo "Cleaning project output..."
	rm -rf $(OUT_DIR)
	rm -rf $(OUT_DIR)/$(RESOURCE_DIR)

run:
	make clean
	make build
	echo "Running project..."
	./$(OUT)

run-release:
	make clean
	make build-release
	echo "Running project in release mode..."
	./$(OUT)

