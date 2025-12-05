#ifndef CUSTOMERFEEDBACKSYSTEM_H
#define CUSTOMERFEEDBACKSYSTEM_H

#include "../../include/core/Customer.h"
#include "../../include/core/RentalSystem.h"
#include <memory>
#include <unordered_map>
#include <vector>
#include <string>

struct Feedback {
    std::string feedbackId;
    std::string customerId;
    std::string itemId;
    int rating;
    std::string comments;
    std::string date;
    std::vector<std::string> categories;
};

class CustomerFeedbackSystem {
private:
    std::shared_ptr<RentalSystem> rentalSystem;
    std::unordered_map<std::string, Feedback> feedbackRecords;
    std::unordered_map<std::string, double> categoryAverageRatings;
    std::vector<std::string> feedbackKeywords;

public:
    CustomerFeedbackSystem(std::shared_ptr<RentalSystem> rentalSys);
    
    bool submitFeedback(const std::string& customerId, const std::string& itemId, int rating, const std::string& comments);
    double calculateCustomerSatisfactionScore(const std::string& customerId) const;
    std::vector<std::string> analyzeFeedbackSentiment(const std::string& feedbackId) const;
    double getCategoryAverageRating(const std::string& category) const;
    std::vector<std::string> generateImprovementSuggestions() const;
    bool isHighValueFeedback(const std::string& feedbackId) const;
    void processFeedbackForRewards(const std::string& customerId);
    std::vector<std::string> getTopRatedItems(int limit) const;
    
    std::shared_ptr<RentalSystem> getRentalSystem() const;
    int getFeedbackCountForCustomer(const std::string& customerId) const;
    double getOverallSatisfactionRate() const;
    
    void setRentalSystem(std::shared_ptr<RentalSystem> system);
    void addFeedbackKeyword(const std::string& keyword);
    void updateCategoryRating(const std::string& category, double rating);
};

#endif