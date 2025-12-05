#include "../../include/business/CustomerFeedbackSystem.h"
#include <algorithm>
#include <cmath>

CustomerFeedbackSystem::CustomerFeedbackSystem(std::shared_ptr<RentalSystem> rentalSys)
    : rentalSystem(rentalSys) {
    
    categoryAverageRatings["VEHICLE"] = 4.2;
    categoryAverageRatings["ELECTRONICS"] = 4.5;
    categoryAverageRatings["CONSTRUCTION"] = 4.0;
    categoryAverageRatings["TOOLS"] = 4.3;
    
    feedbackKeywords = {"excellent", "good", "poor", "broken", "recommend"};
}

bool CustomerFeedbackSystem::submitFeedback(const std::string& customerId, const std::string& itemId, int rating, const std::string& comments) {
    Feedback feedback;
    feedback.feedbackId = "FB" + std::to_string(feedbackRecords.size() + 1000);
    feedback.customerId = customerId;
    feedback.itemId = itemId;
    feedback.rating = std::max(1, std::min(5, rating));
    feedback.comments = comments;
    feedback.date = "2024-01-01";
    
    std::string category = "GENERAL";
    if (itemId.find("VEH") != std::string::npos) category = "VEHICLE";
    else if (itemId.find("ELEC") != std::string::npos) category = "ELECTRONICS";
    feedback.categories = {category};
    
    feedbackRecords[feedback.feedbackId] = feedback;
    
    double currentAvg = categoryAverageRatings[category];
    int feedbackCount = 0;
    for (const auto& [id, fb] : feedbackRecords) {
        if (std::find(fb.categories.begin(), fb.categories.end(), category) != fb.categories.end()) {
            feedbackCount++;
        }
    }
    
    categoryAverageRatings[category] = (currentAvg * (feedbackCount - 1) + rating) / feedbackCount;
    
    if (rating >= 4) {
        processFeedbackForRewards(customerId);
    }
    
    return true;
}

double CustomerFeedbackSystem::calculateCustomerSatisfactionScore(const std::string& customerId) const {
    int totalRating = 0;
    int feedbackCount = 0;
    
    for (const auto& [id, feedback] : feedbackRecords) {
        if (feedback.customerId == customerId) {
            totalRating += feedback.rating;
            feedbackCount++;
        }
    }
    
    if (feedbackCount == 0) return 0.0;
    
    double averageRating = static_cast<double>(totalRating) / feedbackCount;
    double satisfactionScore = (averageRating - 1.0) / 4.0;
    
    return satisfactionScore;
}

std::vector<std::string> CustomerFeedbackSystem::analyzeFeedbackSentiment(const std::string& feedbackId) const {
    auto it = feedbackRecords.find(feedbackId);
    if (it == feedbackRecords.end()) {
        return {"No feedback found"};
    }
    
    const Feedback& feedback = it->second;
    std::vector<std::string> sentiments;
    
    if (feedback.rating >= 4) {
        sentiments.push_back("Positive");
    } else if (feedback.rating <= 2) {
        sentiments.push_back("Negative");
    } else {
        sentiments.push_back("Neutral");
    }
    
    for (const auto& keyword : feedbackKeywords) {
        if (feedback.comments.find(keyword) != std::string::npos) {
            sentiments.push_back("Keyword: " + keyword);
        }
    }
    
    if (feedback.comments.length() > 100) {
        sentiments.push_back("Detailed feedback");
    }
    
    return sentiments;
}

double CustomerFeedbackSystem::getCategoryAverageRating(const std::string& category) const {
    auto it = categoryAverageRatings.find(category);
    return it != categoryAverageRatings.end() ? it->second : 0.0;
}

std::vector<std::string> CustomerFeedbackSystem::generateImprovementSuggestions() const {
    std::vector<std::string> suggestions;
    
    for (const auto& [category, rating] : categoryAverageRatings) {
        if (rating < 4.0) {
            suggestions.push_back("Improve quality for " + category + " category");
        }
    }
    
    int lowRatingCount = 0;
    for (const auto& [id, feedback] : feedbackRecords) {
        if (feedback.rating <= 2) {
            lowRatingCount++;
        }
    }
    
    if (lowRatingCount > feedbackRecords.size() * 0.1) {
        suggestions.push_back("Address recurring customer complaints");
    }
    
    if (suggestions.empty()) {
        suggestions.push_back("Maintain current service quality");
    }
    
    return suggestions;
}

