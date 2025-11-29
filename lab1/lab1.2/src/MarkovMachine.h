#ifndef MARKOVMACHINE_H
#define MARKOVMACHINE_H

#include "MarkovProgram.h"
#include <fstream>

using namespace std;

class MarkovMachine {
private:
    string tape_;
    MarkovProgram program_;
    int currentStep_;

public:
    MarkovMachine();

    void loadProgram(const MarkovProgram& program);
    void loadTape(const string& tape);
    const string& getTape() const;
    int getCurrentStep() const;
    const MarkovProgram& getProgram() const;
    void setCurrentStep(int currentStep);
    bool step();  
    void run(bool log = false);   
    
    void loadFromFile(const string& filename);
    void saveToFile(const string& filename) const;

    friend ostream& operator<<(ostream& out, const MarkovMachine& machine);
    friend istream& operator>>(istream& in, MarkovMachine& machine);
};

#endif
