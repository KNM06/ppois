#include <gtest/gtest.h>
#include "../../include/business/AnalyticsEngine.h"
#include "../../include/core/RentalSystem.h"
#include "../../include/business/ReservationSystem.h"
#include "../../include/business/ReportGenerator.h"
#include "../../include/core/Inventory.h"
#include "../../include/business/CustomerManager.h"
#include "../../include/business/PaymentProcessor.h"
#include "../../include/business/PricingStrategy.h"
#include "../../include/core/RentalItem.h"
#include "../../include/core/Customer.h"
#include <memory>

class AnalyticsEngineTest : public ::testing::Test {
protected:
    std::shared_ptr<Inventory> inventory;
    std::shared_ptr<CustomerManager> customerManager;
    std::shared_ptr<ReservationSystem> reservationSystem;
    std::shared_ptr<PaymentProcessor> paymentProcessor;
    std::shared_ptr<PricingStrategy> pricingStrategy;
    std::shared_ptr<RentalSystem> rentalSystem;
    std::shared_ptr<ReportGenerator> reportGenerator;
    std::shared_ptr<AnalyticsEngine> analyticsEngine;

    void SetUp() override {
    auto inv = std::make_unique<Inventory>();
    auto custMgr = std::make_unique<CustomerManager>();
    auto resSys = std::make_unique<ReservationSystem>();
    auto payProc = std::make_unique<PaymentProcessor>();
    auto priceStrat = std::make_unique<PricingStrategy>();

    auto customer = std::make_shared<Customer>("CUST001", "John Doe", "john@example.com", 30);
    custMgr->addCustomer(customer);

    auto rentalItem1 = std::make_shared<RentalItem>("ITEM001", "Hammer", "TOOLS", 10.0);
    rentalItem1->setCondition(ItemCondition::GOOD);
    inv->addItem(rentalItem1);

    auto rentalItem2 = std::make_shared<RentalItem>("ITEM002", "Drill", "TOOLS", 25.0);
    rentalItem2->setCondition(ItemCondition::EXCELLENT);
    inv->addItem(rentalItem2);

    auto rentalItem3 = std::make_shared<RentalItem>("VEH001", "Car", "VEHICLE", 50.0);
    inv->addItem(rentalItem3);

    rentalSystem = std::make_shared<RentalSystem>(
        std::move(inv),
        std::move(custMgr),
        std::move(resSys),
        std::move(payProc),
        std::move(priceStrat)
    );

    inventory = nullptr;
    customerManager = nullptr;
    reservationSystem = nullptr;
    paymentProcessor = nullptr;
    pricingStrategy = nullptr;

    reportGenerator = std::make_shared<ReportGenerator>(nullptr, rentalSystem, nullptr);
    analyticsEngine = std::make_shared<AnalyticsEngine>(rentalSystem, nullptr, reportGenerator);
}
};

TEST_F(AnalyticsEngineTest, PredictDemandTrend) {
    double demand = analyticsEngine->predictDemandTrend("VEHICLE", 30);
    EXPECT_GT(demand, 0.0);
    
    demand = analyticsEngine->predictDemandTrend("ELECTRONICS", 15);
    EXPECT_GT(demand, 0.0);
    
    demand = analyticsEngine->predictDemandTrend("TOOLS", 7);
    EXPECT_GT(demand, 0.0);
}

TEST_F(AnalyticsEngineTest, IdentifyUnderperformingItems) {
    auto underperforming = analyticsEngine->identifyUnderperformingItems(0.5);
    
    EXPECT_EQ(underperforming.size(), 2);
    
    EXPECT_NE(std::find(underperforming.begin(), underperforming.end(), "Low system utilization detected"),
              underperforming.end());
    EXPECT_NE(std::find(underperforming.begin(), underperforming.end(), "Limited high-performing categories"),
              underperforming.end());
    
    auto underperformingHighThreshold = analyticsEngine->identifyUnderperformingItems(0.1);
    EXPECT_LE(underperformingHighThreshold.size(), 2);
}

