#include <gtest/gtest.h>
#include "../../include/core/PropertyStatus.h"
#include <chrono>

class PropertyStatusTest : public ::testing::Test {
protected:
    void SetUp() override {
        currentTime = std::chrono::system_clock::now();
    }
    
    std::chrono::system_clock::time_point currentTime;
};

TEST_F(PropertyStatusTest, ConstructorWithAllParameters) {
    auto statusDate = currentTime - std::chrono::hours(24 * 30);
    PropertyStatus status("RENTED", "Property is currently rented", 
                         false, false, true, statusDate, 
                         "Tenant signed 12-month lease", 0.0, 30);
    
    EXPECT_EQ(status.getStatusCode(), "RENTED");
    EXPECT_EQ(status.getDescription(), "Property is currently rented");
    EXPECT_FALSE(status.getIsAvailable());
    EXPECT_FALSE(status.getIsUnderMaintenance());
    EXPECT_TRUE(status.getIsRented());
    EXPECT_EQ(status.getStatusDate(), statusDate);
    EXPECT_EQ(status.getStatusReason(), "Tenant signed 12-month lease");
    EXPECT_DOUBLE_EQ(status.getDiscountPercentage(), 0.0);
    EXPECT_EQ(status.getDaysOnMarket(), 30);
}

TEST_F(PropertyStatusTest, ConstructorWithCodeOnly) {
    PropertyStatus availableStatus("AVAILABLE");
    EXPECT_EQ(availableStatus.getStatusCode(), "AVAILABLE");
    EXPECT_TRUE(availableStatus.getIsAvailable());
    EXPECT_FALSE(availableStatus.getIsUnderMaintenance());
    EXPECT_FALSE(availableStatus.getIsRented());
    EXPECT_DOUBLE_EQ(availableStatus.getDiscountPercentage(), 0.0);
    EXPECT_EQ(availableStatus.getDaysOnMarket(), 0);
    
    PropertyStatus maintenanceStatus("MAINTENANCE");
    EXPECT_EQ(maintenanceStatus.getStatusCode(), "MAINTENANCE");
    EXPECT_FALSE(maintenanceStatus.getIsAvailable());
    EXPECT_TRUE(maintenanceStatus.getIsUnderMaintenance());
    EXPECT_FALSE(maintenanceStatus.getIsRented());
    
    PropertyStatus rentedStatus("RENTED");
    EXPECT_EQ(rentedStatus.getStatusCode(), "RENTED");
    EXPECT_FALSE(rentedStatus.getIsAvailable());
    EXPECT_FALSE(rentedStatus.getIsUnderMaintenance());
    EXPECT_TRUE(rentedStatus.getIsRented());
    
    PropertyStatus otherStatus("OTHER");
    EXPECT_EQ(otherStatus.getStatusCode(), "OTHER");
    EXPECT_FALSE(otherStatus.getIsAvailable());
    EXPECT_FALSE(otherStatus.getIsUnderMaintenance());
    EXPECT_FALSE(otherStatus.getIsRented());
}

TEST_F(PropertyStatusTest, DefaultConstructor) {
    PropertyStatus status;
    
    EXPECT_EQ(status.getStatusCode(), "");
    EXPECT_EQ(status.getDescription(), "");
    EXPECT_TRUE(status.getIsAvailable());
    EXPECT_FALSE(status.getIsUnderMaintenance());
    EXPECT_FALSE(status.getIsRented());
    EXPECT_EQ(status.getStatusReason(), "");
    EXPECT_DOUBLE_EQ(status.getDiscountPercentage(), 0.0);
    EXPECT_EQ(status.getDaysOnMarket(), 0);
}

