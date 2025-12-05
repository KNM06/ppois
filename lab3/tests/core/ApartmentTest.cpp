#include <gtest/gtest.h>
#include <memory>
#include "../../include/core/Apartment.h"
#include "../../include/core/Address.h"
#include "../../include/core/PropertyStatus.h"
#include "../../include/core/PropertyType.h"

class ApartmentTest : public ::testing::Test {
protected:
    void SetUp() override {
        address = std::make_unique<Address>("123 Main St", "City", "State", 
                                           "12345", "Country", 40.7128, -74.0060);
        status = std::make_unique<PropertyStatus>("AVAILABLE", "Available for rent",
                                                 true, false, false,
                                                 std::chrono::system_clock::now(),
                                                 "New listing", 0.0, 15);
        type = std::make_unique<PropertyType>("APARTMENT", "Apartment", 
                                             "Residential Apartment", 2, 100, 
                                             800.0, 2000.0, false, "residential", 1.0);
    }
    
    std::unique_ptr<Address> address;
    std::unique_ptr<PropertyStatus> status;
    std::unique_ptr<PropertyType> type;
};

TEST_F(ApartmentTest, ConstructorAndGetters) {
    Apartment apartment("APT001", 85.0, 1500.0, std::move(address),
                       std::move(status), std::move(type), 12, 2, 2,
                       true, true, "1201", 300.0, "Skyline Tower");
    
    EXPECT_EQ(apartment.getId(), "APT001");
    EXPECT_DOUBLE_EQ(apartment.getArea(), 85.0);
    EXPECT_DOUBLE_EQ(apartment.getRentalPrice(), 1500.0);
    EXPECT_EQ(apartment.getFloorNumber(), 12);
    EXPECT_EQ(apartment.getNumberOfBedrooms(), 2);
    EXPECT_EQ(apartment.getNumberOfBathrooms(), 2);
    EXPECT_TRUE(apartment.getHasBalcony());
    EXPECT_TRUE(apartment.getHasParking());
    EXPECT_EQ(apartment.getApartmentNumber(), "1201");
    EXPECT_DOUBLE_EQ(apartment.getMaintenanceFee(), 300.0);
    EXPECT_EQ(apartment.getBuildingName(), "Skyline Tower");
}

TEST_F(ApartmentTest, Setters) {
    Apartment apartment("APT001", 85.0, 1500.0, 
                       std::make_unique<Address>("Test"), 
                       std::make_unique<PropertyStatus>("AVAILABLE"),
                       std::make_unique<PropertyType>("APARTMENT"),
                       12, 2, 2, true, true, "1201", 300.0, "Skyline Tower");
    
    apartment.setMaintenanceFee(350.0);
    EXPECT_DOUBLE_EQ(apartment.getMaintenanceFee(), 350.0);
    
    apartment.setMaintenanceFee(0.0);
    EXPECT_DOUBLE_EQ(apartment.getMaintenanceFee(), 0.0);
    
    apartment.setMaintenanceFee(-100.0);
    EXPECT_DOUBLE_EQ(apartment.getMaintenanceFee(), 0.0);
    
    apartment.setMaintenanceFee(250.75);
    EXPECT_DOUBLE_EQ(apartment.getMaintenanceFee(), 250.75);
    
    apartment.setApartmentNumber("1302");
    EXPECT_EQ(apartment.getApartmentNumber(), "1302");
    
    apartment.setApartmentNumber("");
    EXPECT_EQ(apartment.getApartmentNumber(), "");
    
    apartment.setApartmentNumber("A-101");
    EXPECT_EQ(apartment.getApartmentNumber(), "A-101");
}

