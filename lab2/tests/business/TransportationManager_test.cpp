#include <gtest/gtest.h>
#include "../../include/business/TransportationManager.h"
#include "../../include/core/Inventory.h"
#include "../../include/business/LocationManager.h"
#include "../../include/core/RentalItem.h"
#include "../../include/business/MaintenanceScheduler.h"
#include "../../include/business/DamageAssessor.h"
#include "../../include/business/ReservationSystem.h"
#include <memory>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

class TransportationManagerTest : public ::testing::Test {
protected:
    std::shared_ptr<Inventory> inventory;
    std::shared_ptr<LocationManager> locationManager;
    std::shared_ptr<TransportationManager> transportationManager;
    std::shared_ptr<ReservationSystem> reservationSystem;

    void SetUp() override {
        reservationSystem = std::make_shared<ReservationSystem>();
        
        auto damageAssessor = std::make_shared<DamageAssessor>();
        auto maintenanceScheduler = std::make_shared<MaintenanceScheduler>();
        inventory = std::make_shared<Inventory>(maintenanceScheduler, damageAssessor);
        
        inventory->addItem(std::make_shared<RentalItem>("ITEM001", "Drill", "TOOLS", 20.0));
        inventory->addItem(std::make_shared<RentalItem>("CONST01", "Excavator", "CONSTRUCTION", 500.0));
        inventory->addItem(std::make_shared<RentalItem>("LUX001", "Yacht", "LUXURY", 1000.0));

        locationManager = std::make_shared<LocationManager>(inventory, reservationSystem);
        
        Location loc001 = {"LOC001", "Warehouse A", "A", 1.0, 1.0, 100, {"TOOLS"}};
        Location loc002 = {"LOC002", "Client Site", "B", 11.0, 1.0, 10, {}};
        Location loc003 = {"LOC003", "Far Away", "C", 21.0, 1.0, 50, {}};
        locationManager->addLocation(loc001);
        locationManager->addLocation(loc002);
        locationManager->addLocation(loc003);

        transportationManager = std::make_shared<TransportationManager>(inventory, locationManager);
    }
};

TEST_F(TransportationManagerTest, ScheduleTransportSuccessfully) {
    bool scheduled = transportationManager->scheduleTransport("ITEM001", "LOC001", "LOC002");
    EXPECT_TRUE(scheduled);
    
    auto schedule = transportationManager->getTransportSchedule();
    EXPECT_EQ(schedule.size(), 1u);
}

TEST_F(TransportationManagerTest, ScheduleTransportReturnsFalseIfItemUnavailable) {
    inventory->markItemAsRented("ITEM001");
    bool scheduled = transportationManager->scheduleTransport("ITEM001", "LOC001", "LOC002");
    EXPECT_FALSE(scheduled);
}

TEST_F(TransportationManagerTest, CalculateTransportCostForGeneralItem) {
    double cost = transportationManager->calculateTransportCost("ITEM001", 100.0);
    EXPECT_NEAR(cost, 75.0, 0.001);
}

TEST_F(TransportationManagerTest, CalculateTransportCostForConstructionItem) {
    double cost = transportationManager->calculateTransportCost("CONST01", 100.0);
    EXPECT_NEAR(cost, 115.0, 0.001);
}

TEST_F(TransportationManagerTest, CalculateTransportCostForLuxuryItem) {
    double cost = transportationManager->calculateTransportCost("LUX001", 100.0);
    EXPECT_NEAR(cost, 75, 0.001);
}

TEST_F(TransportationManagerTest, EstimateTransportTimeForTruck) {
    double time = transportationManager->estimateTransportTime(100.0, "TRUCK");
    EXPECT_NEAR(time, 3.5, 0.001);
}

TEST_F(TransportationManagerTest, EstimateTransportTimeForCar) {
    double time = transportationManager->estimateTransportTime(100.0, "CAR");
    EXPECT_NEAR(time, 2.6666, 0.001);
}

TEST_F(TransportationManagerTest, ValidateTransportFeasibilityForConstruction) {
    EXPECT_FALSE(transportationManager->validateTransportFeasibility("CONST01", "CAR"));
    EXPECT_TRUE(transportationManager->validateTransportFeasibility("CONST01", "TRUCK"));
}

TEST_F(TransportationManagerTest, ValidateTransportFeasibilityForLuxury) {
    EXPECT_TRUE(transportationManager->validateTransportFeasibility("LUX001", "SPECIAL"));
    EXPECT_TRUE(transportationManager->validateTransportFeasibility("LUX001", "VAN"));
}

TEST_F(TransportationManagerTest, ValidateTransportFeasibilityForUnknownVehicle) {
    EXPECT_FALSE(transportationManager->validateTransportFeasibility("ITEM001", "BIKE"));
}

TEST_F(TransportationManagerTest, CalculateFuelCostForTruck) {
    double cost = transportationManager->calculateFuelCost(100.0, "TRUCK");
    EXPECT_NEAR(cost, 8.75, 0.001);
}

TEST_F(TransportationManagerTest, CalculateFuelCostForCar) {
    double cost = transportationManager->calculateFuelCost(50.0, "CAR");
    EXPECT_NEAR(cost, 10.5, 0.001);
}

TEST_F(TransportationManagerTest, OptimizeTransportRouteNoOptimizationNeeded) {
    std::vector<std::string> locations = {"A", "B"};
    auto optimized = transportationManager->optimizeTransportRoute(locations);
    EXPECT_EQ(optimized.size(), 2u);
    EXPECT_EQ(optimized[0], "A");
    EXPECT_EQ(optimized[1], "B");
}

TEST_F(TransportationManagerTest, OptimizeTransportRouteSortsMiddleLocations) {
    std::vector<std::string> locations = {"Start", "Z", "A", "M", "End"};
    auto optimized = transportationManager->optimizeTransportRoute(locations);
    
    EXPECT_EQ(optimized.size(), 5u);
    EXPECT_EQ(optimized[0], "Start");
    EXPECT_EQ(optimized[4], "End");
    EXPECT_EQ(optimized[1], "A");
    EXPECT_EQ(optimized[2], "M");
    EXPECT_EQ(optimized[3], "Z");
}

TEST_F(TransportationManagerTest, UpdateVehicleCostUpdatesAndClamps) {
    transportationManager->updateVehicleCost("TRUCK", 0.1);
    EXPECT_DOUBLE_EQ(transportationManager->getVehicleCost("TRUCK"), 0.1);

    transportationManager->updateVehicleCost("TRUCK", -5.0);
    EXPECT_DOUBLE_EQ(transportationManager->getVehicleCost("TRUCK"), 0.0);
}

TEST_F(TransportationManagerTest, SetItemTransportMultiplierClamps) {
    transportationManager->setItemTransportMultiplier("NEW_CAT", 0.05);
    double cost = transportationManager->calculateTransportCost("NEW_CAT_ID", 100.0);
    EXPECT_DOUBLE_EQ(cost, 75.0);
}

TEST_F(TransportationManagerTest, SettersAndGetters) {
    auto newInventory = std::make_shared<Inventory>();
    transportationManager->setInventory(newInventory);
    EXPECT_EQ(transportationManager->getInventory(), newInventory);

    auto newManager = std::make_shared<LocationManager>(newInventory, reservationSystem);
    transportationManager->setLocationManager(newManager);
    EXPECT_EQ(transportationManager->getLocationManager(), newManager);
    
    EXPECT_DOUBLE_EQ(transportationManager->getVehicleCost("VAN"), 0.5);
}