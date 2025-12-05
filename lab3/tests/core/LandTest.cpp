#include <gtest/gtest.h>
#include "../../include/core/Land.h"
#include "../../include/core/Address.h"
#include "../../include/core/PropertyStatus.h"
#include "../../include/core/PropertyType.h"
#include <memory>

class LandTest : public ::testing::Test {
protected:
    std::unique_ptr<Address> createAddress() {
        return std::make_unique<Address>("123 Main St", "Springfield", "IL", 
                                        "62704", "USA", 39.7817, -89.6501);
    }
    
    std::unique_ptr<PropertyStatus> createStatus() {
        return std::make_unique<PropertyStatus>("AVAILABLE", "Available", 
                                               true, false, false, 
                                               std::chrono::system_clock::now(), 
                                               "", 0.0, 10);
    }
    
    std::unique_ptr<PropertyType> createType() {
        return std::make_unique<PropertyType>("LAND", "Land Property");
    }
    
    std::unique_ptr<Land> createBasicLand() {
        return std::make_unique<Land>("L1", 5000.0, 250000.0, 
                                     createAddress(), createStatus(), createType(),
                                     "residential", true, false, 100.0, 50.0,
                                     "flat", true, "loam", 2000.0);
    }
};

TEST_F(LandTest, ConstructorEdgeCases) {
    Land land1("", 0.0, 0.0, nullptr, nullptr, nullptr,
               "", false, false, -100.0, -50.0, "", false, "", -500.0);
    
    EXPECT_EQ(land1.getId(), "");
    EXPECT_DOUBLE_EQ(land1.getArea(), 0.0);
    EXPECT_DOUBLE_EQ(land1.getRentalPrice(), 0.0);
    EXPECT_EQ(land1.getLandUse(), "");
    EXPECT_FALSE(land1.getHasUtilities());
    EXPECT_FALSE(land1.getIsAgricultural());
    EXPECT_DOUBLE_EQ(land1.getFrontage(), -100.0);
    EXPECT_DOUBLE_EQ(land1.getDepth(), -50.0);
    EXPECT_EQ(land1.getTopography(), "");
    EXPECT_FALSE(land1.getHasWaterAccess());
    EXPECT_EQ(land1.getSoilType(), "");
    EXPECT_DOUBLE_EQ(land1.getAnnualTax(), -500.0);
}

TEST_F(LandTest, CopyConstructor) {
    auto land1 = createBasicLand();
    land1->setLandUse("commercial");
    land1->setAnnualTax(2500.0);
    
    Land land2(*land1);
    
    EXPECT_EQ(land2.getId(), "L1");
    EXPECT_DOUBLE_EQ(land2.getArea(), 5000.0);
    EXPECT_DOUBLE_EQ(land2.getRentalPrice(), 250000.0);
    EXPECT_EQ(land2.getLandUse(), "commercial");
    EXPECT_TRUE(land2.getHasUtilities());
    EXPECT_FALSE(land2.getIsAgricultural());
    EXPECT_DOUBLE_EQ(land2.getFrontage(), 100.0);
    EXPECT_DOUBLE_EQ(land2.getDepth(), 50.0);
    EXPECT_EQ(land2.getTopography(), "flat");
    EXPECT_TRUE(land2.getHasWaterAccess());
    EXPECT_EQ(land2.getSoilType(), "loam");
    EXPECT_DOUBLE_EQ(land2.getAnnualTax(), 2500.0);
}

TEST_F(LandTest, CopyAssignment) {
    auto land1 = createBasicLand();
    land1->setLandUse("commercial");
    land1->setAnnualTax(2500.0);
    
    auto land2 = std::make_unique<Land>("L2", 1000.0, 100000.0,
                                       createAddress(), createStatus(), createType(),
                                       "agricultural", false, true, 50.0, 100.0,
                                       "hilly", false, "clay", 1000.0);
    
    *land2 = *land1;
    
    EXPECT_EQ(land2->getId(), "L1");
    EXPECT_DOUBLE_EQ(land2->getArea(), 5000.0);
    EXPECT_DOUBLE_EQ(land2->getRentalPrice(), 250000.0);
    EXPECT_EQ(land2->getLandUse(), "commercial");
    EXPECT_TRUE(land2->getHasUtilities());
    EXPECT_FALSE(land2->getIsAgricultural());
    EXPECT_DOUBLE_EQ(land2->getFrontage(), 100.0);
    EXPECT_DOUBLE_EQ(land2->getDepth(), 50.0);
    EXPECT_EQ(land2->getTopography(), "flat");
    EXPECT_TRUE(land2->getHasWaterAccess());
    EXPECT_EQ(land2->getSoilType(), "loam");
    EXPECT_DOUBLE_EQ(land2->getAnnualTax(), 2500.0);
}

