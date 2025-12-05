#include <gtest/gtest.h>
#include "../../include/items/Electronics.h"
#include "../../include/core/RentalPricing.h"
#include "../../include/core/RentalItem.h"
#include <memory>
#include <string>
#include <algorithm>
#include <cmath>
#include <vector>

class ElectronicsTest : public ::testing::Test {
protected:
    std::shared_ptr<Electronics> camera;
    std::shared_ptr<Electronics> laptop;
    std::shared_ptr<Electronics> vrEquipment;
    std::shared_ptr<Electronics> measurementDevice;

    void SetUp() override {
        camera = std::make_shared<Electronics>("ELEC001", "Canon Camera", "CAMERA", "Canon");
        laptop = std::make_shared<Electronics>("ELEC002", "Business Laptop", "LAPTOP", "Dell");
        vrEquipment = std::make_shared<Electronics>("ELEC003", "VR Headset", "VR_EQUIPMENT", "Oculus");
        measurementDevice = std::make_shared<Electronics>("ELEC004", "Laser Distance Meter", "MEASUREMENT", "Leica");
        
        camera->updateMaintenanceStatus(150);
        laptop->updateMaintenanceStatus(800); 
    }
};

TEST_F(ElectronicsTest, ConstructorInitializationCamera) {
    EXPECT_EQ(camera->getItemId(), "ELEC001");
    EXPECT_EQ(camera->getDeviceType(), "CAMERA");
    EXPECT_EQ(camera->getManufacturer(), "Canon");
}

TEST_F(ElectronicsTest, ConstructorInitializationLaptop) {
    EXPECT_EQ(laptop->getDeviceType(), "LAPTOP");
}

TEST_F(ElectronicsTest, ConstructorInitializationVREquipment) {
    EXPECT_EQ(vrEquipment->getDeviceType(), "VR_EQUIPMENT");
}

TEST_F(ElectronicsTest, CalculatePowerCostZeroConsumption) {
    double cost = laptop->calculatePowerCost(10, 0.20);
    EXPECT_DOUBLE_EQ(cost, 0.0);
}

TEST_F(ElectronicsTest, RequiresTechnicalSupportForSpecializedType) {
    EXPECT_TRUE(vrEquipment->requiresTechnicalSupport());
}

TEST_F(ElectronicsTest, RequiresTechnicalSupportForLongRentalDays) {
    EXPECT_TRUE(laptop->requiresTechnicalSupport());
}

TEST_F(ElectronicsTest, RequiresTechnicalSupportReturnsFalse) {
    EXPECT_FALSE(camera->requiresTechnicalSupport());
}

TEST_F(ElectronicsTest, CalculateAccessoryReplacementCostNormal) {
    double cost = camera->calculateAccessoryReplacementCost();
    EXPECT_NEAR(cost, 67.397260273972606, 0.001);
}

TEST_F(ElectronicsTest, CalculateAccessoryReplacementCostEmptyAccessories) {
    auto generic = std::make_shared<Electronics>("GEN001", "Generic", "AUDIO_EQUIPMENT", "X");
    double cost = generic->calculateAccessoryReplacementCost();
    EXPECT_DOUBLE_EQ(cost, 0.0);
}

TEST_F(ElectronicsTest, GetRecommendedCalibrationIntervalMeasurement) {
    EXPECT_EQ(measurementDevice->getRecommendedCalibrationInterval(), 90);
}

TEST_F(ElectronicsTest, GetRecommendedCalibrationIntervalPhotography) {
    EXPECT_EQ(camera->getRecommendedCalibrationInterval(), 144);
}

TEST_F(ElectronicsTest, GetRecommendedCalibrationIntervalWithUsage) {
    EXPECT_EQ(measurementDevice->getRecommendedCalibrationInterval(), 90);
}

TEST_F(ElectronicsTest, ValidateCompatibilityBeginnerFails) {
    EXPECT_FALSE(vrEquipment->validateCompatibility("BEGINNER"));
}

TEST_F(ElectronicsTest, ValidateCompatibilityBeginnerPasses) {
    auto audio = std::make_shared<Electronics>("AUD001", "Speaker", "AUDIO_EQUIPMENT", "X");
    EXPECT_TRUE(audio->validateCompatibility("BEGINNER"));
}

TEST_F(ElectronicsTest, ValidateCompatibilityIntermediatePasses) {
    EXPECT_TRUE(camera->validateCompatibility("INTERMEDIATE"));
}

TEST_F(ElectronicsTest, CalculateDataTransferCostCamera) {
    double cost = camera->calculateDataTransferCost(10.0);
    EXPECT_DOUBLE_EQ(cost, 5.0);
}

TEST_F(ElectronicsTest, CalculateDataTransferCostLaptop) {
    double cost = laptop->calculateDataTransferCost(200.0);
    EXPECT_DOUBLE_EQ(cost, 10.0);
}

TEST_F(ElectronicsTest, CalculateDataTransferCostDefaultClamped) {
    double cost = measurementDevice->calculateDataTransferCost(10.0);
    EXPECT_DOUBLE_EQ(cost, 5.0);
}