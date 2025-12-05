#include <gtest/gtest.h>
#include "../../include/core/House.h"
#include "../../include/core/Address.h"
#include "../../include/core/PropertyStatus.h"
#include "../../include/core/PropertyType.h"
#include <memory>

class HouseTestAdvanced : public ::testing::Test {
protected:
    std::unique_ptr<Address> createAddress() {
        return std::make_unique<Address>("123 Main St", "Springfield", "IL", 
                                        "62704", "USA", 39.7817, -89.6501);
    }
    
    std::unique_ptr<PropertyStatus> createStatus() {
        return std::make_unique<PropertyStatus>("AVAILABLE", "Available for rent", 
                                               true, false, false, 
                                               std::chrono::system_clock::now(), 
                                               "", 0.0, 10);
    }
    
    std::unique_ptr<PropertyType> createType() {
        return std::make_unique<PropertyType>("RESIDENTIAL", "Residential Property");
    }
    
    std::unique_ptr<House> createBasicHouse() {
        return std::make_unique<House>("H1", 200.0, 500000.0, 
                                      createAddress(), createStatus(), createType(),
                                      2, 300.0, true, true, 2, 2010, 
                                      "Modern", 2000.0, true);
    }
};

TEST_F(HouseTestAdvanced, ConstructorEdgeCases) {
    auto house1 = std::make_unique<House>("H1", 0.0, 0.0, 
                                         nullptr, nullptr, nullptr,
                                         -1, -100.0, false, false, -2, 
                                         0, "", -500.0, false);
    
    EXPECT_EQ(house1->getNumberOfFloors(), -1);
    EXPECT_DOUBLE_EQ(house1->getLandArea(), -100.0);
    EXPECT_EQ(house1->getGarageCapacity(), -2);
    EXPECT_EQ(house1->getYearBuilt(), 0);
    EXPECT_DOUBLE_EQ(house1->getPropertyTax(), -500.0);
}

TEST_F(HouseTestAdvanced, CopyConstructor) {
    auto house1 = createBasicHouse();
    house1->setPropertyTax(2500.0);
    house1->updateGarageCapacity(3);
    
    House house2(*house1);
    
    EXPECT_EQ(house2.getId(), "H1");
    EXPECT_DOUBLE_EQ(house2.getArea(), 200.0);
    EXPECT_DOUBLE_EQ(house2.getRentalPrice(), 500000.0);
    EXPECT_EQ(house2.getNumberOfFloors(), 2);
    EXPECT_DOUBLE_EQ(house2.getLandArea(), 300.0);
    EXPECT_TRUE(house2.getHasGarden());
    EXPECT_TRUE(house2.getHasGarage());
    EXPECT_EQ(house2.getGarageCapacity(), 3);
    EXPECT_EQ(house2.getYearBuilt(), 2010);
    EXPECT_EQ(house2.getArchitecturalStyle(), "Modern");
    EXPECT_DOUBLE_EQ(house2.getPropertyTax(), 2500.0);
    EXPECT_TRUE(house2.getHasSwimmingPool());
}

TEST_F(HouseTestAdvanced, CopyAssignment) {
    auto house1 = createBasicHouse();
    house1->setPropertyTax(2500.0);
    house1->updateGarageCapacity(3);
    
    auto house2 = std::make_unique<House>("H2", 100.0, 250000.0, 
                                         createAddress(), createStatus(), createType(),
                                        1, 150.0, false, false, 1, 2000, 
                                        "Classic", 1000.0, false);
    
    *house2 = *house1;
    
    EXPECT_EQ(house2->getId(), "H1");
    EXPECT_DOUBLE_EQ(house2->getArea(), 200.0);
    EXPECT_DOUBLE_EQ(house2->getRentalPrice(), 500000.0);
    EXPECT_EQ(house2->getNumberOfFloors(), 2);
    EXPECT_DOUBLE_EQ(house2->getLandArea(), 300.0);
    EXPECT_TRUE(house2->getHasGarden());
    EXPECT_TRUE(house2->getHasGarage());
    EXPECT_EQ(house2->getGarageCapacity(), 3);
    EXPECT_EQ(house2->getYearBuilt(), 2010);
    EXPECT_EQ(house2->getArchitecturalStyle(), "Modern");
    EXPECT_DOUBLE_EQ(house2->getPropertyTax(), 2500.0);
    EXPECT_TRUE(house2->getHasSwimmingPool());
}

