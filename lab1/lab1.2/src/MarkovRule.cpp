#include "MarkovRule.h"

MarkovRule::MarkovRule(const string& pattern, const string& replacement, bool isFinal)
    : pattern_(pattern), replacement_(replacement), isFinal_(isFinal) {}

const string& MarkovRule::getPattern() const { 
    return pattern_; 
}

const string& MarkovRule::getReplacement() const { 
    return replacement_; 
}

bool MarkovRule::getIsFinal() const { 
    return isFinal_; 
}

void MarkovRule::setPattern(const string& pattern) { 
    pattern_ = pattern; 
}

void MarkovRule::setReplacement(const string& replacement) { 
    replacement_ = replacement; 
}

void MarkovRule::setIsFinal(bool isFinal) { 
    isFinal_ = isFinal; 
}
bool MarkovRule::operator==(const MarkovRule& otherRule) const {
    return  getPattern() == otherRule.getPattern() && getReplacement() == otherRule.getReplacement();
}

ostream& operator<<(ostream& out, const MarkovRule& rule) {
    out << rule.getPattern() << " -> " << rule.getReplacement();
    if (rule.getIsFinal()) {
        out << " [FINAL]";
    }
    return out;
}

istream& operator>>(istream& in, MarkovRule& rule) {
    string line;
    if (getline(in, line)) {
        size_t arrowPos = line.find("->");
        if (arrowPos != string::npos) {
            rule.setPattern(line.substr(0, arrowPos));
            rule.pattern_.erase(0, rule.pattern_.find_first_not_of(" \t"));
            rule.pattern_.erase(rule.pattern_.find_last_not_of(" \t") + 1);
            
            string replacementPart = line.substr(arrowPos + 2);
            size_t finalPos = replacementPart.find("[FINAL]");
            if (finalPos != string::npos) {
                rule.setIsFinal(true);
                replacementPart = replacementPart.substr(0, finalPos);
            } else {
                rule.setIsFinal(false);
            }
            
            replacementPart.erase(0, replacementPart.find_first_not_of(" \t"));
            replacementPart.erase(replacementPart.find_last_not_of(" \t") + 1);
            rule.setReplacement(replacementPart);
        }
    }
    return in;
}
