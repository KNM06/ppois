#include <gtest/gtest.h>
#include <memory>
#include "../../include/core/CommercialSpace.h"
#include "../../include/core/Address.h"
#include "../../include/core/PropertyStatus.h"
#include "../../include/core/PropertyType.h"

class CommercialSpaceTest : public ::testing::Test {
protected:
    void SetUp() override {
        address = std::make_unique<Address>("123 Commerce St", "Business City", 
                                           "Business State", "12345", "Country", 
                                           40.7128, -74.0060);
        status = std::make_unique<PropertyStatus>("AVAILABLE", "Available for lease",
                                                 true, false, false,
                                                 std::chrono::system_clock::now(),
                                                 "New listing", 0.0, 30);
        type = std::make_unique<PropertyType>("COMMERCIAL", "Commercial", 
                                             "Retail Space", 1, 0, 0.0, 0.0,
                                             false, "commercial", 1.0);
    }
    
    std::unique_ptr<Address> address;
    std::unique_ptr<PropertyStatus> status;
    std::unique_ptr<PropertyType> type;
};

TEST_F(CommercialSpaceTest, ConstructorAndGetters) {
    CommercialSpace space("COM001", 500.0, 12000.0, std::move(address),
                         std::move(status), std::move(type), "retail",
                         true, 25, 800.0, true, "commercial_retail",
                         50, 2400.0, true);
    
    EXPECT_EQ(space.getId(), "COM001");
    EXPECT_DOUBLE_EQ(space.getArea(), 500.0);
    EXPECT_DOUBLE_EQ(space.getRentalPrice(), 12000.0);
    EXPECT_EQ(space.getBusinessType(), "retail");
    EXPECT_TRUE(space.getHasStorefront());
    EXPECT_EQ(space.getParkingSpaces(), 25);
    EXPECT_DOUBLE_EQ(space.getCommonAreaMaintenance(), 800.0);
    EXPECT_TRUE(space.getHasLoadingDock());
    EXPECT_EQ(space.getZoningType(), "commercial_retail");
    EXPECT_EQ(space.getMaxOccupancy(), 50);
    EXPECT_DOUBLE_EQ(space.getAnnualTax(), 2400.0);
    EXPECT_TRUE(space.getIsGroundFloor());
}

TEST_F(CommercialSpaceTest, Setters) {
    CommercialSpace space("COM001", 500.0, 12000.0, 
                         std::make_unique<Address>("Test"), 
                         std::make_unique<PropertyStatus>("AVAILABLE"),
                         std::make_unique<PropertyType>("COMMERCIAL"),
                         "retail", true, 25, 800.0, true, 
                         "commercial_retail", 50, 2400.0, true);
    
    space.setBusinessType("office");
    EXPECT_EQ(space.getBusinessType(), "office");
    
    space.setBusinessType("");
    EXPECT_EQ(space.getBusinessType(), "");
    
    space.setAnnualTax(3000.0);
    EXPECT_DOUBLE_EQ(space.getAnnualTax(), 3000.0);
    
    space.setAnnualTax(0.0);
    EXPECT_DOUBLE_EQ(space.getAnnualTax(), 0.0);
    
    space.setAnnualTax(-500.0);
    EXPECT_DOUBLE_EQ(space.getAnnualTax(), 0.0);
    
    space.setAnnualTax(1500.75);
    EXPECT_DOUBLE_EQ(space.getAnnualTax(), 1500.75);
}

TEST_F(CommercialSpaceTest, CalculateTotalOccupancyCost) {
    CommercialSpace space1("COM001", 500.0, 12000.0, 
                          std::make_unique<Address>("Test"), 
                          std::make_unique<PropertyStatus>("AVAILABLE"),
                          std::make_unique<PropertyType>("COMMERCIAL"),
                          "retail", true, 25, 800.0, true, 
                          "commercial_retail", 50, 2400.0, true);
    
    EXPECT_DOUBLE_EQ(space1.calculateTotalOccupancyCost(), 12000.0 + 800.0 + (2400.0 / 12.0));
    
    CommercialSpace space2("COM002", 300.0, 8000.0,
                          std::make_unique<Address>("Test"), 
                          std::make_unique<PropertyStatus>("AVAILABLE"),
                          std::make_unique<PropertyType>("COMMERCIAL"),
                          "office", false, 10, 400.0, false,
                          "commercial_office", 30, 1200.0, false);
    
    EXPECT_DOUBLE_EQ(space2.calculateTotalOccupancyCost(), 8000.0 + 400.0 + (1200.0 / 12.0));
    
    CommercialSpace space3("COM003", 200.0, 0.0,
                          std::make_unique<Address>("Test"), 
                          std::make_unique<PropertyStatus>("AVAILABLE"),
                          std::make_unique<PropertyType>("COMMERCIAL"),
                          "retail", true, 5, 0.0, false,
                          "commercial_retail", 20, 0.0, true);
    
    EXPECT_DOUBLE_EQ(space3.calculateTotalOccupancyCost(), 0.0);
    
    CommercialSpace space4("COM004", 100.0, 5000.0,
                          std::make_unique<Address>("Test"), 
                          std::make_unique<PropertyStatus>("AVAILABLE"),
                          std::make_unique<PropertyType>("COMMERCIAL"),
                          "office", false, 8, 200.0, false,
                          "commercial_office", 15, -600.0, false);
    
    EXPECT_DOUBLE_EQ(space4.calculateTotalOccupancyCost(), 5000.0 + 200.0 + (-600.0 / 12.0));
}

