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

# Track Header Files so .hpp changes trigger recompilation
HEADERS  := $(shell find $(SRC_DIR) -name '*.hpp' -o -name '*.h')

# C++ Source Files (Excluding main.cpp for the library build)
CPP_SRCS := $(filter-out $(SRC_DIR)/main.cpp, $(shell find $(SRC_DIR) -name '*.cpp'))
OBJS     := $(patsubst $(SRC_DIR)/%, $(BUILD_DIR)/%, $(CPP_SRCS:.cpp=.o))

# C++ Standalone Test Executable (optional - src/main.cpp is a local test
# harness that may not exist; skip cpp_test gracefully when it's absent)
CPP_MAIN := $(wildcard $(SRC_DIR)/main.cpp)
CPP_BIN  := $(BIN_DIR)/cpp_test

# C++ Demo Executable
DEMO_SRC := $(DEMO_DIR)/demo.cpp
DEMO_BIN := $(BIN_DIR)/demo

# C++ Unit Tests (GoogleTest, linked against libctui.a)
TEST_DIR  := tests
TEST_SRCS := $(wildcard $(TEST_DIR)/*.cpp)
TEST_BIN  := $(BIN_DIR)/unit_tests

# Phony Targets
.PHONY: all lib cpp_test demo test clean rebuild

# Default Target
all: lib
ifneq ($(strip $(CPP_MAIN)),)
all: cpp_test
endif

# 1. Compile C++ files into Object Files (.o) - now depends on HEADERS!
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADERS)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 2. Package Object Files into Static Library (.a)
lib: $(LIB_TARGET)

$(LIB_TARGET): $(OBJS)
	@mkdir -p $(BUILD_DIR)
	$(AR) $@ $^

# 3. Build Standalone C++ Test Executable
cpp_test: $(CPP_BIN)

ifneq ($(strip $(CPP_MAIN)),)
$(CPP_BIN): $(CPP_MAIN) $(LIB_TARGET)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $< -L$(BUILD_DIR) -lctui -o $@
else
$(CPP_BIN):
	@echo "Skipping cpp_test: $(SRC_DIR)/main.cpp not found."
endif

# 4. Build & Run the GoogleTest Unit Suite
test: $(TEST_BIN)
	./$(TEST_BIN)

$(TEST_BIN): $(TEST_SRCS) $(HEADERS) $(LIB_TARGET)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(TEST_SRCS) -L$(BUILD_DIR) -lctui -lgtest -lgtest_main -lpthread -o $@

# 5. Build & Run the Demo App
demo: $(DEMO_BIN)
	./$(DEMO_BIN)

$(DEMO_BIN): $(DEMO_SRC) $(LIB_TARGET)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $< -L$(BUILD_DIR) -lctui -o $@

# 6. Clean Build Artifacts & Go Cgo Cache
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

# 7. Full Rebuild
rebuild: clean all