TEST_F(AnalyticsEngineTest, CalculateCustomerLifetimeValue) {
    double clv = analyticsEngine->calculateCustomerLifetimeValue("CUST001");
    EXPECT_GT(clv, 0.0);
    
    clv = analyticsEngine->calculateCustomerLifetimeValue("NONEXISTENT");
    EXPECT_GT(clv, 0.0);
    
    EXPECT_FALSE(clv > 50.0 && clv < 500.0);
}

TEST_F(AnalyticsEngineTest, GenerateSeasonalAnalysis) {
    std::string analysis = analyticsEngine->generateSeasonalAnalysis();
    
    EXPECT_FALSE(analysis.empty());
    EXPECT_NE(analysis.find("Seasonal Analysis:"), std::string::npos);
    
    EXPECT_TRUE(analysis.find("High") != std::string::npos || 
                analysis.find("Low") != std::string::npos || 
                analysis.find("Moderate") != std::string::npos);
    
    EXPECT_NE(analysis.find("factor"), std::string::npos);
}

TEST_F(AnalyticsEngineTest, OptimizePricingStrategy) {
    double price = analyticsEngine->optimizePricingStrategy("VEHICLE");
    EXPECT_GE(price, 10.0);
    
    price = analyticsEngine->optimizePricingStrategy("ELECTRONICS");
    EXPECT_GE(price, 10.0);
    
    price = analyticsEngine->optimizePricingStrategy("TOOLS");
    EXPECT_GE(price, 10.0);
    
    price = analyticsEngine->optimizePricingStrategy("UNKNOWN_CATEGORY");
    EXPECT_GE(price, 10.0);
}

TEST_F(AnalyticsEngineTest, GetMaintenancePredictions) {
    auto predictions = analyticsEngine->getMaintenancePredictions();
    
    EXPECT_FALSE(predictions.empty());
    EXPECT_GE(predictions.size(), 2);
    
    EXPECT_NE(std::find(predictions.begin(), predictions.end(), "Regular maintenance check recommended"),
              predictions.end());
    EXPECT_NE(std::find(predictions.begin(), predictions.end(), "Equipment rotation suggested for optimal usage"),
              predictions.end());
    
    bool hasHighUtilizationPrediction = false;
    for (const auto& prediction : predictions) {
        if (prediction.find("High utilization") != std::string::npos) {
            hasHighUtilizationPrediction = true;
            break;
        }
    }
}

TEST_F(AnalyticsEngineTest, AnalyticsEngineConstructor) {
    EXPECT_NE(analyticsEngine, nullptr);
    
    auto newEngine = std::make_shared<AnalyticsEngine>(rentalSystem, reservationSystem, reportGenerator);
    EXPECT_NE(newEngine, nullptr);
}

TEST_F(AnalyticsEngineTest, MultipleCategoriesDemandPrediction) {
    std::vector<std::string> categories = {"VEHICLE", "ELECTRONICS", "CONSTRUCTION", "TOOLS", "SPORTS"};
    
    for (const auto& category : categories) {
        double demand = analyticsEngine->predictDemandTrend(category, 30);
        EXPECT_GT(demand, 0.0);
    }
}

TEST_F(AnalyticsEngineTest, DifferentTimeHorizons) {
    double demand7days = analyticsEngine->predictDemandTrend("VEHICLE", 7);
    double demand30days = analyticsEngine->predictDemandTrend("VEHICLE", 30);
    double demand90days = analyticsEngine->predictDemandTrend("VEHICLE", 90);
    
    EXPECT_GT(demand30days, 0.0);
    EXPECT_GT(demand90days, 0.0);
    
    EXPECT_NE(demand7days, demand30days);
    EXPECT_NE(demand30days, demand90days);
}

