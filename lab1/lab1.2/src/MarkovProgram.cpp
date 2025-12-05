#include "MarkovProgram.h"

void MarkovProgram::addRule(const MarkovRule& rule) {
    for (const auto& currentRule : rules_) {
        if (currentRule == rule) {
            return;
        }
    }
    rules_.push_back(rule);
}

void MarkovProgram::removeRule(size_t index) {
    if (index < getRuleCount()) {
        rules_.erase(rules_.begin() + index);
    }
}

bool MarkovProgram::removeRule(const string& pattern, const string& replacement) {
    for (auto rule = rules_.begin(); rule != rules_.end(); ++rule) {
        if (rule->getPattern() == pattern && rule->getReplacement() == replacement) {
            rules_.erase(rule);
            return true;
        }
    }
    return false;
}

const MarkovRule& MarkovProgram::getRule(size_t index) const {
    return rules_.at(index);
}

int MarkovProgram::getRuleCount() const {
    return rules_.size();
}

bool MarkovProgram::modifyRule(size_t index, const string& pattern, const string& result, bool isFinal){
    if (index >= getRuleCount()) {
        return false;
    }
    rules_[index] = MarkovRule(pattern, result, isFinal);
    return true;
}

ostream& operator<<(ostream& out, const MarkovProgram& program) {
    for (size_t i = 0; i < program.getRuleCount(); ++i) {
        out << program.rules_[i] << endl;
    }
    return out;
}

istream& operator>>(istream& in, MarkovProgram& program) {
    program.rules_.clear(); 
    
    string line;    
    while (getline(in, line) && !line.empty()) {
        MarkovRule rule("", "");
        istringstream(line) >> rule;
        program.addRule(rule);
    }
    return in;
}
