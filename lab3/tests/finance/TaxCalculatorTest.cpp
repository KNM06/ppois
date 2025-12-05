#include <gtest/gtest.h>
#include "../../include/finance/TaxCalculator.h"
#include "../../include/core/Property.h"
#include "../../include/core/PropertyType.h"
#include "../../include/core/Address.h"
#include <memory>
#include <vector>

class TaxCalculatorTest : public ::testing::Test {
protected:
    std::shared_ptr<PropertyType> createResidentialType() {
        return std::make_shared<PropertyType>("RES", "residential", "Residential Property", 
                                             1, 5, 50.0, 200.0, false, "residential", 1.0);
    }
    
    std::shared_ptr<PropertyType> createCommercialType() {
        return std::make_shared<PropertyType>("COM", "commercial", "Commercial Property", 
                                             0, 0, 0.0, 0.0, true, "commercial", 2.0);
    }
    
    std::shared_ptr<PropertyType> createExemptType() {
        return std::make_shared<PropertyType>("EXEMPT", "exempt", "Tax Exempt Property", 
                                             0, 0, 0.0, 0.0, false, "exempt", 0.0);
    }
    
    std::shared_ptr<Property> createProperty(const std::string& id, double rentalPrice, 
                                            std::shared_ptr<PropertyType> type) {
        return std::make_shared<Property>(id, 100.0, rentalPrice,
                                         std::make_unique<Address>("123 Main St", "City", "State", "12345"),
                                         nullptr, 
                                         std::make_unique<PropertyType>(*type));
    }
    
    std::shared_ptr<Property> createPropertyWithType(const std::string& id, double rentalPrice, 
                                                     const std::string& category) {
        auto type = std::make_shared<PropertyType>("CODE", category, "Description", 
                                                  1, 3, 50.0, 150.0, false, "zoning", 1.0);
        return createProperty(id, rentalPrice, type);
    }
};

TEST_F(TaxCalculatorTest, ConstructorEdgeCases) {
    TaxCalculator calc1("", -0.1, -0.2, -0.3, -5000.0, {}, -1000.0, false);
    
    EXPECT_EQ(calc1.getJurisdiction(), "");
    EXPECT_DOUBLE_EQ(calc1.getPropertyTaxRate(), -0.1);
    EXPECT_DOUBLE_EQ(calc1.getIncomeTaxRate(), -0.2);
    EXPECT_DOUBLE_EQ(calc1.getSalesTaxRate(), -0.29999999999999999);
    EXPECT_DOUBLE_EQ(calc1.getLuxuryTaxThreshold(), -5000.0);
    EXPECT_TRUE(calc1.getTaxExemptCategories().empty());
    EXPECT_DOUBLE_EQ(calc1.getAnnualDeductionLimit(), -1000.0);
    EXPECT_FALSE(calc1.getHasHomesteadExemption());
}

TEST_F(TaxCalculatorTest, CopyConstructor) {
    std::vector<std::string> exempt = {"residential", "commercial"};
    TaxCalculator calc1("NY", 0.01, 0.05, 0.08, 5000.0, exempt, 1000.0, true);
    
    TaxCalculator calc2(calc1);
    
    EXPECT_EQ(calc2.getJurisdiction(), "NY");
    EXPECT_DOUBLE_EQ(calc2.getPropertyTaxRate(), 0.01);
    EXPECT_DOUBLE_EQ(calc2.getIncomeTaxRate(), 0.05);
    EXPECT_DOUBLE_EQ(calc2.getSalesTaxRate(), 0.08);
    EXPECT_DOUBLE_EQ(calc2.getLuxuryTaxThreshold(), 5000.0);
    EXPECT_EQ(calc2.getTaxExemptCategories().size(), 2);
    EXPECT_DOUBLE_EQ(calc2.getAnnualDeductionLimit(), 1000.0);
    EXPECT_TRUE(calc2.getHasHomesteadExemption());
}

