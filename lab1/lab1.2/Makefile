

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
TARGET = MarkovAlgorifm
TEST_TARGET = MarkovAlgorifm_tests
COVERAGE_TARGET = coverage_report

# Основные исходники
SRC_DIR = src
SOURCES = $(SRC_DIR)/MarkovMachine.cpp $(SRC_DIR)/MarkovProgram.cpp $(SRC_DIR)/MarkovRule.cpp $(SRC_DIR)/main.cpp
HEADERS = $(SRC_DIR)/MarkovMachine.h $(SRC_DIR)/MarkovProgram.h $(SRC_DIR)/MarkovRule.h

# Тесты
TEST_DIR = tests
TEST_SOURCES = $(TEST_DIR)/tests_MarkovMachine.cpp $(TEST_DIR)/tests_MarkovProgram.cpp $(TEST_DIR)/tests_MarkovRule.cpp 
TEST_HEADERS = $(SRC_DIR)/MarkovMachine.h $(SRC_DIR)/MarkovProgram.h $(SRC_DIR)/MarkovRule.h

# Google Test флаги
GTEST_DIR = /usr/local
GTEST_LIBS = -lgtest -lgtest_main -lpthread
GTEST_INC = -I$(GTEST_DIR)/include

# Флаги для покрытия
COVERAGE_FLAGS = -fprofile-arcs -ftest-coverage
COVERAGE_LIBS = -lgcov

# Основная программа
$(TARGET): $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES)

# Тесты без покрытия
$(TEST_TARGET): $(SOURCES) $(TEST_SOURCES) $(TEST_HEADERS)
	$(CXX) $(CXXFLAGS) $(GTEST_INC) -o $(TEST_TARGET) \
		$(filter-out $(SRC_DIR)/main.cpp, $(SOURCES)) $(TEST_SOURCES) $(GTEST_LIBS)

# Тесты с покрытием
$(TEST_TARGET)_coverage: $(SOURCES) $(TEST_SOURCES) $(TEST_HEADERS)
	$(CXX) $(CXXFLAGS) $(COVERAGE_FLAGS) $(GTEST_INC) -o $(TEST_TARGET)_coverage \
		$(filter-out $(SRC_DIR)/main.cpp, $(SOURCES)) $(TEST_SOURCES) \
		$(GTEST_LIBS) $(COVERAGE_LIBS)

# Альтернатива с gcovr
coverage-gcovr: $(TEST_TARGET)_coverage
	./$(TEST_TARGET)_coverage
	@which gcovr > /dev/null 2>&1 || (echo "Installing gcovr..." && sudo apt-get install -y gcovr)
	gcovr --root . --exclude "tests/" --exclude "/usr/" --print-summary
	gcovr --root . --exclude "tests/" --exclude "/usr/" --html-details coverage_gcovr.html
	@echo "Coverage report generated in coverage_gcovr.html"


# Очистка
clean:
	rm -f $(TARGET) $(TEST_TARGET) $(TEST_TARGET)_coverage
	rm -f *.gcno *.gcda *.gcov coverage.info
	rm -rf $(COVERAGE_TARGET) coverage_gcovr.html
	rm -f $(SRC_DIR)/*.gcno $(SRC_DIR)/*.gcda $(SRC_DIR)/*.gcov

clean-all: clean
	rm -f *.gch

run: $(TARGET)
	./$(TARGET)

test: $(TEST_TARGET)
	./$(TEST_TARGET)

coverage: coverage-gcovr

debug: $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) -g -o $(TARGET)_debug $(SOURCES)

.PHONY: clean clean-all run test coverage debug
