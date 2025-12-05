#include <gtest/gtest.h>
#include "../../include/core/Amenity.h"

class AmenityTest : public ::testing::Test {
protected:
    std::unique_ptr<Amenity> createBasicAmenity() {
        return std::make_unique<Amenity>("A1", "Swimming Pool", "pool", 
                                        "Outdoor heated swimming pool", true, false,
                                        25.0, 50, "6:00-22:00", true, "fully_accessible");
    }
};

TEST_F(AmenityTest, ConstructorEdgeCases) {
    Amenity amenity("", "", "", "", false, false, -100.0, -50, "", false, "");
    
    EXPECT_EQ(amenity.getAmenityId(), "");
    EXPECT_EQ(amenity.getName(), "");
    EXPECT_EQ(amenity.getType(), "");
    EXPECT_EQ(amenity.getDescription(), "");
    EXPECT_FALSE(amenity.getIsShared());
    EXPECT_FALSE(amenity.getIsAvailable24_7());
    EXPECT_DOUBLE_EQ(amenity.getUsageFee(), -100.0);
    EXPECT_EQ(amenity.getCapacity(), -50);
    EXPECT_EQ(amenity.getOperatingHours(), "");
    EXPECT_FALSE(amenity.getRequiresReservation());
    EXPECT_EQ(amenity.getAccessibility(), "");
}

TEST_F(AmenityTest, CopyConstructor) {
    auto amenity1 = createBasicAmenity();
    amenity1->setUsageFee(30.0);
    amenity1->setOperatingHours("7:00-21:00");
    
    Amenity amenity2(*amenity1);
    
    EXPECT_EQ(amenity2.getAmenityId(), "A1");
    EXPECT_EQ(amenity2.getName(), "Swimming Pool");
    EXPECT_EQ(amenity2.getType(), "pool");
    EXPECT_EQ(amenity2.getDescription(), "Outdoor heated swimming pool");
    EXPECT_TRUE(amenity2.getIsShared());
    EXPECT_FALSE(amenity2.getIsAvailable24_7());
    EXPECT_DOUBLE_EQ(amenity2.getUsageFee(), 30.0);
    EXPECT_EQ(amenity2.getCapacity(), 50);
    EXPECT_EQ(amenity2.getOperatingHours(), "7:00-21:00");
    EXPECT_TRUE(amenity2.getRequiresReservation());
    EXPECT_EQ(amenity2.getAccessibility(), "fully_accessible");
}

TEST_F(AmenityTest, CopyAssignment) {
    auto amenity1 = createBasicAmenity();
    amenity1->setUsageFee(30.0);
    amenity1->setOperatingHours("7:00-21:00");
    
    auto amenity2 = std::make_unique<Amenity>("A2", "Gym", "gym", "Fitness center",
                                             false, true, 0.0, 20, "24/7", false,
                                             "wheelchair_accessible");
    
    *amenity2 = *amenity1;
    
    EXPECT_EQ(amenity2->getAmenityId(), "A1");
    EXPECT_EQ(amenity2->getName(), "Swimming Pool");
    EXPECT_EQ(amenity2->getType(), "pool");
    EXPECT_EQ(amenity2->getDescription(), "Outdoor heated swimming pool");
    EXPECT_TRUE(amenity2->getIsShared());
    EXPECT_FALSE(amenity2->getIsAvailable24_7());
    EXPECT_DOUBLE_EQ(amenity2->getUsageFee(), 30.0);
    EXPECT_EQ(amenity2->getCapacity(), 50);
    EXPECT_EQ(amenity2->getOperatingHours(), "7:00-21:00");
    EXPECT_TRUE(amenity2->getRequiresReservation());
    EXPECT_EQ(amenity2->getAccessibility(), "fully_accessible");
}

