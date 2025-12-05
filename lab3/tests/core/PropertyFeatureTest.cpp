#include <gtest/gtest.h>
#include "../../include/core/PropertyFeature.h"

class PropertyFeatureTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Nothing to set up for now
    }
};

TEST_F(PropertyFeatureTest, ConstructorAndGetters) {
    PropertyFeature feature("F001", "Swimming Pool", "Large outdoor swimming pool",
                           "Luxury", false, true, 25000.0, 
                           "Clean weekly, chemical maintenance monthly", 15, true);
    
    EXPECT_EQ(feature.getFeatureId(), "F001");
    EXPECT_EQ(feature.getName(), "Swimming Pool");
    EXPECT_EQ(feature.getDescription(), "Large outdoor swimming pool");
    EXPECT_EQ(feature.getCategory(), "Luxury");
    EXPECT_FALSE(feature.getIsStandard());
    EXPECT_TRUE(feature.getIsPremium());
    EXPECT_DOUBLE_EQ(feature.getAdditionalCost(), 25000.0);
    EXPECT_EQ(feature.getMaintenanceRequirements(), "Clean weekly, chemical maintenance monthly");
    EXPECT_EQ(feature.getLifespanYears(), 15);
    EXPECT_TRUE(feature.getRequiresProfessionalInstallation());
}

TEST_F(PropertyFeatureTest, Setters) {
    PropertyFeature feature("F001", "Test Feature", "Description", "Category",
                           true, false, 1000.0, "Annual maintenance", 10, false);
    
    feature.setAdditionalCost(1500.0);
    EXPECT_DOUBLE_EQ(feature.getAdditionalCost(), 1500.0);
    
    feature.setAdditionalCost(-500.0);
    EXPECT_DOUBLE_EQ(feature.getAdditionalCost(), 0.0);
    
    feature.setAdditionalCost(0.0);
    EXPECT_DOUBLE_EQ(feature.getAdditionalCost(), 0.0);
    
    feature.setMaintenanceRequirements("Quarterly maintenance");
    EXPECT_EQ(feature.getMaintenanceRequirements(), "Quarterly maintenance");
    
    feature.setMaintenanceRequirements("");
    EXPECT_EQ(feature.getMaintenanceRequirements(), "");
}

TEST_F(PropertyFeatureTest, CalculateAnnualMaintenanceCost) {
    PropertyFeature feature1("F001", "Feature 1", "Desc", "Cat", true, false, 10000.0, "", 10, false);
    EXPECT_DOUBLE_EQ(feature1.calculateAnnualMaintenanceCost(), 200.0); 
    
    PropertyFeature feature2("F002", "Feature 2", "Desc", "Cat", true, false, 5000.0, "", 10, false);
    EXPECT_DOUBLE_EQ(feature2.calculateAnnualMaintenanceCost(), 100.0);
    
    PropertyFeature feature3("F003", "Feature 3", "Desc", "Cat", true, false, 0.0, "", 10, false);
    EXPECT_DOUBLE_EQ(feature3.calculateAnnualMaintenanceCost(), 0.0); 
    
    PropertyFeature feature4("F004", "Feature 4", "Desc", "Cat", true, false, -1000.0, "", 10, false);
    EXPECT_DOUBLE_EQ(feature4.calculateAnnualMaintenanceCost(), -20.0); 
    
    PropertyFeature feature5("F005", "Feature 5", "Desc", "Cat", true, false, 10000.0, "", 0, false);
    EXPECT_DOUBLE_EQ(feature5.calculateAnnualMaintenanceCost(), 0.0); 
}

