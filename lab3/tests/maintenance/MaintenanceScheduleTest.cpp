#include <gtest/gtest.h>
#include <memory>
#include <chrono>
#include "../../include/maintenance/MaintenanceSchedule.h"
#include "../../include/core/Property.h"
#include "../../include/maintenance/MaintenanceTask.h"
#include "../../include/maintenance/MaintenanceRequest.h"
#include "../../include/core/Address.h"
#include "../../include/core/PropertyStatus.h"
#include "../../include/core/PropertyType.h"

class MaintenanceScheduleTest : public ::testing::Test {
protected:
    std::shared_ptr<Property> property;
    std::shared_ptr<MaintenanceTask> task1;
    std::shared_ptr<MaintenanceTask> task2;
    std::vector<std::shared_ptr<MaintenanceTask>> tasks;
    std::chrono::system_clock::time_point start;
    std::chrono::system_clock::time_point end;

    void SetUp() override {
        start = std::chrono::system_clock::now();
        end = start + std::chrono::hours(24*10);
        auto address = std::make_unique<Address>();
        auto status = std::make_unique<PropertyStatus>("AVAILABLE");
        auto type = std::make_unique<PropertyType>();
        property = std::make_shared<Property>("prop1", 100.0, 1000.0, std::move(address), std::move(status), std::move(type));

        auto req = std::make_shared<MaintenanceRequest>();
        task1 = std::make_shared<MaintenanceTask>("t1", req, "desc1", "team1", start, end, end, "pending", 5.0, 0.0, "normal", std::vector<std::string>{"material1"});
        task2 = std::make_shared<MaintenanceTask>("t2", req, "desc2", "team2", start, end, end, "completed", 5.0, 5.0, "normal", std::vector<std::string>{"material2"});
        tasks = {task1, task2};
    }
};

TEST_F(MaintenanceScheduleTest, ConstructorAndGetters) {
    MaintenanceSchedule schedule("sched1", property, "preventive", start, end, "monthly", tasks, 1000.0, 500.0, "active", "teamA");
    EXPECT_EQ(schedule.getScheduleId(), "sched1");
    EXPECT_EQ(schedule.getProperty(), property.get());
    EXPECT_EQ(schedule.getScheduleType(), "preventive");
    EXPECT_EQ(schedule.getStartDate(), start);
    EXPECT_EQ(schedule.getEndDate(), end);
    EXPECT_EQ(schedule.getFrequency(), "monthly");
    EXPECT_EQ(schedule.getScheduledTasks().size(), 2);
    EXPECT_EQ(schedule.getBudgetAllocated(), 1000.0);
    EXPECT_EQ(schedule.getBudgetUsed(), 500.0);
    EXPECT_EQ(schedule.getStatus(), "active");
    EXPECT_EQ(schedule.getAssignedTeam(), "teamA");
}

TEST_F(MaintenanceScheduleTest, CopyAndMoveConstructors) {
    MaintenanceSchedule s1("sched1", property, "preventive", start, end, "monthly", tasks, 1000.0, 500.0, "active", "teamA");
    MaintenanceSchedule s2(s1);
    EXPECT_EQ(s2.getScheduleId(), s1.getScheduleId());
    MaintenanceSchedule s3 = std::move(s1);
    EXPECT_EQ(s3.getScheduleId(), "sched1");
}

TEST_F(MaintenanceScheduleTest, AssignmentOperators) {
    MaintenanceSchedule s1("sched1", property, "preventive", start, end, "monthly", tasks, 1000.0, 500.0, "active", "teamA");
    MaintenanceSchedule s2 = s1;
    EXPECT_EQ(s2.getScheduleId(), "sched1");
    MaintenanceSchedule s3("dummy", property, "routine", start, end, "weekly", {}, 0, 0, "inactive", "");
    s3 = std::move(s1);
    EXPECT_EQ(s3.getScheduleId(), "sched1");
}

TEST_F(MaintenanceScheduleTest, Setters) {
    MaintenanceSchedule schedule("sched1", property, "preventive", start, end, "monthly", tasks, 1000.0, 500.0, "active", "teamA");
    schedule.setStatus("inactive");
    EXPECT_EQ(schedule.getStatus(), "inactive");
    schedule.setBudgetUsed(800.0);
    EXPECT_EQ(schedule.getBudgetUsed(), 800.0);
    schedule.setAssignedTeam("teamB");
    EXPECT_EQ(schedule.getAssignedTeam(), "teamB");
}

TEST_F(MaintenanceScheduleTest, AddTask) {
    MaintenanceSchedule schedule("sched1", property, "preventive", start, end, "monthly", {}, 1000.0, 500.0, "active", "teamA");
    schedule.addTask(task1);
    EXPECT_EQ(schedule.getScheduledTasks().size(), 1);
    EXPECT_EQ(schedule.getScheduledTasks()[0], task1);
}

TEST_F(MaintenanceScheduleTest, IsActive) {
    MaintenanceSchedule schedule("sched1", property, "preventive", start, end, "monthly", tasks, 1000.0, 500.0, "active", "teamA");
    EXPECT_TRUE(schedule.isActive(start + std::chrono::hours(1)));
    schedule.setStatus("inactive");
    EXPECT_FALSE(schedule.isActive(start + std::chrono::hours(1)));
}

TEST_F(MaintenanceScheduleTest, BudgetCalculations) {
    MaintenanceSchedule schedule("sched1", property, "preventive", start, end, "monthly", tasks, 1000.0, 1100.0, "active", "teamA");
    EXPECT_EQ(schedule.calculateBudgetRemaining(), -100.0);
    EXPECT_TRUE(schedule.isOverBudget());
}

TEST_F(MaintenanceScheduleTest, TaskCompletionCalculations) {
    MaintenanceSchedule schedule("sched1", property, "preventive", start, end, "monthly", tasks, 1000.0, 500.0, "active", "teamA");
    EXPECT_EQ(schedule.getCompletedTaskCount(), 1);
    EXPECT_DOUBLE_EQ(schedule.calculateCompletionPercentage(), 50.0);
    EXPECT_FALSE(schedule.requiresBudgetAdjustment());
}

TEST_F(MaintenanceScheduleTest, OverdueTasksAndPendingTasks) {
    MaintenanceSchedule schedule("sched1", property, "preventive", start, end, "monthly", tasks, 1000.0, 500.0, "active", "teamA");
    EXPECT_FALSE(schedule.hasOverdueTasks(start + std::chrono::hours(5)));
    task1->setStatus("pending");
    EXPECT_TRUE(schedule.getPendingTasks().size() >= 1);
}

TEST_F(MaintenanceScheduleTest, AverageTaskDurationAndPreventiveCheck) {
    MaintenanceSchedule schedule("sched1", property, "preventive", start, end, "monthly", tasks, 1000.0, 500.0, "active", "teamA");
    EXPECT_DOUBLE_EQ(schedule.calculateAverageTaskDuration(), 5.0);
    EXPECT_TRUE(schedule.isPreventiveMaintenance());
}