TEST_F(CommercialSpaceTest, IsSuitableForRetail) {
    CommercialSpace retailSuitable("COM001", 500.0, 12000.0,
                                  std::make_unique<Address>("Test"), 
                                  std::make_unique<PropertyStatus>("AVAILABLE"),
                                  std::make_unique<PropertyType>("COMMERCIAL"),
                                  "retail", true, 25, 800.0, true, 
                                  "commercial_retail", 50, 2400.0, true);
    
    EXPECT_TRUE(retailSuitable.isSuitableForRetail());
    
    CommercialSpace noStorefront("COM002", 500.0, 12000.0,
                                std::make_unique<Address>("Test"), 
                                std::make_unique<PropertyStatus>("AVAILABLE"),
                                std::make_unique<PropertyType>("COMMERCIAL"),
                                "retail", false, 25, 800.0, true, 
                                "commercial_retail", 50, 2400.0, true);
    
    EXPECT_FALSE(noStorefront.isSuitableForRetail());
    
    CommercialSpace notGroundFloor("COM003", 500.0, 12000.0,
                                  std::make_unique<Address>("Test"), 
                                  std::make_unique<PropertyStatus>("AVAILABLE"),
                                  std::make_unique<PropertyType>("COMMERCIAL"),
                                  "retail", true, 25, 800.0, true, 
                                  "commercial_retail", 50, 2400.0, false);
    
    EXPECT_FALSE(notGroundFloor.isSuitableForRetail());
    
    CommercialSpace wrongZoning("COM004", 500.0, 12000.0,
                               std::make_unique<Address>("Test"), 
                               std::make_unique<PropertyStatus>("AVAILABLE"),
                               std::make_unique<PropertyType>("COMMERCIAL"),
                               "retail", true, 25, 800.0, true, 
                               "residential", 50, 2400.0, true);
    
    EXPECT_FALSE(wrongZoning.isSuitableForRetail());
    
    CommercialSpace commercialMixedZoning("COM005", 500.0, 12000.0,
                                         std::make_unique<Address>("Test"), 
                                         std::make_unique<PropertyStatus>("AVAILABLE"),
                                         std::make_unique<PropertyType>("COMMERCIAL"),
                                         "retail", true, 25, 800.0, true, 
                                         "mixed_commercial", 50, 2400.0, true);
    
    EXPECT_TRUE(commercialMixedZoning.isSuitableForRetail());
    
    CommercialSpace retailZoning("COM006", 500.0, 12000.0,
                                std::make_unique<Address>("Test"), 
                                std::make_unique<PropertyStatus>("AVAILABLE"),
                                std::make_unique<PropertyType>("COMMERCIAL"),
                                "retail", true, 25, 800.0, true, 
                                "retail_only", 50, 2400.0, true);
    
    EXPECT_FALSE(retailZoning.isSuitableForRetail());
}