TEST_F(PropertyFeatureTest, IsEnergyEfficient) {
    PropertyFeature energyFeature("F001", "Solar Panels", "Energy efficient solar panels",
                                 "energy", true, false, 8000.0, "", 20, true);
    EXPECT_TRUE(energyFeature.isEnergyEfficient());
    
    PropertyFeature sustainabilityFeature("F002", "Rainwater Harvesting", "Collects rainwater",
                                         "sustainability", false, true, 5000.0, "", 15, true);
    EXPECT_TRUE(sustainabilityFeature.isEnergyEfficient());
    
    PropertyFeature nameEfficientFeature("F003", "High Efficiency HVAC", "Energy efficient heating",
                                        "HVAC", true, false, 6000.0, "", 15, true);
    EXPECT_FALSE(nameEfficientFeature.isEnergyEfficient());
    
    PropertyFeature nonEfficientFeature("F004", "Marble Countertops", "Luxury countertops",
                                       "Luxury", false, true, 3000.0, "", 30, true);
    EXPECT_FALSE(nonEfficientFeature.isEnergyEfficient());
    
    PropertyFeature mixedCaseFeature("F005", "Energy-Efficient Windows", "Double pane windows",
                                    "Windows", true, false, 4000.0, "", 25, true);
    EXPECT_FALSE(mixedCaseFeature.isEnergyEfficient());
}

TEST_F(PropertyFeatureTest, IncreasesPropertyValue) {
    PropertyFeature premiumFeature("F001", "Home Theater", "Premium home theater",
                                  "Entertainment", false, true, 15000.0, "", 10, true);
    EXPECT_TRUE(premiumFeature.increasesPropertyValue());
    
    PropertyFeature expensiveFeature("F002", "Gourmet Kitchen", "High-end kitchen",
                                    "Kitchen", false, false, 12000.0, "", 20, true);
    EXPECT_TRUE(expensiveFeature.increasesPropertyValue());
    
    PropertyFeature energyEfficientFeature("F003", "Insulation Upgrade", "Energy efficient",
                                          "energy", true, false, 800.0, "", 25, false);
    EXPECT_TRUE(energyEfficientFeature.increasesPropertyValue());
    
    PropertyFeature standardFeature("F004", "Basic Carpet", "Standard carpet",
                                   "Flooring", true, false, 500.0, "", 7, false);
    EXPECT_FALSE(standardFeature.increasesPropertyValue());
    
    PropertyFeature negativeCostFeature("F005", "Damaged Feature", "Needs repair",
                                       "Other", true, false, -100.0, "", 5, false);
    EXPECT_FALSE(negativeCostFeature.increasesPropertyValue());
}

TEST_F(PropertyFeatureTest, CalculateROI) {
    PropertyFeature premiumFeature("F001", "Feature 1", "Desc", "Cat", false, true, 10000.0, "", 10, true);
    EXPECT_DOUBLE_EQ(premiumFeature.calculateROI(500000.0), 150.0); // premium: 1.5 * 100
    
    PropertyFeature standardFeature("F002", "Feature 2", "Desc", "Cat", true, false, 10000.0, "", 10, true);
    EXPECT_DOUBLE_EQ(standardFeature.calculateROI(500000.0), 120.0); // standard: 1.2 * 100
    
    PropertyFeature zeroPropertyValue("F003", "Feature 3", "Desc", "Cat", false, true, 10000.0, "", 10, true);
    EXPECT_DOUBLE_EQ(zeroPropertyValue.calculateROI(0.0), 0.0);
    
    PropertyFeature zeroCostFeature("F004", "Feature 4", "Desc", "Cat", true, false, 0.0, "", 10, true);
    EXPECT_DOUBLE_EQ(zeroCostFeature.calculateROI(500000.0), 0.0);
    
    PropertyFeature negativePropertyValue("F005", "Feature 5", "Desc", "Cat", true, false, 10000.0, "", 10, true);
    EXPECT_DOUBLE_EQ(negativePropertyValue.calculateROI(-100000.0), 0.0);
    
    PropertyFeature negativeCost("F006", "Feature 6", "Desc", "Cat", true, false, -5000.0, "", 10, true);
    EXPECT_DOUBLE_EQ(negativeCost.calculateROI(500000.0), 0.0);
    
    PropertyFeature smallPropertyValue("F007", "Feature 7", "Desc", "Cat", false, true, 5000.0, "", 10, true);
    EXPECT_DOUBLE_EQ(smallPropertyValue.calculateROI(1000.0), 150.0); // ROI doesn't depend on property value
}