TEST_F(HouseTestAdvanced, MoveConstructor) {
    auto house1 = createBasicHouse();
    House house2(std::move(*house1));
    
    EXPECT_EQ(house2.getId(), "H1");
    EXPECT_DOUBLE_EQ(house2.getArea(), 200.0);
    EXPECT_DOUBLE_EQ(house2.getRentalPrice(), 500000.0);
    EXPECT_EQ(house2.getNumberOfFloors(), 2);
    EXPECT_DOUBLE_EQ(house2.getLandArea(), 300.0);
    EXPECT_TRUE(house2.getHasGarden());
    EXPECT_TRUE(house2.getHasGarage());
    EXPECT_EQ(house2.getGarageCapacity(), 2);
    EXPECT_EQ(house2.getYearBuilt(), 2010);
    EXPECT_EQ(house2.getArchitecturalStyle(), "Modern");
    EXPECT_DOUBLE_EQ(house2.getPropertyTax(), 2000.0);
    EXPECT_TRUE(house2.getHasSwimmingPool());
}

TEST_F(HouseTestAdvanced, MoveAssignment) {
    auto house1 = createBasicHouse();
    auto house2 = std::make_unique<House>("H2", 100.0, 250000.0, 
                                         createAddress(), createStatus(), createType(),
                                        1, 150.0, false, false, 1, 2000, 
                                        "Classic", 1000.0, false);
    
    *house2 = std::move(*house1);
    
    EXPECT_EQ(house2->getId(), "H1");
    EXPECT_DOUBLE_EQ(house2->getArea(), 200.0);
    EXPECT_DOUBLE_EQ(house2->getRentalPrice(), 500000.0);
    EXPECT_EQ(house2->getNumberOfFloors(), 2);
    EXPECT_DOUBLE_EQ(house2->getLandArea(), 300.0);
    EXPECT_TRUE(house2->getHasGarden());
    EXPECT_TRUE(house2->getHasGarage());
    EXPECT_EQ(house2->getGarageCapacity(), 2);
    EXPECT_EQ(house2->getYearBuilt(), 2010);
    EXPECT_EQ(house2->getArchitecturalStyle(), "Modern");
    EXPECT_DOUBLE_EQ(house2->getPropertyTax(), 2000.0);
    EXPECT_TRUE(house2->getHasSwimmingPool());
}

TEST_F(HouseTestAdvanced, SetPropertyTaxEdgeCases) {
    auto house = createBasicHouse();
    
    house->setPropertyTax(-1000.0);
    EXPECT_DOUBLE_EQ(house->getPropertyTax(), 0.0);
    
    house->setPropertyTax(0.0);
    EXPECT_DOUBLE_EQ(house->getPropertyTax(), 0.0);
    
    house->setPropertyTax(1500.0);
    EXPECT_DOUBLE_EQ(house->getPropertyTax(), 1500.0);
}

TEST_F(HouseTestAdvanced, UpdateGarageCapacityEdgeCases) {
    auto house = createBasicHouse();
    
    house->updateGarageCapacity(-5);
    EXPECT_EQ(house->getGarageCapacity(), 0);
    
    house->updateGarageCapacity(0);
    EXPECT_EQ(house->getGarageCapacity(), 0);
    
    house->updateGarageCapacity(4);
    EXPECT_EQ(house->getGarageCapacity(), 4);
}

TEST_F(HouseTestAdvanced, CalculateBuildingToLandRatioEdgeCases) {
    auto house1 = std::make_unique<House>("H1", 200.0, 500000.0, 
                                         createAddress(), createStatus(), createType(),
                                        2, 0.0, true, true, 2, 2010, 
                                         "Modern", 2000.0, true);
    
    double ratio1 = house1->calculateBuildingToLandRatio();
    EXPECT_DOUBLE_EQ(ratio1, 0.0);
    
    auto house2 = std::make_unique<House>("H2", 200.0, 500000.0, 
                                         createAddress(), createStatus(), createType(),
                                        2, -300.0, true, true, 2, 2010, 
                                         "Modern", 2000.0, true);
    
    double ratio2 = house2->calculateBuildingToLandRatio();
    EXPECT_DOUBLE_EQ(ratio2, 0.0);
    
    auto house3 = std::make_unique<House>("H3", 0.0, 500000.0, 
                                         createAddress(), createStatus(), createType(),
                                        2, 300.0, true, true, 2, 2010, 
                                         "Modern", 2000.0, true);
    
    double ratio3 = house3->calculateBuildingToLandRatio();
    EXPECT_DOUBLE_EQ(ratio3, 0.0);
}

