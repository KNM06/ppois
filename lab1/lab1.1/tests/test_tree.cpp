#include "gtest/gtest.h"
#include "../src/tree.h"
#include <string>
#include <sstream>

using namespace std;

class TreeTest : public ::testing::Test {
protected:
    void SetUp() override {
        tree = new Tree("hello", "привет");
        tree->addNodeTree("apple", "яблоко");
        tree->addNodeTree("zebra", "зебра");
        tree->addNodeTree("banana", "банан");
    }

    void TearDown() override {
        delete tree;
    }

    Tree* tree;
};

TEST_F(TreeTest, Constructor) {
    Tree node("test", "тест");
    EXPECT_EQ(node.getEnglish(), "test");
    EXPECT_EQ(node.getRussian(), "тест");
    EXPECT_EQ(node.getLeft(), nullptr);
    EXPECT_EQ(node.getRight(), nullptr);
}

TEST_F(TreeTest, CopyConstructor) {
    Tree copy(*tree);
    EXPECT_EQ(copy.getEnglish(), "hello");
    EXPECT_EQ(copy.getRussian(), "привет");
    EXPECT_NE(copy.getLeft(), nullptr);
    EXPECT_NE(copy.getRight(), nullptr);
}

TEST_F(TreeTest, AddNode) {
    Tree* root = new Tree("middle", "середина");
    
    EXPECT_TRUE(root->addNodeTree("apple", "яблоко"));
    EXPECT_TRUE(root->addNodeTree("zebra", "зебра"));
    
    EXPECT_NE(root->getLeft(), nullptr);
    EXPECT_NE(root->getRight(), nullptr);
    EXPECT_EQ(root->getLeft()->getEnglish(), "apple");
    EXPECT_EQ(root->getRight()->getEnglish(), "zebra");
    
    delete root;
}

TEST_F(TreeTest, FindNode) {
    Tree* found = tree->findNode("apple");
    ASSERT_NE(found, nullptr);
    EXPECT_EQ(found->getEnglish(), "apple");
    EXPECT_EQ(found->getRussian(), "яблоко");
    
    Tree* notFound = tree->findNode("nonexistent");
    EXPECT_EQ(notFound, nullptr);
}

TEST_F(TreeTest, FindNodeConst) {
    const Tree* constTree = tree;
    const Tree* found = constTree->findNode("banana");
    ASSERT_NE(found, nullptr);
    EXPECT_EQ(found->getEnglish(), "banana");
}

TEST_F(TreeTest, CountNodes) {
    EXPECT_EQ(tree->countNodeTree(), 4);
    
    Tree single("single", "один");
    EXPECT_EQ(single.countNodeTree(), 1);
}

TEST_F(TreeTest, DeleteNode) {
    Tree* result = tree->deleteNodeTree("banana");
    EXPECT_NE(result, nullptr);
    EXPECT_EQ(result->findNode("banana"), nullptr);
    
    Tree* complexTree = new Tree("middle", "середина");
    complexTree->addNodeTree("apple", "яблоко");
    complexTree->addNodeTree("zebra", "зебра");
    
    Tree* newRoot = complexTree->deleteNodeTree("middle");
    EXPECT_NE(newRoot, nullptr);
    EXPECT_EQ(newRoot->getEnglish(), "zebra");
    
    delete newRoot;
}

TEST_F(TreeTest, PrintTree) {
    stringstream ss;
    tree->printTree(ss);
    
    string output = ss.str();
    EXPECT_NE(output.find("apple - яблоко"), string::npos);
    EXPECT_NE(output.find("hello - привет"), string::npos);
    EXPECT_NE(output.find("zebra - зебра"), string::npos);
}

TEST_F(TreeTest, EqualityOperator) {
    Tree copy(*tree);
    EXPECT_TRUE(*tree == copy);
    
    Tree different("different", "другой");
    EXPECT_FALSE(*tree == different);
}

TEST_F(TreeTest, CharPointerOperations) {
    Tree root("start", "начало");
    
    EXPECT_TRUE(root.addNodeTree("test", "тест"));
    
    Tree* found = root.findNode("test");
    ASSERT_NE(found, nullptr);
    EXPECT_EQ(found->getRussian(), "тест");
    
    Tree* result = root.deleteNodeTree("test");
    EXPECT_NE(result, nullptr);
}

TEST_F(TreeTest, SaveToFile) {
    EXPECT_TRUE(tree->saveToFile("test_tree.txt"));
    
    ifstream file("test_tree.txt");
    EXPECT_TRUE(file.good());
    
    string firstLine;
    getline(file, firstLine);
    EXPECT_FALSE(firstLine.empty());
    file.close();
    
    remove("test_tree.txt");
}