TEST_F(LandTest, MoveConstructor) {
    auto land1 = createBasicLand();
    Land land2(std::move(*land1));
    
    EXPECT_EQ(land2.getId(), "L1");
    EXPECT_DOUBLE_EQ(land2.getArea(), 5000.0);
    EXPECT_DOUBLE_EQ(land2.getRentalPrice(), 250000.0);
    EXPECT_EQ(land2.getLandUse(), "residential");
    EXPECT_TRUE(land2.getHasUtilities());
    EXPECT_FALSE(land2.getIsAgricultural());
    EXPECT_DOUBLE_EQ(land2.getFrontage(), 100.0);
    EXPECT_DOUBLE_EQ(land2.getDepth(), 50.0);
    EXPECT_EQ(land2.getTopography(), "flat");
    EXPECT_TRUE(land2.getHasWaterAccess());
    EXPECT_EQ(land2.getSoilType(), "loam");
    EXPECT_DOUBLE_EQ(land2.getAnnualTax(), 2000.0);
}

TEST_F(LandTest, MoveAssignment) {
    auto land1 = createBasicLand();
    auto land2 = std::make_unique<Land>("L2", 1000.0, 100000.0,
                                       createAddress(), createStatus(), createType(),
                                       "agricultural", false, true, 50.0, 100.0,
                                       "hilly", false, "clay", 1000.0);
    
    *land2 = std::move(*land1);
    
    EXPECT_EQ(land2->getId(), "L1");
    EXPECT_DOUBLE_EQ(land2->getArea(), 5000.0);
    EXPECT_DOUBLE_EQ(land2->getRentalPrice(), 250000.0);
    EXPECT_EQ(land2->getLandUse(), "residential");
    EXPECT_TRUE(land2->getHasUtilities());
    EXPECT_FALSE(land2->getIsAgricultural());
    EXPECT_DOUBLE_EQ(land2->getFrontage(), 100.0);
    EXPECT_DOUBLE_EQ(land2->getDepth(), 50.0);
    EXPECT_EQ(land2->getTopography(), "flat");
    EXPECT_TRUE(land2->getHasWaterAccess());
    EXPECT_EQ(land2->getSoilType(), "loam");
    EXPECT_DOUBLE_EQ(land2->getAnnualTax(), 2000.0);
}

TEST_F(LandTest, SetAnnualTaxEdgeCases) {
    auto land = createBasicLand();
    
    land->setAnnualTax(-1000.0);
    EXPECT_DOUBLE_EQ(land->getAnnualTax(), 0.0);
    
    land->setAnnualTax(0.0);
    EXPECT_DOUBLE_EQ(land->getAnnualTax(), 0.0);
    
    land->setAnnualTax(1500.0);
    EXPECT_DOUBLE_EQ(land->getAnnualTax(), 1500.0);
}

TEST_F(LandTest, CalculatePerimeterEdgeCases) {
    Land land1("L1", 5000.0, 250000.0, createAddress(), createStatus(), createType(),
               "residential", true, false, -100.0, 50.0, "flat", true, "loam", 2000.0);
    
    double perimeter1 = land1.calculatePerimeter();
    EXPECT_DOUBLE_EQ(perimeter1, 2 * (-100.0 + 50.0));
    
    Land land2("L2", 5000.0, 250000.0, createAddress(), createStatus(), createType(),
               "residential", true, false, 0.0, 0.0, "flat", true, "loam", 2000.0);
    
    double perimeter2 = land2.calculatePerimeter();
    EXPECT_DOUBLE_EQ(perimeter2, 0.0);
    
    Land land3("L3", 5000.0, 250000.0, createAddress(), createStatus(), createType(),
               "residential", true, false, 100.0, -50.0, "flat", true, "loam", 2000.0);
    
    double perimeter3 = land3.calculatePerimeter();
    EXPECT_DOUBLE_EQ(perimeter3, 2 * (100.0 + -50.0));
}

