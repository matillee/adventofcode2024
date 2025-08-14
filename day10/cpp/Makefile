CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -Wpedantic -Werror -g -O2 -fstack-protector-strong -Iinclude
SRC_DIR = cpp
TEST_DIR = tests
BUILD_DIR = build

TARGET = $(BUILD_DIR)/hiking_guide
SRCS = $(SRC_DIR)/main.cpp $(SRC_DIR)/hiking_guide.cpp $(SRC_DIR)/manager.cpp
OBJS = $(BUILD_DIR)/main.o $(BUILD_DIR)/hiking_guide.o $(BUILD_DIR)/manager.o

TEST_TARGET = $(BUILD_DIR)/hiking_guide_test
TEST_SRCS = $(TEST_DIR)/hiking_guide_test.cpp $(SRC_DIR)/hiking_guide.cpp $(SRC_DIR)/manager.cpp
TEST_OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(filter $(SRC_DIR)/%.cpp,$(TEST_SRCS))) \
            $(patsubst $(TEST_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(filter $(TEST_DIR)/%.cpp,$(TEST_SRCS)))

all: $(TARGET)

test: $(TEST_TARGET) ; ./$(TEST_TARGET)

$(BUILD_DIR): ; mkdir -p $(BUILD_DIR)

# Building the main target
$(TARGET): $(OBJS) | $(BUILD_DIR) ; $(CXX) $(CXXFLAGS) -o $@ $^
$(OBJS) : $(SRC_DIR)/hiking_guide.hpp
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR) ; $(CXX) $(CXXFLAGS) -c $< -o $@

# Building the test target
$(TEST_TARGET): $(TEST_OBJS) | $(BUILD_DIR) ; $(CXX) $(CXXFLAGS) -I cpp -o $@ $^ -lgtest -lgtest_main -pthread 
$(TEST_DIR)/hiking_guide_test.cpp: $(SRC_DIR)/hiking_guide.hpp
$(BUILD_DIR)/%.o: $(TEST_DIR)/%.cpp | $(BUILD_DIR) ; $(CXX) $(CXXFLAGS) -I cpp -c $< -o $@

clean: ; rm -rf $(BUILD_DIR)

.PHONY: all clean test

