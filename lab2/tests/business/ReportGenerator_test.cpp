#include <gtest/gtest.h>
#include "../../include/business/ReportGenerator.h"
#include "../../include/core/Inventory.h"
#include "../../include/core/RentalSystem.h"
#include "../../include/business/CustomerManager.h"
#include "../../include/core/RentalItem.h"
#include "../../include/core/Customer.h"
#include "../../include/business/MaintenanceScheduler.h"
#include "../../include/business/DamageAssessor.h"
#include <memory>
#include <string>
#include <algorithm>
#include <vector>
#include <cmath>

class ReportGeneratorTest : public ::testing::Test {
protected:
    std::shared_ptr<Inventory> inventory;
    std::shared_ptr<RentalSystem> rentalSystem;
    std::shared_ptr<CustomerManager> customerManager;
    std::shared_ptr<ReportGenerator> reportGenerator;

    std::shared_ptr<RentalItem> rentalItem1;
    std::shared_ptr<RentalItem> rentalItem2;
    std::shared_ptr<RentalItem> rentalItem3;
    std::shared_ptr<RentalItem> rentalItem4;

    std::shared_ptr<Customer> customer1;
    std::shared_ptr<Customer> customer2;

    void SetUp() override {
        auto damageAssessor = std::make_shared<DamageAssessor>();
        auto maintenanceScheduler = std::make_shared<MaintenanceScheduler>();
        
        inventory = std::make_shared<Inventory>(maintenanceScheduler, damageAssessor);
        customerManager = std::make_shared<CustomerManager>();
        
        rentalSystem = std::make_shared<RentalSystem>();

        reportGenerator = std::make_shared<ReportGenerator>(
            inventory, rentalSystem, customerManager
        );

        rentalItem1 = std::make_shared<RentalItem>("1", "Hammer", "TOOLS", 10.0);
        rentalItem2 = std::make_shared<RentalItem>("2", "Drill", "TOOLS", 50.0);
        rentalItem3 = std::make_shared<RentalItem>("3", "Truck", "VEHICLE", 200.0);
        rentalItem4 = std::make_shared<RentalItem>("4", "Generator", "ELECTRONICS", 300.0);

        inventory->addItem(rentalItem1);
        inventory->addItem(rentalItem2);
        inventory->addItem(rentalItem3);
        inventory->addItem(rentalItem4);
        
        inventory->markItemAsRented("1"); 
        inventory->markItemAsRented("3");

        customer1 = std::make_shared<Customer>("1", "John Doe", "john@test.com", 30);
        customer2 = std::make_shared<Customer>("2", "Jane Smith", "jane@test.com", 28);

        customer1->setStatus(CustomerStatus::VIP);
        customer2->setStatus(CustomerStatus::REGULAR);

        customerManager->addCustomer(customer1);
        customerManager->addCustomer(customer2);
    }
};

TEST_F(ReportGeneratorTest, GenerateFinancialReportWithNullRentalSystem) {
    auto localReportGenerator = std::make_shared<ReportGenerator>(inventory, nullptr, customerManager);
    std::string report = localReportGenerator->generateFinancialReport("2024-01-01", "2024-01-31");
    EXPECT_NE(report.find("Revenue=0.000000"), std::string::npos);
    EXPECT_NE(report.find("Utilization=0.000000"), std::string::npos);
}

TEST_F(ReportGeneratorTest, GenerateInventoryUtilizationReportWithNullInventory) {
    auto localReportGenerator = std::make_shared<ReportGenerator>(nullptr, rentalSystem, customerManager);
    std::string report = localReportGenerator->generateInventoryUtilizationReport();
    EXPECT_NE(report.find("(no inventory)"), std::string::npos);
}

TEST_F(ReportGeneratorTest, GenerateInventoryUtilizationReportWithEmptyInventory) {
    auto emptyInventory = std::make_shared<Inventory>();
    auto localReportGenerator = std::make_shared<ReportGenerator>(emptyInventory, rentalSystem, customerManager);
    std::string report = localReportGenerator->generateInventoryUtilizationReport();
    EXPECT_NE(report.find("(no data)"), std::string::npos);
}

TEST_F(ReportGeneratorTest, GenerateCustomerRetentionReportNormal) {
    std::string report = reportGenerator->generateCustomerRetentionReport();
    EXPECT_NE(report.find("Rate=0.100000"), std::string::npos);
}

TEST_F(ReportGeneratorTest, GenerateCustomerRetentionReportWithNoVIP) {
    auto emptyCustomerManager = std::make_shared<CustomerManager>();
    auto localReportGenerator = std::make_shared<ReportGenerator>(inventory, rentalSystem, emptyCustomerManager);
    std::string report = localReportGenerator->generateCustomerRetentionReport();
    EXPECT_NE(report.find("Rate=0.000000"), std::string::npos);
}

TEST_F(ReportGeneratorTest, GenerateCustomerRetentionReportWithNullCustomerManager) {
    auto localReportGenerator = std::make_shared<ReportGenerator>(inventory, rentalSystem, nullptr);
    std::string report = localReportGenerator->generateCustomerRetentionReport();
    EXPECT_NE(report.find("Rate=0"), std::string::npos);
}

TEST_F(ReportGeneratorTest, GenerateMaintenanceCostReportPositiveMonths) {
    std::string report = reportGenerator->generateMaintenanceCostReport(3);
    EXPECT_NE(report.find("Cost: 450.000000"), std::string::npos);
}

TEST_F(ReportGeneratorTest, GenerateMaintenanceCostReportZeroMonths) {
    std::string report = reportGenerator->generateMaintenanceCostReport(0);
    EXPECT_NE(report.find("Cost: 0"), std::string::npos);
}

TEST_F(ReportGeneratorTest, GetTopPerformingCategoriesNormalLimit) {
    auto topCategories = reportGenerator->getTopPerformingCategories(2);
    
    EXPECT_EQ(topCategories.size(), 2u);
    EXPECT_EQ(topCategories[0], "VEHICLE");
    EXPECT_EQ(topCategories[1], "TOOLS");
}

TEST_F(ReportGeneratorTest, GetTopPerformingCategoriesZeroLimit) {
    auto topCategories = reportGenerator->getTopPerformingCategories(0);
    EXPECT_TRUE(topCategories.empty());
}

TEST_F(ReportGeneratorTest, GetTopPerformingCategoriesLargeLimit) {
    auto topCategories = reportGenerator->getTopPerformingCategories(100);
    EXPECT_EQ(topCategories.size(), 3u);
}

TEST_F(ReportGeneratorTest, GetTopPerformingCategoriesWithNullInventory) {
    auto localReportGenerator = std::make_shared<ReportGenerator>(nullptr, rentalSystem, customerManager);
    auto topCategories = localReportGenerator->getTopPerformingCategories(1);
    EXPECT_TRUE(topCategories.empty());
}

TEST_F(ReportGeneratorTest, GetTopPerformingCategoriesWithEmptyUtilization) {
    auto emptyInventory = std::make_shared<Inventory>();
    auto localReportGenerator = std::make_shared<ReportGenerator>(emptyInventory, rentalSystem, customerManager);
    auto topCategories = localReportGenerator->getTopPerformingCategories(1);
    EXPECT_TRUE(topCategories.empty());
}

TEST_F(ReportGeneratorTest, CalculateCustomerAcquisitionCostReturnsCorrectValue) {
    double cost = reportGenerator->calculateCustomerAcquisitionCost();
    EXPECT_DOUBLE_EQ(cost, 50.0);
}