#include "MarkovMachine.h"

MarkovMachine::MarkovMachine() : currentStep_(0) {}

void MarkovMachine::loadProgram(const MarkovProgram& program) {
    program_ = program;
}

void MarkovMachine::loadTape(const string& tape) {
    tape_ = tape;
    setCurrentStep(0);
}

const string& MarkovMachine::getTape() const {
    return tape_;
}

int MarkovMachine::getCurrentStep() const {
    return currentStep_;
}

const MarkovProgram& MarkovMachine::getProgram() const {
    return program_;
}

void MarkovMachine::setCurrentStep(int currentStep) {
    currentStep_ = currentStep;
}

bool MarkovMachine::step() {
    for (int i = 0; i < getProgram().getRuleCount(); ++i) {
        const MarkovRule& rule = getProgram().getRule(i);
        int pos = getTape().find(rule.getPattern());
        if (pos != string::npos) {
            tape_.replace(pos, rule.getPattern().length(), rule.getReplacement());
            setCurrentStep(getCurrentStep()+1);
            return !rule.getIsFinal();
        }
    }
    return false; 
}

void MarkovMachine::run(bool log) {
    if(log){
        while (step()) {
            cout << *this << endl;
        }
        cout << *this << endl;
    } else {
        while (step()) {}
        cout << *this << endl;
    }
}

void MarkovMachine::loadFromFile(const string& filename) {
    ifstream file(filename);
    if (file.is_open()) {
        file >> *this;
        file.close();
    }
}

void MarkovMachine::saveToFile(const string& filename) const {
    ofstream file(filename);
    if (file.is_open()) {
        file << program_ << endl;
        file << tape_ << endl;
        file.close();
    }
}

ostream& operator<<(ostream& out, const MarkovMachine& machine) {
    out << "Step: " << machine.getCurrentStep() << ", Tape: " << machine.getTape();
    return out;
}

istream& operator>>(istream& in, MarkovMachine& machine) {
    in >> machine.program_;
    
    string line;
    while (getline(in, line)) {
        if (!line.empty()) {
            machine.loadTape(line);
            break;
        }
    }
    
    return in;
}