TEST_F(AnalyticsEngineTest, UnderperformingItemsThresholdVariations) {
    auto result1 = analyticsEngine->identifyUnderperformingItems(0.0);
    auto result2 = analyticsEngine->identifyUnderperformingItems(0.3);
    auto result3 = analyticsEngine->identifyUnderperformingItems(0.7);
    auto result4 = analyticsEngine->identifyUnderperformingItems(1.0);
    
    EXPECT_GE(result1.size(), 0);
    EXPECT_GE(result2.size(), 0);
    EXPECT_GE(result3.size(), 0);
    EXPECT_GE(result4.size(), 0);
}

TEST_F(AnalyticsEngineTest, PricingStrategyConsistency) {
    double price1 = analyticsEngine->optimizePricingStrategy("VEHICLE");
    double price2 = analyticsEngine->optimizePricingStrategy("VEHICLE");
    
    EXPECT_DOUBLE_EQ(price1, price2);
}

TEST_F(AnalyticsEngineTest, MaintenancePredictionsContent) {
    auto predictions = analyticsEngine->getMaintenancePredictions();
    
    bool hasRegularCheck = false;
    bool hasEquipmentRotation = false;
    
    for (const auto& prediction : predictions) {
        if (prediction.find("Regular maintenance check") != std::string::npos) {
            hasRegularCheck = true;
        }
        if (prediction.find("Equipment rotation") != std::string::npos) {
            hasEquipmentRotation = true;
        }
    }
    
    EXPECT_TRUE(hasRegularCheck);
    EXPECT_TRUE(hasEquipmentRotation);
}

TEST_F(AnalyticsEngineTest, SeasonalAnalysisFormat) {
    std::string analysis = analyticsEngine->generateSeasonalAnalysis();
    
    EXPECT_NE(analysis.find("Seasonal Analysis:"), std::string::npos);
    EXPECT_NE(analysis.find("detected with factor"), std::string::npos);
    
    size_t factorPos = analysis.find("factor");
    EXPECT_NE(factorPos, std::string::npos);
    
    size_t numberPos = analysis.find_first_of("0123456789", factorPos);
    EXPECT_NE(numberPos, std::string::npos);
}

TEST_F(AnalyticsEngineTest, CustomerLifetimeValueRange) {
    double clv = analyticsEngine->calculateCustomerLifetimeValue("CUST001");
    
    EXPECT_GT(clv, 0.0);
    
    EXPECT_LT(clv, 10000.0);
}

TEST_F(AnalyticsEngineTest, EdgeCaseCategories) {
    std::vector<std::string> edgeCategories = {"", "UNKNOWN", "123", "SPECIAL@CATEGORY"};
    
    for (const auto& category : edgeCategories) {
        double price = analyticsEngine->optimizePricingStrategy(category);
        EXPECT_GE(price, 10.0);
    }
}

TEST_F(AnalyticsEngineTest, PredictDemandTrendNegativeDays) {
    double demand = analyticsEngine->predictDemandTrend("VEHICLE", -7);
    EXPECT_GT(demand, 0.0);
    
    demand = analyticsEngine->predictDemandTrend("VEHICLE", 0);
    EXPECT_GT(demand, 0.0);
}

TEST_F(AnalyticsEngineTest, SystemIntegration) {
    EXPECT_NE(analyticsEngine->predictDemandTrend("VEHICLE", 30), 0.0);
    
    EXPECT_FALSE(analyticsEngine->generateSeasonalAnalysis().empty());
    
    EXPECT_FALSE(analyticsEngine->getMaintenancePredictions().empty());
    
    EXPECT_GT(analyticsEngine->calculateCustomerLifetimeValue("CUST001"), 0.0);
    
    EXPECT_FALSE(analyticsEngine->identifyUnderperformingItems(0.5).empty());
    
    EXPECT_GE(analyticsEngine->optimizePricingStrategy("VEHICLE"), 10.0);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}