TEST_F(AmenityTest, MoveConstructor) {
    auto amenity1 = createBasicAmenity();
    Amenity amenity2(std::move(*amenity1));
    
    EXPECT_EQ(amenity2.getAmenityId(), "A1");
    EXPECT_EQ(amenity2.getName(), "Swimming Pool");
    EXPECT_EQ(amenity2.getType(), "pool");
    EXPECT_EQ(amenity2.getDescription(), "Outdoor heated swimming pool");
    EXPECT_TRUE(amenity2.getIsShared());
    EXPECT_FALSE(amenity2.getIsAvailable24_7());
    EXPECT_DOUBLE_EQ(amenity2.getUsageFee(), 25.0);
    EXPECT_EQ(amenity2.getCapacity(), 50);
    EXPECT_EQ(amenity2.getOperatingHours(), "6:00-22:00");
    EXPECT_TRUE(amenity2.getRequiresReservation());
    EXPECT_EQ(amenity2.getAccessibility(), "fully_accessible");
}

TEST_F(AmenityTest, MoveAssignment) {
    auto amenity1 = createBasicAmenity();
    auto amenity2 = std::make_unique<Amenity>("A2", "Gym", "gym", "Fitness center",
                                             false, true, 0.0, 20, "24/7", false,
                                             "wheelchair_accessible");
    
    *amenity2 = std::move(*amenity1);
    
    EXPECT_EQ(amenity2->getAmenityId(), "A1");
    EXPECT_EQ(amenity2->getName(), "Swimming Pool");
    EXPECT_EQ(amenity2->getType(), "pool");
    EXPECT_EQ(amenity2->getDescription(), "Outdoor heated swimming pool");
    EXPECT_TRUE(amenity2->getIsShared());
    EXPECT_FALSE(amenity2->getIsAvailable24_7());
    EXPECT_DOUBLE_EQ(amenity2->getUsageFee(), 25.0);
    EXPECT_EQ(amenity2->getCapacity(), 50);
    EXPECT_EQ(amenity2->getOperatingHours(), "6:00-22:00");
    EXPECT_TRUE(amenity2->getRequiresReservation());
    EXPECT_EQ(amenity2->getAccessibility(), "fully_accessible");
}

TEST_F(AmenityTest, SetUsageFeeEdgeCases) {
    auto amenity = createBasicAmenity();
    
    amenity->setUsageFee(-50.0);
    EXPECT_DOUBLE_EQ(amenity->getUsageFee(), 0.0);
    
    amenity->setUsageFee(0.0);
    EXPECT_DOUBLE_EQ(amenity->getUsageFee(), 0.0);
    
    amenity->setUsageFee(75.5);
    EXPECT_DOUBLE_EQ(amenity->getUsageFee(), 75.5);
    
    amenity->setUsageFee(1000.0);
    EXPECT_DOUBLE_EQ(amenity->getUsageFee(), 1000.0);
}

TEST_F(AmenityTest, IsFreeToUseEdgeCases) {
    Amenity amenity1("A1", "Free Amenity", "park", "Free park", true, true,
                    0.0, 100, "24/7", false, "fully_accessible");
    EXPECT_TRUE(amenity1.isFreeToUse());
    
    Amenity amenity2("A2", "Paid Amenity", "gym", "Gym", true, true,
                    25.0, 50, "6:00-22:00", true, "fully_accessible");
    EXPECT_FALSE(amenity2.isFreeToUse());
    
    Amenity amenity3("A3", "Negative Fee", "pool", "Pool", true, true,
                    -10.0, 30, "8:00-20:00", true, "fully_accessible");
    EXPECT_FALSE(amenity3.isFreeToUse());
}

