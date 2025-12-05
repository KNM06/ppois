#include <gtest/gtest.h>
#include <memory>
#include <chrono>
#include "../../include/maintenance/MaintenanceCost.h"
#include "../../include/maintenance/MaintenanceRequest.h"
#include "../../include/core/Property.h"
#include "../../include/core/Tenant.h"
#include "../../include/core/Address.h"
#include "../../include/core/PropertyStatus.h"
#include "../../include/core/PropertyType.h"

std::shared_ptr<MaintenanceRequest> createRequest() {
    auto addr1 = std::make_unique<Address>("City", "Street", "123", "00000");
    auto addr2 = std::make_unique<Address>("City", "Street", "123", "00000");
    auto status = std::make_unique<PropertyStatus>("AVAILABLE");
    auto type = std::make_unique<PropertyType>("HOUSE");

    auto property = std::make_shared<Property>(
        "P1", 100.0, 1500.0, std::move(addr1),
        std::move(status), std::move(type)
    );

    auto tenant = std::make_shared<Tenant>(
        "T1", "John", "Doe", "john@mail.com", "123",
        std::move(addr2), 700, 5000, false, 2
    );

    return std::make_shared<MaintenanceRequest>(
        "R1", property, tenant, "Leak", "plumbing", "high",
        std::chrono::system_clock::now(),
        std::chrono::system_clock::now(),
        "scheduled", 500, 450, "vendor"
    );
}

TEST(MaintenanceCostTest, ConstructorAndGetters) {
    auto req = createRequest();
    auto now = std::chrono::system_clock::now();

    MaintenanceCost cost(
        "C1", req, 100, 200, 50, 30, now,
        "paid", "preferred_vendor", "capital_improvement", true
    );

    EXPECT_EQ(cost.getCostId(), "C1");
    EXPECT_EQ(cost.getMaintenanceRequest(), req.get());
    EXPECT_DOUBLE_EQ(cost.getLaborCost(), 100);
    EXPECT_DOUBLE_EQ(cost.getMaterialCost(), 200);
    EXPECT_DOUBLE_EQ(cost.getEquipmentCost(), 50);
    EXPECT_DOUBLE_EQ(cost.getEmergencySurcharge(), 30);
    EXPECT_EQ(cost.getCostDate(), now);
    EXPECT_EQ(cost.getPaymentStatus(), "paid");
    EXPECT_EQ(cost.getVendor(), "preferred_vendor");
    EXPECT_EQ(cost.getCostCategory(), "capital_improvement");
    EXPECT_TRUE(cost.getIsReimbursable());
}

TEST(MaintenanceCostTest, CopyConstructor) {
    auto req = createRequest();
    auto now = std::chrono::system_clock::now();
    MaintenanceCost original(
        "C1", req, 100, 200, 50, 30, now,
        "paid", "vendor", "repair", false
    );
    MaintenanceCost copy(original);

    EXPECT_EQ(copy.getCostId(), "C1");
    EXPECT_EQ(copy.getMaintenanceRequest(), req.get());
    EXPECT_DOUBLE_EQ(copy.getLaborCost(), 100);
}

TEST(MaintenanceCostTest, MoveConstructor) {
    auto req = createRequest();
    auto now = std::chrono::system_clock::now();

    MaintenanceCost original(
        "C1", req, 100, 200, 50, 30, now,
        "paid", "vendor", "repair", false
    );
    MaintenanceCost moved(std::move(original));

    EXPECT_EQ(moved.getCostId(), "C1");
    EXPECT_EQ(moved.getMaintenanceRequest(), req.get());
}

TEST(MaintenanceCostTest, SettersWorkCorrectly) {
    auto req = createRequest();
    auto now = std::chrono::system_clock::now();
    MaintenanceCost cost(
        "C1", req, 100, 200, 50, 30, now,
        "paid", "vendor", "repair", false
    );

    cost.setLaborCost(300);
    cost.setMaterialCost(-50);
    cost.setPaymentStatus("pending");

    EXPECT_DOUBLE_EQ(cost.getLaborCost(), 300);
    EXPECT_DOUBLE_EQ(cost.getMaterialCost(), 0);
    EXPECT_EQ(cost.getPaymentStatus(), "pending");
}

TEST(MaintenanceCostTest, CalculationMethods) {
    auto req = createRequest();
    auto now = std::chrono::system_clock::now();
    MaintenanceCost cost(
        "C1", req, 100, 200, 50, 30, now,
        "paid", "preferred_vendor", "capital_improvement", false
    );

    EXPECT_DOUBLE_EQ(cost.calculateTotalCost(), 380);
    EXPECT_TRUE(cost.isEmergencyRepair());
    EXPECT_DOUBLE_EQ(cost.calculateTaxAmount(10), 38);
    EXPECT_FALSE(cost.isOverBudget(500));
    EXPECT_TRUE(cost.isOverBudget(300));
    EXPECT_DOUBLE_EQ(cost.calculateVendorDiscount(), 38);
    EXPECT_TRUE(cost.requiresOwnerApproval());
}

TEST(MaintenanceCostTest, DepreciationCapitalLogic) {
    auto req = createRequest();
    auto now = std::chrono::system_clock::now();
    MaintenanceCost cost(
        "C1", req, 100, 200, 50, 0, now,
        "paid", "vendor", "capital_improvement", false
    );

    EXPECT_DOUBLE_EQ(cost.calculateDepreciation(), 35);
    EXPECT_TRUE(cost.isCapitalExpenditure());
}

TEST(MaintenanceCostTest, CostPriorityLogic) {
    auto req = createRequest();
    auto now = std::chrono::system_clock::now();

    MaintenanceCost cost1("C1", req, 10, 10, 10, 10, now, "x", "x", "x", false);
    EXPECT_EQ(cost1.getCostPriority(), "high");

    MaintenanceCost cost2("C2", req, 300, 300, 0, 0, now, "x", "x", "x", false);
    EXPECT_EQ(cost2.getCostPriority(), "medium");

    MaintenanceCost cost3("C3", req, 10, 10, 0, 0, now, "x", "x", "x", false);
    EXPECT_EQ(cost3.getCostPriority(), "low");
}
