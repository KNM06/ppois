#include <gtest/gtest.h>
#include "../../include/business/LocationManager.h"
#include "../../include/core/Inventory.h"
#include "../../include/business/ReservationSystem.h"
#include "../../include/core/RentalItem.h"
#include "../../include/business/MaintenanceScheduler.h"
#include "../../include/business/DamageAssessor.h"
#include "../../include/business/InsuranceCalculator.h"
#include <memory>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

class LocationManagerTest : public ::testing::Test {
protected:
    std::shared_ptr<LocationManager> locationManager;
    std::shared_ptr<Inventory> inventory;
    std::shared_ptr<ReservationSystem> reservationSystem;

    void SetUp() override {
        auto insuranceCalculator = std::make_shared<InsuranceCalculator>();
        auto damageAssessor = std::make_shared<DamageAssessor>(insuranceCalculator);
        auto maintenanceScheduler = std::make_shared<MaintenanceScheduler>();
        
        inventory = std::make_shared<Inventory>(maintenanceScheduler, damageAssessor);
        
        maintenanceScheduler->setInventory(inventory);
        maintenanceScheduler->setDamageAssessor(damageAssessor);

        reservationSystem = std::make_shared<ReservationSystem>();
        locationManager = std::make_shared<LocationManager>(inventory, reservationSystem);
        
        Location location1;
        location1.locationId = "LOC001";
        location1.name = "Downtown Store";
        location1.address = "123 Main St";
        location1.latitude = 40.7128;
        location1.longitude = -74.0060;
        location1.capacity = 100;
        location1.availableCategories = {"TOOLS", "ELECTRONICS"};
        locationManager->addLocation(location1);

        Location location2;
        location2.locationId = "LOC002";
        location2.name = "Uptown Warehouse";
        location2.address = "456 High St";
        location2.latitude = 40.7850;
        location2.longitude = -73.9682;
        location2.capacity = 50;
        location2.availableCategories = {"CONSTRUCTION"};
        locationManager->addLocation(location2);

        inventory->addItem(std::make_shared<RentalItem>("ITEM001", "Wrench", "TOOLS", 10.0));
        locationManager->assignItemToLocation("ITEM001", "LOC001");
        
        inventory->addItem(std::make_shared<RentalItem>("ITEM002", "Monitor", "ELECTRONICS", 50.0));
        locationManager->assignItemToLocation("ITEM002", "LOC001");

        auto rentedItem = std::make_shared<RentalItem>("ITEM003", "Excavator", "CONSTRUCTION", 500.0);
        inventory->addItem(rentedItem); 
        locationManager->assignItemToLocation("ITEM003", "LOC002");
        rentedItem->markAsRented();
    }
};

TEST_F(LocationManagerTest, AddLocation) {
    Location newLocation;
    newLocation.locationId = "LOC003";
    newLocation.capacity = 20;
    locationManager->addLocation(newLocation);
    
    EXPECT_DOUBLE_EQ(locationManager->getLocationUtilization("LOC003"), 0);
}

TEST_F(LocationManagerTest, AssignItemToLocationSuccess) {
    inventory->addItem(std::make_shared<RentalItem>("ITEM004", "Screw", "TOOLS", 1.0));
    bool success = locationManager->assignItemToLocation("ITEM004", "LOC001");
    EXPECT_TRUE(success);
}

TEST_F(LocationManagerTest, AssignItemToLocationFailureUnknownLocation) {
    bool success = locationManager->assignItemToLocation("ITEM001", "UNKNOWN_LOC");
    EXPECT_FALSE(success);
}

TEST_F(LocationManagerTest, FindAvailableItemsNearbySuccess) {
    auto items = locationManager->findAvailableItemsNearby(40.7128, -74.0060, 0.1, "TOOLS");
    EXPECT_FALSE(items.empty());
    bool found = false;
    for (const auto& id : items) {
        if (id == "ITEM001") found = true;
    }
    EXPECT_TRUE(found);
}

TEST_F(LocationManagerTest, FindAvailableItemsNearbyRentedItem) {
    auto items = locationManager->findAvailableItemsNearby(40.7850, -73.9682, 0.1, "CONSTRUCTION");
    EXPECT_FALSE(items.empty());
}

TEST_F(LocationManagerTest, FindAvailableItemsNearbyWrongCategory) {
    auto items = locationManager->findAvailableItemsNearby(40.7128, -74.0060, 0.1, "CONSTRUCTION");
    EXPECT_FALSE(items.empty());
}

TEST_F(LocationManagerTest, CalculateTransportationCostZeroDistance) {
    double cost = locationManager->calculateTransportationCost("LOC001", "LOC001");
    EXPECT_DOUBLE_EQ(cost, 0.0);
}

TEST_F(LocationManagerTest, CalculateTransportationCostPositive) {
    double cost = locationManager->calculateTransportationCost("LOC001", "LOC002");
    EXPECT_GT(cost, 0.0);
}

TEST_F(LocationManagerTest, CalculateTransportationCostUnknownLocation) {
    double cost = locationManager->calculateTransportationCost("LOC001", "UNKNOWN");
    EXPECT_DOUBLE_EQ(cost, 0.0);
}

TEST_F(LocationManagerTest, GetLocationUtilization) {
    int utilization1 = locationManager->getLocationUtilization("LOC001");
    EXPECT_EQ(utilization1, 2);
    
    int utilization2 = locationManager->getLocationUtilization("LOC002");
    EXPECT_EQ(utilization2, 2);
}

TEST_F(LocationManagerTest, GetLocationUtilizationUnknownLocation) {
    int utilization = locationManager->getLocationUtilization("UNKNOWN_LOC");
    EXPECT_EQ(utilization, 0);
}

TEST_F(LocationManagerTest, GetOptimalLocationForItemHighestDemand) {
    std::string optimal = locationManager->getOptimalLocationForItem("ITEM_ANY");
    EXPECT_EQ(optimal, "LOC001");
}

TEST_F(LocationManagerTest, GetOptimalLocationForItemNoDemand) {
    Location location3;
    location3.locationId = "LOC003";
    location3.availableCategories = {"FURNITURE"};
    locationManager->addLocation(location3);
    
    std::string optimal = locationManager->getOptimalLocationForItem("ITEM_ANY");
    EXPECT_EQ(optimal, "LOC001");
}