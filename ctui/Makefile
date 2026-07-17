CXX       := g++
AR        := ar
CXXFLAGS  := -std=c++17 -Wall -Wextra -I.
BUILD_DIR := $(CURDIR)/build

LIB       := $(BUILD_DIR)/libctui.a
TEST_BIN  := $(BUILD_DIR)/ctui_test

# Library sources = every .cpp EXCEPT the test harness (main.cpp).
# Picks up top-level files and one level of subdirs (e.g. mainMenu/mainMenu.cpp).
LIB_SRCS  := $(filter-out main.cpp,$(wildcard *.cpp) $(wildcard */*.cpp))
LIB_OBJS  := $(LIB_SRCS:%.cpp=$(BUILD_DIR)/%.o)
MAIN_OBJ  := $(BUILD_DIR)/main.o

.PHONY: all lib test run clean
all: lib

# The static library — this is what the Go wrapper links against.
lib: $(LIB)

$(LIB): $(LIB_OBJS)
	$(AR) rcs $@ $(LIB_OBJS)

# Standalone test executable = main.cpp + all library sources.
test: $(TEST_BIN)

$(TEST_BIN): $(MAIN_OBJ) $(LIB_OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

run: test
	$(TEST_BIN)

# Generic compile rule; mirrors subdirs under build/ (build/mainMenu/mainMenu.o).
$(BUILD_DIR)/%.o: %.cpp | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)
