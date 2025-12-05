#include <gtest/gtest.h>
#include <memory>
#include <chrono>

#include "../../include/maintenance/MaintenancePriority.h"

TEST(MaintenancePriorityTest, ConstructorAndGetters) {
    MaintenancePriority pr(
        "P1", "high", "Desc", 12, 5, 1.5, true,
        "Escalate", "plumbing,electrical"
    );

    EXPECT_EQ(pr.getPriorityId(), "P1");
    EXPECT_EQ(pr.getPriorityLevel(), "high");
    EXPECT_EQ(pr.getDescription(), "Desc");
    EXPECT_EQ(pr.getResponseTimeHours(), 12);
    EXPECT_EQ(pr.getResolutionTimeDays(), 5);
    EXPECT_DOUBLE_EQ(pr.getCostMultiplier(), 1.5);
    EXPECT_TRUE(pr.getRequiresEmergencyContact());
    EXPECT_EQ(pr.getEscalationProcedure(), "Escalate");
    EXPECT_EQ(pr.getAffectedSystems(), "plumbing,electrical");
}

TEST(MaintenancePriorityTest, CopyConstructor) {
    MaintenancePriority original(
        "P1", "high", "Desc", 12, 5, 1.5, false,
        "Esc", "electrical"
    );
    MaintenancePriority copy(original);

    EXPECT_EQ(copy.getPriorityId(), original.getPriorityId());
    EXPECT_EQ(copy.getPriorityLevel(), original.getPriorityLevel());
    EXPECT_DOUBLE_EQ(copy.getCostMultiplier(), original.getCostMultiplier());
}

TEST(MaintenancePriorityTest, CopyAssignment) {
    MaintenancePriority p1("A", "low", "D1", 10, 3, 1.2, false, "E1", "none");
    MaintenancePriority p2("B", "medium", "D2", 20, 8, 2.0, true, "E2", "heating");

    p2 = p1;

    EXPECT_EQ(p2.getPriorityId(), "A");
    EXPECT_EQ(p2.getPriorityLevel(), "low");
    EXPECT_EQ(p2.getDescription(), "D1");
}

TEST(MaintenancePriorityTest, MoveConstructor) {
    MaintenancePriority original(
        "X", "high", "Desc", 10, 7, 2.0, true, "Esc", "security"
    );
    std::string savedId = original.getPriorityId();

    MaintenancePriority moved(std::move(original));

    EXPECT_EQ(moved.getPriorityId(), savedId);
}

TEST(MaintenancePriorityTest, MoveAssignment) {
    MaintenancePriority p1("ID", "emergency", "D", 4, 1, 3.0, true, "E", "plumbing");
    MaintenancePriority p2("X", "low", "DD", 40, 20, 1.1, false, "EE", "none");

    std::string savedId = p1.getPriorityId();

    p2 = std::move(p1);

    EXPECT_EQ(p2.getPriorityId(), savedId);
}

TEST(MaintenancePriorityTest, SetterMethods) {
    MaintenancePriority p("A", "low", "D", 10, 5, 1.5, false, "Esc", "none");

    p.setResponseTimeHours(-5);
    p.setResolutionTimeDays(0);
    p.setCostMultiplier(0.5);

    EXPECT_EQ(p.getResponseTimeHours(), 1);
    EXPECT_EQ(p.getResolutionTimeDays(), 1);
    EXPECT_DOUBLE_EQ(p.getCostMultiplier(), 1.0);
}

TEST(MaintenancePriorityTest, EmergencyPriorityLogic) {
    MaintenancePriority p1("A", "emergency", "D", 10, 5, 1.0, false, "E", "none");
    EXPECT_TRUE(p1.isEmergencyPriority());

    MaintenancePriority p2("B", "low", "D", 3, 2, 1.0, false, "E", "none");
    EXPECT_TRUE(p2.isEmergencyPriority());

    MaintenancePriority p3("C", "low", "D", 10, 5, 1.0, false, "E", "none");
    EXPECT_FALSE(p3.isEmergencyPriority());
}