TEST_F(PropertyFeatureTest, RequiresRegularMaintenance) {
    PropertyFeature monthlyMaintenance("F001", "Pool", "Swimming pool", "Luxury",
                                      false, true, 25000.0, "Clean monthly", 15, true);
    EXPECT_TRUE(monthlyMaintenance.requiresRegularMaintenance());
    
    PropertyFeature quarterlyMaintenance("F002", "HVAC", "Heating system", "HVAC",
                                       true, false, 8000.0, "Service quarterly", 20, true);
    EXPECT_TRUE(quarterlyMaintenance.requiresRegularMaintenance());
    
    PropertyFeature annualMaintenance("F003", "Roof", "Roof inspection", "Structural",
                                     true, false, 15000.0, "Inspect annual", 30, true);
    EXPECT_TRUE(annualMaintenance.requiresRegularMaintenance());
    
    PropertyFeature noMaintenance("F004", "Paint", "Interior paint", "Aesthetic",
                                 true, false, 3000.0, "None", 5, false);
    EXPECT_FALSE(noMaintenance.requiresRegularMaintenance());
    
    PropertyFeature mixedCase("F005", "Security", "Security system", "Security",
                             false, true, 5000.0, "Check ANNUALLY", 10, true);
    EXPECT_FALSE(mixedCase.requiresRegularMaintenance());
    
    PropertyFeature multipleRequirements("F006", "Garden", "Landscaping", "Exterior",
                                       true, false, 7000.0, "Trim monthly, fertilize quarterly", 10, false);
    EXPECT_TRUE(multipleRequirements.requiresRegularMaintenance());
    
    PropertyFeature emptyRequirements("F007", "Feature", "Desc", "Cat",
                                     true, false, 1000.0, "", 10, false);
    EXPECT_FALSE(emptyRequirements.requiresRegularMaintenance());
}

TEST_F(PropertyFeatureTest, IsSafetyFeature) {
    PropertyFeature safetyCategory("F001", "Fire Alarm", "Fire detection system",
                                  "safety", true, false, 1500.0, "Test monthly", 10, true);
    EXPECT_TRUE(safetyCategory.isSafetyFeature());
    
    PropertyFeature securityCategory("F002", "Security Camera", "Surveillance camera",
                                    "security", false, true, 3000.0, "Clean quarterly", 8, true);
    EXPECT_TRUE(securityCategory.isSafetyFeature());
    
    PropertyFeature alarmInName("F003", "Smoke Alarm System", "Smoke detectors",
                               "Electrical", true, false, 800.0, "Test annually", 7, true);
    EXPECT_FALSE(alarmInName.isSafetyFeature());
    
    PropertyFeature safetyInName("F004", "Child Safety Gate", "Safety gate for stairs",
                                "Childproofing", true, false, 200.0, "None", 5, false);
    EXPECT_FALSE(safetyInName.isSafetyFeature());
    
    PropertyFeature nonSafetyFeature("F005", "Chandelier", "Crystal chandelier",
                                    "Lighting", false, true, 5000.0, "Clean annually", 20, true);
    EXPECT_FALSE(nonSafetyFeature.isSafetyFeature());
    
    PropertyFeature mixedCase("F006", "SECURITY System", "Home security",
                             "Home Automation", false, true, 4000.0, "", 10, true);
    EXPECT_FALSE(mixedCase.isSafetyFeature());
}