TEST_F(CommercialSpaceTest, IsSuitableForOffice) {
    CommercialSpace officeSuitable("COM001", 500.0, 12000.0,
                                  std::make_unique<Address>("Test"), 
                                  std::make_unique<PropertyStatus>("AVAILABLE"),
                                  std::make_unique<PropertyType>("COMMERCIAL"),
                                  "office", false, 10, 800.0, false, 
                                  "commercial_office", 30, 2400.0, false);
    
    EXPECT_TRUE(officeSuitable.isSuitableForOffice());
    
    CommercialSpace withStorefront("COM002", 500.0, 12000.0,
                                  std::make_unique<Address>("Test"), 
                                  std::make_unique<PropertyStatus>("AVAILABLE"),
                                  std::make_unique<PropertyType>("COMMERCIAL"),
                                  "office", true, 10, 800.0, false, 
                                  "commercial_office", 30, 2400.0, false);
    
    EXPECT_FALSE(withStorefront.isSuitableForOffice());
    
    CommercialSpace lowOccupancy("COM003", 500.0, 12000.0,
                                std::make_unique<Address>("Test"), 
                                std::make_unique<PropertyStatus>("AVAILABLE"),
                                std::make_unique<PropertyType>("COMMERCIAL"),
                                "office", false, 10, 800.0, false, 
                                "commercial_office", 9, 2400.0, false);
    
    EXPECT_FALSE(lowOccupancy.isSuitableForOffice());
    
    CommercialSpace exactOccupancy("COM004", 500.0, 12000.0,
                                  std::make_unique<Address>("Test"), 
                                  std::make_unique<PropertyStatus>("AVAILABLE"),
                                  std::make_unique<PropertyType>("COMMERCIAL"),
                                  "office", false, 10, 800.0, false, 
                                  "commercial_office", 10, 2400.0, false);
    
    EXPECT_TRUE(exactOccupancy.isSuitableForOffice());
    
    CommercialSpace lowParking("COM005", 500.0, 12000.0,
                              std::make_unique<Address>("Test"), 
                              std::make_unique<PropertyStatus>("AVAILABLE"),
                              std::make_unique<PropertyType>("COMMERCIAL"),
                              "office", false, 4, 800.0, false, 
                              "commercial_office", 30, 2400.0, false);
    
    EXPECT_FALSE(lowParking.isSuitableForOffice());
    
    CommercialSpace exactParking("COM006", 500.0, 12000.0,
                                std::make_unique<Address>("Test"), 
                                std::make_unique<PropertyStatus>("AVAILABLE"),
                                std::make_unique<PropertyType>("COMMERCIAL"),
                                "office", false, 5, 800.0, false, 
                                "commercial_office", 30, 2400.0, false);
    
    EXPECT_TRUE(exactParking.isSuitableForOffice());
}

TEST_F(CommercialSpaceTest, MeetsZoningRequirements) {
    CommercialSpace space("COM001", 500.0, 12000.0,
                         std::make_unique<Address>("Test"), 
                         std::make_unique<PropertyStatus>("AVAILABLE"),
                         std::make_unique<PropertyType>("COMMERCIAL"),
                         "retail", true, 25, 800.0, true, 
                         "commercial_retail", 50, 2400.0, true);
    
    EXPECT_TRUE(space.meetsZoningRequirements("retail"));
    EXPECT_TRUE(space.meetsZoningRequirements("office"));
    EXPECT_FALSE(space.meetsZoningRequirements("industrial"));
    
    CommercialSpace officeZoning("COM002", 500.0, 12000.0,
                                std::make_unique<Address>("Test"), 
                                std::make_unique<PropertyStatus>("AVAILABLE"),
                                std::make_unique<PropertyType>("COMMERCIAL"),
                                "office", false, 10, 800.0, false, 
                                "commercial_office", 30, 2400.0, false);
    
    EXPECT_TRUE(officeZoning.meetsZoningRequirements("retail"));
    EXPECT_TRUE(officeZoning.meetsZoningRequirements("office"));
    EXPECT_FALSE(officeZoning.meetsZoningRequirements("industrial"));
    
    CommercialSpace industrialZoning("COM003", 1000.0, 18000.0,
                                    std::make_unique<Address>("Test"), 
                                    std::make_unique<PropertyStatus>("AVAILABLE"),
                                    std::make_unique<PropertyType>("COMMERCIAL"),
                                    "industrial", false, 50, 1200.0, true, 
                                    "industrial_heavy", 100, 3600.0, false);
    
    EXPECT_FALSE(industrialZoning.meetsZoningRequirements("retail"));
    EXPECT_FALSE(industrialZoning.meetsZoningRequirements("office"));
    EXPECT_TRUE(industrialZoning.meetsZoningRequirements("industrial"));
    
    CommercialSpace mixedZoning("COM004", 500.0, 12000.0,
                               std::make_unique<Address>("Test"), 
                               std::make_unique<PropertyStatus>("AVAILABLE"),
                               std::make_unique<PropertyType>("COMMERCIAL"),
                               "retail", true, 25, 800.0, true, 
                               "mixed_use_commercial", 50, 2400.0, true);
    
    EXPECT_TRUE(mixedZoning.meetsZoningRequirements("retail"));
    EXPECT_TRUE(mixedZoning.meetsZoningRequirements("office"));
    EXPECT_FALSE(mixedZoning.meetsZoningRequirements("industrial"));
    
    EXPECT_FALSE(space.meetsZoningRequirements("residential"));
    EXPECT_FALSE(space.meetsZoningRequirements(""));
    EXPECT_FALSE(space.meetsZoningRequirements("unknown"));
}

