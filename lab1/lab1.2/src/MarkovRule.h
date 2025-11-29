#ifndef MARKOVRULE_H
#define MARKOVRULE_H

#include <string>
#include <iostream>


using namespace std;

class MarkovRule {
private:
    string pattern_;
    string replacement_;
    bool isFinal_;

public:
    MarkovRule(const string& pattern, const string& replacement, bool isFinal = false);

    const string& getPattern() const;
    const string& getReplacement() const;
    bool getIsFinal() const;

    void setPattern(const string& pattern);
    void setReplacement(const string& replacement);
    void setIsFinal(bool isFinal);

    bool operator==(const MarkovRule& otherRule) const;
    friend ostream& operator<<(ostream& out, const MarkovRule& rule);
    friend istream& operator>>(istream& in, MarkovRule& rule);
};

#endif