bool CustomerFeedbackSystem::isHighValueFeedback(const std::string& feedbackId) const {
    auto it = feedbackRecords.find(feedbackId);
    if (it == feedbackRecords.end()) return false;
    
    const Feedback& feedback = it->second;
    bool hasDetailedComments = feedback.comments.length() > 50;
    bool isCriticalRating = feedback.rating <= 2 || feedback.rating >= 4;
    bool containsKeywords = false;
    
    for (const auto& keyword : feedbackKeywords) {
        if (feedback.comments.find(keyword) != std::string::npos) {
            containsKeywords = true;
            break;
        }
    }
    
    return hasDetailedComments && isCriticalRating && containsKeywords;
}

void CustomerFeedbackSystem::processFeedbackForRewards(const std::string& customerId) {
    int highRatingCount = 0;
    for (const auto& [id, feedback] : feedbackRecords) {
        if (feedback.customerId == customerId && feedback.rating >= 4) {
            highRatingCount++;
        }
    }
    
    if (highRatingCount >= 3) {
        if (rentalSystem) {
            auto customerManager = rentalSystem->getCustomerManager();
            if (customerManager) {
                customerManager->applyLoyaltyBonus(customerId, 50.0);
            }
        }
    }
}

std::vector<std::string> CustomerFeedbackSystem::getTopRatedItems(int limit) const {
    std::vector<std::pair<std::string, double>> itemRatings;
    std::unordered_map<std::string, std::pair<int, int>> itemStats;
    
    for (const auto& [id, feedback] : feedbackRecords) {
        auto& stats = itemStats[feedback.itemId];
        stats.first += feedback.rating;
        stats.second++;
    }
    
    for (const auto& [itemId, stats] : itemStats) {
        double avgRating = static_cast<double>(stats.first) / stats.second;
        itemRatings.emplace_back(itemId, avgRating);
    }
    
    std::sort(itemRatings.begin(), itemRatings.end(), 
              [](const auto& a, const auto& b) { return a.second > b.second; });
    
    std::vector<std::string> topItems;
    for (int i = 0; i < std::min(limit, static_cast<int>(itemRatings.size())); ++i) {
        topItems.push_back(itemRatings[i].first + ": " + std::to_string(itemRatings[i].second));
    }
    
    return topItems;
}

std::shared_ptr<RentalSystem> CustomerFeedbackSystem::getRentalSystem() const {
    return rentalSystem;
}

int CustomerFeedbackSystem::getFeedbackCountForCustomer(const std::string& customerId) const {
    int count = 0;
    for (const auto& [id, feedback] : feedbackRecords) {
        if (feedback.customerId == customerId) {
            count++;
        }
    }
    return count;
}

double CustomerFeedbackSystem::getOverallSatisfactionRate() const {
    if (feedbackRecords.empty()) return 0.0;
    
    int satisfiedCount = 0;
    for (const auto& [id, feedback] : feedbackRecords) {
        if (feedback.rating >= 4) {
            satisfiedCount++;
        }
    }
    
    return static_cast<double>(satisfiedCount) / feedbackRecords.size();
}

void CustomerFeedbackSystem::setRentalSystem(std::shared_ptr<RentalSystem> system) {
    rentalSystem = system;
}

void CustomerFeedbackSystem::addFeedbackKeyword(const std::string& keyword) {
    if (std::find(feedbackKeywords.begin(), feedbackKeywords.end(), keyword) == feedbackKeywords.end()) {
        feedbackKeywords.push_back(keyword);
    }
}

void CustomerFeedbackSystem::updateCategoryRating(const std::string& category, double rating) {
    categoryAverageRatings[category] = std::max(1.0, std::min(5.0, rating));
}