TEST_F(PropertyStatusTest, Setters) {
    PropertyStatus status;
    
    status.setStatusCode("NEW_CODE");
    EXPECT_EQ(status.getStatusCode(), "NEW_CODE");
    
    status.setDescription("New description text");
    EXPECT_EQ(status.getDescription(), "New description text");
    
    status.updateStatus(false, true, false);
    EXPECT_FALSE(status.getIsAvailable());
    EXPECT_TRUE(status.getIsUnderMaintenance());
    EXPECT_FALSE(status.getIsRented());
    
    status.updateStatus(true, false, false);
    EXPECT_TRUE(status.getIsAvailable());
    EXPECT_FALSE(status.getIsUnderMaintenance());
    EXPECT_FALSE(status.getIsRented());
    
    status.updateStatus(false, false, true);
    EXPECT_FALSE(status.getIsAvailable());
    EXPECT_FALSE(status.getIsUnderMaintenance());
    EXPECT_TRUE(status.getIsRented());
}

TEST_F(PropertyStatusTest, SetDiscountPercentage) {
    PropertyStatus status;
    
    status.setDiscountPercentage(10.0);
    EXPECT_DOUBLE_EQ(status.getDiscountPercentage(), 10.0);
    
    status.setDiscountPercentage(0.0);
    EXPECT_DOUBLE_EQ(status.getDiscountPercentage(), 0.0);
    
    status.setDiscountPercentage(50.0);
    EXPECT_DOUBLE_EQ(status.getDiscountPercentage(), 50.0);
    
    status.setDiscountPercentage(-10.0);
    EXPECT_DOUBLE_EQ(status.getDiscountPercentage(), 0.0);
    
    status.setDiscountPercentage(60.0);
    EXPECT_DOUBLE_EQ(status.getDiscountPercentage(), 50.0);
    
    status.setDiscountPercentage(25.5);
    EXPECT_DOUBLE_EQ(status.getDiscountPercentage(), 25.5);
}

TEST_F(PropertyStatusTest, CanShowProperty) {
    PropertyStatus availableStatus("AVAILABLE", "Available", true, false, false, 
                                 currentTime, "", 0.0, 10);
    EXPECT_TRUE(availableStatus.canShowProperty());
    
    PropertyStatus maintenanceStatus("MAINTENANCE", "Under maintenance", false, true, false,
                                   currentTime, "", 0.0, 10);
    EXPECT_FALSE(maintenanceStatus.canShowProperty());
    
    PropertyStatus rentedStatus("RENTED", "Rented", false, false, true,
                              currentTime, "", 0.0, 10);
    EXPECT_FALSE(rentedStatus.canShowProperty());
    
    PropertyStatus availableButMaintenance("AVAILABLE", "Available but maintenance", 
                                          true, true, false, currentTime, "", 0.0, 10);
    EXPECT_FALSE(availableButMaintenance.canShowProperty());
}

TEST_F(PropertyStatusTest, RequiresPriceAdjustment) {
    PropertyStatus newListing("AVAILABLE", "", true, false, false, 
                            currentTime, "", 0.0, 30);
    EXPECT_FALSE(newListing.requiresPriceAdjustment());
    
    PropertyStatus oldListingNoDiscount("AVAILABLE", "", true, false, false,
                                      currentTime, "", 0.0, 61);
    EXPECT_TRUE(oldListingNoDiscount.requiresPriceAdjustment());
    
    PropertyStatus oldListingWithDiscount("AVAILABLE", "", true, false, false,
                                        currentTime, "", 10.0, 61);
    EXPECT_FALSE(oldListingWithDiscount.requiresPriceAdjustment());
    
    PropertyStatus rentedProperty("RENTED", "", false, false, true,
                                currentTime, "", 0.0, 100);
    EXPECT_TRUE(rentedProperty.requiresPriceAdjustment());
    
    PropertyStatus exactThreshold("AVAILABLE", "", true, false, false,
                                currentTime, "", 0.0, 60);
    EXPECT_FALSE(exactThreshold.requiresPriceAdjustment());
}