TEST_F(ApartmentTest, CalculateTotalMonthlyCost) {
    Apartment apartment1("APT001", 85.0, 1500.0, 
                        std::make_unique<Address>("Test"), 
                        std::make_unique<PropertyStatus>("AVAILABLE"),
                        std::make_unique<PropertyType>("APARTMENT"),
                        12, 2, 2, true, true, "1201", 300.0, "Skyline Tower");
    
    EXPECT_DOUBLE_EQ(apartment1.calculateTotalMonthlyCost(), 1800.0);
    
    Apartment apartment2("APT002", 75.0, 1200.0,
                        std::make_unique<Address>("Test"), 
                        std::make_unique<PropertyStatus>("AVAILABLE"),
                        std::make_unique<PropertyType>("APARTMENT"),
                        8, 1, 1, false, true, "801", 200.0, "Sunset View");
    
    EXPECT_DOUBLE_EQ(apartment2.calculateTotalMonthlyCost(), 1400.0);
    
    Apartment apartment3("APT003", 90.0, 1800.0,
                        std::make_unique<Address>("Test"), 
                        std::make_unique<PropertyStatus>("AVAILABLE"),
                        std::make_unique<PropertyType>("APARTMENT"),
                        15, 3, 2, true, true, "1503", 0.0, "Oceanfront");
    
    EXPECT_DOUBLE_EQ(apartment3.calculateTotalMonthlyCost(), 1800.0);
    
    Apartment apartment4("APT004", 100.0, 2000.0,
                        std::make_unique<Address>("Test"), 
                        std::make_unique<PropertyStatus>("AVAILABLE"),
                        std::make_unique<PropertyType>("APARTMENT"),
                        20, 3, 3, true, true, "2001", -150.0, "Luxury Heights");
    
    EXPECT_DOUBLE_EQ(apartment4.calculateTotalMonthlyCost(), 1850.0);
}

TEST_F(ApartmentTest, IsOnUpperFloor) {
    Apartment apartment1("APT001", 85.0, 1500.0, 
                        std::make_unique<Address>("Test"), 
                        std::make_unique<PropertyStatus>("AVAILABLE"),
                        std::make_unique<PropertyType>("APARTMENT"),
                        12, 2, 2, true, true, "1201", 300.0, "Skyline Tower");
    
    EXPECT_TRUE(apartment1.isOnUpperFloor(10));
    EXPECT_TRUE(apartment1.isOnUpperFloor(12));
    EXPECT_FALSE(apartment1.isOnUpperFloor(13));
    EXPECT_TRUE(apartment1.isOnUpperFloor(0));
    EXPECT_TRUE(apartment1.isOnUpperFloor(-5));
    
    Apartment apartment2("APT002", 75.0, 1200.0,
                        std::make_unique<Address>("Test"), 
                        std::make_unique<PropertyStatus>("AVAILABLE"),
                        std::make_unique<PropertyType>("APARTMENT"),
                        3, 1, 1, false, true, "301", 200.0, "Sunset View");
    
    EXPECT_FALSE(apartment2.isOnUpperFloor(5));
    EXPECT_TRUE(apartment2.isOnUpperFloor(3));
    EXPECT_TRUE(apartment2.isOnUpperFloor(2));
    
    Apartment groundFloor("APT003", 80.0, 1100.0,
                         std::make_unique<Address>("Test"), 
                         std::make_unique<PropertyStatus>("AVAILABLE"),
                         std::make_unique<PropertyType>("APARTMENT"),
                         0, 1, 1, false, true, "G01", 150.0, "Garden Apartments");
    
    EXPECT_TRUE(groundFloor.isOnUpperFloor(0));
    EXPECT_FALSE(groundFloor.isOnUpperFloor(1));
}

TEST_F(ApartmentTest, MeetsBedroomRequirements) {
    Apartment apartment1("APT001", 85.0, 1500.0, 
                        std::make_unique<Address>("Test"), 
                        std::make_unique<PropertyStatus>("AVAILABLE"),
                        std::make_unique<PropertyType>("APARTMENT"),
                        12, 2, 2, true, true, "1201", 300.0, "Skyline Tower");
    
    EXPECT_TRUE(apartment1.meetsBedroomRequirements(1, 3));
    EXPECT_TRUE(apartment1.meetsBedroomRequirements(2, 2));
    EXPECT_TRUE(apartment1.meetsBedroomRequirements(1, 2));
    EXPECT_FALSE(apartment1.meetsBedroomRequirements(3, 4));
    EXPECT_FALSE(apartment1.meetsBedroomRequirements(0, 1));
    EXPECT_TRUE(apartment1.meetsBedroomRequirements(2, 5));
    
    EXPECT_TRUE(apartment1.meetsBedroomRequirements(0, 10));
    EXPECT_TRUE(apartment1.meetsBedroomRequirements(-2, 5));
    
    Apartment apartment2("APT002", 75.0, 1200.0,
                        std::make_unique<Address>("Test"), 
                        std::make_unique<PropertyStatus>("AVAILABLE"),
                        std::make_unique<PropertyType>("APARTMENT"),
                        8, 1, 1, false, true, "801", 200.0, "Sunset View");
    
    EXPECT_TRUE(apartment2.meetsBedroomRequirements(1, 1));
    EXPECT_FALSE(apartment2.meetsBedroomRequirements(2, 3));
    EXPECT_TRUE(apartment2.meetsBedroomRequirements(0, 1));
    
    Apartment studio("APT003", 50.0, 900.0,
                    std::make_unique<Address>("Test"), 
                    std::make_unique<PropertyStatus>("AVAILABLE"),
                    std::make_unique<PropertyType>("APARTMENT"),
                    5, 0, 1, false, false, "502", 100.0, "Studio Lofts");
    
    EXPECT_TRUE(studio.meetsBedroomRequirements(0, 0));
    EXPECT_FALSE(studio.meetsBedroomRequirements(1, 2));
    EXPECT_TRUE(studio.meetsBedroomRequirements(-1, 0));
}