TEST_F(HouseTestAdvanced, IsNewConstructionBoundaryCases) {
    auto house1 = std::make_unique<House>("H1", 200.0, 500000.0, 
                                         createAddress(), createStatus(), createType(),
                                        2, 300.0, true, true, 2, 2020, 
                                         "Modern", 2000.0, true);
    
    EXPECT_TRUE(house1->isNewConstruction(2025));
    EXPECT_TRUE(house1->isNewConstruction(2024));
    EXPECT_FALSE(house1->isNewConstruction(2026));
    
    auto house2 = std::make_unique<House>("H2", 200.0, 500000.0, 
                                         createAddress(), createStatus(), createType(),
                                        2, 300.0, true, true, 2, 2015, 
                                         "Modern", 2000.0, true);
    
    EXPECT_FALSE(house2->isNewConstruction(2025));
    
    auto house3 = std::make_unique<House>("H3", 200.0, 500000.0, 
                                         createAddress(), createStatus(), createType(),
                                        2, 300.0, true, true, 2, 2020, 
                                         "Modern", 2000.0, true);
    
    EXPECT_TRUE(house3->isNewConstruction(2020));
}

TEST_F(HouseTestAdvanced, CalculateAnnualPropertyTax) {
    auto house1 = createBasicHouse();
    double annualTax1 = house1->calculateAnnualPropertyTax();
    EXPECT_DOUBLE_EQ(annualTax1, 2000.0 * 12.0);
    
    auto house2 = std::make_unique<House>("H2", 200.0, 500000.0, 
                                         createAddress(), createStatus(), createType(),
                                        2, 300.0, true, true, 2, 2010, 
                                         "Modern", 0.0, true);
    
    double annualTax2 = house2->calculateAnnualPropertyTax();
    EXPECT_DOUBLE_EQ(annualTax2, 0.0);
    
    auto house3 = std::make_unique<House>("H3", 200.0, 500000.0, 
                                         createAddress(), createStatus(), createType(),
                                        2, 300.0, true, true, 2, 2010, 
                                         "Modern", -500.0, true);
    
    double annualTax3 = house3->calculateAnnualPropertyTax();
    EXPECT_DOUBLE_EQ(annualTax3, -500.0 * 12.0);
}

TEST_F(HouseTestAdvanced, HasMultipleParkingSpaces) {
    auto house1 = std::make_unique<House>("H1", 200.0, 500000.0, 
                                         createAddress(), createStatus(), createType(),
                                        2, 300.0, true, true, 2, 2010, 
                                         "Modern", 2000.0, true);
    
    EXPECT_TRUE(house1->hasMultipleParkingSpaces());
    
    auto house2 = std::make_unique<House>("H2", 200.0, 500000.0, 
                                         createAddress(), createStatus(), createType(),
                                        2, 300.0, true, true, 1, 2010, 
                                         "Modern", 2000.0, true);
    
    EXPECT_FALSE(house2->hasMultipleParkingSpaces());
    
    auto house3 = std::make_unique<House>("H3", 200.0, 500000.0, 
                                         createAddress(), createStatus(), createType(),
                                        2, 300.0, true, true, 0, 2010, 
                                         "Modern", 2000.0, true);
    
    EXPECT_FALSE(house3->hasMultipleParkingSpaces());
    
    auto house4 = std::make_unique<House>("H4", 200.0, 500000.0, 
                                         createAddress(), createStatus(), createType(),
                                        2, 300.0, true, true, 5, 2010, 
                                         "Modern", 2000.0, true);
    
    EXPECT_TRUE(house4->hasMultipleParkingSpaces());
}

TEST_F(HouseTestAdvanced, CalculateLandValuePerSqMeterEdgeCases) {
    auto house1 = createBasicHouse();
    double value1 = house1->calculateLandValuePerSqMeter(600000.0);
    EXPECT_DOUBLE_EQ(value1, 600000.0 / 300.0);
    
    auto house2 = std::make_unique<House>("H2", 200.0, 500000.0, 
                                         createAddress(), createStatus(), createType(),
                                        2, 0.0, true, true, 2, 2010, 
                                         "Modern", 2000.0, true);
    
    double value2 = house2->calculateLandValuePerSqMeter(600000.0);
    EXPECT_DOUBLE_EQ(value2, 0.0);
    
    auto house3 = std::make_unique<House>("H3", 200.0, 500000.0, 
                                         createAddress(), createStatus(), createType(),
                                        2, -300.0, true, true, 2, 2010, 
                                         "Modern", 2000.0, true);
    
    double value3 = house3->calculateLandValuePerSqMeter(600000.0);
    EXPECT_DOUBLE_EQ(value3, 0.0);
    
    auto house4 = createBasicHouse();
    double value4 = house4->calculateLandValuePerSqMeter(0.0);
    EXPECT_DOUBLE_EQ(value4, 0.0);
}

