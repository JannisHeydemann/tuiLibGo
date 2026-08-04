# Compiler and Flags
CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -I./src
AR       := ar rcs

# Directories
SRC_DIR   := src
BUILD_DIR := build
BIN_DIR   := bin
DEMO_DIR  := demo

# Target Static Library (for Cgo)
LIB_NAME   := libctui.a
LIB_TARGET := $(BUILD_DIR)/$(LIB_NAME)

# C++ Source Files (Excluding main.cpp for the library build)
CPP_SRCS := $(filter-out $(SRC_DIR)/main.cpp, $(shell find $(SRC_DIR) -name '*.cpp'))
OBJS     := $(patsubst $(SRC_DIR)/%, $(BUILD_DIR)/%, $(CPP_SRCS:.cpp=.o))

# C++ Standalone Test Executable
CPP_MAIN := $(SRC_DIR)/main.cpp
CPP_BIN  := $(BIN_DIR)/cpp_test

# Phony Targets
.PHONY: all lib cpp_test demo clean rebuild

# Default Target
all: lib cpp_test

# 1. Compile C++ files into Object Files (.o)
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 2. Package Object Files into Static Library (.a)
lib: $(LIB_TARGET)

$(LIB_TARGET): $(OBJS)
	@mkdir -p $(BUILD_DIR)
	$(AR) $@ $^

# 3. Build Standalone C++ Test Executable
cpp_test: $(CPP_BIN)

$(CPP_BIN): $(CPP_MAIN) $(LIB_TARGET)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $< -L$(BUILD_DIR) -lctui -o $@

# 4. Run the Go Demo App
demo: lib
	@cd $(DEMO_DIR) && CGO_LDFLAGS="-L$(shell pwd)/$(BUILD_DIR)" go run main.go

# 5. Clean Build Artifacts
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

# 6. Full Rebuild
rebuild: clean all