TEST_F(PropertyStatusTest, CalculateDiscountedPrice) {
    PropertyStatus noDiscount("AVAILABLE", "", true, false, false,
                            currentTime, "", 0.0, 10);
    EXPECT_DOUBLE_EQ(noDiscount.calculateDiscountedPrice(1000.0), 1000.0);
    
    PropertyStatus withDiscount("AVAILABLE", "", true, false, false,
                              currentTime, "", 10.0, 10);
    EXPECT_DOUBLE_EQ(withDiscount.calculateDiscountedPrice(1000.0), 900.0);
    
    PropertyStatus maxDiscount("AVAILABLE", "", true, false, false,
                             currentTime, "", 50.0, 10);
    EXPECT_DOUBLE_EQ(maxDiscount.calculateDiscountedPrice(1000.0), 500.0);
    
    PropertyStatus partialDiscount("AVAILABLE", "", true, false, false,
                                 currentTime, "", 15.5, 10);
    EXPECT_DOUBLE_EQ(partialDiscount.calculateDiscountedPrice(1000.0), 845.0);
    
    EXPECT_DOUBLE_EQ(partialDiscount.calculateDiscountedPrice(0.0), 0.0);
    EXPECT_DOUBLE_EQ(partialDiscount.calculateDiscountedPrice(-1000.0), -845.0);
}

TEST_F(PropertyStatusTest, IsLongTermVacancy) {
    PropertyStatus newVacancy("AVAILABLE", "", true, false, false,
                            currentTime, "", 0.0, 30);
    EXPECT_FALSE(newVacancy.isLongTermVacancy(60));
    EXPECT_TRUE(newVacancy.isLongTermVacancy(29));
    
    PropertyStatus longVacancy("AVAILABLE", "", true, false, false,
                             currentTime, "", 0.0, 91);
    EXPECT_TRUE(longVacancy.isLongTermVacancy(60));
    EXPECT_TRUE(longVacancy.isLongTermVacancy(90));
    EXPECT_FALSE(longVacancy.isLongTermVacancy(91));
    
    PropertyStatus rented("RENTED", "", false, false, true,
                        currentTime, "", 0.0, 100);
    EXPECT_FALSE(rented.isLongTermVacancy(60));
    
    PropertyStatus maintenance("MAINTENANCE", "", false, true, false,
                             currentTime, "", 0.0, 100);
    EXPECT_FALSE(maintenance.isLongTermVacancy(60));
}

TEST_F(PropertyStatusTest, ShouldIncreaseMarketing) {
    PropertyStatus newListing("AVAILABLE", "", true, false, false,
                            currentTime, "", 0.0, 15);
    EXPECT_FALSE(newListing.shouldIncreaseMarketing());
    
    PropertyStatus oldListingNoDiscount("AVAILABLE", "", true, false, false,
                                      currentTime, "", 0.0, 31);
    EXPECT_TRUE(oldListingNoDiscount.shouldIncreaseMarketing());
    
    PropertyStatus oldListingWithDiscount("AVAILABLE", "", true, false, false,
                                        currentTime, "", 10.0, 31);
    EXPECT_FALSE(oldListingWithDiscount.shouldIncreaseMarketing());
    
    PropertyStatus highDiscount("AVAILABLE", "", true, false, false,
                              currentTime, "", 10.1, 31);
    EXPECT_FALSE(highDiscount.shouldIncreaseMarketing());
    
    PropertyStatus exactDays("AVAILABLE", "", true, false, false,
                           currentTime, "", 0.0, 30);
    EXPECT_FALSE(exactDays.shouldIncreaseMarketing());
    
    PropertyStatus notAvailable("RENTED", "", false, false, true,
                              currentTime, "", 0.0, 100);
    EXPECT_FALSE(notAvailable.shouldIncreaseMarketing());
}