TEST_F(CommercialSpaceTest, CalculateParkingRatio) {
    CommercialSpace space1("COM001", 1000.0, 12000.0,
                          std::make_unique<Address>("Test"), 
                          std::make_unique<PropertyStatus>("AVAILABLE"),
                          std::make_unique<PropertyType>("COMMERCIAL"),
                          "retail", true, 20, 800.0, true, 
                          "commercial_retail", 50, 2400.0, true);
    
    EXPECT_DOUBLE_EQ(space1.calculateParkingRatio(), 20.0 / 10.0);
    
    CommercialSpace space2("COM002", 500.0, 12000.0,
                          std::make_unique<Address>("Test"), 
                          std::make_unique<PropertyStatus>("AVAILABLE"),
                          std::make_unique<PropertyType>("COMMERCIAL"),
                          "retail", true, 10, 800.0, true, 
                          "commercial_retail", 50, 2400.0, true);
    
    EXPECT_DOUBLE_EQ(space2.calculateParkingRatio(), 10.0 / 5.0);
    
    CommercialSpace space3("COM003", 0.0, 12000.0,
                          std::make_unique<Address>("Test"), 
                          std::make_unique<PropertyStatus>("AVAILABLE"),
                          std::make_unique<PropertyType>("COMMERCIAL"),
                          "retail", true, 20, 800.0, true, 
                          "commercial_retail", 50, 2400.0, true);
    
    EXPECT_DOUBLE_EQ(space3.calculateParkingRatio(), 0.0);
    
    CommercialSpace space4("COM004", -100.0, 12000.0,
                          std::make_unique<Address>("Test"), 
                          std::make_unique<PropertyStatus>("AVAILABLE"),
                          std::make_unique<PropertyType>("COMMERCIAL"),
                          "retail", true, 20, 800.0, true, 
                          "commercial_retail", 50, 2400.0, true);
    
    EXPECT_DOUBLE_EQ(space4.calculateParkingRatio(), 0.0);
    
    CommercialSpace space5("COM005", 200.0, 12000.0,
                          std::make_unique<Address>("Test"), 
                          std::make_unique<PropertyStatus>("AVAILABLE"),
                          std::make_unique<PropertyType>("COMMERCIAL"),
                          "retail", true, 0, 800.0, true, 
                          "commercial_retail", 50, 2400.0, true);
    
    EXPECT_DOUBLE_EQ(space5.calculateParkingRatio(), 0.0);
    
    CommercialSpace space6("COM006", 200.0, 12000.0,
                          std::make_unique<Address>("Test"), 
                          std::make_unique<PropertyStatus>("AVAILABLE"),
                          std::make_unique<PropertyType>("COMMERCIAL"),
                          "retail", true, 5, 800.0, true, 
                          "commercial_retail", 50, 2400.0, true);
    
    EXPECT_DOUBLE_EQ(space6.calculateParkingRatio(), 5.0 / 2.0);
}

TEST_F(CommercialSpaceTest, HasAdequateParking) {
    CommercialSpace space("COM001", 1000.0, 12000.0,
                         std::make_unique<Address>("Test"), 
                         std::make_unique<PropertyStatus>("AVAILABLE"),
                         std::make_unique<PropertyType>("COMMERCIAL"),
                         "retail", true, 20, 800.0, true, 
                         "commercial_retail", 50, 2400.0, true);
    
    EXPECT_TRUE(space.hasAdequateParking(100));
    EXPECT_TRUE(space.hasAdequateParking(99));
    EXPECT_TRUE(space.hasAdequateParking(101));
    EXPECT_TRUE(space.hasAdequateParking(0));
    EXPECT_TRUE(space.hasAdequateParking(-10));
    
    EXPECT_TRUE(space.hasAdequateParking(20));
    EXPECT_TRUE(space.hasAdequateParking(21));
    
    CommercialSpace space2("COM002", 1000.0, 12000.0,
                          std::make_unique<Address>("Test"), 
                          std::make_unique<PropertyStatus>("AVAILABLE"),
                          std::make_unique<PropertyType>("COMMERCIAL"),
                          "retail", true, 0, 800.0, true, 
                          "commercial_retail", 50, 2400.0, true);
    
    EXPECT_TRUE(space2.hasAdequateParking(0));
    EXPECT_TRUE(space2.hasAdequateParking(1));
    
    CommercialSpace space3("COM003", 1000.0, 12000.0,
                          std::make_unique<Address>("Test"), 
                          std::make_unique<PropertyStatus>("AVAILABLE"),
                          std::make_unique<PropertyType>("COMMERCIAL"),
                          "retail", true, 100, 800.0, true, 
                          "commercial_retail", 50, 2400.0, true);
    
    EXPECT_TRUE(space3.hasAdequateParking(500));
    EXPECT_TRUE(space3.hasAdequateParking(501));
}