TEST_F(AmenityTest, HasCapacityForEdgeCases) {
    Amenity amenity1("A1", "Small Amenity", "room", "Meeting room", false, true,
                    50.0, 10, "9:00-17:00", true, "fully_accessible");
    
    EXPECT_TRUE(amenity1.hasCapacityFor(10));
    EXPECT_TRUE(amenity1.hasCapacityFor(5));
    EXPECT_FALSE(amenity1.hasCapacityFor(11));
    EXPECT_TRUE(amenity1.hasCapacityFor(0));
    EXPECT_TRUE(amenity1.hasCapacityFor(-5));
    
    Amenity amenity2("A2", "Large Amenity", "hall", "Conference hall", true, true,
                    100.0, 200, "8:00-22:00", true, "fully_accessible");
    
    EXPECT_TRUE(amenity2.hasCapacityFor(200));
    EXPECT_TRUE(amenity2.hasCapacityFor(100));
    EXPECT_FALSE(amenity2.hasCapacityFor(201));
    
    Amenity amenity3("A3", "Zero Capacity", "elevator", "Elevator", true, true,
                    0.0, 0, "24/7", false, "fully_accessible");
    
    EXPECT_TRUE(amenity3.hasCapacityFor(0));
    EXPECT_FALSE(amenity3.hasCapacityFor(1));
    
    Amenity amenity4("A4", "Negative Capacity", "room", "Room", false, true,
                    50.0, -10, "9:00-17:00", true, "fully_accessible");
    
    EXPECT_TRUE(amenity4.hasCapacityFor(-20));
    EXPECT_FALSE(amenity4.hasCapacityFor(-5));
}

TEST_F(AmenityTest, CalculateMonthlyRevenueEdgeCases) {
    Amenity amenity1("A1", "Free Amenity", "park", "Park", true, true,
                    0.0, 100, "24/7", false, "fully_accessible");
    
    EXPECT_DOUBLE_EQ(amenity1.calculateMonthlyRevenue(0), 0.0);
    EXPECT_DOUBLE_EQ(amenity1.calculateMonthlyRevenue(100), 0.0);
    EXPECT_DOUBLE_EQ(amenity1.calculateMonthlyRevenue(-50), 0.0);
    
    Amenity amenity2("A2", "Paid Amenity", "gym", "Gym", true, true,
                    25.0, 50, "6:00-22:00", true, "fully_accessible");
    
    EXPECT_DOUBLE_EQ(amenity2.calculateMonthlyRevenue(0), 0.0);
    EXPECT_DOUBLE_EQ(amenity2.calculateMonthlyRevenue(1), 25.0);
    EXPECT_DOUBLE_EQ(amenity2.calculateMonthlyRevenue(10), 250.0);
    EXPECT_DOUBLE_EQ(amenity2.calculateMonthlyRevenue(-10), -250.0);
    
    Amenity amenity3("A3", "Negative Fee", "pool", "Pool", true, true,
                    -10.0, 30, "8:00-20:00", true, "fully_accessible");
    
    EXPECT_DOUBLE_EQ(amenity3.calculateMonthlyRevenue(5), -50.0);
    EXPECT_DOUBLE_EQ(amenity3.calculateMonthlyRevenue(-5), 50.0);
}

TEST_F(AmenityTest, IsAccessibleAllCases) {
    Amenity amenity1("A1", "Fully Accessible", "elevator", "Elevator", true, true,
                    0.0, 10, "24/7", false, "fully_accessible");
    EXPECT_TRUE(amenity1.isAccessible());
    
    Amenity amenity2("A2", "Wheelchair Accessible", "ramp", "Ramp", true, true,
                    0.0, 0, "24/7", false, "wheelchair_accessible");
    EXPECT_TRUE(amenity2.isAccessible());
    
    Amenity amenity3("A3", "Partially Accessible", "stairs", "Stairs", true, true,
                    0.0, 0, "24/7", false, "partial");
    EXPECT_FALSE(amenity3.isAccessible());
    
    Amenity amenity4("A4", "Not Accessible", "ladder", "Ladder", false, true,
                    0.0, 0, "24/7", false, "not_accessible");
    EXPECT_FALSE(amenity4.isAccessible());
    
    Amenity amenity5("A5", "Empty Accessibility", "room", "Room", false, true,
                    0.0, 10, "9:00-17:00", true, "");
    EXPECT_FALSE(amenity5.isAccessible());
    
    Amenity amenity6("A6", "Different Case", "door", "Door", true, true,
                    0.0, 0, "24/7", false, "FULLY_ACCESSIBLE");
    EXPECT_FALSE(amenity6.isAccessible());
}

