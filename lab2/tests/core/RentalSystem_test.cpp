#include <gtest/gtest.h>
#include "../../include/core/RentalSystem.h"
#include "../../include/core/Inventory.h"
#include "../../include/business/CustomerManager.h"
#include "../../include/business/ReservationSystem.h"
#include "../../include/business/PaymentProcessor.h"
#include "../../include/business/PricingStrategy.h"
#include "../../include/core/RentalAgreement.h"
#include "../../include/core/RentalItem.h"
#include "../../include/exceptions/RentalItemUnavailableException.h"
#include "../../include/exceptions/CustomerBlacklistedException.h"
#include "../../include/business/MaintenanceScheduler.h"
#include "../../include/business/DamageAssessor.h"
#include "../../include/business/RecommendationEngine.h"
#include "../../include/business/LateFeeCalculator.h"
#include "../../include/business/DiscountCalculator.h"
#include "../../include/utils/ValidationUtils.h"
#include <memory>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

class RentalSystemTest : public ::testing::Test {
protected:
    std::unique_ptr<RentalSystem> rentalSystem;
    std::shared_ptr<Inventory> inventory;
    std::shared_ptr<CustomerManager> customerManager;
    std::shared_ptr<ReservationSystem> reservationSystem;
    std::shared_ptr<PaymentProcessor> paymentProcessor;
    std::shared_ptr<PricingStrategy> pricingStrategy;
    std::shared_ptr<RentalItem> testItem;
    std::shared_ptr<RentalItem> rentedItem;
    std::shared_ptr<RentalItem> unavailableItem;
    std::shared_ptr<RentalItem> generalItem;
    std::shared_ptr<RentalAgreement> testAgreement;

    void SetUp() override {
        auto recommender = std::make_shared<RecommendationEngine>();
        auto lateCalculator = std::make_shared<LateFeeCalculator>();
        customerManager = std::make_shared<CustomerManager>(recommender, lateCalculator);
        
        auto damageAssessor = std::make_shared<DamageAssessor>();
        auto maintenanceScheduler = std::make_shared<MaintenanceScheduler>();
        inventory = std::make_shared<Inventory>(maintenanceScheduler, damageAssessor);
        
        testItem = std::make_shared<RentalItem>("VEH001", "Car", "VEHICLE", 75.0);
        rentedItem = std::make_shared<RentalItem>("ITEM001", "Drill", "TOOLS", 25.0);
        unavailableItem = std::make_shared<RentalItem>("ITEM002", "Saw", "TOOLS", 25.0);
        generalItem = std::make_shared<RentalItem>("ITEM003", "Wrench", "GENERAL", 10.0);
        
        rentedItem->markAsRented();
        unavailableItem->markAsRented();

        inventory->addItem(testItem);
        inventory->addItem(rentedItem);
        inventory->addItem(unavailableItem);
        inventory->addItem(generalItem);

        auto customer = std::make_shared<Customer>("CUST001", "John", "john@t.com", 30);
        customerManager->addCustomer(customer);
        customerManager->addToBlacklist("BLACKLIST");
        
        reservationSystem = std::make_shared<ReservationSystem>();
        
        auto discountCalculator = std::make_shared<DiscountCalculator>();
        
        rentalSystem = std::make_unique<RentalSystem>(
            std::make_unique<Inventory>(*inventory),
            std::make_unique<CustomerManager>(*customerManager),
            std::make_unique<ReservationSystem>(*reservationSystem),
            std::make_unique<PaymentProcessor>(std::make_shared<PricingStrategy>(discountCalculator), discountCalculator),
            std::make_unique<PricingStrategy>(discountCalculator)
        );
    }
};

TEST_F(RentalSystemTest, DefaultConstructorInitialization) {
    auto defaultSystem = std::make_unique<RentalSystem>();
    EXPECT_DOUBLE_EQ(defaultSystem->getTotalRevenue(), 0.0);
    EXPECT_EQ(defaultSystem->getTotalRentalsProcessed(), 0);
}


TEST_F(RentalSystemTest, GetTotalRevenueInitial) {
    EXPECT_DOUBLE_EQ(rentalSystem->getTotalRevenue(), 0.0);
}

TEST_F(RentalSystemTest, GetTotalRentalsProcessedInitial) {
    EXPECT_EQ(rentalSystem->getTotalRentalsProcessed(), 0);
}

TEST_F(RentalSystemTest, PredictRevenueForPeriodInitial) {
    double revenue = rentalSystem->predictRevenueForPeriod(30);
    EXPECT_GE(revenue, 0.0);
}

TEST_F(RentalSystemTest, GenerateBusinessReport) {
    auto report = rentalSystem->generateBusinessReport();
    EXPECT_FALSE(report.empty());
}

TEST_F(RentalSystemTest, ProcessRentalSuccessfully) {
    auto agreement = rentalSystem->processRental("CUST001", "VEH001", 5);
    EXPECT_NE(agreement, nullptr);
    EXPECT_GT(rentalSystem->getTotalRevenue(), 0.0);
    EXPECT_EQ(rentalSystem->getTotalRentalsProcessed(), 1);
}

TEST_F(RentalSystemTest, ProcessRentalThrowsIfCustomerBlacklisted) {
    EXPECT_THROW({
        rentalSystem->processRental("BLACKLIST", "VEH001", 5);
    }, CustomerBlacklistedException);
}

TEST_F(RentalSystemTest, ProcessRentalThrowsIfItemUnavailable) {
    EXPECT_THROW({
        rentalSystem->processRental("CUST001", "ITEM002", 5);
    }, RentalItemUnavailableException);
}

TEST_F(RentalSystemTest, ProcessReturnWithDamage) {
    auto agreement = rentalSystem->processRental("CUST001", "VEH001", 5);
    
    bool returned = rentalSystem->processReturn(agreement->getAgreementId(), "DAMAGED");
    EXPECT_TRUE(returned);
}

TEST_F(RentalSystemTest, ProcessReturnInvalidAgreement) {
    bool returned = rentalSystem->processReturn("INVALID_AGR", "EXCELLENT");
    EXPECT_FALSE(returned);
}

TEST_F(RentalSystemTest, ValidateRentalFeasibilityTrue) {
    bool feasible = rentalSystem->validateRentalFeasibility("VEH001", "CUST001");
    EXPECT_TRUE(feasible);
}

TEST_F(RentalSystemTest, ValidateRentalFeasibilityFalseUnavailableItem) {
    bool feasible = rentalSystem->validateRentalFeasibility("ITEM002", "CUST001");
    EXPECT_FALSE(feasible);
}

TEST_F(RentalSystemTest, ValidateRentalFeasibilityFalseIneligibleCustomer) {
    bool feasible = rentalSystem->validateRentalFeasibility("VEH001", "BLACKLIST");
    EXPECT_FALSE(feasible);
}

TEST_F(RentalSystemTest, CalculateCustomerLifetimeValue) {
    double ltv = rentalSystem->calculateCustomerLifetimeValue("CUST001");
    EXPECT_GT(ltv, 0.0);
}

TEST_F(RentalSystemTest, ApplySeasonalPricingAdjustments) {
    rentalSystem->applySeasonalPricingAdjustments();
    SUCCEED();
}

TEST_F(RentalSystemTest, OptimizeInventoryDistribution) {
    rentalSystem->optimizeInventoryDistribution();
    SUCCEED();
}