TEST_F(LandTest, IsSuitableForConstructionAllCases) {
    Land land1("L1", 5000.0, 250000.0, createAddress(), createStatus(), createType(),
               "residential", true, false, 100.0, 50.0, "flat", true, "sand", 2000.0);
    EXPECT_TRUE(land1.isSuitableForConstruction());
    
    Land land2("L2", 5000.0, 250000.0, createAddress(), createStatus(), createType(),
               "residential", false, false, 100.0, 50.0, "flat", true, "sand", 2000.0);
    EXPECT_FALSE(land2.isSuitableForConstruction());
    
    Land land3("L3", 5000.0, 250000.0, createAddress(), createStatus(), createType(),
               "residential", true, false, 100.0, 50.0, "mountainous", true, "sand", 2000.0);
    EXPECT_FALSE(land3.isSuitableForConstruction());
    
    Land land4("L4", 5000.0, 250000.0, createAddress(), createStatus(), createType(),
               "residential", true, false, 100.0, 50.0, "flat", true, "wetland", 2000.0);
    EXPECT_FALSE(land4.isSuitableForConstruction());
    
    Land land5("L5", 5000.0, 250000.0, createAddress(), createStatus(), createType(),
               "residential", false, false, 100.0, 50.0, "mountainous", true, "wetland", 2000.0);
    EXPECT_FALSE(land5.isSuitableForConstruction());
}

TEST_F(LandTest, IsSuitableForFarmingAllCases) {
    Land land1("L1", 5000.0, 250000.0, createAddress(), createStatus(), createType(),
               "agricultural", true, true, 100.0, 50.0, "flat", true, "loam", 2000.0);
    EXPECT_TRUE(land1.isSuitableForFarming());
    
    Land land2("L2", 5000.0, 250000.0, createAddress(), createStatus(), createType(),
               "agricultural", true, true, 100.0, 50.0, "flat", true, "clay", 2000.0);
    EXPECT_TRUE(land2.isSuitableForFarming());
    
    Land land3("L3", 5000.0, 250000.0, createAddress(), createStatus(), createType(),
               "agricultural", true, true, 100.0, 50.0, "flat", true, "silt", 2000.0);
    EXPECT_TRUE(land3.isSuitableForFarming());
    
    Land land4("L4", 5000.0, 250000.0, createAddress(), createStatus(), createType(),
               "agricultural", true, false, 100.0, 50.0, "flat", true, "loam", 2000.0);
    EXPECT_FALSE(land4.isSuitableForFarming());
    
    Land land5("L5", 5000.0, 250000.0, createAddress(), createStatus(), createType(),
               "agricultural", true, true, 100.0, 50.0, "flat", false, "loam", 2000.0);
    EXPECT_FALSE(land5.isSuitableForFarming());
    
    Land land6("L6", 5000.0, 250000.0, createAddress(), createStatus(), createType(),
               "agricultural", true, true, 100.0, 50.0, "flat", true, "sand", 2000.0);
    EXPECT_FALSE(land6.isSuitableForFarming());
    
    Land land7("L7", 5000.0, 250000.0, createAddress(), createStatus(), createType(),
               "agricultural", true, true, 100.0, 50.0, "flat", true, "rocky", 2000.0);
    EXPECT_FALSE(land7.isSuitableForFarming());
}

TEST_F(LandTest, CalculateFrontageToDepthRatioEdgeCases) {
    Land land1("L1", 5000.0, 250000.0, createAddress(), createStatus(), createType(),
               "residential", true, false, 100.0, 0.0, "flat", true, "loam", 2000.0);
    
    double ratio1 = land1.calculateFrontageToDepthRatio();
    EXPECT_DOUBLE_EQ(ratio1, 0.0);
    
    Land land2("L2", 5000.0, 250000.0, createAddress(), createStatus(), createType(),
               "residential", true, false, 100.0, -50.0, "flat", true, "loam", 2000.0);
    
    double ratio2 = land2.calculateFrontageToDepthRatio();
    EXPECT_DOUBLE_EQ(ratio2, 0.0);
    
    Land land3("L3", 5000.0, 250000.0, createAddress(), createStatus(), createType(),
               "residential", true, false, 0.0, 50.0, "flat", true, "loam", 2000.0);
    
    double ratio3 = land3.calculateFrontageToDepthRatio();
    EXPECT_DOUBLE_EQ(ratio3, 0.0);
    
    Land land4("L4", 5000.0, 250000.0, createAddress(), createStatus(), createType(),
               "residential", true, false, 75.0, 25.0, "flat", true, "loam", 2000.0);
    
    double ratio4 = land4.calculateFrontageToDepthRatio();
    EXPECT_DOUBLE_EQ(ratio4, 75.0 / 25.0);
}