TEST_F(PropertyStatusTest, IsEligibleForPromotion) {
    PropertyStatus newListing("AVAILABLE", "", true, false, false,
                            currentTime, "", 0.0, 30);
    EXPECT_FALSE(newListing.isEligibleForPromotion());
    
    PropertyStatus eligible("AVAILABLE", "", true, false, false,
                          currentTime, "", 0.0, 46);
    EXPECT_TRUE(eligible.isEligibleForPromotion());
    
    PropertyStatus exactDays("AVAILABLE", "", true, false, false,
                           currentTime, "", 0.0, 45);
    EXPECT_FALSE(exactDays.isEligibleForPromotion());
    
    PropertyStatus underMaintenance("AVAILABLE", "", true, true, false,
                                  currentTime, "", 0.0, 46);
    EXPECT_FALSE(underMaintenance.isEligibleForPromotion());
    
    PropertyStatus notAvailable("RENTED", "", false, false, true,
                              currentTime, "", 0.0, 46);
    EXPECT_FALSE(notAvailable.isEligibleForPromotion());
    
    PropertyStatus rentedButAvailable("AVAILABLE", "", false, false, true,
                                    currentTime, "", 0.0, 46);
    EXPECT_FALSE(rentedButAvailable.isEligibleForPromotion());
}

TEST_F(PropertyStatusTest, NeedsMaintenanceCheck) {
    PropertyStatus available("AVAILABLE", "", true, false, false,
                           currentTime, "", 0.0, 10);
    EXPECT_FALSE(available.needsMaintenanceCheck());
    
    PropertyStatus underMaintenance("MAINTENANCE", "", false, true, false,
                                  currentTime, "", 0.0, 10);
    EXPECT_TRUE(underMaintenance.needsMaintenanceCheck());
    
    PropertyStatus rented("RENTED", "", false, false, true,
                        currentTime, "", 0.0, 10);
    EXPECT_FALSE(rented.needsMaintenanceCheck());
    
    PropertyStatus notAvailableNotRented("SOLD", "", false, false, false,
                                       currentTime, "", 0.0, 10);
    EXPECT_TRUE(notAvailableNotRented.needsMaintenanceCheck());
    
    PropertyStatus availableAndMaintenance("AVAILABLE", "", true, true, false,
                                         currentTime, "", 0.0, 10);
    EXPECT_TRUE(availableAndMaintenance.needsMaintenanceCheck());
}

TEST_F(PropertyStatusTest, GetMarketAnalysis) {
    PropertyStatus newListing("AVAILABLE", "", true, false, false,
                            currentTime, "", 0.0, 10);
    EXPECT_EQ(newListing.getMarketAnalysis(), "new_listing");
    
    PropertyStatus normalListing("AVAILABLE", "", true, false, false,
                               currentTime, "", 0.0, 30);
    EXPECT_EQ(normalListing.getMarketAnalysis(), "normal_market");
    
    PropertyStatus slowListing("AVAILABLE", "", true, false, false,
                             currentTime, "", 0.0, 60);
    EXPECT_EQ(slowListing.getMarketAnalysis(), "slow_market");
    
    PropertyStatus stagnantListing("AVAILABLE", "", true, false, false,
                                 currentTime, "", 0.0, 100);
    EXPECT_EQ(stagnantListing.getMarketAnalysis(), "stagnant_listing");
    
    PropertyStatus edgeCase1("AVAILABLE", "", true, false, false,
                           currentTime, "", 0.0, 15);
    EXPECT_EQ(edgeCase1.getMarketAnalysis(), "normal_market");
    
    PropertyStatus edgeCase2("AVAILABLE", "", true, false, false,
                           currentTime, "", 0.0, 45);
    EXPECT_EQ(edgeCase2.getMarketAnalysis(), "slow_market");
    
    PropertyStatus edgeCase3("AVAILABLE", "", true, false, false,
                           currentTime, "", 0.0, 90);
    EXPECT_EQ(edgeCase3.getMarketAnalysis(), "stagnant_listing");
    
    PropertyStatus zeroDays("AVAILABLE", "", true, false, false,
                          currentTime, "", 0.0, 0);
    EXPECT_EQ(zeroDays.getMarketAnalysis(), "new_listing");
    
    PropertyStatus negativeDays("AVAILABLE", "", true, false, false,
                              currentTime, "", 0.0, -10);
    EXPECT_EQ(negativeDays.getMarketAnalysis(), "new_listing");
}

