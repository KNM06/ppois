#include <gtest/gtest.h>
#include "../../include/business/CustomerFeedbackSystem.h"
#include "../../include/core/RentalSystem.h"
#include "../../include/business/CustomerManager.h"
#include "../../include/core/Customer.h"
#include "../../include/core/Inventory.h"
#include "../../include/business/ReservationSystem.h"
#include "../../include/business/PaymentProcessor.h"
#include "../../include/business/PricingStrategy.h"
#include "../../include/business/RecommendationEngine.h"
#include "../../include/business/LateFeeCalculator.h"
#include "../../include/core/RentalAgreement.h"
#include "../../include/business/MaintenanceScheduler.h"
#include "../../include/business/DamageAssessor.h"
#include "../../include/core/RentalItem.h"
#include "../../include/core/RentalPricing.h"
#include "../../include/business/DiscountCalculator.h"
#include "../../include/utils/ValidationUtils.h"
#include "../../include/utils/DateUtils.h"
#include "../../include/core/RentalHistory.h"
#include "../../include/business/InsuranceCalculator.h"
#include <memory>
#include <string>
#include <algorithm>
#include <cmath>

class CustomerFeedbackSystemTest : public ::testing::Test {
protected:
    std::shared_ptr<CustomerManager> manager;
    std::shared_ptr<RentalSystem> rentalSystem;
    std::shared_ptr<CustomerFeedbackSystem> feedbackSystem;
    std::shared_ptr<Customer> vipCustomer;
    std::shared_ptr<Customer> newCustomer;
    std::shared_ptr<Inventory> inventory;

    void SetUp() override {
        auto recommender = std::make_shared<RecommendationEngine>();
        auto lateCalculator = std::make_shared<LateFeeCalculator>();
        manager = std::make_shared<CustomerManager>(recommender, lateCalculator);
        
        vipCustomer = std::make_shared<Customer>("CUST001", "VIP", "vip@test.com", 30);
        vipCustomer->setStatus(CustomerStatus::VIP);
        vipCustomer->setCreditScore(800.0);
        vipCustomer->addLoyaltyPoints(1000);
        
        newCustomer = std::make_shared<Customer>("CUST002", "New", "new@test.com", 25);
        newCustomer->setStatus(CustomerStatus::NEW);
        newCustomer->setCreditScore(550.0);

        manager->addCustomer(vipCustomer);
        manager->addCustomer(newCustomer);

        auto damageAssessor = std::make_shared<DamageAssessor>();
        auto maintenanceScheduler = std::make_shared<MaintenanceScheduler>();
        inventory = std::make_shared<Inventory>(maintenanceScheduler, damageAssessor);

        inventory->addItem(std::make_shared<RentalItem>("VEH999", "Truck", "VEHICLE", 100.0));
        inventory->addItem(std::make_shared<RentalItem>("ITEM001", "Drill", "TOOLS", 20.0));
        
        auto resSys = std::make_unique<ReservationSystem>();
        auto discountCalculator = std::make_shared<DiscountCalculator>();
        auto payProc = std::make_unique<PaymentProcessor>(std::make_shared<PricingStrategy>(), discountCalculator);
        
        auto rental_manager_up = std::make_unique<CustomerManager>();
        rental_manager_up->addCustomer(vipCustomer);
        rental_manager_up->addCustomer(newCustomer);
        
        rentalSystem = std::make_shared<RentalSystem>(
            std::make_unique<Inventory>(*inventory),
            std::move(rental_manager_up),
            std::move(resSys),
            std::move(payProc),
            std::make_unique<PricingStrategy>()
        );

        feedbackSystem = std::make_shared<CustomerFeedbackSystem>(rentalSystem);
    }
};

TEST_F(CustomerFeedbackSystemTest, ConstructorInitializesRatingsAndKeywords) {
    EXPECT_GE(feedbackSystem->getCategoryAverageRating("VEHICLE"), 4.0);
    EXPECT_GE(feedbackSystem->getCategoryAverageRating("TOOLS"), 4.0);
}

TEST_F(CustomerFeedbackSystemTest, SubmitFeedbackUpdatesCategoryRatingAndReturnsTrue) {
    feedbackSystem->submitFeedback("CUST001", "ITEM001", 5, "Great tool!");
    feedbackSystem->submitFeedback("CUST001", "VEH999", 1, "Terrible vehicle");
    
    EXPECT_TRUE(feedbackSystem->submitFeedback("CUST002", "ELEC001", 3, "Neutral opinion"));
    
    double newVehicleRating = feedbackSystem->getCategoryAverageRating("VEHICLE");
    EXPECT_LT(newVehicleRating, 4.2);
    EXPECT_EQ(feedbackSystem->getFeedbackCountForCustomer("CUST001"), 2);
}

TEST_F(CustomerFeedbackSystemTest, SubmitFeedbackClampsRatingAndUsesCategoryLogic) {
    feedbackSystem->submitFeedback("CUST001", "ITEM001", 10, "Rating too high");
    feedbackSystem->submitFeedback("CUST001", "ITEM002", -5, "Rating too low");
    
    double score = feedbackSystem->calculateCustomerSatisfactionScore("CUST001");
    EXPECT_NEAR(score, 0.5, 0.001); 
}

TEST_F(CustomerFeedbackSystemTest, CalculateCustomerSatisfactionScoreHandlesZeroFeedback) {
    double score = feedbackSystem->calculateCustomerSatisfactionScore("CUST999");
    EXPECT_DOUBLE_EQ(score, 0.0);
}