TEST(MaintenancePriorityTest, HighPriorityLogic) {
    MaintenancePriority p1("A", "high", "D", 30, 5, 1.0, false, "E", "none");
    EXPECT_TRUE(p1.isHighPriority());

    MaintenancePriority p2("B", "low", "D", 20, 5, 1.0, false, "E", "none");
    EXPECT_TRUE(p2.isHighPriority());

    MaintenancePriority p3("C", "low", "D", 50, 5, 1.0, false, "E", "none");
    EXPECT_FALSE(p3.isHighPriority());
}

TEST(MaintenancePriorityTest, AdjustedCost) {
    MaintenancePriority p("A", "low", "D", 10, 5, 2.0, false, "E", "none");
    EXPECT_DOUBLE_EQ(p.calculateAdjustedCost(100), 200.0);
}

TEST(MaintenancePriorityTest, RequiresImmediateAttention) {
    MaintenancePriority p1("A", "low", "D", 100, 10, 1.2, true, "E", "none");
    EXPECT_TRUE(p1.requiresImmediateAttention());

    MaintenancePriority p2("A", "emergency", "D", 100, 10, 1.2, false, "E", "none");
    EXPECT_TRUE(p2.requiresImmediateAttention());

    MaintenancePriority p3("A", "low", "D", 50, 10, 1.2, false, "E", "none");
    EXPECT_FALSE(p3.requiresImmediateAttention());
}

TEST(MaintenancePriorityTest, CanBeScheduledLogic) {
    MaintenancePriority p1("A", "low", "D", 50, 3, 1.0, false, "E", "none");
    EXPECT_TRUE(p1.canBeScheduled());

    MaintenancePriority p2("A", "emergency", "D", 2, 3, 1.0, false, "E", "none");
    EXPECT_FALSE(p2.canBeScheduled());

    MaintenancePriority p3("A", "low", "D", 20, 1, 1.0, false, "E", "none");
    EXPECT_FALSE(p3.canBeScheduled());
}

TEST(MaintenancePriorityTest, AffectsCriticalSystems) {
    MaintenancePriority p1("A", "low", "D", 10, 5, 1.0, false, "E", "electrical");
    MaintenancePriority p2("B", "low", "D", 10, 5, 1.0, false, "E", "gardening");

    EXPECT_TRUE(p1.affectsCriticalSystems());
    EXPECT_FALSE(p2.affectsCriticalSystems());
}

TEST(MaintenancePriorityTest, SLACompliance) {
    MaintenancePriority p("A", "low", "D", 10, 5, 1.0, false, "E", "none");

    EXPECT_EQ(p.calculateSLACompliance(5), 100);
    EXPECT_EQ(p.calculateSLACompliance(15), 75);
    EXPECT_EQ(p.calculateSLACompliance(25), 50);
    EXPECT_EQ(p.calculateSLACompliance(40), 0);
}

TEST(MaintenancePriorityTest, RiskLevelLogic) {
    MaintenancePriority p1("A", "low", "D", 3, 5, 1.0, false, "E", "electrical");
    EXPECT_EQ(p1.getRiskLevel(), "critical");

    MaintenancePriority p2("B", "high", "D", 20, 5, 1.0, false, "E", "none");
    EXPECT_EQ(p2.getRiskLevel(), "high");

    MaintenancePriority p3("C", "medium", "D", 30, 5, 1.0, false, "E", "none");
    EXPECT_EQ(p3.getRiskLevel(), "medium");

    MaintenancePriority p4("D", "low", "D", 50, 5, 1.0, false, "E", "none");
    EXPECT_EQ(p4.getRiskLevel(), "low");
}

TEST(MaintenancePriorityTest, RequiresManagementApproval) {
    MaintenancePriority p1("A", "emergency", "D", 4, 5, 1.0, false, "E", "none");
    EXPECT_TRUE(p1.requiresManagementApproval());

    MaintenancePriority p2("B", "low", "D", 12, 5, 3.0, false, "E", "none");
    EXPECT_TRUE(p2.requiresManagementApproval());

    MaintenancePriority p3("C", "low", "D", 12, 5, 1.5, false, "E", "none");
    EXPECT_FALSE(p3.requiresManagementApproval());
}
