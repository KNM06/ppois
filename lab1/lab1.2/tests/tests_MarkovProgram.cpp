#include <gtest/gtest.h>
#include "../src/MarkovProgram.h"

TEST(MarkovProgramTest, DefaultConstructor) {
    MarkovProgram program;
    
    EXPECT_EQ(program.getRuleCount(), 0);
}

TEST(MarkovProgramTest, AddRule) {
    MarkovProgram program;
    MarkovRule rule1("a", "b");
    MarkovRule rule2("c", "d");
    
    program.addRule(rule1);
    program.addRule(rule2);
    
    EXPECT_EQ(program.getRuleCount(), 2);
    EXPECT_EQ(program.getRule(0).getPattern(), "a");
    EXPECT_EQ(program.getRule(1).getPattern(), "c");
}

TEST(MarkovProgramTest, AddDuplicateRule) {
    MarkovProgram program;
    MarkovRule rule1("a", "b");
    MarkovRule rule2("a", "b"); // Дубликат
    
    program.addRule(rule1);
    program.addRule(rule2); // Не должен добавиться
    
    EXPECT_EQ(program.getRuleCount(), 1);
}

TEST(MarkovProgramTest, RemoveRuleByIndex) {
    MarkovProgram program;
    program.addRule(MarkovRule("a", "b"));
    program.addRule(MarkovRule("c", "d"));
    program.addRule(MarkovRule("e", "f"));
    
    program.removeRule(1); // Удаляем второй элемент
    
    EXPECT_EQ(program.getRuleCount(), 2);
    EXPECT_EQ(program.getRule(0).getPattern(), "a");
    EXPECT_EQ(program.getRule(1).getPattern(), "e");
}

TEST(MarkovProgramTest, RemoveRuleByIndexOutOfBounds) {
    MarkovProgram program;
    program.addRule(MarkovRule("a", "b"));
    
    // Не должно быть ошибки при выходе за границы
    program.removeRule(5);
    program.removeRule(-1);
    
    EXPECT_EQ(program.getRuleCount(), 1);
}

TEST(MarkovProgramTest, RemoveRuleByPattern) {
    MarkovProgram program;
    program.addRule(MarkovRule("a", "b"));
    program.addRule(MarkovRule("c", "d"));
    program.addRule(MarkovRule("e", "f"));
    
    bool removed = program.removeRule("c", "d");
    
    EXPECT_TRUE(removed);
    EXPECT_EQ(program.getRuleCount(), 2);
    EXPECT_EQ(program.getRule(0).getPattern(), "a");
    EXPECT_EQ(program.getRule(1).getPattern(), "e");
}

TEST(MarkovProgramTest, RemoveRuleByPatternNotFound) {
    MarkovProgram program;
    program.addRule(MarkovRule("a", "b"));
    
    bool removed = program.removeRule("x", "y");
    
    EXPECT_FALSE(removed);
    EXPECT_EQ(program.getRuleCount(), 1);
}

TEST(MarkovProgramTest, ModifyRule) {
    MarkovProgram program;
    program.addRule(MarkovRule("a", "b", false));
    program.addRule(MarkovRule("c", "d", true));
    
    bool modified = program.modifyRule(0, "x", "y", true);
    
    EXPECT_TRUE(modified);
    EXPECT_EQ(program.getRule(0).getPattern(), "x");
    EXPECT_EQ(program.getRule(0).getReplacement(), "y");
    EXPECT_TRUE(program.getRule(0).getIsFinal());
    // Второе правило не должно измениться
    EXPECT_EQ(program.getRule(1).getPattern(), "c");
}

TEST(MarkovProgramTest, ModifyRuleInvalidIndex) {
    MarkovProgram program;
    program.addRule(MarkovRule("a", "b"));
    
    bool modified = program.modifyRule(5, "x", "y", true);
    
    EXPECT_FALSE(modified);
    EXPECT_EQ(program.getRule(0).getPattern(), "a"); // Правило не изменилось
}

TEST(MarkovProgramTest, GetRuleOutOfBounds) {
    MarkovProgram program;
    program.addRule(MarkovRule("a", "b"));
    
    // Должно бросить исключение при неверном индексе
    EXPECT_THROW(program.getRule(5), std::out_of_range);
}

TEST(MarkovProgramTest, OutputOperator) {
    MarkovProgram program;
    program.addRule(MarkovRule("a", "b", false));
    program.addRule(MarkovRule("c", "d", true));
    
    std::ostringstream oss;
    oss << program;
    
    std::string output = oss.str();
    EXPECT_NE(output.find("a -> b"), std::string::npos);
    EXPECT_NE(output.find("c -> d [FINAL]"), std::string::npos);
    EXPECT_EQ(output.find("MarkovProgram"), std::string::npos);
}

TEST(MarkovProgramTest, InputOperator) {
    stringstream iss("a -> b\nc -> d [FINAL]\n\n");
    
    MarkovProgram program;
    iss >> program;
    
    EXPECT_EQ(program.getRuleCount(), 2);
    EXPECT_EQ(program.getRule(0).getPattern(), "a");
    EXPECT_EQ(program.getRule(0).getReplacement(), "b");
    EXPECT_FALSE(program.getRule(0).getIsFinal());
    EXPECT_EQ(program.getRule(1).getPattern(), "c");
    EXPECT_EQ(program.getRule(1).getReplacement(), "d");
    EXPECT_TRUE(program.getRule(1).getIsFinal());
}

TEST(MarkovProgramTest, InputOperatorEmpty) {
    stringstream iss("");
    
    MarkovProgram program;
    program.addRule(MarkovRule("existing", "rule"));
    
    iss >> program; // Должен очистить правила
    
    EXPECT_EQ(program.getRuleCount(), 0);
}
