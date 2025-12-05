#include "../../include/finance/TaxCalculator.h"
#include <algorithm>
#include <cmath>

TaxCalculator::TaxCalculator(const std::string& jurisdiction, double propertyRate,
                             double incomeRate, double salesRate, double luxuryThreshold,
                             const std::vector<std::string>& exemptCategories,
                             double deductionLimit, bool homesteadExemption)
    : jurisdiction(jurisdiction), propertyTaxRate(propertyRate),
      incomeTaxRate(incomeRate), salesTaxRate(salesRate),
      luxuryTaxThreshold(luxuryThreshold), taxExemptCategories(exemptCategories),
      annualDeductionLimit(deductionLimit), hasHomesteadExemption(homesteadExemption) {}

TaxCalculator::TaxCalculator(const TaxCalculator& other)
    : jurisdiction(other.jurisdiction), propertyTaxRate(other.propertyTaxRate),
      incomeTaxRate(other.incomeTaxRate), salesTaxRate(other.salesTaxRate),
      luxuryTaxThreshold(other.luxuryTaxThreshold),
      taxExemptCategories(other.taxExemptCategories),
      annualDeductionLimit(other.annualDeductionLimit),
      hasHomesteadExemption(other.hasHomesteadExemption) {}

TaxCalculator& TaxCalculator::operator=(const TaxCalculator& other) {
    if (this != &other) {
        jurisdiction = other.jurisdiction;
        propertyTaxRate = other.propertyTaxRate;
        incomeTaxRate = other.incomeTaxRate;
        salesTaxRate = other.salesTaxRate;
        luxuryTaxThreshold = other.luxuryTaxThreshold;
        taxExemptCategories = other.taxExemptCategories;
        annualDeductionLimit = other.annualDeductionLimit;
        hasHomesteadExemption = other.hasHomesteadExemption;
    }
    return *this;
}

TaxCalculator::TaxCalculator(TaxCalculator&& other) noexcept
    : jurisdiction(std::move(other.jurisdiction)), propertyTaxRate(other.propertyTaxRate),
      incomeTaxRate(other.incomeTaxRate), salesTaxRate(other.salesTaxRate),
      luxuryTaxThreshold(other.luxuryTaxThreshold),
      taxExemptCategories(std::move(other.taxExemptCategories)),
      annualDeductionLimit(other.annualDeductionLimit),
      hasHomesteadExemption(other.hasHomesteadExemption) {}

TaxCalculator& TaxCalculator::operator=(TaxCalculator&& other) noexcept {
    if (this != &other) {
        jurisdiction = std::move(other.jurisdiction);
        propertyTaxRate = other.propertyTaxRate;
        incomeTaxRate = other.incomeTaxRate;
        salesTaxRate = other.salesTaxRate;
        luxuryTaxThreshold = other.luxuryTaxThreshold;
        taxExemptCategories = std::move(other.taxExemptCategories);
        annualDeductionLimit = other.annualDeductionLimit;
        hasHomesteadExemption = other.hasHomesteadExemption;
    }
    return *this;
}

TaxCalculator::~TaxCalculator() {}

std::string TaxCalculator::getJurisdiction() const { return jurisdiction; }
double TaxCalculator::getPropertyTaxRate() const { return propertyTaxRate; }
double TaxCalculator::getIncomeTaxRate() const { return incomeTaxRate; }
double TaxCalculator::getSalesTaxRate() const { return salesTaxRate; }
double TaxCalculator::getLuxuryTaxThreshold() const { return luxuryTaxThreshold; }
std::vector<std::string> TaxCalculator::getTaxExemptCategories() const { return taxExemptCategories; }
double TaxCalculator::getAnnualDeductionLimit() const { return annualDeductionLimit; }
bool TaxCalculator::getHasHomesteadExemption() const { return hasHomesteadExemption; }

void TaxCalculator::setPropertyTaxRate(double newRate) {
    propertyTaxRate = std::max(0.0, newRate);
}

void TaxCalculator::setIncomeTaxRate(double newRate) {
    incomeTaxRate = std::max(0.0, newRate);
}

void TaxCalculator::setSalesTaxRate(double newRate) {
    salesTaxRate = std::max(0.0, newRate);
}

void TaxCalculator::setLuxuryTaxThreshold(double newThreshold) {
    luxuryTaxThreshold = std::max(0.0, newThreshold);
}

double TaxCalculator::calculatePropertyTax(std::shared_ptr<Property> property) const {
    if (!property || isPropertyTaxExempt(property)) return 0.0;
    
    double baseTax = property->getRentalPrice() * 12 * propertyTaxRate;
    if (qualifiesForHomesteadExemption(property)) {
        baseTax *= 0.8;
    }
    return baseTax;
}

double TaxCalculator::calculateRentalIncomeTax(double annualRentalIncome) const {
    if (annualRentalIncome <= 0) return 0.0;
    return annualRentalIncome * incomeTaxRate;
}

double TaxCalculator::calculateSalesTax(double transactionAmount) const {
    if (transactionAmount <= 0) return 0.0;
    return transactionAmount * salesTaxRate;
}

bool TaxCalculator::isPropertyTaxExempt(std::shared_ptr<Property> property) const {
    if (!property) return false;
    
    for (const auto& exemptCategory : taxExemptCategories) {
        if (property->getType() && 
            property->getType()->getCategory().find(exemptCategory) != std::string::npos) {
            return true;
        }
    }
    return false;
}

double TaxCalculator::calculateLuxuryTax(std::shared_ptr<Property> property) const {
    if (!property || property->getRentalPrice() <= luxuryTaxThreshold) return 0.0;
    
    double excessAmount = property->getRentalPrice() - luxuryTaxThreshold;
    return excessAmount * 0.1;
}

double TaxCalculator::calculateTotalTaxBurden(std::shared_ptr<Property> property, double annualIncome) const {
    double propertyTax = calculatePropertyTax(property);
    double incomeTax = calculateRentalIncomeTax(annualIncome);
    double luxuryTax = calculateLuxuryTax(property);
    
    return propertyTax + incomeTax + luxuryTax;
}

double TaxCalculator::calculateTaxDeductions(double expenses) const {
    return std::min(expenses, annualDeductionLimit);
}

bool TaxCalculator::qualifiesForHomesteadExemption(std::shared_ptr<Property> property) const {
    if (!hasHomesteadExemption || !property) return false;
    
    return property->getType() && 
           property->getType()->getCategory() == "residential" &&
           property->getRentalPrice() < 5000.0;
}

double TaxCalculator::calculateEffectiveTaxRate(double totalTax, double totalValue) const {
    if (totalValue <= 0) return 0.0;
    return (totalTax / totalValue) * 100.0;
}