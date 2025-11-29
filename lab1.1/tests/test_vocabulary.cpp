#include "gtest/gtest.h"
#include "../src/vocabulary.h"
#include <string>
#include <sstream>
#include <fstream>

using namespace std;

class VocabularyTest : public ::testing::Test {
protected:
    void SetUp() override {
        vac += {"hello", "привет"};
        vac += {"world", "мир"};
        vac += {"apple", "яблоко"};
    }

    Vocabulary vac;
};

TEST_F(VocabularyTest, Constructors) {
    Vocabulary empty;
    EXPECT_EQ(empty.getRoot(), nullptr);
    
    Vocabulary copy(vac);
    EXPECT_FALSE(copy == empty);
}

TEST_F(VocabularyTest, AssignmentOperator) {
    Vocabulary assigned;
    assigned = vac;
    EXPECT_TRUE(assigned == vac);
    
    assigned = assigned;
    EXPECT_TRUE(assigned == vac);
}

TEST_F(VocabularyTest, AddWords) {
    Vocabulary testVac;
    testVac += make_pair("test", "тест");
    string word = testVac["test"];
    EXPECT_EQ(word, "тест");
}

TEST_F(VocabularyTest, RemoveWords) {
    EXPECT_EQ(vac["apple"], "яблоко");
    
    vac -= "apple";
    EXPECT_EQ(vac["apple"], "");
    
    vac -= "nonexistent";
    EXPECT_EQ(vac["nonexistent"], "");
}

TEST_F(VocabularyTest, RemoveCharPointer) {
    vac -= "world";
    EXPECT_EQ(vac["world"], "");
}

TEST_F(VocabularyTest, SearchOperator) {
    const Vocabulary& constVac = vac;
    EXPECT_EQ(constVac["hello"], "привет");
    EXPECT_EQ(constVac["nonexistent"], "");
    EXPECT_EQ(vac["new_word"], "");
}

TEST_F(VocabularyTest, SearchOperatorCharPointer) {
    EXPECT_EQ(vac["hello"], "привет");
    EXPECT_EQ(vac["nonexistent_char"], "");
}

TEST_F(VocabularyTest, ComparisonOperators) {
    Vocabulary same;
    same += {"hello", "привет"};
    same += {"world", "мир"};
    same += {"apple", "яблоко"};
    
    EXPECT_TRUE(vac == same);
    EXPECT_FALSE(vac != same);
    
    Vocabulary different;
    different += {"different", "другой"};
    
    EXPECT_FALSE(vac == different);
    EXPECT_TRUE(vac != different);
}

TEST_F(VocabularyTest, OutputOperator) {
    stringstream ss;
    ss << vac;
    
    string output = ss.str();
    EXPECT_NE(output.find("Англо-русский словарь:"), string::npos);
    EXPECT_NE(output.find("hello - привет"), string::npos);
}

TEST_F(VocabularyTest, InputOperator) {
    stringstream ss;
    ss << "cat кот\n";
    ss << "dog собака\n";
    ss << "house дом\n";
    
    Vocabulary newVac;
    ss >> newVac;
    
    EXPECT_EQ(newVac["cat"], "кот");
    EXPECT_EQ(newVac["dog"], "собака");
    EXPECT_EQ(newVac["house"], "дом");
}

TEST_F(VocabularyTest, LoadFromFile) {
    ofstream file("test_vocab.txt");
    file << "red красный\n";
    file << "blue синий\n";
    file << "green зеленый\n";
    file.close();
    
    Vocabulary fileVac;
    EXPECT_TRUE(fileVac.loadFromFile("test_vocab.txt"));
    
    EXPECT_EQ(fileVac["red"], "красный");
    EXPECT_EQ(fileVac["blue"], "синий");
    EXPECT_EQ(fileVac["green"], "зеленый");
    
    remove("test_vocab.txt");
}

TEST_F(VocabularyTest, SaveToFile) {
    Vocabulary testVac;
    testVac += {"cat", "кот"};
    testVac += {"dog", "собака"};
    
    EXPECT_TRUE(testVac.saveToFile("test_save.txt"));
    
    ifstream checkFile("test_save.txt");
    EXPECT_TRUE(checkFile.good());
    checkFile.close();
    
    Vocabulary loaded;
    EXPECT_TRUE(loaded.loadFromFile("test_save.txt"));
    
    EXPECT_EQ(loaded["cat"], "кот");
    EXPECT_EQ(loaded["dog"], "собака");
    
    remove("test_save.txt");
}

TEST_F(VocabularyTest, EmptyVocabulary) {
    Vocabulary empty;
    
    stringstream ss;
    ss << empty;
    string output = ss.str();
    EXPECT_NE(output.find("Словарь пуст"), string::npos);
    
    EXPECT_EQ(empty["any"], "");
    empty -= "any"; 
}

TEST_F(VocabularyTest, EdgeCases) {
    Vocabulary empty;
    
    empty -= "word";
    EXPECT_EQ(empty.getRoot(), nullptr);
    
    empty += {"first", "первый"};
    EXPECT_NE(empty.getRoot(), nullptr);
    
    empty -= "first";
    EXPECT_EQ(empty.getRoot(), nullptr);
}