TEST_F(TaxCalculatorTest, CopyAssignment) {
    std::vector<std::string> exempt1 = {"residential"};
    TaxCalculator calc1("NY", 0.01, 0.05, 0.08, 5000.0, exempt1, 1000.0, true);
    
    std::vector<std::string> exempt2 = {"commercial"};
    TaxCalculator calc2("CA", 0.02, 0.06, 0.07, 6000.0, exempt2, 2000.0, false);
    
    calc2 = calc1;
    
    EXPECT_EQ(calc2.getJurisdiction(), "NY");
    EXPECT_DOUBLE_EQ(calc2.getPropertyTaxRate(), 0.01);
    EXPECT_DOUBLE_EQ(calc2.getIncomeTaxRate(), 0.05);
    EXPECT_DOUBLE_EQ(calc2.getSalesTaxRate(), 0.08);
    EXPECT_DOUBLE_EQ(calc2.getLuxuryTaxThreshold(), 5000.0);
    EXPECT_EQ(calc2.getTaxExemptCategories().size(), 1);
    EXPECT_DOUBLE_EQ(calc2.getAnnualDeductionLimit(), 1000.0);
    EXPECT_TRUE(calc2.getHasHomesteadExemption());
}

TEST_F(TaxCalculatorTest, MoveConstructor) {
    std::vector<std::string> exempt = {"residential"};
    TaxCalculator calc1("NY", 0.01, 0.05, 0.08, 5000.0, exempt, 1000.0, true);
    
    TaxCalculator calc2(std::move(calc1));
    
    EXPECT_EQ(calc2.getJurisdiction(), "NY");
    EXPECT_DOUBLE_EQ(calc2.getPropertyTaxRate(), 0.01);
    EXPECT_DOUBLE_EQ(calc2.getIncomeTaxRate(), 0.05);
    EXPECT_DOUBLE_EQ(calc2.getSalesTaxRate(), 0.08);
    EXPECT_DOUBLE_EQ(calc2.getLuxuryTaxThreshold(), 5000.0);
    EXPECT_EQ(calc2.getTaxExemptCategories().size(), 1);
    EXPECT_DOUBLE_EQ(calc2.getAnnualDeductionLimit(), 1000.0);
    EXPECT_TRUE(calc2.getHasHomesteadExemption());
}

TEST_F(TaxCalculatorTest, MoveAssignment) {
    std::vector<std::string> exempt1 = {"residential"};
    TaxCalculator calc1("NY", 0.01, 0.05, 0.08, 5000.0, exempt1, 1000.0, true);
    
    std::vector<std::string> exempt2 = {"commercial"};
    TaxCalculator calc2("CA", 0.02, 0.06, 0.07, 6000.0, exempt2, 2000.0, false);
    
    calc2 = std::move(calc1);
    
    EXPECT_EQ(calc2.getJurisdiction(), "NY");
    EXPECT_DOUBLE_EQ(calc2.getPropertyTaxRate(), 0.01);
    EXPECT_DOUBLE_EQ(calc2.getIncomeTaxRate(), 0.05);
    EXPECT_DOUBLE_EQ(calc2.getSalesTaxRate(), 0.08);
    EXPECT_DOUBLE_EQ(calc2.getLuxuryTaxThreshold(), 5000.0);
    EXPECT_EQ(calc2.getTaxExemptCategories().size(), 1);
    EXPECT_DOUBLE_EQ(calc2.getAnnualDeductionLimit(), 1000.0);
    EXPECT_TRUE(calc2.getHasHomesteadExemption());
}

TEST_F(TaxCalculatorTest, SetPropertyTaxRateEdgeCases) {
    TaxCalculator calc("NY", 0.01, 0.05, 0.08, 5000.0, {}, 1000.0, true);
    
    calc.setPropertyTaxRate(-0.1);
    EXPECT_DOUBLE_EQ(calc.getPropertyTaxRate(), 0.0);
    
    calc.setPropertyTaxRate(0.0);
    EXPECT_DOUBLE_EQ(calc.getPropertyTaxRate(), 0.0);
    
    calc.setPropertyTaxRate(0.02);
    EXPECT_DOUBLE_EQ(calc.getPropertyTaxRate(), 0.02);
    
    calc.setPropertyTaxRate(1.5);
    EXPECT_DOUBLE_EQ(calc.getPropertyTaxRate(), 1.5);
}

