#ifndef VOCABULARY_H
#define VOCABULARY_H

#include "tree.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class Vocabulary{
    private:
    Tree* root_;
    public:
    Vocabulary();
    Vocabulary(const Vocabulary& copiedVocabulary);
    ~Vocabulary();
    Tree* getRoot() const;
    void setRoot(Tree* root);
    Vocabulary& operator=(const Vocabulary& other);
    Vocabulary& operator+=(const pair<string, string>& pairWords);
    Vocabulary& operator-=(const string& english);
    string operator[](const string& english) const;
    string& operator[](const string& english);
    bool operator==(const Vocabulary& other) const;
    bool operator!=(const Vocabulary& other) const;
    friend ostream& operator<<(ostream& out, const Vocabulary& vocabulary);
    friend istream& operator>>(istream& in, Vocabulary& vocabulary);
    void printVocabulary(ostream& out) const;
    bool loadFromFile(const string& filename);
    bool saveToFile(const string& filename) const;


};

#endif 