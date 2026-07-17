CXX       := g++
AR        := ar
CXXFLAGS  := -std=c++17 -Wall -Wextra -I.
GTEST     := -lgtest -lgtest_main -lpthread
BUILD_DIR := $(CURDIR)/build
LIB       := $(BUILD_DIR)/libctui.a
TEST_BIN  := $(BUILD_DIR)/ctui_test

# Library sources = every .cpp EXCEPT the test harness (main.cpp).
LIB_SRCS  := $(filter-out main.cpp,$(wildcard *.cpp) $(wildcard */*.cpp))
LIB_OBJS  := $(LIB_SRCS:%.cpp=$(BUILD_DIR)/%.o)
MAIN_OBJ  := $(BUILD_DIR)/main.o

# GoogleTest sources live under tests/ (e.g. tests/foo_test.cpp).
GTEST_SRCS := $(wildcard tests/*.cpp)
GTEST_OBJS := $(GTEST_SRCS:%.cpp=$(BUILD_DIR)/%.o)

.PHONY: all lib test run clean

all: lib

# The static library — this is what the Go wrapper links against.
lib: $(LIB)
$(LIB): $(LIB_OBJS)
	$(AR) rcs $@ $(LIB_OBJS)

# Test executable = gtest sources + all library objects (NO main.o).
test: $(TEST_BIN)
$(TEST_BIN): $(GTEST_OBJS) $(LIB_OBJS)
	$(CXX) $(CXXFLAGS) $^ $(GTEST) -o $@

run: test
	$(TEST_BIN)

# Generic compile rule; mirrors subdirs under build/.
$(BUILD_DIR)/%.o: %.cpp | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)