TEST_F(TaxCalculatorTest, SetIncomeTaxRateEdgeCases) {
    TaxCalculator calc("NY", 0.01, 0.05, 0.08, 5000.0, {}, 1000.0, true);
    
    calc.setIncomeTaxRate(-0.1);
    EXPECT_DOUBLE_EQ(calc.getIncomeTaxRate(), 0.0);
    
    calc.setIncomeTaxRate(0.0);
    EXPECT_DOUBLE_EQ(calc.getIncomeTaxRate(), 0.0);
    
    calc.setIncomeTaxRate(0.1);
    EXPECT_DOUBLE_EQ(calc.getIncomeTaxRate(), 0.1);
    
    calc.setIncomeTaxRate(0.99);
    EXPECT_DOUBLE_EQ(calc.getIncomeTaxRate(), 0.99);
}

TEST_F(TaxCalculatorTest, SetSalesTaxRateEdgeCases) {
    TaxCalculator calc("NY", 0.01, 0.05, 0.08, 5000.0, {}, 1000.0, true);
    
    calc.setSalesTaxRate(-0.1);
    EXPECT_DOUBLE_EQ(calc.getSalesTaxRate(), 0.0);
    
    calc.setSalesTaxRate(0.0);
    EXPECT_DOUBLE_EQ(calc.getSalesTaxRate(), 0.0);
    
    calc.setSalesTaxRate(0.15);
    EXPECT_DOUBLE_EQ(calc.getSalesTaxRate(), 0.15);
    
    calc.setSalesTaxRate(0.25);
    EXPECT_DOUBLE_EQ(calc.getSalesTaxRate(), 0.25);
}

TEST_F(TaxCalculatorTest, SetLuxuryTaxThresholdEdgeCases) {
    TaxCalculator calc("NY", 0.01, 0.05, 0.08, 5000.0, {}, 1000.0, true);
    
    calc.setLuxuryTaxThreshold(-1000.0);
    EXPECT_DOUBLE_EQ(calc.getLuxuryTaxThreshold(), 0.0);
    
    calc.setLuxuryTaxThreshold(0.0);
    EXPECT_DOUBLE_EQ(calc.getLuxuryTaxThreshold(), 0.0);
    
    calc.setLuxuryTaxThreshold(10000.0);
    EXPECT_DOUBLE_EQ(calc.getLuxuryTaxThreshold(), 10000.0);
    
    calc.setLuxuryTaxThreshold(5000.0);
    EXPECT_DOUBLE_EQ(calc.getLuxuryTaxThreshold(), 5000.0);
}

TEST_F(TaxCalculatorTest, CalculatePropertyTaxNullProperty) {
    TaxCalculator calc("NY", 0.01, 0.05, 0.08, 5000.0, {}, 1000.0, true);
    
    double tax = calc.calculatePropertyTax(nullptr);
    EXPECT_DOUBLE_EQ(tax, 0.0);
}

TEST_F(TaxCalculatorTest, CalculatePropertyTaxExempt) {
    std::vector<std::string> exempt = {"residential"};
    TaxCalculator calc("NY", 0.01, 0.05, 0.08, 5000.0, exempt, 1000.0, false);
    
    auto property = createProperty("P1", 3000.0, createResidentialType());
    double tax = calc.calculatePropertyTax(property);
    
    EXPECT_DOUBLE_EQ(tax, 0.0);
}

TEST_F(TaxCalculatorTest, CalculatePropertyTaxNonExempt) {
    std::vector<std::string> exempt = {"commercial"};
    TaxCalculator calc("NY", 0.01, 0.05, 0.08, 5000.0, exempt, 1000.0, false);
    
    auto property = createProperty("P1", 3000.0, createResidentialType());
    double tax = calc.calculatePropertyTax(property);
    
    double expected = 3000.0 * 12 * 0.01;
    EXPECT_DOUBLE_EQ(tax, expected);
}

TEST_F(TaxCalculatorTest, CalculatePropertyTaxWithHomesteadExemption) {
    std::vector<std::string> exempt = {"commercial"};
    TaxCalculator calc("NY", 0.01, 0.05, 0.08, 5000.0, exempt, 1000.0, true);
    
    auto property = createProperty("P1", 3000.0, createResidentialType());
    double tax = calc.calculatePropertyTax(property);
    
    double expected = 3000.0 * 12 * 0.01 * 0.8;
    EXPECT_DOUBLE_EQ(tax, expected);
}