TEST_F(ApartmentTest, CalculatePricePerBedroom) {
    Apartment apartment1("APT001", 85.0, 1500.0, 
                        std::make_unique<Address>("Test"), 
                        std::make_unique<PropertyStatus>("AVAILABLE"),
                        std::make_unique<PropertyType>("APARTMENT"),
                        12, 2, 2, true, true, "1201", 300.0, "Skyline Tower");
    
    EXPECT_DOUBLE_EQ(apartment1.calculatePricePerBedroom(), 750.0);
    
    Apartment apartment2("APT002", 75.0, 1200.0,
                        std::make_unique<Address>("Test"), 
                        std::make_unique<PropertyStatus>("AVAILABLE"),
                        std::make_unique<PropertyType>("APARTMENT"),
                        8, 1, 1, false, true, "801", 200.0, "Sunset View");
    
    EXPECT_DOUBLE_EQ(apartment2.calculatePricePerBedroom(), 1200.0);
    
    Apartment apartment3("APT003", 90.0, 1800.0,
                        std::make_unique<Address>("Test"), 
                        std::make_unique<PropertyStatus>("AVAILABLE"),
                        std::make_unique<PropertyType>("APARTMENT"),
                        15, 3, 2, true, true, "1503", 0.0, "Oceanfront");
    
    EXPECT_DOUBLE_EQ(apartment3.calculatePricePerBedroom(), 600.0);
    
    Apartment studio("APT004", 50.0, 900.0,
                    std::make_unique<Address>("Test"), 
                    std::make_unique<PropertyStatus>("AVAILABLE"),
                    std::make_unique<PropertyType>("APARTMENT"),
                    5, 0, 1, false, false, "502", 100.0, "Studio Lofts");
    
    EXPECT_DOUBLE_EQ(studio.calculatePricePerBedroom(), 0.0);
    
    Apartment negativeBedrooms("APT005", 60.0, 1000.0,
                              std::make_unique<Address>("Test"), 
                              std::make_unique<PropertyStatus>("AVAILABLE"),
                              std::make_unique<PropertyType>("APARTMENT"),
                              3, -1, 1, false, true, "301", 150.0, "Test Building");
    
    EXPECT_DOUBLE_EQ(negativeBedrooms.calculatePricePerBedroom(), 0.0);
    
    apartment1.setRentalPrice(1600.0);
    EXPECT_DOUBLE_EQ(apartment1.calculatePricePerBedroom(), 800.0);
    
    apartment1.setRentalPrice(0.0);
    EXPECT_DOUBLE_EQ(apartment1.calculatePricePerBedroom(), 0.0);
}

