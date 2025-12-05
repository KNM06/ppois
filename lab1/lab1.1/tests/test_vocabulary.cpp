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

// Тест конструкторов
TEST_F(VocabularyTest, Constructors) {
    Vocabulary empty;
    EXPECT_EQ(empty.getRoot(), nullptr);
    
    Vocabulary copy(vac);
    EXPECT_FALSE(copy == empty);
}

// Тест оператора присваивания
TEST_F(VocabularyTest, AssignmentOperator) {
    Vocabulary assigned;
    assigned = vac;
    EXPECT_TRUE(assigned == vac);
    
    // Самоприсваивание
    assigned = assigned;
    EXPECT_TRUE(assigned == vac);
}

// Тест добавления слов
TEST_F(VocabularyTest, AddWords) {
    Vocabulary testVac;
    
    // Добавление парой string
    testVac += make_pair("test", "тест");
    string word = testVac["test"];
    EXPECT_EQ(word, "тест");
}


// Тест удаления слов
TEST_F(VocabularyTest, RemoveWords) {
    EXPECT_EQ(vac["apple"], "яблоко");
    
    vac -= "apple";
    EXPECT_EQ(vac["apple"], "");
    
    // Удаление несуществующего слова
    vac -= "nonexistent";
    EXPECT_EQ(vac["nonexistent"], "");
}

// Тест удаления char*
TEST_F(VocabularyTest, RemoveCharPointer) {
    vac -= "world";
    EXPECT_EQ(vac["world"], "");
}

// Тест оператора [] (поиск)
TEST_F(VocabularyTest, SearchOperator) {
    // Const версия
    const Vocabulary& constVac = vac;
    EXPECT_EQ(constVac["hello"], "привет");
    EXPECT_EQ(constVac["nonexistent"], "");
    
    // Неconst версия (создает если нет)
    EXPECT_EQ(vac["new_word"], "");
    // Теперь слово должно добавиться
    EXPECT_EQ(vac["new_word"], "");
}

// Тест оператора [] для char*
TEST_F(VocabularyTest, SearchOperatorCharPointer) {
    EXPECT_EQ(vac["hello"], "привет");
    EXPECT_EQ(vac["nonexistent_char"], "");
}

// Тест сравнения словарей
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

// Тест вывода в поток
TEST_F(VocabularyTest, OutputOperator) {
    stringstream ss;
    ss << vac;
    
    string output = ss.str();
    EXPECT_NE(output.find("Англо-русский словарь:"), string::npos);
    EXPECT_NE(output.find("hello - привет"), string::npos);
}

// Тест ввода из потока
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

// Тест загрузки из файла
TEST_F(VocabularyTest, LoadFromFile) {
    // Создаем тестовый файл
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
    
    // Очистка
    remove("test_vocab.txt");
}

// Тест сохранения в файл
TEST_F(VocabularyTest, SaveToFile) {
    // Создаем простой словарь для теста
    Vocabulary testVac;
    testVac += {"cat", "кот"};
    testVac += {"dog", "собака"};
    
    EXPECT_TRUE(testVac.saveToFile("test_save.txt"));
    
    // Проверяем что файл создан
    ifstream checkFile("test_save.txt");
    EXPECT_TRUE(checkFile.good());
    checkFile.close();
    
    // Загружаем в новый словарь
    Vocabulary loaded;
    EXPECT_TRUE(loaded.loadFromFile("test_save.txt"));
    
    // Проверяем содержимое
    EXPECT_EQ(loaded["cat"], "кот");
    EXPECT_EQ(loaded["dog"], "собака");
    
    // Очистка
    remove("test_save.txt");
}

// Тест пустого словаря
TEST_F(VocabularyTest, EmptyVocabulary) {
    Vocabulary empty;
    
    stringstream ss;
    ss << empty;
    string output = ss.str();
    EXPECT_NE(output.find("Словарь пуст"), string::npos);
    
    EXPECT_EQ(empty["any"], "");
    empty -= "any"; // Не должно падать
}

// Тест граничных случаев
TEST_F(VocabularyTest, EdgeCases) {
    Vocabulary empty;
    
    // Операции с пустым словарем
    empty -= "word";
    EXPECT_EQ(empty.getRoot(), nullptr);
    
    // Добавление в пустой словарь
    empty += {"first", "первый"};
    EXPECT_NE(empty.getRoot(), nullptr);
    
    // Удаление единственного элемента
    empty -= "first";
    EXPECT_EQ(empty.getRoot(), nullptr);
}