TEST_F(LandTest, HasDevelopmentPotentialAllCases) {
    Land land1("L1", 5000.0, 250000.0, createAddress(), createStatus(), createType(),
               "residential", true, false, 100.0, 50.0, "flat", true, "loam", 2000.0);
    EXPECT_TRUE(land1.hasDevelopmentPotential());
    
    Land land2("L2", 5000.0, 250000.0, createAddress(), createStatus(), createType(),
               "residential", false, false, 100.0, 50.0, "flat", true, "loam", 2000.0);
    EXPECT_FALSE(land2.hasDevelopmentPotential());
    
    Land land3("L3", 5000.0, 250000.0, createAddress(), createStatus(), createType(),
               "residential", true, false, 20.0, 50.0, "flat", true, "loam", 2000.0);
    EXPECT_FALSE(land3.hasDevelopmentPotential());
}

TEST_F(LandTest, CalculateLandValuePerAcreEdgeCases) {
    Land land1("L1", 0.0, 250000.0, createAddress(), createStatus(), createType(),
               "residential", true, false, 100.0, 50.0, "flat", true, "loam", 2000.0);
    
    double value1 = land1.calculateLandValuePerAcre();
    EXPECT_DOUBLE_EQ(value1, 0.0);
    
    Land land2("L2", -5000.0, 250000.0, createAddress(), createStatus(), createType(),
               "residential", true, false, 100.0, 50.0, "flat", true, "loam", 2000.0);
    
    double value2 = land2.calculateLandValuePerAcre();
    EXPECT_DOUBLE_EQ(value2, 0.0);
    
    Land land3("L3", 5000.0, 0.0, createAddress(), createStatus(), createType(),
               "residential", true, false, 100.0, 50.0, "flat", true, "loam", 2000.0);
    
    double value3 = land3.calculateLandValuePerAcre();
    EXPECT_DOUBLE_EQ(value3, 0.0);
    
    Land land4("L4", 5000.0, -250000.0, createAddress(), createStatus(), createType(),
               "residential", true, false, 100.0, 50.0, "flat", true, "loam", 2000.0);
    
    double value4 = land4.calculateLandValuePerAcre();
    EXPECT_DOUBLE_EQ(value4, -250000.0 / (5000.0 / 4046.86));
    
    Land land5("L5", 4046.86, 100000.0, createAddress(), createStatus(), createType(),
               "residential", true, false, 100.0, 50.0, "flat", true, "loam", 2000.0);
    
    double value5 = land5.calculateLandValuePerAcre();
    EXPECT_DOUBLE_EQ(value5, 100000.0);
}

TEST_F(LandTest, RequiresZoningChangeAllCases) {
    Land land1("L1", 5000.0, 250000.0, createAddress(), createStatus(), createType(),
               "residential", true, false, 100.0, 50.0, "flat", true, "loam", 2000.0);
    
    EXPECT_FALSE(land1.requiresZoningChange("residential"));
    EXPECT_TRUE(land1.requiresZoningChange("commercial"));
    EXPECT_TRUE(land1.requiresZoningChange("industrial"));
    EXPECT_FALSE(land1.requiresZoningChange("conservation"));
    
    Land land2("L2", 5000.0, 250000.0, createAddress(), createStatus(), createType(),
               "commercial", true, false, 100.0, 50.0, "flat", true, "loam", 2000.0);
    
    EXPECT_TRUE(land2.requiresZoningChange("residential"));
    EXPECT_FALSE(land2.requiresZoningChange("commercial"));
    EXPECT_TRUE(land2.requiresZoningChange("industrial"));
    EXPECT_FALSE(land2.requiresZoningChange("conservation"));
    
    Land land3("L3", 5000.0, 250000.0, createAddress(), createStatus(), createType(),
               "industrial", true, false, 100.0, 50.0, "flat", true, "loam", 2000.0);
    
    EXPECT_TRUE(land3.requiresZoningChange("residential"));
    EXPECT_TRUE(land3.requiresZoningChange("commercial"));
    EXPECT_FALSE(land3.requiresZoningChange("industrial"));
    EXPECT_FALSE(land3.requiresZoningChange("conservation"));
    
    Land land4("L4", 5000.0, 250000.0, createAddress(), createStatus(), createType(),
               "", true, false, 100.0, 50.0, "flat", true, "loam", 2000.0);
    
    EXPECT_TRUE(land4.requiresZoningChange("residential"));
    EXPECT_TRUE(land4.requiresZoningChange("commercial"));
    EXPECT_TRUE(land4.requiresZoningChange("industrial"));
    EXPECT_FALSE(land4.requiresZoningChange("conservation"));
}

