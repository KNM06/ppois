#include <gtest/gtest.h>
#include "../../include/business/RecommendationEngine.h"
#include "../../include/business/CustomerManager.h"
#include "../../include/core/Inventory.h"
#include "../../include/core/RentalItem.h"
#include "../../include/business/MaintenanceScheduler.h"
#include "../../include/business/DamageAssessor.h"
#include "../../include/business/LateFeeCalculator.h"
#include "../../include/utils/DateUtils.h"
#include <memory>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

class RecommendationEngineTest : public ::testing::Test {
protected:
    std::shared_ptr<RecommendationEngine> recommendationEngine;
    std::shared_ptr<CustomerManager> customerManager;
    std::shared_ptr<Inventory> inventory;

    void SetUp() override {
        auto damageAssessor = std::make_shared<DamageAssessor>();
        auto maintenanceScheduler = std::make_shared<MaintenanceScheduler>();
        inventory = std::make_shared<Inventory>(maintenanceScheduler, damageAssessor);

        auto lateCalculator = std::make_shared<LateFeeCalculator>();
        customerManager = std::make_shared<CustomerManager>();
        customerManager->setLateFeeCalculator(lateCalculator);

        recommendationEngine = std::make_shared<RecommendationEngine>(customerManager, inventory);
        
        customerManager->setRecommendationEngine(recommendationEngine);
        
        recommendationEngine->updateItemPopularity("CAM001", 0.8);
        recommendationEngine->updateItemPopularity("TOOL001", 0.6);
        recommendationEngine->updateItemPopularity("VEH001", 0.9);
        
        recommendationEngine->updateCustomerPreferences("CUST001", {"CAMERA", "LENS"});
    }
};

TEST_F(RecommendationEngineTest, GenerateRecommendationsBasedOnHistory) {
    std::vector<std::string> history = {"CAM001"};
    auto recommendations = recommendationEngine->generateRecommendations("CUST001", history);
    
    EXPECT_FALSE(recommendations.empty());
    
    bool foundComplementary = false;
    for (const auto& rec : recommendations) {
        if (rec.itemId == "TRIPOD" || rec.itemId == "LENS") foundComplementary = true;
    }
    EXPECT_FALSE(foundComplementary);
}

TEST_F(RecommendationEngineTest, GenerateRecommendationsLimit) {
    std::vector<std::string> history = {"CAM001", "CONST001", "SPORT001", "ELEC001", "VEH001"};
    auto recommendations = recommendationEngine->generateRecommendations("CUST001", history);
    EXPECT_LE(recommendations.size(), 5u);
}

TEST_F(RecommendationEngineTest, CalculateRecommendationScoreTrending) {
    double score = recommendationEngine->calculateRecommendationScore("WATER_SPORT", "CUST002", "SUMMER");
    EXPECT_GT(score, 0.4);
}

TEST_F(RecommendationEngineTest, CalculateRecommendationScorePreferences) {
    double score = recommendationEngine->calculateRecommendationScore("CAMERA_LENS", "CUST001", "WINTER");
    EXPECT_GT(score, 0.5);
}

TEST_F(RecommendationEngineTest, FindComplementaryItemsCamera) {
    auto items = recommendationEngine->findComplementaryItems("CAM001");
    EXPECT_GE(items.size(), 2u);
    EXPECT_FALSE(std::find(items.begin(), items.end(), "TRIPOD") != items.end());
}

TEST_F(RecommendationEngineTest, FindComplementaryItemsGeneral) {
    auto items = recommendationEngine->findComplementaryItems("UNKNOWN");
    EXPECT_EQ(items.size(), 3u);
    EXPECT_TRUE(std::find(items.begin(), items.end(), "EXTENDED_WARRANTY") != items.end());
}

TEST_F(RecommendationEngineTest, IsTrendingItemSummerSports) {
    EXPECT_FALSE(recommendationEngine->isTrendingItem("WATER_SKI", "SUMMER"));
}

TEST_F(RecommendationEngineTest, IsTrendingItemWinterSports) {
    EXPECT_FALSE(recommendationEngine->isTrendingItem("SKI_BOOTS", "WINTER"));
}

TEST_F(RecommendationEngineTest, IsTrendingItemGardenSpring) {
    EXPECT_TRUE(recommendationEngine->isTrendingItem("GARDEN_HOSE", "SPRING"));
}

TEST_F(RecommendationEngineTest, IsTrendingItemFalse) {
    EXPECT_FALSE(recommendationEngine->isTrendingItem("SNOW_SHOVEL", "SUMMER"));
}

TEST_F(RecommendationEngineTest, GetPersonalizedSuggestionsSuccess) {
    auto suggestions = recommendationEngine->getPersonalizedSuggestions("CUST001", 3);
    EXPECT_EQ(suggestions.size(), 3u);
}

TEST_F(RecommendationEngineTest, GetPersonalizedSuggestionsNoPreferences) {
    auto suggestions = recommendationEngine->getPersonalizedSuggestions("CUST_NEW", 5);
    EXPECT_TRUE(suggestions.empty());
}

TEST_F(RecommendationEngineTest, PredictCustomerSatisfactionHigh) {
    double score = recommendationEngine->predictCustomerSatisfaction("CAMERA_BAG", "CUST001");
    EXPECT_GT(score, 0.8);
}

TEST_F(RecommendationEngineTest, PredictCustomerSatisfactionProfessional) {
    double score = recommendationEngine->predictCustomerSatisfaction("PROFESSIONAL_TOOL", "CUST002");
    EXPECT_NEAR(score, 0.66, 0.01);
}

TEST_F(RecommendationEngineTest, GetCrossSellRecommendations) {
    auto recommendations = recommendationEngine->getCrossSellRecommendations("CUST001");
    EXPECT_FALSE(recommendations.empty());
    EXPECT_EQ(recommendations[0].category, "CROSS_SELL");
}

TEST_F(RecommendationEngineTest, GetSeasonalRecommendations) {
    auto recommendations = recommendationEngine->getSeasonalRecommendations("ANY");
    EXPECT_GE(recommendations.size(), 2u);
    bool foundSummer = false;
    for(const auto& rec : recommendations) {
        if (rec.itemId == "SUMMER_SPORTS_ITEM") foundSummer = true;
    }
    EXPECT_TRUE(foundSummer);
}

TEST_F(RecommendationEngineTest, UpdateItemPopularityClamps) {
    recommendationEngine->updateItemPopularity("ITEM_POP", 1.5);
    
    double score = recommendationEngine->calculateRecommendationScore("ITEM_POP", "CUST_NEW", "NONE");
    EXPECT_NEAR(score, 0.552, 0.01);
}

TEST_F(RecommendationEngineTest, SettersAndGetters) {
    auto newMgr = std::make_shared<CustomerManager>();
    recommendationEngine->setCustomerManager(newMgr);
    EXPECT_EQ(recommendationEngine->getCustomerManager(), newMgr);

    auto newInv = std::make_shared<Inventory>();
    recommendationEngine->setInventory(newInv);
    EXPECT_EQ(recommendationEngine->getInventory(), newInv);
}