TEST_F(PropertyFeatureTest, CalculateDepreciation) {
    PropertyFeature feature1("F001", "Feature 1", "Desc", "Cat", true, false, 10000.0, "", 10, false);
    EXPECT_DOUBLE_EQ(feature1.calculateDepreciation(), 1000.0); 
    
    PropertyFeature feature2("F002", "Feature 2", "Desc", "Cat", true, false, 5000.0, "", 5, false);
    EXPECT_DOUBLE_EQ(feature2.calculateDepreciation(), 1000.0); 
    
    PropertyFeature feature3("F003", "Feature 3", "Desc", "Cat", true, false, 0.0, "", 10, false);
    EXPECT_DOUBLE_EQ(feature3.calculateDepreciation(), 0.0); 
    
    PropertyFeature feature4("F004", "Feature 4", "Desc", "Cat", true, false, 10000.0, "", 0, false);
    EXPECT_DOUBLE_EQ(feature4.calculateDepreciation(), 0.0); 
    
    PropertyFeature feature5("F005", "Feature 5", "Desc", "Cat", true, false, 10000.0, "", -5, false);
    EXPECT_DOUBLE_EQ(feature5.calculateDepreciation(), 0.0); 
    
    PropertyFeature feature6("F006", "Feature 6", "Desc", "Cat", true, false, -5000.0, "", 10, false);
    EXPECT_DOUBLE_EQ(feature6.calculateDepreciation(), -500.0); 
    
    PropertyFeature feature7("F007", "Feature 7", "Desc", "Cat", true, false, 7500.0, "", 3, false);
    EXPECT_DOUBLE_EQ(feature7.calculateDepreciation(), 2500.0); 
}

TEST_F(PropertyFeatureTest, IsLuxuryFeature) {
    PropertyFeature luxuryFeature("F001", "Wine Cellar", "Temperature controlled",
                                 "Luxury", false, true, 25000.0, "Maintain humidity", 20, true);
    EXPECT_TRUE(luxuryFeature.isLuxuryFeature());
    
    PropertyFeature premiumButCheap("F002", "Designer Lighting", "Custom lighting",
                                   "Lighting", false, true, 4000.0, "Clean annually", 15, true);
    EXPECT_FALSE(premiumButCheap.isLuxuryFeature());
    
    PropertyFeature expensiveButNotPremium("F003", "Concrete Floor", "Polished concrete",
                                          "Flooring", true, false, 10000.0, "Seal annually", 25, true);
    EXPECT_FALSE(expensiveButNotPremium.isLuxuryFeature());
    
    PropertyFeature borderlineCase("F004", "Hot Tub", "Outdoor hot tub",
                                  "Luxury", false, true, 5000.0, "Clean monthly", 10, true);
    EXPECT_FALSE(borderlineCase.isLuxuryFeature());
    
    PropertyFeature exactThreshold("F005", "Sauna", "Finnish sauna",
                                  "Wellness", false, true, 5000.01, "Maintain heater", 15, true);
    EXPECT_TRUE(exactThreshold.isLuxuryFeature());
    
    PropertyFeature negativeCost("F006", "Damaged Luxury", "Needs repair",
                                "Luxury", false, true, -10000.0, "None", 0, false);
    EXPECT_FALSE(negativeCost.isLuxuryFeature());
}

TEST_F(PropertyFeatureTest, GetFeatureGrade) {
    PropertyFeature luxuryGrade("F001", "Home Theater", "Premium theater",
                               "Entertainment", false, true, 15000.0, "", 10, true);
    EXPECT_EQ(luxuryGrade.getFeatureGrade(), "luxury");
    
    PropertyFeature premiumGrade1("F002", "Gourmet Kitchen", "High-end kitchen",
                                "Kitchen", false, true, 8000.0, "", 20, true);
    EXPECT_EQ(premiumGrade1.getFeatureGrade(), "premium");
    
    PropertyFeature premiumGrade2("F003", "Smart Home", "Home automation",
                                "Technology", false, true, 5000.0, "", 10, true);
    EXPECT_EQ(premiumGrade2.getFeatureGrade(), "premium");
    
    PropertyFeature standardGrade("F004", "Standard Flooring", "Basic flooring",
                                 "Flooring", true, false, 3000.0, "", 10, false);
    EXPECT_EQ(standardGrade.getFeatureGrade(), "standard");
    
    PropertyFeature basicGrade("F005", "Basic Feature", "Simple feature",
                              "General", false, false, 500.0, "", 5, false);
    EXPECT_EQ(basicGrade.getFeatureGrade(), "basic");
    
    PropertyFeature premiumButZeroCost("F006", "Premium Feature", "Description",
                                      "Category", false, true, 0.0, "", 10, true);
    EXPECT_EQ(premiumButZeroCost.getFeatureGrade(), "premium");
    
    PropertyFeature standardButExpensive("F007", "Standard Expensive", "Description",
                                        "Category", true, false, 20000.0, "", 20, true);
    EXPECT_EQ(standardButExpensive.getFeatureGrade(), "standard");
}

