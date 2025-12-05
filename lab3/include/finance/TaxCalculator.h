#ifndef TAXCALCULATOR_H
#define TAXCALCULATOR_H

#include <string>
#include <memory>
#include <vector>
#include "../../include/core/Property.h"
#include "../../include/finance/RentalContract.h"

class TaxCalculator {
private:
    std::string jurisdiction;
    double propertyTaxRate;
    double incomeTaxRate;
    double salesTaxRate;
    double luxuryTaxThreshold;
    std::vector<std::string> taxExemptCategories;
    double annualDeductionLimit;
    bool hasHomesteadExemption;

public:
    TaxCalculator(const std::string& jurisdiction, double propertyRate,
                  double incomeRate, double salesRate, double luxuryThreshold,
                  const std::vector<std::string>& exemptCategories,
                  double deductionLimit, bool homesteadExemption);
    
    TaxCalculator(const TaxCalculator& other);
    TaxCalculator& operator=(const TaxCalculator& other);
    TaxCalculator(TaxCalculator&& other) noexcept;
    TaxCalculator& operator=(TaxCalculator&& other) noexcept;
    ~TaxCalculator();

    std::string getJurisdiction() const;
    double getPropertyTaxRate() const;
    double getIncomeTaxRate() const;
    double getSalesTaxRate() const;
    double getLuxuryTaxThreshold() const;
    std::vector<std::string> getTaxExemptCategories() const;
    double getAnnualDeductionLimit() const;
    bool getHasHomesteadExemption() const;

    void setPropertyTaxRate(double newRate);
    void setIncomeTaxRate(double newRate);
    void setSalesTaxRate(double newRate);
    void setLuxuryTaxThreshold(double newThreshold);

    double calculatePropertyTax(std::shared_ptr<Property> property) const;
    double calculateRentalIncomeTax(double annualRentalIncome) const;
    double calculateSalesTax(double transactionAmount) const;
    bool isPropertyTaxExempt(std::shared_ptr<Property> property) const;
    double calculateLuxuryTax(std::shared_ptr<Property> property) const;
    double calculateTotalTaxBurden(std::shared_ptr<Property> property, double annualIncome) const;
    double calculateTaxDeductions(double expenses) const;
    bool qualifiesForHomesteadExemption(std::shared_ptr<Property> property) const;
    double calculateEffectiveTaxRate(double totalTax, double totalValue) const;
};

#endif