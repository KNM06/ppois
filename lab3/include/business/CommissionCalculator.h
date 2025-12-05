#ifndef COMMISSIONCALCULATOR_H
#define COMMISSIONCALCULATOR_H

#include <string>
#include <memory>
#include "../../include/finance/RentalContract.h"
#include "../../include/core/Agent.h"

class CommissionCalculator {
private:
    double baseCommissionRate;
    double luxuryPropertyRate;
    double commercialPropertyRate;
    double seniorAgentBonus;
    double volumeDiscountThreshold;
    double teamCommissionSplit;
    std::string commissionStructure;

public:
    CommissionCalculator(double baseRate, double luxuryRate, double commercialRate,
                        double seniorBonus, double volumeThreshold, double teamSplit,
                        const std::string& structure);
    
    CommissionCalculator(const CommissionCalculator& other);
    CommissionCalculator& operator=(const CommissionCalculator& other);
    CommissionCalculator(CommissionCalculator&& other) noexcept;
    CommissionCalculator& operator=(CommissionCalculator&& other) noexcept;
    ~CommissionCalculator();

    double getBaseCommissionRate() const;
    double getLuxuryPropertyRate() const;
    double getCommercialPropertyRate() const;
    double getSeniorAgentBonus() const;
    double getVolumeDiscountThreshold() const;
    double getTeamCommissionSplit() const;
    std::string getCommissionStructure() const;

    void setBaseCommissionRate(double newRate);
    void setLuxuryPropertyRate(double newRate);
    void setCommercialPropertyRate(double newRate);
    void setSeniorAgentBonus(double newBonus);

    double calculateCommission(std::shared_ptr<RentalContract> contract, 
                              std::shared_ptr<Agent> agent) const;
    double calculateLuxuryCommission(double propertyValue) const;
    double calculateCommercialCommission(double annualRent) const;
    double calculateSeniorAgentBonus(double baseCommission, std::shared_ptr<Agent> agent) const;
    double calculateVolumeDiscount(double totalCommission, double annualVolume) const;
    double calculateTeamCommission(double totalCommission, int teamSize) const;
    double calculateNetCommission(double grossCommission, double taxRate) const;
    bool qualifiesForBonus(std::shared_ptr<Agent> agent, double dealSize) const;
    double calculatePerformanceMultiplier(std::shared_ptr<Agent> agent) const;
    std::string getCommissionTier(double commissionAmount) const;
};

#endif