TEST_F(PropertyFeatureTest, CopyConstructor) {
    PropertyFeature original("F001", "Solar Panels", "Energy efficient",
                            "energy", true, false, 12000.0, "Clean annually", 25, true);
    original.setAdditionalCost(11000.0);
    
    PropertyFeature copy(original);
    
    EXPECT_EQ(copy.getFeatureId(), "F001");
    EXPECT_EQ(copy.getName(), "Solar Panels");
    EXPECT_EQ(copy.getDescription(), "Energy efficient");
    EXPECT_EQ(copy.getCategory(), "energy");
    EXPECT_TRUE(copy.getIsStandard());
    EXPECT_FALSE(copy.getIsPremium());
    EXPECT_DOUBLE_EQ(copy.getAdditionalCost(), 11000.0);
    EXPECT_EQ(copy.getMaintenanceRequirements(), "Clean annually");
    EXPECT_EQ(copy.getLifespanYears(), 25);
    EXPECT_TRUE(copy.getRequiresProfessionalInstallation());
}

TEST_F(PropertyFeatureTest, CopyAssignment) {
    PropertyFeature original("F001", "Original Feature", "Original description",
                            "Original Category", true, false, 5000.0, "Original maintenance", 10, false);
    
    PropertyFeature assigned("F002", "Assigned Feature", "Assigned description",
                           "Assigned Category", false, true, 10000.0, "Assigned maintenance", 5, true);
    
    assigned = original;
    
    EXPECT_EQ(assigned.getFeatureId(), "F001");
    EXPECT_EQ(assigned.getName(), "Original Feature");
    EXPECT_EQ(assigned.getDescription(), "Original description");
    EXPECT_EQ(assigned.getCategory(), "Original Category");
    EXPECT_TRUE(assigned.getIsStandard());
    EXPECT_FALSE(assigned.getIsPremium());
    EXPECT_DOUBLE_EQ(assigned.getAdditionalCost(), 5000.0);
    EXPECT_EQ(assigned.getMaintenanceRequirements(), "Original maintenance");
    EXPECT_EQ(assigned.getLifespanYears(), 10);
    EXPECT_FALSE(assigned.getRequiresProfessionalInstallation());
}

TEST_F(PropertyFeatureTest, MoveConstructor) {
    PropertyFeature original("F001", "Move Feature", "Move description",
                            "Move Category", false, true, 7000.0, "Move maintenance", 8, true);
    
    PropertyFeature moved(std::move(original));
    
    EXPECT_EQ(moved.getFeatureId(), "F001");
    EXPECT_EQ(moved.getName(), "Move Feature");
    EXPECT_EQ(moved.getDescription(), "Move description");
    EXPECT_EQ(moved.getCategory(), "Move Category");
    EXPECT_FALSE(moved.getIsStandard());
    EXPECT_TRUE(moved.getIsPremium());
    EXPECT_DOUBLE_EQ(moved.getAdditionalCost(), 7000.0);
    EXPECT_EQ(moved.getMaintenanceRequirements(), "Move maintenance");
    EXPECT_EQ(moved.getLifespanYears(), 8);
    EXPECT_TRUE(moved.getRequiresProfessionalInstallation());
}