TEST_F(LandTest, CalculateInfrastructureCostAllCases) {
    Land land1("L1", 5000.0, 250000.0, createAddress(), createStatus(), createType(),
               "residential", true, false, 100.0, 50.0, "flat", true, "loam", 2000.0);
    
    double cost1 = land1.calculateInfrastructureCost();
    EXPECT_DOUBLE_EQ(cost1, 0.0);
    
    Land land2("L2", 5000.0, 250000.0, createAddress(), createStatus(), createType(),
               "residential", false, false, 100.0, 50.0, "flat", true, "loam", 2000.0);
    
    double cost2 = land2.calculateInfrastructureCost();
    EXPECT_DOUBLE_EQ(cost2, 5000.0 * 5.0);
    
    Land land3("L3", 5000.0, 250000.0, createAddress(), createStatus(), createType(),
               "residential", true, false, 100.0, 50.0, "mountainous", true, "loam", 2000.0);
    
    double cost3 = land3.calculateInfrastructureCost();
    EXPECT_DOUBLE_EQ(cost3, 5000.0 * 10.0);
    
    Land land4("L4", 5000.0, 250000.0, createAddress(), createStatus(), createType(),
               "residential", true, false, 100.0, 50.0, "flat", false, "loam", 2000.0);
    
    double cost4 = land4.calculateInfrastructureCost();
    EXPECT_DOUBLE_EQ(cost4, 5000.0);
    
    Land land5("L5", 5000.0, 250000.0, createAddress(), createStatus(), createType(),
               "residential", false, false, 100.0, 50.0, "mountainous", false, "loam", 2000.0);
    
    double cost5 = land5.calculateInfrastructureCost();
    EXPECT_DOUBLE_EQ(cost5, (5000.0 * 5.0) + (5000.0 * 10.0) + 5000.0);
    
    Land land6("L6", 0.0, 250000.0, createAddress(), createStatus(), createType(),
               "residential", false, false, 100.0, 50.0, "mountainous", false, "loam", 2000.0);
    
    double cost6 = land6.calculateInfrastructureCost();
    EXPECT_DOUBLE_EQ(cost6, 5000.0);
    
    Land land7("L7", -5000.0, 250000.0, createAddress(), createStatus(), createType(),
               "residential", false, false, 100.0, 50.0, "mountainous", false, "loam", 2000.0);
    
    double cost7 = land7.calculateInfrastructureCost();
    EXPECT_DOUBLE_EQ(cost7, (-5000.0 * 5.0) + (-5000.0 * 10.0) + 5000.0);
}

TEST_F(LandTest, CalculateInfrastructureCostDifferentTopography) {
    Land land1("L1", 5000.0, 250000.0, createAddress(), createStatus(), createType(),
               "residential", false, false, 100.0, 50.0, "hilly", false, "loam", 2000.0);
    
    double cost1 = land1.calculateInfrastructureCost();
    EXPECT_DOUBLE_EQ(cost1, (5000.0 * 5.0) + 5000.0);
    
    Land land2("L2", 5000.0, 250000.0, createAddress(), createStatus(), createType(),
               "residential", false, false, 100.0, 50.0, "sloping", false, "loam", 2000.0);
    
    double cost2 = land2.calculateInfrastructureCost();
    EXPECT_DOUBLE_EQ(cost2, (5000.0 * 5.0) + 5000.0);
}