TEST_F(ApartmentTest, HasPremiumFeatures) {
    Apartment premiumApartment("APT001", 85.0, 1500.0, 
                              std::make_unique<Address>("Test"), 
                              std::make_unique<PropertyStatus>("AVAILABLE"),
                              std::make_unique<PropertyType>("APARTMENT"),
                              12, 2, 2, true, true, "1201", 300.0, "Skyline Tower");
    
    EXPECT_TRUE(premiumApartment.hasPremiumFeatures());
    
    Apartment noBalcony("APT002", 75.0, 1200.0,
                       std::make_unique<Address>("Test"), 
                       std::make_unique<PropertyStatus>("AVAILABLE"),
                       std::make_unique<PropertyType>("APARTMENT"),
                       8, 2, 2, false, true, "801", 200.0, "Sunset View");
    
    EXPECT_FALSE(noBalcony.hasPremiumFeatures());
    
    Apartment noParking("APT003", 90.0, 1800.0,
                       std::make_unique<Address>("Test"), 
                       std::make_unique<PropertyStatus>("AVAILABLE"),
                       std::make_unique<PropertyType>("APARTMENT"),
                       15, 3, 2, true, false, "1503", 0.0, "Oceanfront");
    
    EXPECT_FALSE(noParking.hasPremiumFeatures());
    
    Apartment oneBathroom("APT004", 80.0, 1400.0,
                         std::make_unique<Address>("Test"), 
                         std::make_unique<PropertyStatus>("AVAILABLE"),
                         std::make_unique<PropertyType>("APARTMENT"),
                         10, 2, 1, true, true, "1002", 250.0, "City Center");
    
    EXPECT_FALSE(oneBathroom.hasPremiumFeatures());
    
    Apartment allPremium("APT005", 100.0, 2200.0,
                        std::make_unique<Address>("Test"), 
                        std::make_unique<PropertyStatus>("AVAILABLE"),
                        std::make_unique<PropertyType>("APARTMENT"),
                        20, 3, 3, true, true, "2001", 400.0, "Luxury Heights");
    
    EXPECT_TRUE(allPremium.hasPremiumFeatures());
    
    Apartment none("APT006", 60.0, 900.0,
                  std::make_unique<Address>("Test"), 
                  std::make_unique<PropertyStatus>("AVAILABLE"),
                  std::make_unique<PropertyType>("APARTMENT"),
                  5, 1, 1, false, false, "501", 100.0, "Basic Apartments");
    
    EXPECT_FALSE(none.hasPremiumFeatures());
}

TEST_F(ApartmentTest, IsSuitableForFamilies) {
    Apartment familySuitable("APT001", 85.0, 1500.0, 
                            std::make_unique<Address>("Test"), 
                            std::make_unique<PropertyStatus>("AVAILABLE"),
                            std::make_unique<PropertyType>("APARTMENT"),
                            12, 2, 2, true, true, "1201", 300.0, "Skyline Tower");
    
    EXPECT_TRUE(familySuitable.isSuitableForFamilies());
    
    Apartment oneBedroom("APT002", 75.0, 1200.0,
                        std::make_unique<Address>("Test"), 
                        std::make_unique<PropertyStatus>("AVAILABLE"),
                        std::make_unique<PropertyType>("APARTMENT"),
                        8, 1, 1, false, true, "801", 200.0, "Sunset View");
    
    EXPECT_FALSE(oneBedroom.isSuitableForFamilies());
    
    Apartment noBathroom("APT003", 90.0, 1800.0,
                        std::make_unique<Address>("Test"), 
                        std::make_unique<PropertyStatus>("AVAILABLE"),
                        std::make_unique<PropertyType>("APARTMENT"),
                        15, 3, 0, true, true, "1503", 0.0, "Oceanfront");
    
    EXPECT_FALSE(noBathroom.isSuitableForFamilies());
    
    Apartment noParking("APT004", 80.0, 1400.0,
                       std::make_unique<Address>("Test"), 
                       std::make_unique<PropertyStatus>("AVAILABLE"),
                       std::make_unique<PropertyType>("APARTMENT"),
                       10, 2, 1, true, false, "1002", 250.0, "City Center");
    
    EXPECT_FALSE(noParking.isSuitableForFamilies());
    
    Apartment largeFamily("APT005", 120.0, 2200.0,
                         std::make_unique<Address>("Test"), 
                         std::make_unique<PropertyStatus>("AVAILABLE"),
                         std::make_unique<PropertyType>("APARTMENT"),
                         20, 4, 3, true, true, "2001", 400.0, "Luxury Heights");
    
    EXPECT_TRUE(largeFamily.isSuitableForFamilies());
    
    Apartment minimal("APT006", 70.0, 1100.0,
                     std::make_unique<Address>("Test"), 
                     std::make_unique<PropertyStatus>("AVAILABLE"),
                     std::make_unique<PropertyType>("APARTMENT"),
                     6, 2, 1, false, true, "602", 150.0, "Affordable Homes");
    
    EXPECT_TRUE(minimal.isSuitableForFamilies());
    
    Apartment studio("APT007", 50.0, 900.0,
                    std::make_unique<Address>("Test"), 
                    std::make_unique<PropertyStatus>("AVAILABLE"),
                    std::make_unique<PropertyType>("APARTMENT"),
                    5, 0, 1, false, false, "502", 100.0, "Studio Lofts");
    
    EXPECT_FALSE(studio.isSuitableForFamilies());
}