TEST_F(AmenityTest, RequiresMaintenanceStaffAllCases) {
    Amenity amenity1("A1", "Pool", "pool", "Swimming pool", true, false,
                    25.0, 50, "6:00-22:00", true, "fully_accessible");
    EXPECT_TRUE(amenity1.requiresMaintenanceStaff());
    
    Amenity amenity2("A2", "Gym", "gym", "Fitness center", true, true,
                    50.0, 30, "24/7", false, "fully_accessible");
    EXPECT_TRUE(amenity2.requiresMaintenanceStaff());
    
    Amenity amenity3("A3", "Spa", "spa", "Spa facility", false, true,
                    100.0, 10, "9:00-21:00", true, "fully_accessible");
    EXPECT_TRUE(amenity3.requiresMaintenanceStaff());
    
    Amenity amenity4("A4", "Elevator", "elevator", "Elevator", true, true,
                    0.0, 10, "24/7", false, "fully_accessible");
    EXPECT_TRUE(amenity4.requiresMaintenanceStaff());
    
    Amenity amenity5("A5", "Parking", "parking", "Parking lot", true, true,
                    0.0, 100, "24/7", false, "fully_accessible");
    EXPECT_FALSE(amenity5.requiresMaintenanceStaff());
    
    Amenity amenity6("A6", "Laundry", "laundry", "Laundry room", true, true,
                    5.0, 20, "6:00-23:00", false, "fully_accessible");
    EXPECT_FALSE(amenity6.requiresMaintenanceStaff());
    
    Amenity amenity7("A7", "Unknown Type", "unknown", "Unknown amenity", true, true,
                    0.0, 10, "24/7", false, "fully_accessible");
    EXPECT_FALSE(amenity7.requiresMaintenanceStaff());
}

TEST_F(AmenityTest, IsRecreationalAllCases) {
    Amenity amenity1("A1", "Pool", "pool", "Swimming pool", true, false,
                    25.0, 50, "6:00-22:00", true, "fully_accessible");
    EXPECT_TRUE(amenity1.isRecreational());
    
    Amenity amenity2("A2", "Gym", "gym", "Fitness center", true, true,
                    50.0, 30, "24/7", false, "fully_accessible");
    EXPECT_TRUE(amenity2.isRecreational());
    
    Amenity amenity3("A3", "Tennis Court", "tennis_court", "Tennis court", false, true,
                    20.0, 4, "7:00-21:00", true, "fully_accessible");
    EXPECT_TRUE(amenity3.isRecreational());
    
    Amenity amenity4("A4", "Playground", "playground", "Children's playground", true, true,
                    0.0, 20, "8:00-20:00", false, "fully_accessible");
    EXPECT_TRUE(amenity4.isRecreational());
    
    Amenity amenity5("A5", "Sports Field", "sports_field", "Sports field", true, true,
                    0.0, 100, "6:00-22:00", true, "fully_accessible");
    EXPECT_TRUE(amenity5.isRecreational());
    
    Amenity amenity6("A6", "Parking", "parking", "Parking lot", true, true,
                    0.0, 100, "24/7", false, "fully_accessible");
    EXPECT_FALSE(amenity6.isRecreational());
    
    Amenity amenity7("A7", "Laundry", "laundry", "Laundry room", true, true,
                    5.0, 20, "6:00-23:00", false, "fully_accessible");
    EXPECT_FALSE(amenity7.isRecreational());
    
    Amenity amenity8("A8", "Recreation Room", "recreation_room", "Recreation room", true, true,
                    10.0, 30, "9:00-22:00", true, "fully_accessible");
    EXPECT_FALSE(amenity8.isRecreational());
}

