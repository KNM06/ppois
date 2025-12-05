#ifndef RENTALHISTORY_H
#define RENTALHISTORY_H

#include <vector>
#include <string>
#include <chrono>

struct RentalRecord {
    std::string rentalId;
    std::string itemId;
    std::string category;
    double amount;
    std::string startDate;
    std::string endDate;
    bool completedSuccessfully;
    bool paidOnTime;
    bool returnedLate;
};

class RentalHistory {
private:
    std::vector<RentalRecord> records;
    double totalAmountSpent;
    int successfulRentals;
    int lateReturns;

public:
    RentalHistory();
    
    void addRental(double amount);
    double getOnTimePaymentRatio() const;
    double getLateReturnRatio() const;
    int getRentalCountForCategory(const std::string& category) const;
    bool hasRentedCategory(const std::string& category) const;
    double getAverageRentalAmount() const;
    
    int getSuccessfulRentalCount() const { return successfulRentals; }
    double getTotalAmountSpent() const { return totalAmountSpent; }
};

#endif