TEST_F(ApartmentTest, CalculateSpaceEfficiency) {
    Apartment apartment1("APT001", 85.0, 1500.0, 
                        std::make_unique<Address>("Test"), 
                        std::make_unique<PropertyStatus>("AVAILABLE"),
                        std::make_unique<PropertyType>("APARTMENT"),
                        12, 2, 2, true, true, "1201", 300.0, "Skyline Tower");
    
    double bedroomArea = 2 * 12.0;
    double bathroomArea = 2 * 6.0;
    double livingArea = 85.0 - bedroomArea - bathroomArea;
    double expectedEfficiency = (livingArea / 85.0) * 100.0;
    
    EXPECT_DOUBLE_EQ(apartment1.calculateSpaceEfficiency(), expectedEfficiency);
    
    Apartment apartment2("APT002", 75.0, 1200.0,
                        std::make_unique<Address>("Test"), 
                        std::make_unique<PropertyStatus>("AVAILABLE"),
                        std::make_unique<PropertyType>("APARTMENT"),
                        8, 1, 1, false, true, "801", 200.0, "Sunset View");
    
    bedroomArea = 1 * 12.0;
    bathroomArea = 1 * 6.0;
    livingArea = 75.0 - bedroomArea - bathroomArea;
    expectedEfficiency = (livingArea / 75.0) * 100.0;
    
    EXPECT_DOUBLE_EQ(apartment2.calculateSpaceEfficiency(), expectedEfficiency);
    
    Apartment studio("APT003", 50.0, 900.0,
                    std::make_unique<Address>("Test"), 
                    std::make_unique<PropertyStatus>("AVAILABLE"),
                    std::make_unique<PropertyType>("APARTMENT"),
                    5, 0, 1, false, false, "502", 100.0, "Studio Lofts");
    
    bedroomArea = 0 * 12.0;
    bathroomArea = 1 * 6.0;
    livingArea = 50.0 - bedroomArea - bathroomArea;
    expectedEfficiency = (livingArea / 50.0) * 100.0;
    
    EXPECT_DOUBLE_EQ(studio.calculateSpaceEfficiency(), expectedEfficiency);
    
    Apartment largeApartment("APT004", 120.0, 2200.0,
                            std::make_unique<Address>("Test"), 
                            std::make_unique<PropertyStatus>("AVAILABLE"),
                            std::make_unique<PropertyType>("APARTMENT"),
                            20, 3, 2, true, true, "2001", 400.0, "Luxury Heights");
    
    bedroomArea = 3 * 12.0;
    bathroomArea = 2 * 6.0;
    livingArea = 120.0 - bedroomArea - bathroomArea;
    expectedEfficiency = (livingArea / 120.0) * 100.0;
    
    EXPECT_DOUBLE_EQ(largeApartment.calculateSpaceEfficiency(), expectedEfficiency);
    
    Apartment zeroArea("APT005", 0.0, 1000.0,
                      std::make_unique<Address>("Test"), 
                      std::make_unique<PropertyStatus>("AVAILABLE"),
                      std::make_unique<PropertyType>("APARTMENT"),
                      3, 2, 1, false, true, "301", 150.0, "Test Building");
    
    EXPECT_DOUBLE_EQ(zeroArea.calculateSpaceEfficiency(), 0.0);
    
    Apartment negativeArea("APT006", -50.0, 800.0,
                          std::make_unique<Address>("Test"), 
                          std::make_unique<PropertyStatus>("AVAILABLE"),
                          std::make_unique<PropertyType>("APARTMENT"),
                          2, 1, 1, false, false, "201", 100.0, "Test Building");
    
    EXPECT_DOUBLE_EQ(negativeArea.calculateSpaceEfficiency(), 0.0);
    
    Apartment manyBedroomsBathrooms("APT007", 100.0, 1800.0,
                                   std::make_unique<Address>("Test"), 
                                   std::make_unique<PropertyStatus>("AVAILABLE"),
                                   std::make_unique<PropertyType>("APARTMENT"),
                                   10, 5, 4, true, true, "1003", 300.0, "Large Family");
    
    bedroomArea = 5 * 12.0;
    bathroomArea = 4 * 6.0;
    livingArea = 100.0 - bedroomArea - bathroomArea;
    expectedEfficiency = (livingArea / 100.0) * 100.0;
    
    EXPECT_DOUBLE_EQ(manyBedroomsBathrooms.calculateSpaceEfficiency(), expectedEfficiency);
    
    if (livingArea < 0) {
        EXPECT_TRUE(manyBedroomsBathrooms.calculateSpaceEfficiency() < 0);
    }
}

