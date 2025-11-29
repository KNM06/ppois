#ifndef MARKOVPROGRAM_H
#define MARKOVPROGRAM_H
#include "MarkovRule.h"
#include <vector>
#include <sstream>
using namespace std;

class MarkovProgram {
private:
    vector<MarkovRule> rules_;

public:
    void addRule(const MarkovRule& rule);
    void removeRule(size_t index);
    bool removeRule(const string& pattern, const string& replacement);
    const MarkovRule& getRule(size_t index) const;
    int getRuleCount() const;
    bool modifyRule(size_t index, const string& pattern, const string& result, bool isFinal);
    friend ostream& operator<<(ostream& out, const MarkovProgram& program);
    friend istream& operator>>(istream& in, MarkovProgram& program);
};

#endif