TEST_F(AmenityTest, CalculateMaintenanceCostAllCases) {
    Amenity amenity1("A1", "Pool", "pool", "Swimming pool", true, false,
                    25.0, 50, "6:00-22:00", true, "fully_accessible");
    double cost1 = amenity1.calculateMaintenanceCost();
    EXPECT_DOUBLE_EQ(cost1, 500.0 + (25.0 * 0.1));
    
    Amenity amenity2("A2", "Gym", "gym", "Fitness center", true, true,
                    50.0, 30, "24/7", false, "fully_accessible");
    double cost2 = amenity2.calculateMaintenanceCost();
    EXPECT_DOUBLE_EQ(cost2, 300.0 + (50.0 * 0.1));
    
    Amenity amenity3("A3", "Elevator", "elevator", "Elevator", true, true,
                    0.0, 10, "24/7", false, "fully_accessible");
    double cost3 = amenity3.calculateMaintenanceCost();
    EXPECT_DOUBLE_EQ(cost3, 200.0 + (0.0 * 0.1));
    
    Amenity amenity4("A4", "Tennis Court", "tennis_court", "Tennis court", false, true,
                    20.0, 4, "7:00-21:00", true, "fully_accessible");
    double cost4 = amenity4.calculateMaintenanceCost();
    EXPECT_DOUBLE_EQ(cost4, 150.0 + (20.0 * 0.1));
    
    Amenity amenity5("A5", "Playground", "playground", "Children's playground", true, true,
                    0.0, 20, "8:00-20:00", false, "fully_accessible");
    double cost5 = amenity5.calculateMaintenanceCost();
    EXPECT_DOUBLE_EQ(cost5, 150.0 + (0.0 * 0.1));
    
    Amenity amenity6("A6", "Parking", "parking", "Parking lot", true, true,
                    0.0, 100, "24/7", false, "fully_accessible");
    double cost6 = amenity6.calculateMaintenanceCost();
    EXPECT_DOUBLE_EQ(cost6, 50.0 + (0.0 * 0.1));
    
    Amenity amenity7("A7", "Laundry", "laundry", "Laundry room", true, true,
                    5.0, 20, "6:00-23:00", false, "fully_accessible");
    double cost7 = amenity7.calculateMaintenanceCost();
    EXPECT_DOUBLE_EQ(cost7, 50.0 + (5.0 * 0.1));
    
    Amenity amenity8("A8", "Unknown", "unknown", "Unknown amenity", true, true,
                    10.0, 10, "24/7", false, "fully_accessible");
    double cost8 = amenity8.calculateMaintenanceCost();
    EXPECT_DOUBLE_EQ(cost8, 50.0 + (10.0 * 0.1));
    
    Amenity amenity9("A9", "Negative Fee", "gym", "Gym", true, true,
                    -50.0, 30, "24/7", false, "fully_accessible");
    double cost9 = amenity9.calculateMaintenanceCost();
    EXPECT_DOUBLE_EQ(cost9, 300.0 + (-50.0 * 0.1));
}

TEST_F(AmenityTest, IsEssentialAllCases) {
    Amenity amenity1("A1", "Parking", "parking", "Parking lot", true, true,
                    0.0, 100, "24/7", false, "fully_accessible");
    EXPECT_TRUE(amenity1.isEssential());
    
    Amenity amenity2("A2", "Laundry", "laundry", "Laundry room", true, true,
                    5.0, 20, "6:00-23:00", false, "fully_accessible");
    EXPECT_TRUE(amenity2.isEssential());
    
    Amenity amenity3("A3", "Elevator", "elevator", "Elevator", true, true,
                    0.0, 10, "24/7", false, "fully_accessible");
    EXPECT_TRUE(amenity3.isEssential());
    
    Amenity amenity4("A4", "Security", "security", "Security system", true, true,
                    0.0, 0, "24/7", false, "fully_accessible");
    EXPECT_TRUE(amenity4.isEssential());
    
    Amenity amenity5("A5", "Essential Water", "water", "Essential water supply", true, true,
                    0.0, 0, "24/7", false, "fully_accessible");
    EXPECT_FALSE(amenity5.isEssential());
    
    Amenity amenity6("A6", "Pool", "pool", "Swimming pool", true, false,
                    25.0, 50, "6:00-22:00", true, "fully_accessible");
    EXPECT_FALSE(amenity6.isEssential());
    
    Amenity amenity7("A7", "Non-essential Gym", "gym", "Gym facility", true, true,
                    50.0, 30, "24/7", false, "fully_accessible");
    EXPECT_TRUE(amenity7.isEssential());
    
    Amenity amenity8("A8", "Essential in Name", "room", "Essential meeting room", true, true,
                    0.0, 20, "9:00-17:00", true, "fully_accessible");
    EXPECT_FALSE(amenity8.isEssential());
    
    Amenity amenity9("A9", "Empty Name", "", "Empty name amenity", true, true,
                    0.0, 10, "24/7", false, "fully_accessible");
    EXPECT_FALSE(amenity9.isEssential());
}