TEST_F(ApartmentTest, CopyConstructor) {
    Apartment original("APT001", 85.0, 1500.0, std::move(address),
                      std::move(status), std::move(type), 12, 2, 2,
                      true, true, "1201", 300.0, "Skyline Tower");
    
    original.setRentalPrice(1600.0);
    original.setMaintenanceFee(350.0);
    
    Apartment copy(original);
    
    EXPECT_EQ(copy.getId(), "APT001");
    EXPECT_DOUBLE_EQ(copy.getArea(), 85.0);
    EXPECT_DOUBLE_EQ(copy.getRentalPrice(), 1600.0);
    EXPECT_EQ(copy.getFloorNumber(), 12);
    EXPECT_EQ(copy.getNumberOfBedrooms(), 2);
    EXPECT_EQ(copy.getNumberOfBathrooms(), 2);
    EXPECT_TRUE(copy.getHasBalcony());
    EXPECT_TRUE(copy.getHasParking());
    EXPECT_EQ(copy.getApartmentNumber(), "1201");
    EXPECT_DOUBLE_EQ(copy.getMaintenanceFee(), 350.0);
    EXPECT_EQ(copy.getBuildingName(), "Skyline Tower");
}

TEST_F(ApartmentTest, CopyAssignment) {
    Apartment original("APT001", 85.0, 1500.0, 
                      std::make_unique<Address>("Original"), 
                      std::make_unique<PropertyStatus>("AVAILABLE"),
                      std::make_unique<PropertyType>("APARTMENT"),
                      12, 2, 2, true, true, "1201", 300.0, "Skyline Tower");
    
    Apartment assigned("APT002", 75.0, 1200.0,
                      std::make_unique<Address>("Assigned"), 
                      std::make_unique<PropertyStatus>("RENTED"),
                      std::make_unique<PropertyType>("APARTMENT"),
                      8, 1, 1, false, true, "801", 200.0, "Sunset View");
    
    assigned = original;
    
    EXPECT_EQ(assigned.getId(), "APT001");
    EXPECT_DOUBLE_EQ(assigned.getArea(), 85.0);
    EXPECT_DOUBLE_EQ(assigned.getRentalPrice(), 1500.0);
    EXPECT_EQ(assigned.getFloorNumber(), 12);
    EXPECT_EQ(assigned.getNumberOfBedrooms(), 2);
    EXPECT_EQ(assigned.getNumberOfBathrooms(), 2);
    EXPECT_TRUE(assigned.getHasBalcony());
    EXPECT_TRUE(assigned.getHasParking());
    EXPECT_EQ(assigned.getApartmentNumber(), "1201");
    EXPECT_DOUBLE_EQ(assigned.getMaintenanceFee(), 300.0);
    EXPECT_EQ(assigned.getBuildingName(), "Skyline Tower");
}

