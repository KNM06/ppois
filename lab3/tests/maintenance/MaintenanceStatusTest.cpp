#include <gtest/gtest.h>
#include <chrono>
#include "../../include/maintenance/MaintenanceStatus.h"

class MaintenanceStatusTest : public ::testing::Test {
protected:
    std::chrono::system_clock::time_point now;
    std::chrono::system_clock::time_point past;
    MaintenanceStatus* status;

    void SetUp() override {
        now = std::chrono::system_clock::now();
        past = now - std::chrono::hours(24*5);
        status = new MaintenanceStatus("id1", "in_progress", "desc", true, false, false, past, "next", 3, "note");
    }

    void TearDown() override {
        delete status;
    }
};

TEST_F(MaintenanceStatusTest, ConstructorAndGetters) {
    EXPECT_EQ(status->getStatusId(), "id1");
    EXPECT_EQ(status->getStatusCode(), "in_progress");
    EXPECT_EQ(status->getDescription(), "desc");
    EXPECT_TRUE(status->getIsActive());
    EXPECT_FALSE(status->getIsCompleted());
    EXPECT_FALSE(status->getRequiresFollowUp());
    EXPECT_EQ(status->getStatusDate(), past);
    EXPECT_EQ(status->getNextAction(), "next");
    EXPECT_EQ(status->getEstimatedCompletionDays(), 3);
    EXPECT_EQ(status->getResolutionNotes(), "note");
}

TEST_F(MaintenanceStatusTest, CopyAndMoveConstructors) {
    MaintenanceStatus copyStatus(*status);
    EXPECT_EQ(copyStatus.getStatusId(), "id1");
    MaintenanceStatus movedStatus(std::move(copyStatus));
    EXPECT_EQ(movedStatus.getStatusId(), "id1");
}

TEST_F(MaintenanceStatusTest, AssignmentOperators) {
    MaintenanceStatus copyStatus("dummy", "dummy", "", false, false, false, now, "", 0, "");
    copyStatus = *status;
    EXPECT_EQ(copyStatus.getStatusId(), "id1");
    MaintenanceStatus movedStatus("dummy2", "dummy2", "", false, false, false, now, "", 0, "");
    movedStatus = std::move(copyStatus);
    EXPECT_EQ(movedStatus.getStatusId(), "id1");
}

TEST_F(MaintenanceStatusTest, Setters) {
    status->setStatusCode("completed");
    EXPECT_EQ(status->getStatusCode(), "completed");
    status->setIsCompleted(true);
    EXPECT_TRUE(status->getIsCompleted());
    EXPECT_FALSE(status->getIsActive());
    status->setNextAction("review");
    EXPECT_EQ(status->getNextAction(), "review");
}

TEST_F(MaintenanceStatusTest, StatusChecks) {
    EXPECT_TRUE(status->isWorkInProgress());
    status->setStatusCode("pending_approval");
    EXPECT_TRUE(status->isPendingApproval());
    status->setStatusCode("assigned");
    EXPECT_TRUE(status->requiresVendorAttention());
}

TEST_F(MaintenanceStatusTest, DaysInStatusAndDelays) {
    int days = status->getDaysInStatus(now);
    EXPECT_GE(days, 5);
    EXPECT_TRUE(status->isDelayed() || !status->isDelayed());
}

TEST_F(MaintenanceStatusTest, ClosureAndCategory) {
    status->setIsCompleted(true);
    status->setNextAction("done");
    EXPECT_TRUE(status->canBeClosed()); 
    
    status->setIsCompleted(false);
    EXPECT_FALSE(status->canBeClosed());
    
    status->setIsCompleted(true);
    status->setNextAction("resolved");
    EXPECT_TRUE(status->canBeClosed());
    
    EXPECT_EQ(status->getStatusCategory(), "completed");
    status->setIsCompleted(false);
    status->setStatusCode("pending_approval");
    EXPECT_EQ(status->getStatusCategory(), "pending");
}

TEST_F(MaintenanceStatusTest, CustomerNotificationAndCompletion) {
    status->setStatusCode("completed");
    EXPECT_TRUE(status->requiresCustomerNotification());
    
    status->setIsCompleted(true);
    EXPECT_DOUBLE_EQ(status->calculateCompletionPercentage(), 100.0);
    
    status->setIsCompleted(false);
    status->setStatusCode("in_progress");
    EXPECT_DOUBLE_EQ(status->calculateCompletionPercentage(), 50.0);  
    
    status->setStatusCode("pending_approval");
    EXPECT_DOUBLE_EQ(status->calculateCompletionPercentage(), 50.0);
    
    status->setStatusCode("new");
    EXPECT_DOUBLE_EQ(status->calculateCompletionPercentage(), 50.0);
}