TEST_F(TaxCalculatorTest, CalculatePropertyTaxNoHomesteadExemptionHighPrice) {
    TaxCalculator calc("NY", 0.01, 0.05, 0.08, 5000.0, {}, 1000.0, true);
    
    auto property = createProperty("P1", 6000.0, createResidentialType());
    double tax = calc.calculatePropertyTax(property);
    
    double expected = 6000.0 * 12 * 0.01;
    EXPECT_DOUBLE_EQ(tax, expected);
}

TEST_F(TaxCalculatorTest, CalculateRentalIncomeTaxEdgeCases) {
    TaxCalculator calc("NY", 0.01, 0.05, 0.08, 5000.0, {}, 1000.0, true);
    
    double tax1 = calc.calculateRentalIncomeTax(0.0);
    EXPECT_DOUBLE_EQ(tax1, 0.0);
    
    double tax2 = calc.calculateRentalIncomeTax(-1000.0);
    EXPECT_DOUBLE_EQ(tax2, 0.0);
    
    double tax3 = calc.calculateRentalIncomeTax(10000.0);
    EXPECT_DOUBLE_EQ(tax3, 10000.0 * 0.05);
    
    TaxCalculator calc2("NY", 0.01, 0.0, 0.08, 5000.0, {}, 1000.0, true);
    double tax4 = calc2.calculateRentalIncomeTax(10000.0);
    EXPECT_DOUBLE_EQ(tax4, 0.0);
}

TEST_F(TaxCalculatorTest, CalculateSalesTaxEdgeCases) {
    TaxCalculator calc("NY", 0.01, 0.05, 0.08, 5000.0, {}, 1000.0, true);
    
    double tax1 = calc.calculateSalesTax(0.0);
    EXPECT_DOUBLE_EQ(tax1, 0.0);
    
    double tax2 = calc.calculateSalesTax(-1000.0);
    EXPECT_DOUBLE_EQ(tax2, 0.0);
    
    double tax3 = calc.calculateSalesTax(1000.0);
    EXPECT_DOUBLE_EQ(tax3, 1000.0 * 0.08);
    
    double tax4 = calc.calculateSalesTax(500.0);
    EXPECT_DOUBLE_EQ(tax4, 500.0 * 0.08);
    
    TaxCalculator calc2("NY", 0.01, 0.05, 0.0, 5000.0, {}, 1000.0, true);
    double tax5 = calc2.calculateSalesTax(1000.0);
    EXPECT_DOUBLE_EQ(tax5, 0.0);
}

TEST_F(TaxCalculatorTest, IsPropertyTaxExemptEdgeCases) {
    std::vector<std::string> exempt = {"residential", "church"};
    TaxCalculator calc("NY", 0.01, 0.05, 0.08, 5000.0, exempt, 1000.0, true);
    
    EXPECT_FALSE(calc.isPropertyTaxExempt(nullptr));
    
    auto residentialProp = createPropertyWithType("P1", 3000.0, "residential");
    EXPECT_TRUE(calc.isPropertyTaxExempt(residentialProp));
    
    auto churchProp = createPropertyWithType("P2", 3000.0, "church property");
    EXPECT_TRUE(calc.isPropertyTaxExempt(churchProp));
    
    auto commercialProp = createPropertyWithType("P3", 3000.0, "commercial");
    EXPECT_FALSE(calc.isPropertyTaxExempt(commercialProp));
    
    auto partialMatchProp = createPropertyWithType("P4", 3000.0, "residential apartment");
    EXPECT_TRUE(calc.isPropertyTaxExempt(partialMatchProp));
    
    auto noMatchProp = createPropertyWithType("P5", 3000.0, "industrial");
    EXPECT_FALSE(calc.isPropertyTaxExempt(noMatchProp));
    
    auto emptyCategoryProp = createPropertyWithType("P6", 3000.0, "");
    EXPECT_FALSE(calc.isPropertyTaxExempt(emptyCategoryProp));
}

TEST_F(TaxCalculatorTest, IsPropertyTaxExemptPropertyWithoutType) {
    std::vector<std::string> exempt = {"residential"};
    TaxCalculator calc("NY", 0.01, 0.05, 0.08, 5000.0, exempt, 1000.0, true);
    
    auto property = std::make_shared<Property>("P1", "123 Main St", 100.0);
    EXPECT_FALSE(calc.isPropertyTaxExempt(property));
}

