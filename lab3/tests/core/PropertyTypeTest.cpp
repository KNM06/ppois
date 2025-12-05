#include <gtest/gtest.h>
#include "../../include/core/PropertyType.h"

class PropertyTypeTest : public ::testing::Test {
protected:
    void SetUp() override {}
};

TEST_F(PropertyTypeTest, ConstructorAndGetters) {
    PropertyType type("APT", "residential", "Apartment Building", 1, 4, 30.0, 200.0, false, "residential", 1.2);
    
    EXPECT_EQ(type.getTypeCode(), "APT");
    EXPECT_EQ(type.getCategory(), "residential");
    EXPECT_EQ(type.getDescription(), "Apartment Building");
    EXPECT_EQ(type.getMinBedrooms(), 1);
    EXPECT_EQ(type.getMaxBedrooms(), 4);
    EXPECT_DOUBLE_EQ(type.getMinArea(), 30.0);
    EXPECT_DOUBLE_EQ(type.getMaxArea(), 200.0);
    EXPECT_FALSE(type.getRequiresSpecialLicense());
    EXPECT_EQ(type.getZoningCategory(), "residential");
    EXPECT_DOUBLE_EQ(type.getBaseTaxRate(), 1.2);
}

TEST_F(PropertyTypeTest, ConfigurationValidation) {
    PropertyType type("APT", "residential", "Apartment", 1, 4, 30.0, 200.0, false, "residential", 1.2);
    
    EXPECT_TRUE(type.isValidConfiguration(2, 100.0));
    EXPECT_TRUE(type.isValidConfiguration(1, 30.0));
    EXPECT_TRUE(type.isValidConfiguration(4, 200.0));
    EXPECT_FALSE(type.isValidConfiguration(0, 100.0));
    EXPECT_FALSE(type.isValidConfiguration(5, 100.0));
    EXPECT_FALSE(type.isValidConfiguration(2, 20.0));
    EXPECT_FALSE(type.isValidConfiguration(2, 250.0));
}

TEST_F(PropertyTypeTest, TaxAmountCalculation) {
    PropertyType type("APT", "residential", "Apartment", 1, 4, 30.0, 200.0, false, "residential", 1.2);
    
    double propertyValue = 500000.0;
    double taxAmount = type.calculateTaxAmount(propertyValue);
    
    EXPECT_DOUBLE_EQ(taxAmount, 6000.0);
}

TEST_F(PropertyTypeTest, CommercialResidentialClassification) {
    PropertyType residential("APT", "residential", "Apartment", 1, 4, 30.0, 200.0, false, "residential", 1.2);
    PropertyType commercial("OFFICE", "commercial", "Office Space", 0, 0, 50.0, 1000.0, false, "commercial", 2.0);
    PropertyType industrial("IND", "industrial", "Industrial", 0, 0, 100.0, 5000.0, true, "industrial", 2.5);
    
    EXPECT_TRUE(residential.isResidential());
    EXPECT_FALSE(residential.isCommercial());
    
    EXPECT_TRUE(commercial.isCommercial());
    EXPECT_FALSE(commercial.isResidential());
    
    EXPECT_TRUE(industrial.isCommercial());
    EXPECT_FALSE(industrial.isResidential());
}

TEST_F(PropertyTypeTest, AdditionalPermitsRequirement) {
    PropertyType residential("APT", "residential", "Apartment", 1, 4, 30.0, 200.0, false, "residential", 1.2);
    PropertyType commercial("OFFICE", "commercial", "Office Space", 0, 0, 50.0, 1000.0, false, "commercial", 2.0);
    PropertyType licensed("BAR", "commercial", "Bar", 0, 0, 50.0, 300.0, true, "commercial", 3.0);
    
    EXPECT_FALSE(residential.requiresAdditionalPermits("residential"));
    EXPECT_TRUE(commercial.requiresAdditionalPermits("restaurant"));
    EXPECT_TRUE(commercial.requiresAdditionalPermits("medical"));
    EXPECT_TRUE(licensed.requiresAdditionalPermits("any"));
}