TEST_F(PropertyFeatureTest, MoveAssignment) {
    PropertyFeature original("F001", "Original Move", "Original move description",
                            "Move Cat", true, false, 3000.0, "Original move maintenance", 12, false);
    
    PropertyFeature moved("F002", "Target Feature", "Target description",
                        "Target Cat", false, true, 8000.0, "Target maintenance", 6, true);
    
    moved = std::move(original);
    
    EXPECT_EQ(moved.getFeatureId(), "F001");
    EXPECT_EQ(moved.getName(), "Original Move");
    EXPECT_EQ(moved.getDescription(), "Original move description");
    EXPECT_EQ(moved.getCategory(), "Move Cat");
    EXPECT_TRUE(moved.getIsStandard());
    EXPECT_FALSE(moved.getIsPremium());
    EXPECT_DOUBLE_EQ(moved.getAdditionalCost(), 3000.0);
    EXPECT_EQ(moved.getMaintenanceRequirements(), "Original move maintenance");
    EXPECT_EQ(moved.getLifespanYears(), 12);
    EXPECT_FALSE(moved.getRequiresProfessionalInstallation());
}

TEST_F(PropertyFeatureTest, SelfAssignment) {
    PropertyFeature feature("F001", "Self Feature", "Self description",
                          "Self Category", true, false, 4000.0, "Self maintenance", 15, true);
    
    feature = feature;
    
    EXPECT_EQ(feature.getFeatureId(), "F001");
    EXPECT_EQ(feature.getName(), "Self Feature");
    EXPECT_EQ(feature.getDescription(), "Self description");
    EXPECT_EQ(feature.getCategory(), "Self Category");
    EXPECT_TRUE(feature.getIsStandard());
    EXPECT_FALSE(feature.getIsPremium());
    EXPECT_DOUBLE_EQ(feature.getAdditionalCost(), 4000.0);
    EXPECT_EQ(feature.getMaintenanceRequirements(), "Self maintenance");
    EXPECT_EQ(feature.getLifespanYears(), 15);
    EXPECT_TRUE(feature.getRequiresProfessionalInstallation());
}

TEST_F(PropertyFeatureTest, EdgeCases) {
    PropertyFeature emptyFeature("", "", "", "", false, false, 0.0, "", 0, false);
    
    EXPECT_EQ(emptyFeature.getFeatureId(), "");
    EXPECT_EQ(emptyFeature.getName(), "");
    EXPECT_EQ(emptyFeature.getDescription(), "");
    EXPECT_EQ(emptyFeature.getCategory(), "");
    EXPECT_FALSE(emptyFeature.getIsStandard());
    EXPECT_FALSE(emptyFeature.getIsPremium());
    EXPECT_DOUBLE_EQ(emptyFeature.getAdditionalCost(), 0.0);
    EXPECT_EQ(emptyFeature.getMaintenanceRequirements(), "");
    EXPECT_EQ(emptyFeature.getLifespanYears(), 0);
    EXPECT_FALSE(emptyFeature.getRequiresProfessionalInstallation());
    
    EXPECT_DOUBLE_EQ(emptyFeature.calculateAnnualMaintenanceCost(), 0.0);
    EXPECT_FALSE(emptyFeature.isEnergyEfficient());
    EXPECT_FALSE(emptyFeature.increasesPropertyValue());
    EXPECT_DOUBLE_EQ(emptyFeature.calculateROI(100000.0), 0.0);
    EXPECT_FALSE(emptyFeature.requiresRegularMaintenance());
    EXPECT_FALSE(emptyFeature.isSafetyFeature());
    EXPECT_DOUBLE_EQ(emptyFeature.calculateDepreciation(), 0.0);
    EXPECT_FALSE(emptyFeature.isLuxuryFeature());
    EXPECT_EQ(emptyFeature.getFeatureGrade(), "basic");
    
    emptyFeature.setAdditionalCost(-1000.0);
    EXPECT_DOUBLE_EQ(emptyFeature.getAdditionalCost(), 0.0);
}

