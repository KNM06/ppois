CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -I./include
TARGET = rental_system
TEST_TARGET = rental_system_tests

SRC_DIR = src
CORE_DIR = $(SRC_DIR)/core
ITEMS_DIR = $(SRC_DIR)/items
BUSINESS_DIR = $(SRC_DIR)/business
EXCEPTIONS_DIR = $(SRC_DIR)/exceptions
UTILS_DIR = $(SRC_DIR)/utils

CORE_SOURCES = $(wildcard $(CORE_DIR)/*.cpp)
ITEMS_SOURCES = $(wildcard $(ITEMS_DIR)/*.cpp)
BUSINESS_SOURCES = $(wildcard $(BUSINESS_DIR)/*.cpp)
EXCEPTIONS_SOURCES = $(wildcard $(EXCEPTIONS_DIR)/*.cpp)
UTILS_SOURCES = $(wildcard $(UTILS_DIR)/*.cpp)

SOURCES = $(CORE_SOURCES) $(ITEMS_SOURCES) $(BUSINESS_SOURCES) $(EXCEPTIONS_SOURCES) $(UTILS_SOURCES) $(SRC_DIR)/main.cpp

TEST_DIR = tests
TEST_SOURCES = $(wildcard $(TEST_DIR)/*/*.cpp) $(wildcard $(TEST_DIR)/*.cpp)

GTEST_LIBS = -lgtest -lgtest_main -lpthread

COVERAGE_FLAGS = -fprofile-arcs -ftest-coverage
COVERAGE_LIBS = -lgcov

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES)

$(TEST_TARGET): $(filter-out $(SRC_DIR)/main.cpp, $(SOURCES)) $(TEST_SOURCES)
	$(CXX) $(CXXFLAGS) -o $(TEST_TARGET) \
		$(filter-out $(SRC_DIR)/main.cpp, $(SOURCES)) $(TEST_SOURCES) $(GTEST_LIBS)

$(TEST_TARGET)_coverage: $(filter-out $(SRC_DIR)/main.cpp, $(SOURCES)) $(TEST_SOURCES)
	$(CXX) $(CXXFLAGS) $(COVERAGE_FLAGS) -o $(TEST_TARGET)_coverage \
		$(filter-out $(SRC_DIR)/main.cpp, $(SOURCES)) $(TEST_SOURCES) \
		$(GTEST_LIBS) $(COVERAGE_LIBS)

coverage: $(TEST_TARGET)_coverage
	./$(TEST_TARGET)_coverage
	@which gcovr > /dev/null 2>&1 || (echo "Installing gcovr..." && sudo apt-get install -y gcovr)
	gcovr --root . --exclude "tests/" --exclude "/usr/" --print-summary
	gcovr --root . --exclude "tests/" --exclude "/usr/" --html-details coverage_report.html
	@echo "Coverage report generated in coverage_report.html"

run: $(TARGET)
	./$(TARGET)

test: $(TEST_TARGET)
	./$(TEST_TARGET)

clean:
	rm -f $(TARGET) $(TEST_TARGET) $(TEST_TARGET)_coverage
	rm -f *.gcno *.gcda *.gcov coverage.info
	rm -rf coverage_report coverage_report.html
	find $(SRC_DIR) -name "*.gcno" -delete
	find $(SRC_DIR) -name "*.gcda" -delete
	find $(SRC_DIR) -name "*.gcov" -delete

.PHONY: clean run test coverage