TEST_F(CommercialSpaceTest, CalculateTaxDeduction) {
    CommercialSpace retail("COM001", 1000.0, 12000.0,
                          std::make_unique<Address>("Test"), 
                          std::make_unique<PropertyStatus>("AVAILABLE"),
                          std::make_unique<PropertyType>("COMMERCIAL"),
                          "retail", true, 20, 800.0, true, 
                          "commercial_retail", 50, 2400.0, true);
    
    EXPECT_DOUBLE_EQ(retail.calculateTaxDeduction(), 2400.0 * 0.15);
    
    CommercialSpace restaurant("COM002", 1000.0, 12000.0,
                              std::make_unique<Address>("Test"), 
                              std::make_unique<PropertyStatus>("AVAILABLE"),
                              std::make_unique<PropertyType>("COMMERCIAL"),
                              "restaurant", true, 20, 800.0, true, 
                              "commercial_retail", 50, 2400.0, true);
    
    EXPECT_DOUBLE_EQ(restaurant.calculateTaxDeduction(), 2400.0 * 0.15);
    
    CommercialSpace office("COM003", 1000.0, 12000.0,
                          std::make_unique<Address>("Test"), 
                          std::make_unique<PropertyStatus>("AVAILABLE"),
                          std::make_unique<PropertyType>("COMMERCIAL"),
                          "office", false, 20, 800.0, false, 
                          "commercial_office", 50, 2400.0, false);
    
    EXPECT_DOUBLE_EQ(office.calculateTaxDeduction(), 2400.0 * 0.10);
    
    CommercialSpace medical("COM004", 1000.0, 12000.0,
                           std::make_unique<Address>("Test"), 
                           std::make_unique<PropertyStatus>("AVAILABLE"),
                           std::make_unique<PropertyType>("COMMERCIAL"),
                           "medical", false, 20, 800.0, false, 
                           "commercial_office", 50, 2400.0, false);
    
    EXPECT_DOUBLE_EQ(medical.calculateTaxDeduction(), 2400.0 * 0.10);
    
    CommercialSpace industrial("COM005", 1000.0, 12000.0,
                              std::make_unique<Address>("Test"), 
                              std::make_unique<PropertyStatus>("AVAILABLE"),
                              std::make_unique<PropertyType>("COMMERCIAL"),
                              "industrial", false, 20, 800.0, true, 
                              "industrial", 50, 2400.0, false);
    
    EXPECT_DOUBLE_EQ(industrial.calculateTaxDeduction(), 0.0);
    
    CommercialSpace other("COM006", 1000.0, 12000.0,
                         std::make_unique<Address>("Test"), 
                         std::make_unique<PropertyStatus>("AVAILABLE"),
                         std::make_unique<PropertyType>("COMMERCIAL"),
                         "other", false, 20, 800.0, false, 
                         "commercial", 50, 2400.0, false);
    
    EXPECT_DOUBLE_EQ(other.calculateTaxDeduction(), 0.0);
    
    CommercialSpace zeroTax("COM007", 1000.0, 12000.0,
                           std::make_unique<Address>("Test"), 
                           std::make_unique<PropertyStatus>("AVAILABLE"),
                           std::make_unique<PropertyType>("COMMERCIAL"),
                           "retail", true, 20, 800.0, true, 
                           "commercial_retail", 50, 0.0, true);
    
    EXPECT_DOUBLE_EQ(zeroTax.calculateTaxDeduction(), 0.0);
    
    CommercialSpace negativeTax("COM008", 1000.0, 12000.0,
                               std::make_unique<Address>("Test"), 
                               std::make_unique<PropertyStatus>("AVAILABLE"),
                               std::make_unique<PropertyType>("COMMERCIAL"),
                               "retail", true, 20, 800.0, true, 
                               "commercial_retail", 50, -1000.0, true);
    
    EXPECT_DOUBLE_EQ(negativeTax.calculateTaxDeduction(), -1000.0 * 0.15);
}

