#include "../../include/core/RentalAgreement.h"
#include "../../include/core/Customer.h"
#include "../../include/core/RentalItem.h"
#include "../../include/business/InsuranceCalculator.h"
#include "../../include/business/LateFeeCalculator.h"
#include <cmath>
#include <algorithm>
#include <random>

RentalAgreement::RentalAgreement(const std::string& customerId, const std::string& itemId, int durationDays)
    : customerId(customerId), itemId(itemId), durationDays(durationDays), totalAmount(0.0),
      securityDeposit(0.0), isActive(false), insuranceIncluded(false), lateFeePerDay(25.0),
      insuranceCalculator(nullptr), lateFeeCalculator(nullptr) {
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(100000, 999999);
    agreementId = "AGR" + std::to_string(dis(gen));
}

RentalAgreement::RentalAgreement(const std::string& customerId, const std::string& itemId, int durationDays,
                               std::shared_ptr<InsuranceCalculator> insuranceCalc,
                               std::shared_ptr<LateFeeCalculator> lateCalc)
    : customerId(customerId), itemId(itemId), durationDays(durationDays), totalAmount(0.0),
      securityDeposit(0.0), isActive(false), insuranceIncluded(false), lateFeePerDay(25.0),
      insuranceCalculator(insuranceCalc), lateFeeCalculator(lateCalc) {
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(100000, 999999);
    agreementId = "AGR" + std::to_string(dis(gen));
}

double RentalAgreement::calculateEarlyTerminationFee(int daysUsed) const {
    if (daysUsed >= durationDays || !isActive) {
        return 0.0;
    }
    
    double dailyRate = totalAmount / durationDays;
    int remainingDays = durationDays - daysUsed;
    
    double remainingValue = dailyRate * remainingDays;
    double terminationFee = remainingValue * 0.5;
    
    double fixedFee = 25.0;
    
    return terminationFee + fixedFee;
}

double RentalAgreement::calculatePotentialLateFee(int extraDays) const {
    if (extraDays <= 0) return 0.0;
    
    double baseLateFee = lateFeePerDay * extraDays;
    
    if (extraDays > 7) {
        baseLateFee *= 1.5;
    }
    if (extraDays > 14) {
        baseLateFee *= 2.0;
    }
    
    double percentageFee = totalAmount * 0.02 * extraDays;
    
    return baseLateFee + percentageFee;
}

bool RentalAgreement::isOverdue(const std::string& currentDate) const {
    if (!isActive) return false;
    
    return currentDate > endDate;
}

double RentalAgreement::calculateSecurityDepositReturn(const std::string& itemCondition) const {
    if (!isActive) return securityDeposit;
    
    double returnAmount = securityDeposit;
    
    if (itemCondition == "DAMAGED") {
        returnAmount *= 0.5;
    } else if (itemCondition == "HEAVILY_DAMAGED") {
        returnAmount *= 0.2;
    } else if (itemCondition == "LOST") {
        returnAmount = 0.0;
    }
    
    if (isOverdue("2024-12-01")) {
        returnAmount -= calculatePotentialLateFee(1) * 0.5;
    }
    
    return std::max(0.0, returnAmount);
}

void RentalAgreement::applyDamagePenalty(double damageCost) {
    if (isActive && damageCost > 0) {
        if (damageCost <= securityDeposit) {
            securityDeposit -= damageCost;
        } else {
            totalAmount += (damageCost - securityDeposit);
            securityDeposit = 0.0;
        }
    }
}

bool RentalAgreement::validateAgreementTerms() const {
    if (durationDays < 1) return false;
    
    if (durationDays > 365) return false;
    
    if (totalAmount <= 0) return false;
    
    if (securityDeposit > totalAmount * 0.5) return false;
    
    return true;
}

double RentalAgreement::calculateTotalInsuranceCost() const {
    if (!insuranceCalculator || !insuranceIncluded) {
        return 0.0;
    }
    
    return insuranceCalculator->calculateInsurancePremium("GENERAL", totalAmount, "REGULAR", durationDays);
}

void RentalAgreement::addAdditionalCharge(const std::string& description, double amount) {
    additionalCharges[description] = amount;
    totalAmount += amount;
}

double RentalAgreement::getTotalAdditionalCharges() const {
    double total = 0.0;
    for (const auto& [desc, amount] : additionalCharges) {
        total += amount;
    }
    return total;
}

std::shared_ptr<InsuranceCalculator> RentalAgreement::getInsuranceCalculator() const {
    return insuranceCalculator;
}

std::shared_ptr<LateFeeCalculator> RentalAgreement::getLateFeeCalculator() const {
    return lateFeeCalculator;
}

void RentalAgreement::setInsuranceCalculator(std::shared_ptr<InsuranceCalculator> calculator) {
    insuranceCalculator = calculator;
}

void RentalAgreement::setLateFeeCalculator(std::shared_ptr<LateFeeCalculator> calculator) {
    lateFeeCalculator = calculator;
}

void RentalAgreement::finalizeAgreement(double amount, bool insurance) {
    totalAmount = amount;
    insuranceIncluded = insurance;
    isActive = true;
    
    securityDeposit = amount * 0.1;
    if (insurance) {
        securityDeposit *= 0.5;
    }
    
    startDate = "2024-01-01";
    endDate = "2024-01-01";
}

void RentalAgreement::closeAgreement() {
    isActive = false;
}