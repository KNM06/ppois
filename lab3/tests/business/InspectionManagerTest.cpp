#include <gtest/gtest.h>
#include <memory>
#include <chrono>
#include "../../include/business/InspectionManager.h"
#include "../../include/core/Property.h"
#include "../../include/core/Tenant.h"
#include "../../include/core/Address.h"
#include "../../include/core/PropertyStatus.h"
#include "../../include/core/PropertyType.h"

class InspectionManagerTest : public ::testing::Test {
protected:
    std::shared_ptr<Property> property;
    std::shared_ptr<Tenant> tenant;
    std::vector<std::string> checklist;
    std::chrono::system_clock::time_point now;

    void SetUp() override {
        auto address = std::make_unique<Address>("123 Main St", "City", "State", "12345");
        auto status = std::make_unique<PropertyStatus>(
            "AVAILABLE", "Property is available", true, false, false,
            std::chrono::system_clock::now(), "", 0.0, 0
        );
        auto type = std::make_unique<PropertyType>(
            "APT", "residential", "Apartment", 1, 5, 50.0, 200.0, false, "residential", 1.5
        );

        property = std::make_shared<Property>("prop1", 100.0, 1000.0,
                                              std::move(address),
                                              std::move(status),
                                              std::move(type));

        auto tenantAddress = std::make_unique<Address>("456 Elm St", "City", "State", "12345");
        tenant = std::make_shared<Tenant>("ten1", "John", "Doe", "john@example.com",
                                          "1234567890", std::move(tenantAddress),
                                          700, 3000, false, 1);

        checklist = {"doors", "windows", "plumbing"};
        now = std::chrono::system_clock::now();
    }
};

TEST_F(InspectionManagerTest, ConstructorAndGetters) {
    InspectionManager manager(30, 70.0, checklist);
    EXPECT_EQ(manager.getInspectionFrequencyDays(), 30);
    EXPECT_DOUBLE_EQ(manager.getPassThreshold(), 70.0);
    EXPECT_EQ(manager.getInspectionChecklist(), checklist);
}

TEST_F(InspectionManagerTest, SettersAndAddChecklistItem) {
    InspectionManager manager(30, 70.0, checklist);
    manager.setInspectionFrequencyDays(45);
    EXPECT_EQ(manager.getInspectionFrequencyDays(), 45);
    manager.setPassThreshold(80.0);
    EXPECT_DOUBLE_EQ(manager.getPassThreshold(), 80.0);
    manager.addChecklistItem("roof");
    auto newChecklist = checklist;
    newChecklist.push_back("roof");
    EXPECT_EQ(manager.getInspectionChecklist(), newChecklist);
}

TEST_F(InspectionManagerTest, ScheduleAndDueInspection) {
    InspectionManager manager(30, 70.0, checklist);
    
    auto pastInspectionDate = now - std::chrono::hours(24*31);
    EXPECT_TRUE(manager.scheduleInspection(property, pastInspectionDate, tenant));
    
    EXPECT_TRUE(manager.isInspectionDue(property, now));
    
    auto fifteenDaysAgo = now - std::chrono::hours(24*15);
    EXPECT_FALSE(manager.isInspectionDue(property, fifteenDaysAgo));
}

TEST_F(InspectionManagerTest, ConductInspection) {
    InspectionManager manager(30, 70.0, checklist);
    manager.scheduleInspection(property, now, tenant);
    std::vector<std::string> results = {"pass", "fail", "satisfactory"};
    double score = manager.conductInspection(property, results);
    EXPECT_DOUBLE_EQ(score, (2.0/3.0)*100.0);
}

TEST_F(InspectionManagerTest, OverdueInspections) {
    InspectionManager manager(30, 70.0, checklist);
    manager.scheduleInspection(property, now - std::chrono::hours(24*31), tenant);
    auto overdue = manager.getOverdueInspections(now);
    EXPECT_EQ(overdue.size(), 1);
    EXPECT_EQ(overdue[0]->getId(), "prop1");
}

TEST_F(InspectionManagerTest, RequiresFollowUp) {
    InspectionManager manager(30, 70.0, checklist);
    EXPECT_TRUE(manager.requiresFollowUp(50.0));
    EXPECT_FALSE(manager.requiresFollowUp(80.0));
}

TEST_F(InspectionManagerTest, DaysUntilNextInspection) {
    InspectionManager manager(30, 70.0, checklist);
    manager.scheduleInspection(property, now, tenant);
    EXPECT_LE(manager.getDaysUntilNextInspection(property, now), 30);
}

TEST_F(InspectionManagerTest, GenerateInspectionReport) {
    InspectionManager manager(30, 70.0, checklist);
    manager.scheduleInspection(property, now, tenant);
    std::string report = manager.generateInspectionReport(property, 80.0);
    EXPECT_NE(report.find("PASS"), std::string::npos);
    report = manager.generateInspectionReport(property, 60.0);
    EXPECT_NE(report.find("FAIL"), std::string::npos);
}

TEST_F(InspectionManagerTest, PropertyConditionScoreAndCompliance) {
    InspectionManager manager(30, 70.0, checklist);
    manager.scheduleInspection(property, now - std::chrono::hours(24*15), tenant);
    double score = manager.calculatePropertyConditionScore(property);
    EXPECT_GE(score, 0.0);
    EXPECT_LE(score, 100.0);
    EXPECT_EQ(manager.isPropertyCompliant(property), score >= 70.0);
}
