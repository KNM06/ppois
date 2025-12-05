#include "MarkovMachine.h"
#include "MarkovProgram.h"
#include "MarkovRule.h"

int main(){

    MarkovProgram program_;
    program_.addRule(MarkovRule("|+", "+|", false));
    program_.addRule(MarkovRule("+", "", true));

    MarkovMachine machine;
    machine.loadProgram(program_);
    machine.loadTape("||+|||");

    machine.run();


    return 0;
}