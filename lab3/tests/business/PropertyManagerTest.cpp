#include <gtest/gtest.h>
#include <memory>
#include <chrono>
#include "../../include/business/PropertyManager.h"
#include "../../include/core/Property.h"
#include "../../include/finance/RentalContract.h"
#include "../../include/maintenance/MaintenanceRequest.h"

class PropertyManagerTest : public ::testing::Test {
protected:
    std::shared_ptr<Property> prop1;
    std::shared_ptr<Property> prop2;
    std::shared_ptr<RentalContract> contract1;
    std::shared_ptr<MaintenanceRequest> request1;
    PropertyManager manager{50.0, 5.0};

    void SetUp() override {
        auto address = std::make_unique<Address>();
        auto statusAvailable = std::make_unique<PropertyStatus>("AVAILABLE");
        auto type = std::make_unique<PropertyType>("Apartment");
        prop1 = std::make_shared<Property>("P1", 100.0, 1000.0,
                                           std::move(address),
                                           std::move(statusAvailable),
                                           std::move(type));

        auto address2 = std::make_unique<Address>();
        auto statusRented = std::make_unique<PropertyStatus>("RENTED");
        auto type2 = std::make_unique<PropertyType>("House");
        prop2 = std::make_shared<Property>("P2", 200.0, 2000.0,
                                           std::move(address2),
                                           std::move(statusRented),
                                           std::move(type2));

        auto tenant = std::make_shared<Tenant>();
        auto owner = std::make_shared<Owner>();
        auto now = std::chrono::system_clock::now();
        contract1 = std::make_shared<RentalContract>("C1", prop1, tenant, owner,
                                                     now - std::chrono::hours(24*30),
                                                     now + std::chrono::hours(24*30),
                                                     1000.0, 1000.0, "monthly");

        auto requestDate = now - std::chrono::hours(24*10);
        auto scheduledDate = now + std::chrono::hours(24*5);
        request1 = std::make_shared<MaintenanceRequest>("R1", prop1, tenant,
                                                        "Leak", "plumbing", "high",
                                                        requestDate, scheduledDate,
                                                        "open", 500.0, 0.0, "Vendor1");

        manager.addProperty(prop1);
        manager.addProperty(prop2);
        manager.propertyContracts[prop1->getId()] = contract1;
        manager.propertyMaintenance[prop1->getId()].push_back(request1);
    }
};

TEST_F(PropertyManagerTest, TestGetSetVacancyRateThreshold) {
    EXPECT_DOUBLE_EQ(manager.getVacancyRateThreshold(), 50.0);
    manager.setVacancyRateThreshold(30.0);
    EXPECT_DOUBLE_EQ(manager.getVacancyRateThreshold(), 30.0);
    manager.setVacancyRateThreshold(-10.0);
    EXPECT_DOUBLE_EQ(manager.getVacancyRateThreshold(), 0.0);
    manager.setVacancyRateThreshold(200.0);
    EXPECT_DOUBLE_EQ(manager.getVacancyRateThreshold(), 100.0);
}

TEST_F(PropertyManagerTest, TestGetSetMaintenanceBudgetPercentage) {
    EXPECT_DOUBLE_EQ(manager.getMaintenanceBudgetPercentage(), 5.0);
    manager.setMaintenanceBudgetPercentage(10.0);
    EXPECT_DOUBLE_EQ(manager.getMaintenanceBudgetPercentage(), 10.0);
    manager.setMaintenanceBudgetPercentage(-5.0);
    EXPECT_DOUBLE_EQ(manager.getMaintenanceBudgetPercentage(), 0.0);
}

TEST_F(PropertyManagerTest, TestAddRemoveProperty) {
    auto prop3 = std::make_shared<Property>("P3", 50.0, 500.0,
                                            std::make_unique<Address>(),
                                            std::make_unique<PropertyStatus>("AVAILABLE"),
                                            std::make_unique<PropertyType>("Studio"));
    EXPECT_TRUE(manager.addProperty(prop3));
    EXPECT_FALSE(manager.addProperty(nullptr));
    EXPECT_TRUE(manager.removeProperty("P3"));
    EXPECT_FALSE(manager.removeProperty("NotExist"));
}

