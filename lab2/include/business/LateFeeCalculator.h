#ifndef LATEFEECALCULATOR_H
#define LATEFEECALCULATOR_H

#include "../../include/business/CustomerManager.h"
#include "../../include/utils/DateUtils.h"
#include <string>
#include <unordered_map>
#include <memory>

class CustomerManager;

class LateFeeCalculator {
private:
    std::unordered_map<std::string, double> categoryLateRates;
    std::unordered_map<std::string, double> customerPenaltyMultipliers;
    std::shared_ptr<CustomerManager> customerManager;
    double baseLateFeePerDay;
    std::unordered_map<std::string, int> gracePeriodDays;

public:
    LateFeeCalculator();
    LateFeeCalculator(std::shared_ptr<CustomerManager> custManager);
    
    double calculateLateFee(const std::string& category, double dailyRate, int lateDays, 
                          const std::string& customerType) const;
    double calculateProgressivePenalty(int lateDays, double baseFee) const;
    bool shouldSuspendRentalPrivileges(const std::string& customerId, int totalLateDays) const;
    double calculateLostRevenuePenalty(const std::string& category, int lateDays) const;
    double calculateAdministrativeFee(int lateDays) const;
    bool isGracePeriodApplicable(const std::string& customerType, int lateDays) const;
    double calculateTotalPenalty(const std::string& customerId, const std::string& category, 
                               int lateDays, double dailyRate) const;
    int getGracePeriodForCustomer(const std::string& customerType) const;
    
    double getBaseLateFeePerDay() const { return baseLateFeePerDay; }
    std::shared_ptr<CustomerManager> getCustomerManager() const;
    
    void updateCategoryLateRate(const std::string& category, double rate);
    void setCustomerManager(std::shared_ptr<CustomerManager> manager);
    void setGracePeriod(const std::string& customerType, int days);
};

#endif