TEST_F(ApartmentTest, MoveConstructor) {
    Apartment original("APT001", 85.0, 1500.0, std::move(address),
                      std::move(status), std::move(type), 12, 2, 2,
                      true, true, "1201", 300.0, "Skyline Tower");
    
    Apartment moved(std::move(original));
    
    EXPECT_EQ(moved.getId(), "APT001");
    EXPECT_DOUBLE_EQ(moved.getArea(), 85.0);
    EXPECT_DOUBLE_EQ(moved.getRentalPrice(), 1500.0);
    EXPECT_EQ(moved.getFloorNumber(), 12);
    EXPECT_EQ(moved.getNumberOfBedrooms(), 2);
    EXPECT_EQ(moved.getNumberOfBathrooms(), 2);
    EXPECT_TRUE(moved.getHasBalcony());
    EXPECT_TRUE(moved.getHasParking());
    EXPECT_EQ(moved.getApartmentNumber(), "1201");
    EXPECT_DOUBLE_EQ(moved.getMaintenanceFee(), 300.0);
    EXPECT_EQ(moved.getBuildingName(), "Skyline Tower");
}

TEST_F(ApartmentTest, MoveAssignment) {
    Apartment original("APT001", 85.0, 1500.0, 
                      std::make_unique<Address>("Original"), 
                      std::make_unique<PropertyStatus>("AVAILABLE"),
                      std::make_unique<PropertyType>("APARTMENT"),
                      12, 2, 2, true, true, "1201", 300.0, "Skyline Tower");
    
    Apartment moved("APT002", 75.0, 1200.0,
                   std::make_unique<Address>("Moved"), 
                   std::make_unique<PropertyStatus>("RENTED"),
                   std::make_unique<PropertyType>("APARTMENT"),
                   8, 1, 1, false, true, "801", 200.0, "Sunset View");
    
    moved = std::move(original);
    
    EXPECT_EQ(moved.getId(), "APT001");
    EXPECT_DOUBLE_EQ(moved.getArea(), 85.0);
    EXPECT_DOUBLE_EQ(moved.getRentalPrice(), 1500.0);
    EXPECT_EQ(moved.getFloorNumber(), 12);
    EXPECT_EQ(moved.getNumberOfBedrooms(), 2);
    EXPECT_EQ(moved.getNumberOfBathrooms(), 2);
    EXPECT_TRUE(moved.getHasBalcony());
    EXPECT_TRUE(moved.getHasParking());
    EXPECT_EQ(moved.getApartmentNumber(), "1201");
    EXPECT_DOUBLE_EQ(moved.getMaintenanceFee(), 300.0);
    EXPECT_EQ(moved.getBuildingName(), "Skyline Tower");
}

TEST_F(ApartmentTest, SelfAssignment) {
    Apartment apartment("APT001", 85.0, 1500.0, 
                       std::make_unique<Address>("Test"), 
                       std::make_unique<PropertyStatus>("AVAILABLE"),
                       std::make_unique<PropertyType>("APARTMENT"),
                       12, 2, 2, true, true, "1201", 300.0, "Skyline Tower");
    
    apartment.setMaintenanceFee(350.0);
    apartment.setApartmentNumber("1302");
    
    apartment = apartment;
    
    EXPECT_EQ(apartment.getId(), "APT001");
    EXPECT_DOUBLE_EQ(apartment.getArea(), 85.0);
    EXPECT_DOUBLE_EQ(apartment.getRentalPrice(), 1500.0);
    EXPECT_EQ(apartment.getFloorNumber(), 12);
    EXPECT_EQ(apartment.getNumberOfBedrooms(), 2);
    EXPECT_EQ(apartment.getNumberOfBathrooms(), 2);
    EXPECT_TRUE(apartment.getHasBalcony());
    EXPECT_TRUE(apartment.getHasParking());
    EXPECT_EQ(apartment.getApartmentNumber(), "1302");
    EXPECT_DOUBLE_EQ(apartment.getMaintenanceFee(), 350.0);
    EXPECT_EQ(apartment.getBuildingName(), "Skyline Tower");
}

TEST_F(ApartmentTest, MixedFunctionalMethods) {
    Apartment apartment("APT001", 85.0, 1500.0, 
                       std::make_unique<Address>("Test"), 
                       std::make_unique<PropertyStatus>("AVAILABLE"),
                       std::make_unique<PropertyType>("APARTMENT"),
                       12, 2, 2, true, true, "1201", 300.0, "Skyline Tower");
    
    EXPECT_DOUBLE_EQ(apartment.calculateTotalMonthlyCost(), 1800.0);
    EXPECT_TRUE(apartment.isOnUpperFloor(10));
    EXPECT_TRUE(apartment.meetsBedroomRequirements(1, 3));
    EXPECT_DOUBLE_EQ(apartment.calculatePricePerBedroom(), 750.0);
    EXPECT_TRUE(apartment.hasPremiumFeatures());
    EXPECT_TRUE(apartment.isSuitableForFamilies());
    EXPECT_GT(apartment.calculateSpaceEfficiency(), 0.0);
}