TEST_F(CommercialSpaceTest, IsPremiumLocation) {
    CommercialSpace premium("COM001", 1000.0, 12000.0,
                           std::make_unique<Address>("Test"), 
                           std::make_unique<PropertyStatus>("AVAILABLE"),
                           std::make_unique<PropertyType>("COMMERCIAL"),
                           "retail", true, 15, 800.0, true, 
                           "commercial_retail", 50, 2400.0, true);
    
    EXPECT_TRUE(premium.isPremiumLocation());
    
    CommercialSpace notGroundFloor("COM002", 1000.0, 12000.0,
                                  std::make_unique<Address>("Test"), 
                                  std::make_unique<PropertyStatus>("AVAILABLE"),
                                  std::make_unique<PropertyType>("COMMERCIAL"),
                                  "retail", true, 15, 800.0, true, 
                                  "commercial_retail", 50, 2400.0, false);
    
    EXPECT_FALSE(notGroundFloor.isPremiumLocation());
    
    CommercialSpace noStorefront("COM003", 1000.0, 12000.0,
                                std::make_unique<Address>("Test"), 
                                std::make_unique<PropertyStatus>("AVAILABLE"),
                                std::make_unique<PropertyType>("COMMERCIAL"),
                                "retail", false, 15, 800.0, true, 
                                "commercial_retail", 50, 2400.0, true);
    
    EXPECT_FALSE(noStorefront.isPremiumLocation());
    
    CommercialSpace noLoadingDock("COM004", 1000.0, 12000.0,
                                 std::make_unique<Address>("Test"), 
                                 std::make_unique<PropertyStatus>("AVAILABLE"),
                                 std::make_unique<PropertyType>("COMMERCIAL"),
                                 "retail", true, 15, 800.0, false, 
                                 "commercial_retail", 50, 2400.0, true);
    
    EXPECT_FALSE(noLoadingDock.isPremiumLocation());
    
    CommercialSpace lowParking("COM005", 1000.0, 12000.0,
                              std::make_unique<Address>("Test"), 
                              std::make_unique<PropertyStatus>("AVAILABLE"),
                              std::make_unique<PropertyType>("COMMERCIAL"),
                              "retail", true, 9, 800.0, true, 
                              "commercial_retail", 50, 2400.0, true);
    
    EXPECT_FALSE(lowParking.isPremiumLocation());
    
    CommercialSpace exactParking("COM006", 1000.0, 12000.0,
                                std::make_unique<Address>("Test"), 
                                std::make_unique<PropertyStatus>("AVAILABLE"),
                                std::make_unique<PropertyType>("COMMERCIAL"),
                                "retail", true, 10, 800.0, true, 
                                "commercial_retail", 50, 2400.0, true);
    
    EXPECT_TRUE(exactParking.isPremiumLocation());
    
    CommercialSpace allFalse("COM007", 1000.0, 12000.0,
                            std::make_unique<Address>("Test"), 
                            std::make_unique<PropertyStatus>("AVAILABLE"),
                            std::make_unique<PropertyType>("COMMERCIAL"),
                            "retail", false, 5, 800.0, false, 
                            "commercial_retail", 50, 2400.0, false);
    
    EXPECT_FALSE(allFalse.isPremiumLocation());
}

TEST_F(CommercialSpaceTest, CopyConstructor) {
    CommercialSpace original("COM001", 500.0, 12000.0, std::move(address),
                            std::move(status), std::move(type), "retail",
                            true, 25, 800.0, true, "commercial_retail",
                            50, 2400.0, true);
    
    original.setRentalPrice(13000.0);
    original.setBusinessType("restaurant");
    
    CommercialSpace copy(original);
    
    EXPECT_EQ(copy.getId(), "COM001");
    EXPECT_DOUBLE_EQ(copy.getArea(), 500.0);
    EXPECT_DOUBLE_EQ(copy.getRentalPrice(), 13000.0);
    EXPECT_EQ(copy.getBusinessType(), "restaurant");
    EXPECT_TRUE(copy.getHasStorefront());
    EXPECT_EQ(copy.getParkingSpaces(), 25);
    EXPECT_DOUBLE_EQ(copy.getCommonAreaMaintenance(), 800.0);
    EXPECT_TRUE(copy.getHasLoadingDock());
    EXPECT_EQ(copy.getZoningType(), "commercial_retail");
    EXPECT_EQ(copy.getMaxOccupancy(), 50);
    EXPECT_DOUBLE_EQ(copy.getAnnualTax(), 2400.0);
    EXPECT_TRUE(copy.getIsGroundFloor());
}

TEST_F(CommercialSpaceTest, CopyAssignment) {
    CommercialSpace original("COM001", 500.0, 12000.0, 
                           std::make_unique<Address>("Original"), 
                           std::make_unique<PropertyStatus>("AVAILABLE"),
                           std::make_unique<PropertyType>("COMMERCIAL"),
                           "retail", true, 25, 800.0, true, 
                           "commercial_retail", 50, 2400.0, true);
    
    CommercialSpace assigned("COM002", 300.0, 8000.0,
                           std::make_unique<Address>("Assigned"), 
                           std::make_unique<PropertyStatus>("RENTED"),
                           std::make_unique<PropertyType>("OFFICE"),
                           "office", false, 10, 400.0, false,
                           "commercial_office", 30, 1200.0, false);
    
    assigned = original;
    
    EXPECT_EQ(assigned.getId(), "COM001");
    EXPECT_DOUBLE_EQ(assigned.getArea(), 500.0);
    EXPECT_DOUBLE_EQ(assigned.getRentalPrice(), 12000.0);
    EXPECT_EQ(assigned.getBusinessType(), "retail");
    EXPECT_TRUE(assigned.getHasStorefront());
    EXPECT_EQ(assigned.getParkingSpaces(), 25);
    EXPECT_DOUBLE_EQ(assigned.getCommonAreaMaintenance(), 800.0);
    EXPECT_TRUE(assigned.getHasLoadingDock());
    EXPECT_EQ(assigned.getZoningType(), "commercial_retail");
    EXPECT_EQ(assigned.getMaxOccupancy(), 50);
    EXPECT_DOUBLE_EQ(assigned.getAnnualTax(), 2400.0);
    EXPECT_TRUE(assigned.getIsGroundFloor());
}

