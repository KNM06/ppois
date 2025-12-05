#include <gtest/gtest.h>
#include "../src/MarkovRule.h"

TEST(MarkovRuleTest, ConstructorAndGetters) {
    MarkovRule rule("abc", "def", true);
    
    EXPECT_EQ(rule.getPattern(), "abc");
    EXPECT_EQ(rule.getReplacement(), "def");
    EXPECT_TRUE(rule.getIsFinal());
}

TEST(MarkovRuleTest, DefaultFinalFlag) {
    MarkovRule rule("test", "result");
    
    EXPECT_EQ(rule.getPattern(), "test");
    EXPECT_EQ(rule.getReplacement(), "result");
    EXPECT_FALSE(rule.getIsFinal());
}

TEST(MarkovRuleTest, Setters) {
    MarkovRule rule("old", "old", false);
    
    rule.setPattern("new_pattern");
    rule.setReplacement("new_replacement");
    rule.setIsFinal(true);
    
    EXPECT_EQ(rule.getPattern(), "new_pattern");
    EXPECT_EQ(rule.getReplacement(), "new_replacement");
    EXPECT_TRUE(rule.getIsFinal());
}

TEST(MarkovRuleTest, EqualityOperator) {
    MarkovRule rule1("abc", "def", true);
    MarkovRule rule2("abc", "def", true);
    MarkovRule rule3("xyz", "def", true);
    MarkovRule rule4("abc", "xyz", true);
    MarkovRule rule5("abc", "def", false);
    
    EXPECT_TRUE(rule1 == rule2);
    EXPECT_FALSE(rule1 == rule3);
    EXPECT_FALSE(rule1 == rule4);
    EXPECT_TRUE(rule1 == rule5); // Ваша реализация игнорирует isFinal
}

TEST(MarkovRuleTest, OutputOperator) {
    MarkovRule rule1("hello", "world", false);
    MarkovRule rule2("test", "final", true);
    
    ostringstream oss1, oss2;
    oss1 << rule1;
    oss2 << rule2;
    
    EXPECT_EQ(oss1.str(), "hello -> world");
    EXPECT_EQ(oss2.str(), "test -> final [FINAL]");
}

TEST(MarkovRuleTest, InputOperator) {
    stringstream iss1("hello -> world");
    stringstream iss2("test -> result [FINAL]");
    stringstream iss3("  spaced  ->  text  ");
    
    MarkovRule rule1("", "");
    MarkovRule rule2("", "");
    MarkovRule rule3("", "");
    
    iss1 >> rule1;
    iss2 >> rule2;
    iss3 >> rule3;
    
    EXPECT_EQ(rule1.getPattern(), "hello");
    EXPECT_EQ(rule1.getReplacement(), "world");
    EXPECT_FALSE(rule1.getIsFinal());
    
    EXPECT_EQ(rule2.getPattern(), "test");
    EXPECT_EQ(rule2.getReplacement(), "result");
    EXPECT_TRUE(rule2.getIsFinal());
    
    EXPECT_EQ(rule3.getPattern(), "spaced");
    EXPECT_EQ(rule3.getReplacement(), "text");
    EXPECT_FALSE(rule3.getIsFinal());
}

TEST(MarkovRuleTest, InputOperatorInvalidFormat) {
    stringstream iss("invalid format without arrow");
    MarkovRule rule("initial", "initial", true);
    
    iss >> rule;
    
    // Правило не должно измениться при неверном формате
    EXPECT_EQ(rule.getPattern(), "initial");
    EXPECT_EQ(rule.getReplacement(), "initial");
    EXPECT_TRUE(rule.getIsFinal());
}