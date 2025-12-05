CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -I./include
TARGET = property_management_system
TEST_TARGET = property_management_system_tests
COVERAGE_TARGET = coverage_report

SRC_DIR = src
CORE_DIR = $(SRC_DIR)/core
FINANCE_DIR = $(SRC_DIR)/finance
BUSINESS_DIR = $(SRC_DIR)/business
EXCEPTIONS_DIR = $(SRC_DIR)/exceptions
MAINTENANCE_DIR = $(SRC_DIR)/maintenance
REPORTING_DIR = $(SRC_DIR)/reporting

CORE_SOURCES = $(wildcard $(CORE_DIR)/*.cpp)
FINANCE_SOURCES = $(wildcard $(FINANCE_DIR)/*.cpp)
BUSINESS_SOURCES = $(wildcard $(BUSINESS_DIR)/*.cpp)
EXCEPTIONS_SOURCES = $(wildcard $(EXCEPTIONS_DIR)/*.cpp)
MAINTENANCE_SOURCES = $(wildcard $(MAINTENANCE_DIR)/*.cpp)
REPORTING_SOURCES = $(wildcard $(REPORTING_DIR)/*.cpp)

SOURCES = $(CORE_SOURCES) $(FINANCE_SOURCES) $(BUSINESS_SOURCES) $(EXCEPTIONS_SOURCES) \
          $(MAINTENANCE_SOURCES) $(REPORTING_SOURCES) $(SRC_DIR)/main.cpp

HEADERS = $(wildcard include/core/*.h) $(wildcard include/finance/*.h) \
          $(wildcard include/business/*.h) $(wildcard include/exceptions/*.h) \
          $(wildcard include/maintenance/*.h) $(wildcard include/reporting/*.h)

TEST_DIR = tests
TEST_SOURCES = $(shell find $(TEST_DIR) -name "*.cpp")
TEST_HEADERS = $(HEADERS)

GTEST_DIR = /usr/local
GTEST_INC = -I$(GTEST_DIR)/include
GTEST_LIBS = -lgtest_main -lgtest -lpthread

COVERAGE_FLAGS = -fprofile-arcs -ftest-coverage
COVERAGE_LIBS = -lgcov

$(TARGET): $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES)

$(TEST_TARGET): $(filter-out $(SRC_DIR)/main.cpp, $(SOURCES)) $(TEST_SOURCES) $(TEST_HEADERS)
	$(CXX) $(CXXFLAGS) $(GTEST_INC) -o $(TEST_TARGET) \
		$(filter-out $(SRC_DIR)/main.cpp, $(SOURCES)) $(TEST_SOURCES) $(GTEST_LIBS)

$(TEST_TARGET)_coverage: $(filter-out $(SRC_DIR)/main.cpp, $(SOURCES)) $(TEST_SOURCES) $(TEST_HEADERS)
	$(CXX) $(CXXFLAGS) $(COVERAGE_FLAGS) $(GTEST_INC) -o $(TEST_TARGET)_coverage \
		$(filter-out $(SRC_DIR)/main.cpp, $(SOURCES)) $(TEST_SOURCES) $(GTEST_LIBS) $(COVERAGE_LIBS)

coverage-gcovr: $(TEST_TARGET)_coverage
	./$(TEST_TARGET)_coverage
	@which gcovr > /dev/null 2>&1 || (echo "Installing gcovr..." && sudo apt-get install -y gcovr)
	gcovr --root . --exclude "tests/" --exclude "/usr/" --print-summary
	gcovr --root . --exclude "tests/" --exclude "/usr/" --html-details coverage_gcovr.html
	@echo "Coverage report generated in coverage_gcovr.html"

clean:
	rm -f $(TARGET) $(TEST_TARGET) $(TEST_TARGET)_coverage
	rm -f *.gcno *.gcda *.gcov coverage.info
	rm -rf $(COVERAGE_TARGET) coverage_gcovr.html
	rm -f $(SRC_DIR)/*.gcno $(SRC_DIR)/*.gcda $(SRC_DIR)/*.gcov
	rm -f $(CORE_DIR)/*.gcno $(CORE_DIR)/*.gcda $(CORE_DIR)/*.gcov
	rm -f $(BUSINESS_DIR)/*.gcno $(BUSINESS_DIR)/*.gcda $(BUSINESS_DIR)/*.gcov
	rm -f $(EXCEPTIONS_DIR)/*.gcno $(EXCEPTIONS_DIR)/*.gcda $(EXCEPTIONS_DIR)/*.gcov
	rm -f $(MAINTENANCE_DIR)/*.gcno $(MAINTENANCE_DIR)/*.gcda $(MAINTENANCE_DIR)/*.gcov
	rm -f $(REPORTING_DIR)/*.gcno $(REPORTING_DIR)/*.gcda $(REPORTING_DIR)/*.gcov

clean-all: clean
	rm -f *.gch
	rm -f include/*/*.gch

run: $(TARGET)
	./$(TARGET)

test: $(TEST_TARGET)
	./$(TEST_TARGET)

coverage: coverage-gcovr

.PHONY: clean clean-all run test coverage