TEST_F(PropertyStatusTest, CopyConstructor) {
    auto statusDate = currentTime - std::chrono::hours(24 * 45);
    PropertyStatus original("AVAILABLE", "Available property", true, false, false,
                          statusDate, "Fresh listing", 15.5, 45);
    
    PropertyStatus copy(original);
    
    EXPECT_EQ(copy.getStatusCode(), "AVAILABLE");
    EXPECT_EQ(copy.getDescription(), "Available property");
    EXPECT_TRUE(copy.getIsAvailable());
    EXPECT_FALSE(copy.getIsUnderMaintenance());
    EXPECT_FALSE(copy.getIsRented());
    EXPECT_EQ(copy.getStatusDate(), statusDate);
    EXPECT_EQ(copy.getStatusReason(), "Fresh listing");
    EXPECT_DOUBLE_EQ(copy.getDiscountPercentage(), 15.5);
    EXPECT_EQ(copy.getDaysOnMarket(), 45);
}

TEST_F(PropertyStatusTest, CopyAssignment) {
    auto statusDate = currentTime - std::chrono::hours(24 * 45);
    PropertyStatus original("AVAILABLE", "Available property", true, false, false,
                          statusDate, "Fresh listing", 15.5, 45);
    
    PropertyStatus assigned("RENTED", "Rented property", false, false, true,
                          currentTime, "Lease active", 0.0, 10);
    
    assigned = original;
    
    EXPECT_EQ(assigned.getStatusCode(), "AVAILABLE");
    EXPECT_EQ(assigned.getDescription(), "Available property");
    EXPECT_TRUE(assigned.getIsAvailable());
    EXPECT_FALSE(assigned.getIsUnderMaintenance());
    EXPECT_FALSE(assigned.getIsRented());
    EXPECT_EQ(assigned.getStatusDate(), statusDate);
    EXPECT_EQ(assigned.getStatusReason(), "Fresh listing");
    EXPECT_DOUBLE_EQ(assigned.getDiscountPercentage(), 15.5);
    EXPECT_EQ(assigned.getDaysOnMarket(), 45);
}

TEST_F(PropertyStatusTest, MoveConstructor) {
    auto statusDate = currentTime - std::chrono::hours(24 * 45);
    PropertyStatus original("AVAILABLE", "Available property", true, false, false,
                          statusDate, "Fresh listing", 15.5, 45);
    
    PropertyStatus moved(std::move(original));
    
    EXPECT_EQ(moved.getStatusCode(), "AVAILABLE");
    EXPECT_EQ(moved.getDescription(), "Available property");
    EXPECT_TRUE(moved.getIsAvailable());
    EXPECT_FALSE(moved.getIsUnderMaintenance());
    EXPECT_FALSE(moved.getIsRented());
    EXPECT_EQ(moved.getStatusDate(), statusDate);
    EXPECT_EQ(moved.getStatusReason(), "Fresh listing");
    EXPECT_DOUBLE_EQ(moved.getDiscountPercentage(), 15.5);
    EXPECT_EQ(moved.getDaysOnMarket(), 45);
}

TEST_F(PropertyStatusTest, MoveAssignment) {
    auto statusDate = currentTime - std::chrono::hours(24 * 45);
    PropertyStatus original("AVAILABLE", "Available property", true, false, false,
                          statusDate, "Fresh listing", 15.5, 45);
    
    PropertyStatus moved("RENTED", "Rented property", false, false, true,
                       currentTime, "Lease active", 0.0, 10);
    
    moved = std::move(original);
    
    EXPECT_EQ(moved.getStatusCode(), "AVAILABLE");
    EXPECT_EQ(moved.getDescription(), "Available property");
    EXPECT_TRUE(moved.getIsAvailable());
    EXPECT_FALSE(moved.getIsUnderMaintenance());
    EXPECT_FALSE(moved.getIsRented());
    EXPECT_EQ(moved.getStatusDate(), statusDate);
    EXPECT_EQ(moved.getStatusReason(), "Fresh listing");
    EXPECT_DOUBLE_EQ(moved.getDiscountPercentage(), 15.5);
    EXPECT_EQ(moved.getDaysOnMarket(), 45);
}

