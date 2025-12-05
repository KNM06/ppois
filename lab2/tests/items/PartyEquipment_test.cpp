#include <gtest/gtest.h>
#include "../../include/items/PartyEquipment.h"
#include "../../include/core/RentalItem.h"
#include "../../include/core/RentalPricing.h"
#include <memory>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

class PartyEquipmentTest : public ::testing::Test {
protected:
    std::shared_ptr<PartyEquipment> soundSystem;
    std::shared_ptr<PartyEquipment> tent;
    std::shared_ptr<PartyEquipment> tablesAndChairs;

    void SetUp() override {
        soundSystem = std::make_shared<PartyEquipment>("PARTY001", "Sound System", "SOUND_SYSTEM", 100);
        tent = std::make_shared<PartyEquipment>("PARTY002", "Large Tent", "TENT", 500);
        tablesAndChairs = std::make_shared<PartyEquipment>("PARTY003", "Chairs Set", "TABLES_CHAIRS", 50);
    }
};

TEST_F(PartyEquipmentTest, ConstructorInitializationSoundSystem) {
    EXPECT_EQ(soundSystem->getItemId(), "PARTY001");
    EXPECT_EQ(soundSystem->getPartyType(), "SOUND_SYSTEM");
    EXPECT_EQ(soundSystem->getGuestCapacity(), 100);
    EXPECT_TRUE(soundSystem->getRequiresSetup());
}

TEST_F(PartyEquipmentTest, CalculateSetupCostSoundSystem) {
    double cost = soundSystem->calculateSetupCost(3);
    EXPECT_NEAR(cost, 240.0, 0.001);
}

TEST_F(PartyEquipmentTest, CalculateSetupCostTablesAndChairs) {
    double cost = tablesAndChairs->calculateSetupCost(1);
    EXPECT_NEAR(cost, 72.0, 0.001);
}

TEST_F(PartyEquipmentTest, ValidateVenueCompatibilityFailsRequiredSpace) {
    EXPECT_FALSE(tent->validateVenueCompatibility("OUTDOOR", 749.9));
}

TEST_F(PartyEquipmentTest, ValidateVenueCompatibilityFailsIndoorTent) {
    EXPECT_FALSE(tent->validateVenueCompatibility("INDOOR", 1000.0));
}

TEST_F(PartyEquipmentTest, ValidateVenueCompatibilityOutdoorHighPower) {
    EXPECT_TRUE(soundSystem->validateVenueCompatibility("OUTDOOR", 200.0));
}

TEST_F(PartyEquipmentTest, ValidateVenueCompatibilityOutdoorHighSound) {
    EXPECT_TRUE(soundSystem->validateVenueCompatibility("OUTDOOR", 200.0));
}

TEST_F(PartyEquipmentTest, CalculatePowerConsumptionSoundSystem) {
    double consumption = soundSystem->calculatePowerConsumption(5);
    EXPECT_NEAR(consumption, 4.95, 0.001);
}

TEST_F(PartyEquipmentTest, CalculatePowerConsumptionNoAccessories) {
    auto generic = std::make_shared<PartyEquipment>("P004", "Lighting", "LIGHTING", 50);
    double consumption = generic->calculatePowerConsumption(10);
    EXPECT_NEAR(consumption, 8.0999999999999996, 0.001);
}

TEST_F(PartyEquipmentTest, GetRecommendedGuestCountSoundSystemClamped) {
    EXPECT_EQ(soundSystem->getRecommendedGuestCount(), 100);
}

TEST_F(PartyEquipmentTest, GetRecommendedGuestCountTent) {
    EXPECT_EQ(tent->getRecommendedGuestCount(), 400);
}

TEST_F(PartyEquipmentTest, CalculateDamageRiskFestival) {
    double risk = soundSystem->calculateDamageRisk("FESTIVAL");
    EXPECT_NEAR(risk, 0.225, 0.001);
}

TEST_F(PartyEquipmentTest, CalculateDamageRiskWedding) {
    double risk = soundSystem->calculateDamageRisk("WEDDING");
    EXPECT_NEAR(risk, 0.09, 0.001);
}

TEST_F(PartyEquipmentTest, CalculateDamageRiskDefault) {
    double risk = soundSystem->calculateDamageRisk("OTHER");
    EXPECT_NEAR(risk, 0.1125, 0.001);
}

TEST_F(PartyEquipmentTest, RequiresLicensedOperatorReturnsTrue) {
    EXPECT_TRUE(soundSystem->requiresLicensedOperator());
    
    auto powerful = std::make_shared<PartyEquipment>("P005", "Powerful", "SPECIAL_EFFECTS", 50);
    EXPECT_TRUE(powerful->requiresLicensedOperator());
}

TEST_F(PartyEquipmentTest, RequiresLicensedOperatorReturnsFalse) {
    EXPECT_FALSE(tablesAndChairs->requiresLicensedOperator());
}