TEST_F(PropertyTypeTest, TypeConversionPossibility) {
    PropertyType residential("APT", "residential", "Apartment", 1, 4, 30.0, 200.0, false, "residential", 1.2);
    PropertyType commercial("OFFICE", "commercial", "Office Space", 0, 0, 50.0, 1000.0, false, "commercial", 2.0);
    PropertyType mixedUse("MIXED", "mixed_use", "Mixed Use", 1, 4, 50.0, 400.0, false, "mixed_use", 1.5);
    
    EXPECT_FALSE(residential.canBeConvertedTo(commercial));
    EXPECT_TRUE(commercial.canBeConvertedTo(residential));
    EXPECT_TRUE(mixedUse.canBeConvertedTo(residential));
    EXPECT_TRUE(mixedUse.canBeConvertedTo(commercial));
    EXPECT_TRUE(residential.canBeConvertedTo(residential));
}

TEST_F(PropertyTypeTest, InsurancePremiumCalculation) {
    PropertyType residential("APT", "residential", "Apartment", 1, 4, 30.0, 200.0, false, "residential", 1.2);
    PropertyType commercial("OFFICE", "commercial", "Office Space", 0, 0, 50.0, 1000.0, false, "commercial", 2.0);
    PropertyType licensed("BAR", "commercial", "Bar", 0, 0, 50.0, 300.0, true, "commercial", 3.0);
    
    double propertyValue = 500000.0;
    
    double residentialPremium = residential.calculateInsurancePremium(propertyValue);
    double commercialPremium = commercial.calculateInsurancePremium(propertyValue);
    double licensedPremium = licensed.calculateInsurancePremium(propertyValue);
    
    EXPECT_DOUBLE_EQ(residentialPremium, 500.0);
    EXPECT_DOUBLE_EQ(commercialPremium, 750.0);
    EXPECT_DOUBLE_EQ(licensedPremium, 900.0);
}

TEST_F(PropertyTypeTest, RiskCategory) {
    PropertyType residential("APT", "residential", "Apartment", 1, 4, 30.0, 200.0, false, "residential", 1.2);
    PropertyType commercial("OFFICE", "commercial", "Office Space", 0, 0, 50.0, 1000.0, false, "commercial", 2.0);
    PropertyType licensed("BAR", "commercial", "Bar", 0, 0, 50.0, 300.0, true, "commercial", 3.0);
    
    EXPECT_EQ(residential.getRiskCategory(), "low_risk");
    EXPECT_EQ(commercial.getRiskCategory(), "medium_risk");
    EXPECT_EQ(licensed.getRiskCategory(), "high_risk");
}

TEST_F(PropertyTypeTest, ZoningRequirements) {
    PropertyType residential("APT", "residential", "Apartment", 1, 4, 30.0, 200.0, false, "residential", 1.2);
    PropertyType mixedUse("MIXED", "mixed_use", "Mixed Use", 1, 4, 50.0, 400.0, false, "mixed_use", 1.5);
    
    EXPECT_TRUE(residential.meetsZoningRequirements("residential"));
    EXPECT_FALSE(residential.meetsZoningRequirements("commercial"));
    EXPECT_TRUE(mixedUse.meetsZoningRequirements("residential"));
    EXPECT_TRUE(mixedUse.meetsZoningRequirements("commercial"));
    EXPECT_TRUE(mixedUse.meetsZoningRequirements("any"));
}

TEST_F(PropertyTypeTest, SetterMethods) {
    PropertyType type("APT", "residential", "Apartment", 1, 4, 30.0, 200.0, false, "residential", 1.2);
    
    type.setDescription("Luxury Apartment Building");
    type.setBaseTaxRate(1.5);
    
    EXPECT_EQ(type.getDescription(), "Luxury Apartment Building");
    EXPECT_DOUBLE_EQ(type.getBaseTaxRate(), 1.5);
}

TEST_F(PropertyTypeTest, TaxRateBounds) {
    PropertyType type("APT", "residential", "Apartment", 1, 4, 30.0, 200.0, false, "residential", 1.2);
    
    type.setBaseTaxRate(-1.0);
    EXPECT_DOUBLE_EQ(type.getBaseTaxRate(), 0.0);
}