TEST_F(PropertyManagerTest, TestCalculatePropertyVacancyRate) {
    EXPECT_DOUBLE_EQ(manager.calculatePropertyVacancyRate("P1"), 0.0);
    EXPECT_DOUBLE_EQ(manager.calculatePropertyVacancyRate("P2"), 100.0);
}

TEST_F(PropertyManagerTest, TestCalculateMaintenanceBudget) {
    double budget = prop1->getRentalPrice() * 12 * manager.getMaintenanceBudgetPercentage() / 100.0;
    EXPECT_DOUBLE_EQ(manager.calculateMaintenanceBudget(prop1), budget);
    EXPECT_DOUBLE_EQ(manager.calculateMaintenanceBudget(nullptr), 0.0);
}

TEST_F(PropertyManagerTest, TestGetVacantProperties) {
    auto vacant = manager.getVacantProperties();
    ASSERT_EQ(vacant.size(), 1);
    EXPECT_EQ(vacant[0]->getId(), "P2");
}

TEST_F(PropertyManagerTest, TestGetPropertiesNeedingMaintenance) {
    auto needing = manager.getPropertiesNeedingMaintenance();
    ASSERT_EQ(needing.size(), 1);
    EXPECT_EQ(needing[0]->getId(), "P1");
}

TEST_F(PropertyManagerTest, TestCalculateTotalPortfolioValue) {
    double total = prop1->getRentalPrice() * 100 + prop2->getRentalPrice() * 100;
    EXPECT_DOUBLE_EQ(manager.calculateTotalPortfolioValue(), total);
}

TEST_F(PropertyManagerTest, TestCalculateAverageRent) {
    double avg = (prop1->getRentalPrice() + prop2->getRentalPrice()) / 2.0;
    EXPECT_DOUBLE_EQ(manager.calculateAverageRent(), avg);
}

TEST_F(PropertyManagerTest, TestGetUnderperformingProperties) {
    auto underperforming = manager.getUnderperformingProperties();
    ASSERT_EQ(underperforming.size(), 1);
    EXPECT_EQ(underperforming[0]->getId(), "P2");
}

TEST_F(PropertyManagerTest, TestShouldConsiderSale) {
    EXPECT_TRUE(manager.shouldConsiderSale(prop2));
    EXPECT_FALSE(manager.shouldConsiderSale(prop1));
    EXPECT_FALSE(manager.shouldConsiderSale(nullptr));
}

TEST_F(PropertyManagerTest, TestCalculateROI) {
    double annualRent = prop1->getRentalPrice() * 12;
    double maintenance = manager.calculateMaintenanceBudget(prop1);
    double propertyValue = prop1->getRentalPrice() * 100;
    double expectedROI = ((annualRent - maintenance) / propertyValue) * 100.0;
    EXPECT_DOUBLE_EQ(manager.calculateROI(prop1), expectedROI);
    EXPECT_DOUBLE_EQ(manager.calculateROI(nullptr), 0.0);
}

TEST_F(PropertyManagerTest, TestCopyAndMoveConstructors) {
    PropertyManager copyManager = manager;
    EXPECT_DOUBLE_EQ(copyManager.getVacancyRateThreshold(), manager.getVacancyRateThreshold());
    EXPECT_DOUBLE_EQ(copyManager.getMaintenanceBudgetPercentage(), manager.getMaintenanceBudgetPercentage());

    PropertyManager moveManager = std::move(copyManager);
    EXPECT_DOUBLE_EQ(moveManager.getVacancyRateThreshold(), manager.getVacancyRateThreshold());
    EXPECT_DOUBLE_EQ(moveManager.getMaintenanceBudgetPercentage(), manager.getMaintenanceBudgetPercentage());
}