TEST_F(AmenityTest, GetAmenityTierAllCases) {
    Amenity amenity1("A1", "Premium Shared", "gym", "Gym", true, true,
                    150.0, 30, "24/7", true, "fully_accessible");
    EXPECT_EQ(amenity1.getAmenityTier(), "premium");
    
    Amenity amenity2("A2", "Premium Not Shared", "pool", "Pool", false, true,
                    50.0, 20, "6:00-22:00", true, "fully_accessible");
    EXPECT_EQ(amenity2.getAmenityTier(), "premium");
    
    Amenity amenity3("A3", "Standard With Fee", "tennis_court", "Tennis court", true, true,
                    75.0, 4, "7:00-21:00", false, "fully_accessible");
    EXPECT_EQ(amenity3.getAmenityTier(), "standard");
    
    Amenity amenity4("A4", "Standard With Reservation", "room", "Meeting room", true, true,
                    0.0, 10, "9:00-17:00", true, "fully_accessible");
    EXPECT_EQ(amenity4.getAmenityTier(), "standard");
    
    Amenity amenity5("A5", "Basic Free", "park", "Park", true, true,
                    0.0, 100, "24/7", false, "fully_accessible");
    EXPECT_EQ(amenity5.getAmenityTier(), "basic");
    
    Amenity amenity6("A6", "Premium Edge Case", "gym", "Gym", true, true,
                    100.0, 30, "24/7", false, "fully_accessible");
    EXPECT_EQ(amenity6.getAmenityTier(), "standard");
    
    Amenity amenity7("A7", "Premium Just Above", "gym", "Gym", true, true,
                    100.01, 30, "24/7", false, "fully_accessible");
    EXPECT_EQ(amenity7.getAmenityTier(), "premium");
    
    Amenity amenity8("A8", "Standard Edge Case", "room", "Room", true, true,
                    0.01, 10, "9:00-17:00", false, "fully_accessible");
    EXPECT_EQ(amenity8.getAmenityTier(), "standard");
    
    Amenity amenity9("A9", "Negative Fee", "park", "Park", true, true,
                    -50.0, 100, "24/7", false, "fully_accessible");
    EXPECT_EQ(amenity9.getAmenityTier(), "basic");
}

TEST_F(AmenityTest, SetOperatingHoursEdgeCases) {
    auto amenity = createBasicAmenity();
    
    amenity->setOperatingHours("");
    EXPECT_EQ(amenity->getOperatingHours(), "");
    
    amenity->setOperatingHours("  ");
    EXPECT_EQ(amenity->getOperatingHours(), "  ");
    
    amenity->setOperatingHours("24/7");
    EXPECT_EQ(amenity->getOperatingHours(), "24/7");
    
    amenity->setOperatingHours("Mon-Fri: 9:00-18:00, Sat: 10:00-16:00, Sun: Closed");
    EXPECT_EQ(amenity->getOperatingHours(), "Mon-Fri: 9:00-18:00, Sat: 10:00-16:00, Sun: Closed");
}

TEST_F(AmenityTest, ConstructorWithSpacesAndSpecialChars) {
    Amenity amenity("A-123", "Pool & Spa Complex", "pool_spa", 
                   "Heated indoor pool with spa area and sauna", true, false,
                   75.50, 25, "6:00 AM - 10:00 PM", true, "ADA_compliant");
    
    EXPECT_EQ(amenity.getAmenityId(), "A-123");
    EXPECT_EQ(amenity.getName(), "Pool & Spa Complex");
    EXPECT_EQ(amenity.getType(), "pool_spa");
    EXPECT_EQ(amenity.getDescription(), "Heated indoor pool with spa area and sauna");
    EXPECT_DOUBLE_EQ(amenity.getUsageFee(), 75.50);
    EXPECT_EQ(amenity.getOperatingHours(), "6:00 AM - 10:00 PM");
    EXPECT_EQ(amenity.getAccessibility(), "ADA_compliant");
}