TEST_F(TaxCalculatorTest, CalculateLuxuryTaxEdgeCases) {
    TaxCalculator calc("NY", 0.01, 0.05, 0.08, 5000.0, {}, 1000.0, true);
    
    EXPECT_DOUBLE_EQ(calc.calculateLuxuryTax(nullptr), 0.0);
    
    auto property1 = createProperty("P1", 5000.0, createResidentialType());
    EXPECT_DOUBLE_EQ(calc.calculateLuxuryTax(property1), 0.0);
    
    auto property2 = createProperty("P2", 5000.1, createResidentialType());
    EXPECT_DOUBLE_EQ(calc.calculateLuxuryTax(property2), 0.010000000000036381);
    
    auto property3 = createProperty("P3", 6000.0, createResidentialType());
    EXPECT_DOUBLE_EQ(calc.calculateLuxuryTax(property3), 100.0);
    
    auto property4 = createProperty("P4", 0.0, createResidentialType());
    EXPECT_DOUBLE_EQ(calc.calculateLuxuryTax(property4), 0.0);
    
    auto property5 = createProperty("P5", -1000.0, createResidentialType());
    EXPECT_DOUBLE_EQ(calc.calculateLuxuryTax(property5), 0.0);
}

TEST_F(TaxCalculatorTest, CalculateLuxuryTaxZeroThreshold) {
    TaxCalculator calc("NY", 0.01, 0.05, 0.08, 0.0, {}, 1000.0, true);
    
    auto property = createProperty("P1", 1000.0, createResidentialType());
    EXPECT_DOUBLE_EQ(calc.calculateLuxuryTax(property), 100.0);
}

TEST_F(TaxCalculatorTest, CalculateTotalTaxBurdenEdgeCases) {
    TaxCalculator calc("NY", 0.01, 0.05, 0.08, 5000.0, {}, 1000.0, true);
    
    EXPECT_DOUBLE_EQ(calc.calculateTotalTaxBurden(nullptr, 10000.0), 500.0);
    
    auto property = createProperty("P1", 3000.0, createResidentialType());
    
    double total1 = calc.calculateTotalTaxBurden(property, 0.0);
    double expected1 = calc.calculatePropertyTax(property) + 
                      calc.calculateRentalIncomeTax(0.0) + 
                      calc.calculateLuxuryTax(property);
    EXPECT_DOUBLE_EQ(total1, expected1);
    
    double total2 = calc.calculateTotalTaxBurden(property, -10000.0);
    double expected2 = calc.calculatePropertyTax(property) + 
                      calc.calculateRentalIncomeTax(-10000.0) + 
                      calc.calculateLuxuryTax(property);
    EXPECT_DOUBLE_EQ(total2, expected2);
}

TEST_F(TaxCalculatorTest, CalculateTaxDeductionsEdgeCases) {
    TaxCalculator calc("NY", 0.01, 0.05, 0.08, 5000.0, {}, 1000.0, true);
    
    EXPECT_DOUBLE_EQ(calc.calculateTaxDeductions(0.0), 0.0);
    EXPECT_DOUBLE_EQ(calc.calculateTaxDeductions(500.0), 500.0);
    EXPECT_DOUBLE_EQ(calc.calculateTaxDeductions(1000.0), 1000.0);
    EXPECT_DOUBLE_EQ(calc.calculateTaxDeductions(1500.0), 1000.0);
    EXPECT_DOUBLE_EQ(calc.calculateTaxDeductions(-500.0), -500.0);
    
    TaxCalculator calc2("NY", 0.01, 0.05, 0.08, 5000.0, {}, 0.0, true);
    EXPECT_DOUBLE_EQ(calc2.calculateTaxDeductions(1500.0), 0.0);
    
    TaxCalculator calc3("NY", 0.01, 0.05, 0.08, 5000.0, {}, -1000.0, true);
    EXPECT_DOUBLE_EQ(calc3.calculateTaxDeductions(1500.0), -1000.0);
}

