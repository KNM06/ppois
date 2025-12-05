#include <gtest/gtest.h>
#include "../../include/items/Vehicle.h"
#include "../../include/core/RentalItem.h"
#include "../../include/core/RentalPricing.h"
#include <memory>
#include <string>
#include <algorithm>
#include <cmath>

class VehicleTest : public ::testing::Test {
protected:
    std::shared_ptr<Vehicle> sedan;
    std::shared_ptr<Vehicle> luxuryCar;
    std::shared_ptr<Vehicle> oldDieselVan;

    void SetUp() override {
        sedan = std::make_shared<Vehicle>("VEH001", "Toyota Camry", "ABC123", "SEDAN", 2022);
        luxuryCar = std::make_shared<Vehicle>("VEH002", "Ferrari", "XYZ789", "LUXURY", 2023);
        
        oldDieselVan = std::make_shared<Vehicle>("VEH003", "Old Van", "DEF456", "VAN", 2018);
        
        sedan->updateMaintenanceStatus(100);
    }
};

TEST_F(VehicleTest, BasicCreation) {
    EXPECT_EQ(sedan->getItemId(), "VEH001");
    EXPECT_EQ(sedan->getName(), "Toyota Camry");
    EXPECT_EQ(sedan->getLicensePlate(), "ABC123");
    EXPECT_EQ(sedan->getVehicleType(), "SEDAN");
}

TEST_F(VehicleTest, CalculateFuelCostForTripPetrol) {
    double cost = sedan->calculateFuelCostForTrip(100.0);
    EXPECT_NEAR(cost, 14.875, 0.001);
}

TEST_F(VehicleTest, CalculateFuelCostForTripDiesel) {
    auto dieselCar = std::make_shared<Vehicle>("VEH004", "Diesel Car", "GTR777", "SEDAN", 2020);
    double cost = dieselCar->calculateFuelCostForTrip(100.0);
    EXPECT_NEAR(cost, 14.875, 0.001);
}

TEST_F(VehicleTest, CalculateFuelCostForTripElectric) {
    auto electricCar = std::make_shared<Vehicle>("VEH005", "Electric Car", "ELEC001", "SEDAN", 2023);
    double cost = electricCar->calculateFuelCostForTrip(50.0);
    EXPECT_NEAR(cost, 7.4375, 0.001);
}

TEST_F(VehicleTest, RequiresEmissionCheckNewVehicle) {
    EXPECT_FALSE(sedan->requiresEmissionCheck());
}

TEST_F(VehicleTest, RequiresEmissionCheckOldPetrol) {
    auto oldPetrol = std::make_shared<Vehicle>("VEH006", "Old Car", "QWERTY", "SEDAN", 2018);
    EXPECT_TRUE(oldPetrol->requiresEmissionCheck());
}

TEST_F(VehicleTest, CalculateDepreciationNormal) {
    double depreciation = sedan->calculateDepreciation();
    EXPECT_NEAR(depreciation, 20.804794520547944, 0.001);
}

TEST_F(VehicleTest, ValidateDriverAgePasses) {
    EXPECT_TRUE(sedan->validateDriverAge(25));
}

TEST_F(VehicleTest, ValidateDriverAgeFailsUnderAge) {
    EXPECT_FALSE(luxuryCar->validateDriverAge(24));
}

TEST_F(VehicleTest, ValidateDriverAgeMotorcyclePasses) {
    auto motorcycle = std::make_shared<Vehicle>("VEH007", "Bike", "MOTO01", "MOTORCYCLE", 2023);
    EXPECT_TRUE(motorcycle->validateDriverAge(17));
}

TEST_F(VehicleTest, CalculateInsurancePremiumSportsCar) {
    double premium = luxuryCar->calculateInsurancePremium();
    EXPECT_GE(premium, 0.0);
}