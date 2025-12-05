#include "../../include/core/RentalHistory.h"

#include <algorithm>

RentalHistory::RentalHistory() 
    : totalAmountSpent(0.0), successfulRentals(0), lateReturns(0) {}

void RentalHistory::addRental(double amount) {
    RentalRecord record;
    record.amount = amount;
    record.completedSuccessfully = true;
    record.paidOnTime = true;
    record.returnedLate = false;
    
    records.push_back(record);
    totalAmountSpent += amount;
    successfulRentals++;
}

double RentalHistory::getOnTimePaymentRatio() const {
    if (records.empty()) return 1.0;
    
    int onTimeCount = 0;
    for (const auto& record : records) {
        if (record.paidOnTime) {
            onTimeCount++;
        }
    }
    
    return static_cast<double>(onTimeCount) / records.size();
}

double RentalHistory::getLateReturnRatio() const {
    if (records.empty()) return 0.0;
    
    int lateCount = 0;
    for (const auto& record : records) {
        if (record.returnedLate) {
            lateCount++;
        }
    }
    
    return static_cast<double>(lateCount) / records.size();
}

int RentalHistory::getRentalCountForCategory(const std::string& category) const {
    int count = 0;
    for (const auto& record : records) {
        if (record.category == category) {
            count++;
        }
    }
    return count;
}

bool RentalHistory::hasRentedCategory(const std::string& category) const {
    return getRentalCountForCategory(category) > 0;
}

double RentalHistory::getAverageRentalAmount() const {
    if (records.empty()) return 0.0;
    return totalAmountSpent / records.size();
}