TEST_F(CommercialSpaceTest, MoveConstructor) {
    CommercialSpace original("COM001", 500.0, 12000.0, std::move(address),
                            std::move(status), std::move(type), "retail",
                            true, 25, 800.0, true, "commercial_retail",
                            50, 2400.0, true);
    
    CommercialSpace moved(std::move(original));
    
    EXPECT_EQ(moved.getId(), "COM001");
    EXPECT_DOUBLE_EQ(moved.getArea(), 500.0);
    EXPECT_DOUBLE_EQ(moved.getRentalPrice(), 12000.0);
    EXPECT_EQ(moved.getBusinessType(), "retail");
    EXPECT_TRUE(moved.getHasStorefront());
    EXPECT_EQ(moved.getParkingSpaces(), 25);
    EXPECT_DOUBLE_EQ(moved.getCommonAreaMaintenance(), 800.0);
    EXPECT_TRUE(moved.getHasLoadingDock());
    EXPECT_EQ(moved.getZoningType(), "commercial_retail");
    EXPECT_EQ(moved.getMaxOccupancy(), 50);
    EXPECT_DOUBLE_EQ(moved.getAnnualTax(), 2400.0);
    EXPECT_TRUE(moved.getIsGroundFloor());
}

TEST_F(CommercialSpaceTest, MoveAssignment) {
    CommercialSpace original("COM001", 500.0, 12000.0, 
                           std::make_unique<Address>("Original"), 
                           std::make_unique<PropertyStatus>("AVAILABLE"),
                           std::make_unique<PropertyType>("COMMERCIAL"),
                           "retail", true, 25, 800.0, true, 
                           "commercial_retail", 50, 2400.0, true);
    
    CommercialSpace moved("COM002", 300.0, 8000.0,
                        std::make_unique<Address>("Moved"), 
                        std::make_unique<PropertyStatus>("RENTED"),
                        std::make_unique<PropertyType>("OFFICE"),
                        "office", false, 10, 400.0, false,
                        "commercial_office", 30, 1200.0, false);
    
    moved = std::move(original);
    
    EXPECT_EQ(moved.getId(), "COM001");
    EXPECT_DOUBLE_EQ(moved.getArea(), 500.0);
    EXPECT_DOUBLE_EQ(moved.getRentalPrice(), 12000.0);
    EXPECT_EQ(moved.getBusinessType(), "retail");
    EXPECT_TRUE(moved.getHasStorefront());
    EXPECT_EQ(moved.getParkingSpaces(), 25);
    EXPECT_DOUBLE_EQ(moved.getCommonAreaMaintenance(), 800.0);
    EXPECT_TRUE(moved.getHasLoadingDock());
    EXPECT_EQ(moved.getZoningType(), "commercial_retail");
    EXPECT_EQ(moved.getMaxOccupancy(), 50);
    EXPECT_DOUBLE_EQ(moved.getAnnualTax(), 2400.0);
    EXPECT_TRUE(moved.getIsGroundFloor());
}

TEST_F(CommercialSpaceTest, SelfAssignment) {
    CommercialSpace space("COM001", 500.0, 12000.0, 
                        std::make_unique<Address>("Test"), 
                        std::make_unique<PropertyStatus>("AVAILABLE"),
                        std::make_unique<PropertyType>("COMMERCIAL"),
                        "retail", true, 25, 800.0, true, 
                        "commercial_retail", 50, 2400.0, true);
    
    space.setBusinessType("restaurant");
    space.setAnnualTax(3000.0);
    
    space = space;
    
    EXPECT_EQ(space.getId(), "COM001");
    EXPECT_DOUBLE_EQ(space.getArea(), 500.0);
    EXPECT_DOUBLE_EQ(space.getRentalPrice(), 12000.0);
    EXPECT_EQ(space.getBusinessType(), "restaurant");
    EXPECT_TRUE(space.getHasStorefront());
    EXPECT_EQ(space.getParkingSpaces(), 25);
    EXPECT_DOUBLE_EQ(space.getCommonAreaMaintenance(), 800.0);
    EXPECT_TRUE(space.getHasLoadingDock());
    EXPECT_EQ(space.getZoningType(), "commercial_retail");
    EXPECT_EQ(space.getMaxOccupancy(), 50);
    EXPECT_DOUBLE_EQ(space.getAnnualTax(), 3000.0);
    EXPECT_TRUE(space.getIsGroundFloor());
}