TEST_F(ApartmentTest, EdgeCasesAndBoundaryConditions) {
    Apartment apartment("EDGE", 0.0, 0.0,
                       std::make_unique<Address>("Test"), 
                       std::make_unique<PropertyStatus>("AVAILABLE"),
                       std::make_unique<PropertyType>("APARTMENT"),
                       0, 0, 0, false, false, "", 0.0, "");
    
    EXPECT_DOUBLE_EQ(apartment.calculateTotalMonthlyCost(), 0.0);
    EXPECT_TRUE(apartment.isOnUpperFloor(0));
    EXPECT_FALSE(apartment.isOnUpperFloor(1));
    EXPECT_TRUE(apartment.meetsBedroomRequirements(0, 0));
    EXPECT_FALSE(apartment.meetsBedroomRequirements(1, 2));
    EXPECT_DOUBLE_EQ(apartment.calculatePricePerBedroom(), 0.0);
    EXPECT_FALSE(apartment.hasPremiumFeatures());
    EXPECT_FALSE(apartment.isSuitableForFamilies());
    EXPECT_DOUBLE_EQ(apartment.calculateSpaceEfficiency(), 0.0);
    
    apartment.setMaintenanceFee(-100.0);
    EXPECT_DOUBLE_EQ(apartment.getMaintenanceFee(), 0.0);
    
    Apartment negativeValues("NEG", -50.0, -1000.0,
                            std::make_unique<Address>("Test"), 
                            std::make_unique<PropertyStatus>("AVAILABLE"),
                            std::make_unique<PropertyType>("APARTMENT"),
                           -2, -1, -1, false, false, "NEG", -200.0, "");
    
    EXPECT_DOUBLE_EQ(negativeValues.calculateTotalMonthlyCost(), -1200.0);
    EXPECT_TRUE(negativeValues.isOnUpperFloor(-5));
    EXPECT_FALSE(negativeValues.isOnUpperFloor(-1));
    EXPECT_TRUE(negativeValues.meetsBedroomRequirements(-2, 0));
    EXPECT_DOUBLE_EQ(negativeValues.calculatePricePerBedroom(), 0.0);
    EXPECT_FALSE(negativeValues.hasPremiumFeatures());
    EXPECT_FALSE(negativeValues.isSuitableForFamilies());
    EXPECT_DOUBLE_EQ(negativeValues.calculateSpaceEfficiency(), 0.0);
}

TEST_F(ApartmentTest, InheritedPropertyMethods) {
    Apartment apartment("APT001", 85.0, 1500.0, std::move(address),
                       std::move(status), std::move(type), 12, 2, 2,
                       true, true, "1201", 300.0, "Skyline Tower");
    
    EXPECT_EQ(apartment.getId(), "APT001");
    EXPECT_DOUBLE_EQ(apartment.getArea(), 85.0);
    EXPECT_DOUBLE_EQ(apartment.getRentalPrice(), 1500.0);
    EXPECT_NE(apartment.getAddress(), nullptr);
    EXPECT_NE(apartment.getStatus(), nullptr);
    EXPECT_NE(apartment.getType(), nullptr);
    
    apartment.setRentalPrice(1600.0);
    EXPECT_DOUBLE_EQ(apartment.getRentalPrice(), 1600.0);
    
    apartment.updateArea(90.0);
    EXPECT_DOUBLE_EQ(apartment.getArea(), 90.0);
    
    EXPECT_DOUBLE_EQ(apartment.calculateRentalYield(19200.0), (19200.0 / (1600.0 * 12)) * 100);
    EXPECT_TRUE(apartment.isAvailableForRent());
    EXPECT_DOUBLE_EQ(apartment.calculatePricePerSqMeter(), 1600.0 / 90.0);
    EXPECT_TRUE(apartment.meetsAreaRequirements(80.0, 100.0));
    EXPECT_FALSE(apartment.meetsAreaRequirements(100.0, 120.0));
}