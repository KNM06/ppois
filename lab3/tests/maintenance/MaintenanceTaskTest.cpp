#include <gtest/gtest.h>
#include <chrono>
#include <vector>
#include <string>
#include <memory>
#include "../../include/maintenance/MaintenanceTask.h"
#include "../../include/maintenance/MaintenanceRequest.h"

class MaintenanceTaskTest : public ::testing::Test {
protected:
    std::shared_ptr<MaintenanceRequest> request;
    std::shared_ptr<MaintenanceTask> task;
    std::chrono::system_clock::time_point start;
    std::chrono::system_clock::time_point end;

    void SetUp() override {
        start = std::chrono::system_clock::now();
        end = start + std::chrono::hours(48);
        request = std::make_shared<MaintenanceRequest>();
        task = std::make_shared<MaintenanceTask>(
            "t1", request, "desc", "team1", start, end, end,
            "pending", 5.0, 0.0, "normal", std::vector<std::string>{"material1"}
        );
    }
};

TEST_F(MaintenanceTaskTest, ConstructorAndGetters) {
    EXPECT_EQ(task->getTaskId(), "t1");
    EXPECT_EQ(task->getRequest(), request.get());
    EXPECT_EQ(task->getTaskDescription(), "desc");
    EXPECT_EQ(task->getAssignedTo(), "team1");
    EXPECT_EQ(task->getStartDate(), start);
    EXPECT_EQ(task->getEstimatedCompletion(), end);
    EXPECT_EQ(task->getActualCompletion(), end);
    EXPECT_EQ(task->getStatus(), "pending");
    EXPECT_DOUBLE_EQ(task->getEstimatedHours(), 5.0);
    EXPECT_DOUBLE_EQ(task->getActualHours(), 0.0);
    EXPECT_EQ(task->getPriority(), "normal");
    EXPECT_EQ(task->getRequiredMaterials().size(), 1);
}

TEST_F(MaintenanceTaskTest, CopyAndMoveConstructors) {
    MaintenanceTask copyTask(*task);
    EXPECT_EQ(copyTask.getTaskId(), "t1");
    MaintenanceTask movedTask(std::move(copyTask));
    EXPECT_EQ(movedTask.getTaskId(), "t1");
}

TEST_F(MaintenanceTaskTest, AssignmentOperators) {
    MaintenanceTask copyTask("dummy", request, "", "", start, end, end, "pending", 0.0, 0.0, "", {});
    copyTask = *task;
    EXPECT_EQ(copyTask.getTaskId(), "t1");
    MaintenanceTask movedTask("dummy2", request, "", "", start, end, end, "pending", 0.0, 0.0, "", {});
    movedTask = std::move(copyTask);
    EXPECT_EQ(movedTask.getTaskId(), "t1");
}

TEST_F(MaintenanceTaskTest, Setters) {
    task->setStatus("completed");
    EXPECT_EQ(task->getStatus(), "completed");
    task->setAssignedTo("team2");
    EXPECT_EQ(task->getAssignedTo(), "team2");
    auto newTime = start + std::chrono::hours(10);
    task->setActualCompletion(newTime);
    EXPECT_EQ(task->getActualCompletion(), newTime);
    task->setActualHours(8.0);
    EXPECT_DOUBLE_EQ(task->getActualHours(), 8.0);
    task->setActualHours(-5.0);
    EXPECT_DOUBLE_EQ(task->getActualHours(), 0.0);
}

TEST_F(MaintenanceTaskTest, TaskStatusChecks) {
    task->setStatus("pending");
    EXPECT_FALSE(task->isCompleted());
    task->setStatus("completed");
    EXPECT_TRUE(task->isCompleted());
    task->setStatus("pending");
    EXPECT_TRUE(task->isOverdue(end + std::chrono::hours(1)));
    EXPECT_FALSE(task->isOverdue(start));
}

TEST_F(MaintenanceTaskTest, TaskCalculations) {
    task->setStatus("completed");
    task->setActualCompletion(end + std::chrono::hours(5));
    EXPECT_DOUBLE_EQ(task->calculateTimeVariance(), 5.0);
    task->setActualHours(10.0);
    EXPECT_DOUBLE_EQ(task->calculateLaborCost(20.0), 200.0);
    EXPECT_DOUBLE_EQ(task->calculateEfficiency(), 50.0);
    EXPECT_EQ(task->getDaysToComplete(), 3);  
}

TEST_F(MaintenanceTaskTest, PriorityAndDelegation) {
    task->setStatus("pending");
    EXPECT_FALSE(task->isHighPriority());
    task->setStatus("pending");
    EXPECT_TRUE(task->canBeDelegated());
    task->setActualHours(0.0);
    EXPECT_DOUBLE_EQ(task->calculateEfficiency(), 0.0);
}

TEST_F(MaintenanceTaskTest, SpecializedTools) {
    std::vector<std::string> materials = {"specialized tool"};
    task = std::make_shared<MaintenanceTask>("t2", request, "desc", "team1", start, end, end, "pending", 5.0, 0.0, "normal", materials);
    EXPECT_TRUE(task->requiresSpecializedTools());
}