TEST_F(CommercialSpaceTest, MixedFunctionalMethods) {
    CommercialSpace space("COM001", 750.0, 15000.0,
                         std::make_unique<Address>("Test"), 
                         std::make_unique<PropertyStatus>("AVAILABLE"),
                         std::make_unique<PropertyType>("COMMERCIAL"),
                         "retail", true, 30, 1000.0, true, 
                         "commercial_retail", 75, 3000.0, true);
    
    EXPECT_DOUBLE_EQ(space.calculateTotalOccupancyCost(), 15000.0 + 1000.0 + (3000.0 / 12.0));
    EXPECT_TRUE(space.isSuitableForRetail());
    EXPECT_FALSE(space.isSuitableForOffice());
    EXPECT_TRUE(space.meetsZoningRequirements("retail"));
    EXPECT_TRUE(space.meetsZoningRequirements("office"));
    EXPECT_FALSE(space.meetsZoningRequirements("industrial"));
    EXPECT_DOUBLE_EQ(space.calculateParkingRatio(), 30.0 / 7.5);
    EXPECT_TRUE(space.hasAdequateParking(150));
    EXPECT_TRUE(space.hasAdequateParking(151));
    EXPECT_DOUBLE_EQ(space.calculateTaxDeduction(), 3000.0 * 0.15);
    EXPECT_TRUE(space.isPremiumLocation());
}

TEST_F(CommercialSpaceTest, EdgeCasesAndBoundaryConditions) {
    CommercialSpace space("EDGE", 0.0, 0.0,
                         std::make_unique<Address>("Test"), 
                         std::make_unique<PropertyStatus>("AVAILABLE"),
                         std::make_unique<PropertyType>("COMMERCIAL"),
                         "", false, 0, 0.0, false, 
                         "", 0, 0.0, false);
    
    EXPECT_DOUBLE_EQ(space.calculateTotalOccupancyCost(), 0.0);
    EXPECT_FALSE(space.isSuitableForRetail());
    EXPECT_FALSE(space.isSuitableForOffice());
    EXPECT_FALSE(space.meetsZoningRequirements("retail"));
    EXPECT_FALSE(space.meetsZoningRequirements("office"));
    EXPECT_FALSE(space.meetsZoningRequirements("industrial"));
    EXPECT_DOUBLE_EQ(space.calculateParkingRatio(), 0.0);
    EXPECT_TRUE(space.hasAdequateParking(0));
    EXPECT_TRUE(space.hasAdequateParking(1));
    EXPECT_DOUBLE_EQ(space.calculateTaxDeduction(), 0.0);
    EXPECT_FALSE(space.isPremiumLocation());
    
    space.setAnnualTax(-1000.0);
    EXPECT_DOUBLE_EQ(space.getAnnualTax(), 0.0);
}

TEST_F(CommercialSpaceTest, InheritedPropertyMethods) {
    CommercialSpace space("COM001", 500.0, 12000.0, std::move(address),
                         std::move(status), std::move(type), "retail",
                         true, 25, 800.0, true, "commercial_retail",
                         50, 2400.0, true);
    
    EXPECT_EQ(space.getId(), "COM001");
    EXPECT_DOUBLE_EQ(space.getArea(), 500.0);
    EXPECT_DOUBLE_EQ(space.getRentalPrice(), 12000.0);
    EXPECT_NE(space.getAddress(), nullptr);
    EXPECT_NE(space.getStatus(), nullptr);
    EXPECT_NE(space.getType(), nullptr);
    
    space.setRentalPrice(13000.0);
    EXPECT_DOUBLE_EQ(space.getRentalPrice(), 13000.0);
    
    space.updateArea(550.0);
    EXPECT_DOUBLE_EQ(space.getArea(), 550.0);
    
    EXPECT_DOUBLE_EQ(space.calculateRentalYield(156000.0), (156000.0 / (13000.0 * 12)) * 100);
    EXPECT_TRUE(space.isAvailableForRent());
    EXPECT_DOUBLE_EQ(space.calculatePricePerSqMeter(), 13000.0 / 550.0);
    EXPECT_TRUE(space.meetsAreaRequirements(400.0, 600.0));
    EXPECT_FALSE(space.meetsAreaRequirements(600.0, 700.0));
}