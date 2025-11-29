#include <gtest/gtest.h>
#include "../src/MarkovMachine.h"
#include "../src/MarkovProgram.h"
#include <sstream>
#include <fstream>
#include <filesystem>

class MarkovMachineTest : public ::testing::Test {
protected:
    void SetUp() override {
        program_.addRule(MarkovRule("|+", "+|", false));
        program_.addRule(MarkovRule("+", "", true));
        program_.addRule(MarkovRule("a", "b", false));
    }
    MarkovProgram program_;
};

TEST_F(MarkovMachineTest, Constructor) {
    MarkovMachine machine;
    
    EXPECT_TRUE(machine.getTape().empty());
    EXPECT_EQ(machine.getCurrentStep(), 0);
    EXPECT_EQ(machine.getProgram().getRuleCount(), 0);
}

TEST_F(MarkovMachineTest, LoadProgram) {
    MarkovMachine machine;
    machine.loadProgram(program_);
    
    EXPECT_EQ(machine.getProgram().getRuleCount(), 3);
}

TEST_F(MarkovMachineTest, LoadTape) {
    MarkovMachine machine;
    machine.loadTape("test tape");
    
    EXPECT_EQ(machine.getTape(), "test tape");
    EXPECT_EQ(machine.getCurrentStep(), 0);
}

TEST_F(MarkovMachineTest, StepWithNoRules) {
    MarkovMachine machine;
    machine.loadTape("test");
    
    bool shouldContinue = machine.step();
    
    EXPECT_FALSE(shouldContinue);
    EXPECT_EQ(machine.getCurrentStep(), 0);
    EXPECT_EQ(machine.getTape(), "test"); 
}

TEST_F(MarkovMachineTest, StepWithMatchingRule) {
    MarkovMachine machine;
    machine.loadProgram(program_);
    machine.loadTape("||+||");
    
    bool shouldContinue = machine.step();
    
    EXPECT_TRUE(shouldContinue);
    EXPECT_EQ(machine.getCurrentStep(), 1);
    EXPECT_EQ(machine.getTape(), "|+|||");
}

TEST_F(MarkovMachineTest, StepWithFinalRule) {
    MarkovMachine machine;
    machine.loadProgram(program_);
    machine.loadTape("+");
    
    bool shouldContinue = machine.step();
    
    EXPECT_FALSE(shouldContinue); 
    EXPECT_EQ(machine.getCurrentStep(), 1);
    EXPECT_EQ(machine.getTape(), "");
}

TEST_F(MarkovMachineTest, StepNoMatchingRule) {
    MarkovMachine machine;
    machine.loadProgram(program_);
    machine.loadTape("xyz");
    
    bool shouldContinue = machine.step();
    
    EXPECT_FALSE(shouldContinue);
    EXPECT_EQ(machine.getCurrentStep(), 0);
    EXPECT_EQ(machine.getTape(), "xyz");
}

TEST_F(MarkovMachineTest, RunWithoutLogging) {
    MarkovMachine machine;
    machine.loadProgram(program_);
    machine.loadTape("||+||");
    
    machine.run(false);
    
    EXPECT_EQ(machine.getTape(), "||||");
    EXPECT_GT(machine.getCurrentStep(), 0);
}

TEST_F(MarkovMachineTest, RunWithLogging) {
    MarkovMachine machine;
    machine.loadProgram(program_);
    machine.loadTape("a");
    
    testing::internal::CaptureStdout();
    machine.run(true);
    string output = testing::internal::GetCapturedStdout();
    
    EXPECT_EQ(machine.getTape(), "b");
}

TEST_F(MarkovMachineTest, SetCurrentStep) {
    MarkovMachine machine;
    machine.setCurrentStep(5);
    
    EXPECT_EQ(machine.getCurrentStep(), 5);
}

TEST_F(MarkovMachineTest, OutputOperator) {
    MarkovMachine machine;
    machine.loadProgram(program_);
    machine.loadTape("test");
    machine.setCurrentStep(3);
    
    ostringstream oss;
    oss << machine;
    
    EXPECT_EQ(oss.str(), "Step: 3, Tape: test");
}

TEST_F(MarkovMachineTest, InputOperator) {
    stringstream iss("a -> b\nc -> d [FINAL]\n\ntest tape\n");
    
    MarkovMachine machine;
    iss >> machine;
    
    EXPECT_EQ(machine.getProgram().getRuleCount(), 2);
    EXPECT_EQ(machine.getTape(), "test tape");
    EXPECT_EQ(machine.getCurrentStep(), 0);
}


TEST_F(MarkovMachineTest, SaveAndLoadFile) {
    MarkovMachine machine;
    
    MarkovProgram Program;
    Program.addRule(MarkovRule("a", "b", false));
    Program.addRule(MarkovRule("b", "c", true));
    
    machine.loadProgram(Program);
    machine.loadTape("saved tape");
    
    const std::string filename = "test_machine.txt";
    
    // Сохраняем
    machine.saveToFile(filename);
    
    // Загружаем в новую машину
    MarkovMachine loadedMachine;
    loadedMachine.loadFromFile(filename);
    
    // Проверяем что загрузилось корректно
    EXPECT_EQ(loadedMachine.getProgram().getRuleCount(), 2);
    EXPECT_EQ(loadedMachine.getTape(), "saved tape");
    EXPECT_EQ(loadedMachine.getCurrentStep(), 0);
    remove(filename.c_str());
}

TEST_F(MarkovMachineTest, LoadFromNonexistentFile) {
    MarkovMachine machine;
    
    machine.loadFromFile("nonexistent_file.txt");
    
    EXPECT_TRUE(machine.getTape().empty());
    EXPECT_EQ(machine.getProgram().getRuleCount(), 0);
}

TEST_F(MarkovMachineTest, ComplexComputation) {
    MarkovProgram program;
    program.addRule(MarkovRule("|+", "+|", false));
    program.addRule(MarkovRule("+", "", true));
    
    MarkovMachine machine;
    machine.loadProgram(program);
    machine.loadTape("||+|||"); 
    
    machine.run(false);
    
    EXPECT_EQ(machine.getTape(), "|||||"); 
    EXPECT_EQ(machine.getCurrentStep(), 3);
}