TEST_F(HouseTestAdvanced, RequiresMajorRenovation) {
    auto house1 = std::make_unique<House>("H1", 200.0, 500000.0, 
                                         createAddress(), createStatus(), createType(),
                                        2, 300.0, true, false, 0, 1990, 
                                         "Modern", 2000.0, false);
    
    EXPECT_TRUE(house1->requiresMajorRenovation(2025));
    
    auto house2 = std::make_unique<House>("H2", 200.0, 500000.0, 
                                         createAddress(), createStatus(), createType(),
                                        2, 300.0, true, true, 2, 1990, 
                                         "Modern", 2000.0, false);
    
    EXPECT_FALSE(house2->requiresMajorRenovation(2025));
    
    auto house3 = std::make_unique<House>("H3", 200.0, 500000.0, 
                                         createAddress(), createStatus(), createType(),
                                        2, 300.0, true, false, 0, 1990, 
                                         "Modern", 2000.0, true);
    
    EXPECT_FALSE(house3->requiresMajorRenovation(2025));
    
    auto house4 = std::make_unique<House>("H4", 200.0, 500000.0, 
                                         createAddress(), createStatus(), createType(),
                                        2, 300.0, true, false, 0, 2005, 
                                         "Modern", 2000.0, false);
    
    EXPECT_FALSE(house4->requiresMajorRenovation(2025));
    
    auto house5 = std::make_unique<House>("H5", 200.0, 500000.0, 
                                         createAddress(), createStatus(), createType(),
                                        2, 300.0, true, true, 2, 1990, 
                                         "Modern", 2000.0, true);
    
    EXPECT_FALSE(house5->requiresMajorRenovation(2025));
}

TEST_F(HouseTestAdvanced, CalculatePremiumFeaturesValue) {
    auto house1 = std::make_unique<House>("H1", 200.0, 1000000.0, 
                                         createAddress(), createStatus(), createType(),
                                        1, 300.0, false, false, 0, 2010, 
                                         "Modern", 2000.0, false);
    
    double premium1 = house1->calculatePremiumFeaturesValue();
    EXPECT_DOUBLE_EQ(premium1, 0.0);
    
    auto house2 = std::make_unique<House>("H2", 200.0, 1000000.0, 
                                         createAddress(), createStatus(), createType(),
                                        1, 300.0, true, false, 0, 2010, 
                                         "Modern", 2000.0, false);
    
    double premium2 = house2->calculatePremiumFeaturesValue();
    EXPECT_DOUBLE_EQ(premium2, 1000000.0 * 0.08);
    
    auto house3 = std::make_unique<House>("H3", 200.0, 1000000.0, 
                                         createAddress(), createStatus(), createType(),
                                        1, 300.0, false, true, 0, 2010, 
                                         "Modern", 2000.0, false);
    
    double premium3 = house3->calculatePremiumFeaturesValue();
    EXPECT_DOUBLE_EQ(premium3, 1000000.0 * 0.10);
    
    auto house4 = std::make_unique<House>("H4", 200.0, 1000000.0, 
                                         createAddress(), createStatus(), createType(),
                                        1, 300.0, false, false, 0, 2010, 
                                         "Modern", 2000.0, true);
    
    double premium4 = house4->calculatePremiumFeaturesValue();
    EXPECT_DOUBLE_EQ(premium4, 1000000.0 * 0.15);
    
    auto house5 = std::make_unique<House>("H5", 200.0, 1000000.0, 
                                         createAddress(), createStatus(), createType(),
                                        2, 300.0, false, false, 0, 2010, 
                                         "Modern", 2000.0, false);
    
    double premium5 = house5->calculatePremiumFeaturesValue();
    EXPECT_DOUBLE_EQ(premium5, 1000000.0 * 0.05);
    
    auto house6 = std::make_unique<House>("H6", 200.0, 1000000.0, 
                                         createAddress(), createStatus(), createType(),
                                        2, 300.0, true, true, 2, 2010, 
                                         "Modern", 2000.0, true);
    
    double premium6 = house6->calculatePremiumFeaturesValue();
    double expected = 1000000.0 * (0.15 + 0.10 + 0.08 + 0.05);
    EXPECT_DOUBLE_EQ(premium6, expected);
}

TEST_F(HouseTestAdvanced, CalculatePremiumFeaturesValueZeroRentalPrice) {
    auto house = std::make_unique<House>("H1", 200.0, 0.0, 
                                        createAddress(), createStatus(), createType(),
                                        2, 300.0, true, true, 2, 2010, 
                                        "Modern", 2000.0, true);
    
    double premium = house->calculatePremiumFeaturesValue();
    EXPECT_DOUBLE_EQ(premium, 0.0);
}

TEST_F(HouseTestAdvanced, CalculatePremiumFeaturesValueNegativeRentalPrice) {
    auto house = std::make_unique<House>("H1", 200.0, -1000000.0, 
                                        createAddress(), createStatus(), createType(),
                                        2, 300.0, true, true, 2, 2010, 
                                        "Modern", 2000.0, true);
    
    double premium = house->calculatePremiumFeaturesValue();
    double expected = -1000000.0 * (0.15 + 0.10 + 0.08 + 0.05);
    EXPECT_DOUBLE_EQ(premium, expected);
}