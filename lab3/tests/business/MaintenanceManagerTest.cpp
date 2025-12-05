#include <gtest/gtest.h>
#include <memory>
#include <chrono>

#include "../../include/business/MaintenanceManager.h"
#include "../../include/maintenance/MaintenanceRequest.h"
#include "../../include/core/Property.h"
#include "../../include/core/Tenant.h"
#include "../../include/core/Address.h"
#include "../../include/core/PropertyType.h"
#include "../../include/core/PropertyStatus.h"

class MaintenanceManagerTest : public ::testing::Test {
protected:
    std::shared_ptr<Property> property;
    std::shared_ptr<Tenant> tenant;
    std::shared_ptr<MaintenanceManager> manager;
    std::chrono::system_clock::time_point now;

    void SetUp() override {

        auto addr1 = std::make_unique<Address>(
            "Main", "City", "State", "00000"
        );

        auto status1 = std::make_unique<PropertyStatus>(
            "AVAILABLE",
            "Available",
            true,
            false,
            false,
            std::chrono::system_clock::now(),
            "",
            0.0,
            0
        );

        auto type1 = std::make_unique<PropertyType>(
            "APT",
            "residential",
            "Apartment",
            1,
            5,
            20.0,
            500.0,
            false,
            "residential",
            1.2
        );

        property = std::make_shared<Property>(
            "prop1",
            100.0,
            1000.0,
            std::move(addr1),
            std::move(status1),
            std::move(type1)
        );

        auto addrT = std::make_unique<Address>(
            "Tenant St", "City", "State", "11111"
        );

        tenant = std::make_shared<Tenant>(
            "ten1",
            "John",
            "Doe",
            "john@example.com",
            "123456789",
            std::move(addrT),
            700,
            3000,
            false,
            1
        );

        now = std::chrono::system_clock::now();
        manager = std::make_shared<MaintenanceManager>(5000.0, 3, now);
    }
};

TEST_F(MaintenanceManagerTest, GettersAndSetters) {
    EXPECT_EQ(manager->getBudgetThreshold(), 5000.0);
    EXPECT_EQ(manager->getMaxConcurrentRequests(), 3);
    EXPECT_EQ(manager->getLastInspectionDate(), now);

    manager->setBudgetThreshold(6000.0);
    EXPECT_EQ(manager->getBudgetThreshold(), 6000.0);

    manager->setMaxConcurrentRequests(5);
    EXPECT_EQ(manager->getMaxConcurrentRequests(), 5);

    auto newDate = now + std::chrono::hours(24);
    manager->updateLastInspectionDate(newDate);
    EXPECT_EQ(manager->getLastInspectionDate(), newDate);
}

TEST_F(MaintenanceManagerTest, CreateMaintenanceRequest) {
    bool result = manager->createMaintenanceRequest(
        property,
        tenant,
        "Fix door",
        "plumbing",
        "high"
    );

    EXPECT_TRUE(result);

    auto pending = manager->getPendingRequests();
    EXPECT_EQ(pending.size(), 1);
    EXPECT_EQ(pending[0]->getProperty()->getId(), "prop1");
    EXPECT_EQ(pending[0]->getTenant()->getTenantId(), "ten1");
}

TEST_F(MaintenanceManagerTest, ScheduleAndCompleteMaintenance) {
    manager->createMaintenanceRequest(property, tenant, "Fix leak", "plumbing", "emergency");
    auto requests = manager->getPendingRequests();
    std::string reqId = requests[0]->getRequestId();

    bool scheduled = manager->scheduleMaintenance(reqId, now + std::chrono::hours(24), "Vendor1");
    EXPECT_TRUE(scheduled);
    EXPECT_EQ(requests[0]->getStatus(), "scheduled");
    EXPECT_EQ(requests[0]->getAssignedVendor(), "Vendor1");

    bool completed = manager->completeMaintenance(reqId, 200.0);
    EXPECT_TRUE(completed);
    EXPECT_EQ(requests[0]->getStatus(), "completed");
    EXPECT_EQ(requests[0]->getActualCost(), 200.0);
}

TEST_F(MaintenanceManagerTest, GetEmergencyRequests) {
    manager->createMaintenanceRequest(property, tenant, "Fix leak", "plumbing", "emergency");
    auto emergency = manager->getEmergencyRequests();
    EXPECT_EQ(emergency.size(), 1);
    EXPECT_EQ(emergency[0]->getPriority(), "emergency");
}

TEST_F(MaintenanceManagerTest, CalculateMaintenanceCosts) {
    manager->createMaintenanceRequest(property, tenant, "Fix leak", "plumbing", "high");
    auto reqs = manager->getPendingRequests();

    reqs[0]->setStatus("completed");
    reqs[0]->setActualCost(300.0);

    double cost = manager->calculateMaintenanceCosts(
        now - std::chrono::hours(1),
        now + std::chrono::hours(1)
    );

    EXPECT_EQ(cost, 300.0);
}

TEST_F(MaintenanceManagerTest, IsPropertyUnderMaintenance) {
    manager->createMaintenanceRequest(property, tenant, "Fix leak", "plumbing", "high");
    EXPECT_TRUE(manager->isPropertyUnderMaintenance("prop1"));

    auto reqs = manager->getPendingRequests();
    std::string reqId = reqs[0]->getRequestId();
    manager->completeMaintenance(reqId, 100.0);

    EXPECT_FALSE(manager->isPropertyUnderMaintenance("prop1"));
}

TEST_F(MaintenanceManagerTest, CalculateMaintenanceBudget) {
    EXPECT_EQ(manager->calculateMaintenanceBudget(100000.0), 1000.0);
}

TEST_F(MaintenanceManagerTest, NeedsRoutineInspection) {
    EXPECT_FALSE(manager->needsRoutineInspection(now));

    auto oldDate = now - std::chrono::hours(24 * 91);
    manager->updateLastInspectionDate(oldDate);

    EXPECT_TRUE(manager->needsRoutineInspection(now));
}

TEST_F(MaintenanceManagerTest, VendorRatings) {
    manager->rateVendor("VendorA", 4.5);
    EXPECT_DOUBLE_EQ(manager->getVendorRating("VendorA"), 4.5);
    EXPECT_DOUBLE_EQ(manager->getVendorRating("UnknownVendor"), 0.0);
}

TEST_F(MaintenanceManagerTest, GetOverdueMaintenance) {
    manager->createMaintenanceRequest(property, tenant, "Fix leak", "plumbing", "emergency");
    auto overdue = manager->getOverdueMaintenance(now);
    EXPECT_EQ(overdue.size(), 1);
}

