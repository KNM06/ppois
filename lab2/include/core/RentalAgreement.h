#ifndef RENTALAGREEMENT_H
#define RENTALAGREEMENT_H

#include "../../include/core/Customer.h"
#include "../../include/core/RentalItem.h"
#include "../../include/business/InsuranceCalculator.h"
#include "../../include/business/LateFeeCalculator.h"
#include <string>
#include <memory>

class Customer;
class RentalItem;
class InsuranceCalculator;
class LateFeeCalculator;

class RentalAgreement {
private:
    std::string agreementId;
    std::string customerId;
    std::string itemId;
    int durationDays;
    double totalAmount;
    double securityDeposit;
    std::string startDate;
    std::string endDate;
    bool isActive;
    bool insuranceIncluded;
    double lateFeePerDay;
    std::shared_ptr<InsuranceCalculator> insuranceCalculator;
    std::shared_ptr<LateFeeCalculator> lateFeeCalculator;
    std::unordered_map<std::string, double> additionalCharges;

public:
    RentalAgreement(const std::string& customerId, const std::string& itemId, int durationDays);
    RentalAgreement(const std::string& customerId, const std::string& itemId, int durationDays,
                   std::shared_ptr<InsuranceCalculator> insuranceCalc,
                   std::shared_ptr<LateFeeCalculator> lateCalc);
    
    double calculateEarlyTerminationFee(int daysUsed) const;
    double calculatePotentialLateFee(int extraDays) const;
    bool isOverdue(const std::string& currentDate) const;
    double calculateSecurityDepositReturn(const std::string& itemCondition) const;
    void applyDamagePenalty(double damageCost);
    bool validateAgreementTerms() const;
    double calculateTotalInsuranceCost() const;
    void addAdditionalCharge(const std::string& description, double amount);
    double getTotalAdditionalCharges() const;
    
    std::string getAgreementId() const { return agreementId; }
    double getTotalAmount() const { return totalAmount; }
    bool getIsActive() const { return isActive; }
    std::string getCustomerId() const { return customerId; }
    std::string getItemId() const { return itemId; }
    std::shared_ptr<InsuranceCalculator> getInsuranceCalculator() const;
    std::shared_ptr<LateFeeCalculator> getLateFeeCalculator() const;
    
    void setInsuranceCalculator(std::shared_ptr<InsuranceCalculator> calculator);
    void setLateFeeCalculator(std::shared_ptr<LateFeeCalculator> calculator);
    
    void finalizeAgreement(double amount, bool insurance);
    void closeAgreement();
};

#endif