TEST_F(TaxCalculatorTest, QualifiesForHomesteadExemptionEdgeCases) {
    TaxCalculator calc("NY", 0.01, 0.05, 0.08, 5000.0, {}, 1000.0, true);
    
    EXPECT_FALSE(calc.qualifiesForHomesteadExemption(nullptr));
    
    auto property1 = createProperty("P1", 3000.0, createResidentialType());
    EXPECT_TRUE(calc.qualifiesForHomesteadExemption(property1));
    
    auto property2 = createProperty("P2", 5000.0, createResidentialType());
    EXPECT_FALSE(calc.qualifiesForHomesteadExemption(property2));
    
    auto property3 = createProperty("P3", 3000.0, createCommercialType());
    EXPECT_FALSE(calc.qualifiesForHomesteadExemption(property3));
    
    auto property4 = createProperty("P4", 1000.0, createResidentialType());
    EXPECT_TRUE(calc.qualifiesForHomesteadExemption(property4));
    
    auto property5 = createProperty("P5", 4999.99, createResidentialType());
    EXPECT_TRUE(calc.qualifiesForHomesteadExemption(property5));
    
    TaxCalculator calc2("NY", 0.01, 0.05, 0.08, 5000.0, {}, 1000.0, false);
    auto property6 = createProperty("P6", 3000.0, createResidentialType());
    EXPECT_FALSE(calc2.qualifiesForHomesteadExemption(property6));
    
    auto propertyNoType = std::make_shared<Property>("P7", "123 Main St", 100.0);
    EXPECT_FALSE(calc.qualifiesForHomesteadExemption(propertyNoType));
}

TEST_F(TaxCalculatorTest, CalculateEffectiveTaxRateEdgeCases) {
    TaxCalculator calc("NY", 0.01, 0.05, 0.08, 5000.0, {}, 1000.0, true);
    
    EXPECT_DOUBLE_EQ(calc.calculateEffectiveTaxRate(0.0, 0.0), 0.0);
    EXPECT_DOUBLE_EQ(calc.calculateEffectiveTaxRate(0.0, 1000.0), 0.0);
    EXPECT_DOUBLE_EQ(calc.calculateEffectiveTaxRate(1000.0, 0.0), 0.0);
    EXPECT_DOUBLE_EQ(calc.calculateEffectiveTaxRate(1000.0, -5000.0), 0.0);
    EXPECT_DOUBLE_EQ(calc.calculateEffectiveTaxRate(-1000.0, 5000.0), -20.0);
    
    double rate1 = calc.calculateEffectiveTaxRate(500.0, 10000.0);
    EXPECT_DOUBLE_EQ(rate1, 5.0);
    
    double rate2 = calc.calculateEffectiveTaxRate(1000.0, 5000.0);
    EXPECT_DOUBLE_EQ(rate2, 20.0);
    
    double rate3 = calc.calculateEffectiveTaxRate(2500.0, 10000.0);
    EXPECT_DOUBLE_EQ(rate3, 25.0);
}

TEST_F(TaxCalculatorTest, QualifiesForHomesteadExemptionWithPropertyWithoutType) {
    TaxCalculator calc("NY", 0.01, 0.05, 0.08, 5000.0, {}, 1000.0, true);
    
    auto property = std::make_shared<Property>("P1", 3000.0, 2000.0, 
                                              std::make_unique<Address>("123 Main St", "City", "State", "12345"),
                                              nullptr, nullptr);
    
    EXPECT_FALSE(calc.qualifiesForHomesteadExemption(property));
}

TEST_F(TaxCalculatorTest, CalculatePropertyTaxWithPartialExemptMatch) {
    std::vector<std::string> exempt = {"partial"};
    TaxCalculator calc("NY", 0.01, 0.05, 0.08, 5000.0, exempt, 1000.0, false);
    
    auto type = std::make_shared<PropertyType>("CODE", "partial match category", "Description", 
                                              1, 3, 50.0, 150.0, false, "zoning", 1.0);
    auto property = createProperty("P1", 3000.0, type);
    
    EXPECT_TRUE(calc.isPropertyTaxExempt(property));
    
    double tax = calc.calculatePropertyTax(property);
    EXPECT_DOUBLE_EQ(tax, 0.0);
}

TEST_F(TaxCalculatorTest, EmptyExemptCategories) {
    std::vector<std::string> exempt;
    TaxCalculator calc("NY", 0.01, 0.05, 0.08, 5000.0, exempt, 1000.0, false);
    
    auto property = createProperty("P1", 3000.0, createResidentialType());
    
    EXPECT_FALSE(calc.isPropertyTaxExempt(property));
    
    double tax = calc.calculatePropertyTax(property);
    double expected = 3000.0 * 12 * 0.01;
    EXPECT_DOUBLE_EQ(tax, expected);
}