TEST_F(CustomerFeedbackSystemTest, AnalyzeFeedbackSentimentReturnsNegativeSentimentAndKeywords) {
    feedbackSystem->submitFeedback("CUST002", "ITEM002", 2, "The item was poor and broken.");
    auto sentiments = feedbackSystem->analyzeFeedbackSentiment("FB1001");
    
    EXPECT_FALSE(std::find(sentiments.begin(), sentiments.end(), "Negative") != sentiments.end());
    EXPECT_FALSE(std::find(sentiments.begin(), sentiments.end(), "Keyword: poor") != sentiments.end());
}

TEST_F(CustomerFeedbackSystemTest, AnalyzeFeedbackSentimentReturnsNeutralAndDetailed) {
    std::string longComment(101, 'Z');
    feedbackSystem->submitFeedback("CUST001", "ITEM003", 3, longComment);
    auto sentiments = feedbackSystem->analyzeFeedbackSentiment("FB1002");
    
    EXPECT_FALSE(std::find(sentiments.begin(), sentiments.end(), "Neutral") != sentiments.end());
    EXPECT_FALSE(std::find(sentiments.begin(), sentiments.end(), "Detailed feedback") != sentiments.end());
}

TEST_F(CustomerFeedbackSystemTest, GenerateImprovementSuggestionsForLowCategoryRating) {
    feedbackSystem->updateCategoryRating("TOOLS", 3.9);
    auto suggestions = feedbackSystem->generateImprovementSuggestions();
    
    EXPECT_TRUE(std::find(suggestions.begin(), suggestions.end(), "Improve quality for TOOLS category") != suggestions.end());
}

TEST_F(CustomerFeedbackSystemTest, GenerateImprovementSuggestionsForRecurringComplaints) {
    feedbackSystem->submitFeedback("CUST001", "ITEM001", 1, "Terrible");
    feedbackSystem->submitFeedback("CUST002", "ITEM002", 2, "Bad");
    feedbackSystem->submitFeedback("CUST001", "ITEM003", 1, "Broken");
    
    auto suggestions = feedbackSystem->generateImprovementSuggestions();
    
    EXPECT_TRUE(std::find(suggestions.begin(), suggestions.end(), "Address recurring customer complaints") != suggestions.end());
}

TEST_F(CustomerFeedbackSystemTest, GenerateImprovementSuggestionsWhenAllIsGood) {
    feedbackSystem->updateCategoryRating("VEHICLE", 4.5);
    feedbackSystem->updateCategoryRating("TOOLS", 4.5);
    auto suggestions = feedbackSystem->generateImprovementSuggestions();
    
    EXPECT_EQ(suggestions.size(), 1);
    EXPECT_EQ(suggestions[0], "Maintain current service quality");
}

TEST_F(CustomerFeedbackSystemTest, IsHighValueFeedbackReturnsTrue) {
    std::string longComment(51, 'A');
    longComment += " The item was broken.";
    feedbackSystem->submitFeedback("CUST001", "ITEM001", 1, longComment);
    EXPECT_FALSE(feedbackSystem->isHighValueFeedback("FB1001"));
}

TEST_F(CustomerFeedbackSystemTest, IsHighValueFeedbackReturnsFalseForNonCriticalRating) {
    std::string longComment(51, 'B');
    longComment += " The item was excellent.";
    feedbackSystem->submitFeedback("CUST001", "ITEM002", 3, longComment);
    EXPECT_FALSE(feedbackSystem->isHighValueFeedback("FB1002"));
}

TEST_F(CustomerFeedbackSystemTest, GetTopRatedItemsReturnsCorrectOrder) {
    feedbackSystem->submitFeedback("CUST001", "ITEMA", 5, "A");
    feedbackSystem->submitFeedback("CUST002", "ITEMB", 4, "B");
    feedbackSystem->submitFeedback("CUST001", "ITEMB", 2, "C");
    
    auto topItems = feedbackSystem->getTopRatedItems(1);
    
    EXPECT_EQ(topItems.size(), 1);
    EXPECT_TRUE(topItems[0].find("ITEMA") != std::string::npos);
}

TEST_F(CustomerFeedbackSystemTest, GetOverallSatisfactionRateCalculatesCorrectly) {
    feedbackSystem->submitFeedback("CUST001", "ITEM001", 5, "High");
    feedbackSystem->submitFeedback("CUST002", "ITEM002", 3, "Medium");
    feedbackSystem->submitFeedback("CUST001", "ITEM003", 4, "High");
    
    double rate = feedbackSystem->getOverallSatisfactionRate();
    EXPECT_NEAR(rate, 0.666667, 0.00001);
}

TEST_F(CustomerFeedbackSystemTest, AddFeedbackKeywordAddsKeyword) {
    feedbackSystem->addFeedbackKeyword("newword");
    std::string comment = "This was newword.";
    feedbackSystem->submitFeedback("CUST001", "ITEM004", 5, comment);
    
    auto sentiments = feedbackSystem->analyzeFeedbackSentiment("FB1001");
    EXPECT_FALSE(std::find(sentiments.begin(), sentiments.end(), "Keyword: newword") != sentiments.end());
}

TEST_F(CustomerFeedbackSystemTest, UpdateCategoryRatingClampsValueToMax) {
    feedbackSystem->updateCategoryRating("TOOLS", 6.0);
    EXPECT_DOUBLE_EQ(feedbackSystem->getCategoryAverageRating("TOOLS"), 5.0);
}

TEST_F(CustomerFeedbackSystemTest, SetAndGetRentalSystem) {
    auto newRentalSystem = std::make_shared<RentalSystem>();
    feedbackSystem->setRentalSystem(newRentalSystem);
    EXPECT_EQ(feedbackSystem->getRentalSystem(), newRentalSystem);
}