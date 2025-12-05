#ifndef TREE_H
#define TREE_H
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class Tree{
    private:
    string english_;
    string russian_;
    Tree* left_;
    Tree* right_;
    public:
    Tree(const string& english, const string& russian);
    Tree(const Tree& copiedTree);
    ~Tree();
    string getEnglish() const;
    string getRussian() const;
    Tree* getLeft() const;
    Tree* getRight() const;
    void setEnglish(const string& english);
    void setRussian(const string& russian);
    void setLeft(Tree* left);
    void setRight(Tree* right);
    bool addNodeTree(const string& english, const string& russian);
    Tree* deleteNodeTree(const string& english);
    Tree* findNode(const string& english);
    const Tree* findNode(const string& english) const;
    int countNodeTree() const;
    void printTree(ostream& out) const;
    bool operator==(const Tree& other) const;
    bool saveToFile(const string& filename) const;
    void saveToOut(ostream& out) const;

};


#endif