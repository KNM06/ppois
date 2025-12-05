#include "../../include/business/CommissionCalculator.h"
#include <algorithm>
#include <cmath>

CommissionCalculator::CommissionCalculator(double baseRate, double luxuryRate, double commercialRate,
                                           double seniorBonus, double volumeThreshold, double teamSplit,
                                           const std::string& structure)
    : baseCommissionRate(baseRate), luxuryPropertyRate(luxuryRate),
      commercialPropertyRate(commercialRate), seniorAgentBonus(seniorBonus),
      volumeDiscountThreshold(volumeThreshold), teamCommissionSplit(teamSplit),
      commissionStructure(structure) {}

CommissionCalculator::CommissionCalculator(const CommissionCalculator& other)
    : baseCommissionRate(other.baseCommissionRate), luxuryPropertyRate(other.luxuryPropertyRate),
      commercialPropertyRate(other.commercialPropertyRate), seniorAgentBonus(other.seniorAgentBonus),
      volumeDiscountThreshold(other.volumeDiscountThreshold), teamCommissionSplit(other.teamCommissionSplit),
      commissionStructure(other.commissionStructure) {}

CommissionCalculator& CommissionCalculator::operator=(const CommissionCalculator& other) {
    if (this != &other) {
        baseCommissionRate = other.baseCommissionRate;
        luxuryPropertyRate = other.luxuryPropertyRate;
        commercialPropertyRate = other.commercialPropertyRate;
        seniorAgentBonus = other.seniorAgentBonus;
        volumeDiscountThreshold = other.volumeDiscountThreshold;
        teamCommissionSplit = other.teamCommissionSplit;
        commissionStructure = other.commissionStructure;
    }
    return *this;
}

CommissionCalculator::CommissionCalculator(CommissionCalculator&& other) noexcept
    : baseCommissionRate(other.baseCommissionRate), luxuryPropertyRate(other.luxuryPropertyRate),
      commercialPropertyRate(other.commercialPropertyRate), seniorAgentBonus(other.seniorAgentBonus),
      volumeDiscountThreshold(other.volumeDiscountThreshold), teamCommissionSplit(other.teamCommissionSplit),
      commissionStructure(std::move(other.commissionStructure)) {}

CommissionCalculator& CommissionCalculator::operator=(CommissionCalculator&& other) noexcept {
    if (this != &other) {
        baseCommissionRate = other.baseCommissionRate;
        luxuryPropertyRate = other.luxuryPropertyRate;
        commercialPropertyRate = other.commercialPropertyRate;
        seniorAgentBonus = other.seniorAgentBonus;
        volumeDiscountThreshold = other.volumeDiscountThreshold;
        teamCommissionSplit = other.teamCommissionSplit;
        commissionStructure = std::move(other.commissionStructure);
    }
    return *this;
}

CommissionCalculator::~CommissionCalculator() {}

double CommissionCalculator::getBaseCommissionRate() const { return baseCommissionRate; }
double CommissionCalculator::getLuxuryPropertyRate() const { return luxuryPropertyRate; }
double CommissionCalculator::getCommercialPropertyRate() const { return commercialPropertyRate; }
double CommissionCalculator::getSeniorAgentBonus() const { return seniorAgentBonus; }
double CommissionCalculator::getVolumeDiscountThreshold() const { return volumeDiscountThreshold; }
double CommissionCalculator::getTeamCommissionSplit() const { return teamCommissionSplit; }
std::string CommissionCalculator::getCommissionStructure() const { return commissionStructure; }

void CommissionCalculator::setBaseCommissionRate(double newRate) {
    baseCommissionRate = std::max(0.0, newRate);
}

void CommissionCalculator::setLuxuryPropertyRate(double newRate) {
    luxuryPropertyRate = std::max(0.0, newRate);
}

void CommissionCalculator::setCommercialPropertyRate(double newRate) {
    commercialPropertyRate = std::max(0.0, newRate);
}

void CommissionCalculator::setSeniorAgentBonus(double newBonus) {
    seniorAgentBonus = std::max(0.0, newBonus);
}

double CommissionCalculator::calculateCommission(std::shared_ptr<RentalContract> contract, 
                                                std::shared_ptr<Agent> agent) const {
    if (!contract || !agent) return 0.0;
    
    double baseCommission = contract->getMonthlyRent() * 12 * baseCommissionRate;
    double performanceMultiplier = calculatePerformanceMultiplier(agent);
    
    return baseCommission * performanceMultiplier;
}

double CommissionCalculator::calculateLuxuryCommission(double propertyValue) const {
    return propertyValue * luxuryPropertyRate;
}

double CommissionCalculator::calculateCommercialCommission(double annualRent) const {
    return annualRent * commercialPropertyRate;
}

double CommissionCalculator::calculateSeniorAgentBonus(double baseCommission, std::shared_ptr<Agent> agent) const {
    if (!agent || agent->getYearsOfExperience() < 10) return 0.0;
    return baseCommission * seniorAgentBonus;
}

double CommissionCalculator::calculateVolumeDiscount(double totalCommission, double annualVolume) const {
    if (annualVolume < volumeDiscountThreshold) return totalCommission;
    
    double discountRate = 0.0;
    if (annualVolume >= 1000000.0) discountRate = 0.10;
    else if (annualVolume >= 500000.0) discountRate = 0.05;
    else if (annualVolume >= volumeDiscountThreshold) discountRate = 0.02;
    
    return totalCommission * (1.0 - discountRate);
}

double CommissionCalculator::calculateTeamCommission(double totalCommission, int teamSize) const {
    if (teamSize <= 1) return totalCommission;
    return totalCommission * teamCommissionSplit;
}

double CommissionCalculator::calculateNetCommission(double grossCommission, double taxRate) const {
    return grossCommission * (1.0 - (taxRate / 100.0));
}

bool CommissionCalculator::qualifiesForBonus(std::shared_ptr<Agent> agent, double dealSize) const {
    if (!agent) return false;
    return dealSize > 500000.0 || agent->getYearsOfExperience() >= 15;
}

double CommissionCalculator::calculatePerformanceMultiplier(std::shared_ptr<Agent> agent) const {
    if (!agent) return 1.0;
    
    double multiplier = 1.0;
    if (agent->getYearsOfExperience() >= 20) multiplier += 0.15;
    else if (agent->getYearsOfExperience() >= 10) multiplier += 0.10;
    else if (agent->getYearsOfExperience() >= 5) multiplier += 0.05;
    
    if (agent->getCertificationCount() >= 5) multiplier += 0.10;
    else if (agent->getCertificationCount() >= 3) multiplier += 0.05;
    
    return multiplier;
}

std::string CommissionCalculator::getCommissionTier(double commissionAmount) const {
    if (commissionAmount > 50000.0) return "platinum";
    if (commissionAmount > 25000.0) return "gold";
    if (commissionAmount > 10000.0) return "silver";
    if (commissionAmount > 5000.0) return "bronze";
    return "standard";
}