TEST_F(PropertyFeatureTest, CombinedMethods) {
    PropertyFeature energyFeature("F001", "High Efficiency Solar Panels", "Energy saving",
                                 "energy", false, true, 18000.0, "Clean quarterly, inspect annually", 25, true);
    
    EXPECT_TRUE(energyFeature.isEnergyEfficient());
    EXPECT_TRUE(energyFeature.increasesPropertyValue());
    EXPECT_DOUBLE_EQ(energyFeature.calculateAnnualMaintenanceCost(), 360.0);
    EXPECT_DOUBLE_EQ(energyFeature.calculateROI(500000.0), 150.0);
    EXPECT_TRUE(energyFeature.requiresRegularMaintenance());
    EXPECT_FALSE(energyFeature.isSafetyFeature());
    EXPECT_DOUBLE_EQ(energyFeature.calculateDepreciation(), 720.0);
    EXPECT_TRUE(energyFeature.isLuxuryFeature());
    EXPECT_EQ(energyFeature.getFeatureGrade(), "luxury");
    
    PropertyFeature safetyFeature("F002", "Fire Alarm System with CO Detection", "Safety feature",
                                 "safety", true, false, 2500.0, "Test monthly, battery annual", 10, true);
    
    EXPECT_FALSE(safetyFeature.isEnergyEfficient());
    EXPECT_TRUE(safetyFeature.increasesPropertyValue());
    EXPECT_DOUBLE_EQ(safetyFeature.calculateAnnualMaintenanceCost(), 50.0);
    EXPECT_DOUBLE_EQ(safetyFeature.calculateROI(500000.0), 120.0);
    EXPECT_TRUE(safetyFeature.requiresRegularMaintenance());
    EXPECT_TRUE(safetyFeature.isSafetyFeature());
    EXPECT_DOUBLE_EQ(safetyFeature.calculateDepreciation(), 250.0);
    EXPECT_FALSE(safetyFeature.isLuxuryFeature());
    EXPECT_EQ(safetyFeature.getFeatureGrade(), "standard");
}

TEST_F(PropertyFeatureTest, CaseSensitivity) {
    PropertyFeature mixedCase1("F001", "ENERGY Efficient Windows", "Double pane",
                              "ENERGY", true, false, 5000.0, "Clean ANNUALLY", 20, true);
    EXPECT_FALSE(mixedCase1.isEnergyEfficient());
    EXPECT_FALSE(mixedCase1.requiresRegularMaintenance());
    
    PropertyFeature mixedCase2("F002", "Security ALARM", "Home security",
                              "SECURITY", false, true, 3000.0, "Test QUARTERLY", 8, true);
    EXPECT_FALSE(mixedCase2.isSafetyFeature());
    EXPECT_FALSE(mixedCase2.requiresRegularMaintenance());
    
    PropertyFeature mixedCase3("F003", "SAFETY Railings", "Stair safety",
                              "Construction", true, false, 1500.0, "Inspect MONTHLY", 15, false);
    EXPECT_FALSE(mixedCase3.isSafetyFeature());
    EXPECT_FALSE(mixedCase3.requiresRegularMaintenance());
}

TEST_F(PropertyFeatureTest, NegativeAndZeroValues) {
    PropertyFeature negativeFeature("F001", "Negative", "Desc", "Cat",
                                   true, false, -10000.0, "", -5, false);
    
    EXPECT_DOUBLE_EQ(negativeFeature.getAdditionalCost(), -10000.0);
    EXPECT_DOUBLE_EQ(negativeFeature.calculateAnnualMaintenanceCost(), 0.0);
    EXPECT_FALSE(negativeFeature.increasesPropertyValue());
    EXPECT_DOUBLE_EQ(negativeFeature.calculateROI(100000.0), 0.0);
    EXPECT_DOUBLE_EQ(negativeFeature.calculateDepreciation(), 0.0);
    EXPECT_FALSE(negativeFeature.isLuxuryFeature());
    
    PropertyFeature zeroLifespanFeature("F002", "Zero Lifespan", "Desc", "Cat",
                                       true, false, 10000.0, "", 0, true);
    
    EXPECT_DOUBLE_EQ(zeroLifespanFeature.calculateAnnualMaintenanceCost(), 0.0);
    EXPECT_DOUBLE_EQ(zeroLifespanFeature.calculateDepreciation(), 0.0);
}