TEST_F(PropertyStatusTest, SelfAssignment) {
    PropertyStatus status("AVAILABLE", "Test", true, false, false,
                        currentTime, "Test reason", 10.0, 20);
    
    status = status;
    
    EXPECT_EQ(status.getStatusCode(), "AVAILABLE");
    EXPECT_EQ(status.getDescription(), "Test");
    EXPECT_TRUE(status.getIsAvailable());
    EXPECT_FALSE(status.getIsUnderMaintenance());
    EXPECT_FALSE(status.getIsRented());
    EXPECT_EQ(status.getStatusReason(), "Test reason");
    EXPECT_DOUBLE_EQ(status.getDiscountPercentage(), 10.0);
    EXPECT_EQ(status.getDaysOnMarket(), 20);
}

TEST_F(PropertyStatusTest, EdgeCasesAndBoundaryConditions) {
    PropertyStatus emptyStatus("", "", false, false, false, currentTime, "", -10.0, -5);
    
    EXPECT_EQ(emptyStatus.getStatusCode(), "");
    EXPECT_EQ(emptyStatus.getDescription(), "");
    EXPECT_FALSE(emptyStatus.getIsAvailable());
    EXPECT_FALSE(emptyStatus.getIsUnderMaintenance());
    EXPECT_FALSE(emptyStatus.getIsRented());
    EXPECT_EQ(emptyStatus.getStatusReason(), "");
    EXPECT_DOUBLE_EQ(emptyStatus.getDiscountPercentage(), -10.0);
    EXPECT_EQ(emptyStatus.getDaysOnMarket(), -5);
    
    emptyStatus.setDiscountPercentage(-20.0);
    EXPECT_DOUBLE_EQ(emptyStatus.getDiscountPercentage(), 0.0);
    
    emptyStatus.setDiscountPercentage(100.0);
    EXPECT_DOUBLE_EQ(emptyStatus.getDiscountPercentage(), 50.0);
    
    EXPECT_DOUBLE_EQ(emptyStatus.calculateDiscountedPrice(100.0), 50.0);
    
    EXPECT_FALSE(emptyStatus.canShowProperty());
    EXPECT_FALSE(emptyStatus.requiresPriceAdjustment());
    EXPECT_FALSE(emptyStatus.isLongTermVacancy(10));
    EXPECT_FALSE(emptyStatus.shouldIncreaseMarketing());
    EXPECT_FALSE(emptyStatus.isEligibleForPromotion());
    EXPECT_TRUE(emptyStatus.needsMaintenanceCheck());
    EXPECT_EQ(emptyStatus.getMarketAnalysis(), "new_listing");
}

TEST_F(PropertyStatusTest, StatusTransitionScenarios) {
    PropertyStatus status;
    
    status.updateStatus(true, false, false);
    EXPECT_TRUE(status.getIsAvailable());
    EXPECT_FALSE(status.getIsUnderMaintenance());
    EXPECT_FALSE(status.getIsRented());
    EXPECT_TRUE(status.canShowProperty());
    EXPECT_FALSE(status.needsMaintenanceCheck());
    
    status.updateStatus(true, true, false);
    EXPECT_TRUE(status.getIsAvailable());
    EXPECT_TRUE(status.getIsUnderMaintenance());
    EXPECT_FALSE(status.getIsRented());
    EXPECT_FALSE(status.canShowProperty());
    EXPECT_TRUE(status.needsMaintenanceCheck());
    
    status.updateStatus(false, false, true);
    EXPECT_FALSE(status.getIsAvailable());
    EXPECT_FALSE(status.getIsUnderMaintenance());
    EXPECT_TRUE(status.getIsRented());
    EXPECT_FALSE(status.canShowProperty());
    EXPECT_FALSE(status.needsMaintenanceCheck());
    
    status.updateStatus(false, false, false);
    EXPECT_FALSE(status.getIsAvailable());
    EXPECT_FALSE(status.getIsUnderMaintenance());
    EXPECT_FALSE(status.getIsRented());
    EXPECT_FALSE(status.canShowProperty());
    EXPECT_TRUE(status.needsMaintenanceCheck());
}


