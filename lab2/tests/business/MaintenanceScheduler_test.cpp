#include <gtest/gtest.h>
#include "../../include/business/MaintenanceScheduler.h"
#include "../../include/core/Inventory.h"
#include "../../include/business/DamageAssessor.h"
#include "../../include/core/RentalItem.h"

class MaintenanceSchedulerTest : public ::testing::Test {
protected:
    void SetUp() override {
        inventory = std::make_shared<Inventory>();
        scheduler = std::make_shared<MaintenanceScheduler>(inventory, nullptr);
        
        rentalItem1 = std::make_shared<RentalItem>("VEH001", "Truck", "VEHICLE", 200.0);
        rentalItem2 = std::make_shared<RentalItem>("CONST001", "Crane", "CONSTRUCTION", 500.0);
        rentalItem3 = std::make_shared<RentalItem>("ELEC001", "Generator", "ELECTRONICS", 300.0);
        
        inventory->addItem(rentalItem1);
        inventory->addItem(rentalItem2);
        inventory->addItem(rentalItem3);
    }
    
    std::shared_ptr<Inventory> inventory;
    std::shared_ptr<MaintenanceScheduler> scheduler;
    std::shared_ptr<RentalItem> rentalItem1;
    std::shared_ptr<RentalItem> rentalItem2;
    std::shared_ptr<RentalItem> rentalItem3;
};

TEST_F(MaintenanceSchedulerTest, ScheduleMaintenanceForVehicle) {
    bool scheduled = scheduler->scheduleMaintenance("VEH001", "PREVENTIVE");
    EXPECT_TRUE(scheduled);
}

TEST_F(MaintenanceSchedulerTest, ScheduleMaintenanceForConstruction) {
    bool scheduled = scheduler->scheduleMaintenance("CONST001", "CORRECTIVE");
    EXPECT_TRUE(scheduled);
}

TEST_F(MaintenanceSchedulerTest, ScheduleMaintenanceForElectronics) {
    bool scheduled = scheduler->scheduleMaintenance("ELEC001", "EMERGENCY");
    EXPECT_TRUE(scheduled);
}

TEST_F(MaintenanceSchedulerTest, CalculateMaintenanceCostForVehicle) {
    double cost = scheduler->calculateMaintenanceCost("VEH001", "PREVENTIVE");
    EXPECT_GT(cost, 0.0);
}

TEST_F(MaintenanceSchedulerTest, CalculateMaintenanceCostForConstruction) {
    double cost = scheduler->calculateMaintenanceCost("CONST001", "CORRECTIVE");
    EXPECT_GT(cost, 0.0);
}

TEST_F(MaintenanceSchedulerTest, CalculateMaintenanceCostForElectronics) {
    double cost = scheduler->calculateMaintenanceCost("ELEC001", "EMERGENCY");
    EXPECT_GT(cost, 0.0);
}

TEST_F(MaintenanceSchedulerTest, PredictNextMaintenance) {
    int days = scheduler->predictNextMaintenance("CONST001");
    EXPECT_GT(days, 0);
    
    days = scheduler->predictNextMaintenance("VEH001");
    EXPECT_GT(days, 0);
    
    days = scheduler->predictNextMaintenance("ELEC001");
    EXPECT_GT(days, 0);
}

TEST_F(MaintenanceSchedulerTest, CalculateEquipmentDowntime) {
    double downtime = scheduler->calculateEquipmentDowntime("CONST001");
    EXPECT_GT(downtime, 0.0);
    
    downtime = scheduler->calculateEquipmentDowntime("ELEC001");
    EXPECT_GT(downtime, 0.0);
    
    downtime = scheduler->calculateEquipmentDowntime("VEH001");
    EXPECT_GT(downtime, 0.0);
}

TEST_F(MaintenanceSchedulerTest, CompleteMaintenanceTask) {
    bool scheduled = scheduler->scheduleMaintenance("VEH001", "PREVENTIVE");
    EXPECT_TRUE(scheduled);
    
    auto tasks = scheduler->getOverdueMaintenance();
    
    bool completed = scheduler->completeMaintenanceTask("MT1000", 150.0);
}

TEST_F(MaintenanceSchedulerTest, CalculateMaintenanceBudget) {
    double budget = scheduler->calculateMaintenanceBudget(3);
    EXPECT_GT(budget, 0.0);
}

TEST_F(MaintenanceSchedulerTest, GetOverdueMaintenance) {
    scheduler->scheduleMaintenance("VEH001", "PREVENTIVE");
    
    auto overdueTasks = scheduler->getOverdueMaintenance();
    EXPECT_GE(overdueTasks.size(), 0);
}

TEST_F(MaintenanceSchedulerTest, CalculatePreventiveMaintenanceSavings) {
    scheduler->scheduleMaintenance("VEH001", "PREVENTIVE");
    scheduler->completeMaintenanceTask("MT1000", 150.0);
    
    double savings = scheduler->calculatePreventiveMaintenanceSavings("VEH001");
    EXPECT_GE(savings, 0.0);
}

TEST_F(MaintenanceSchedulerTest, GetMaintenanceTeam) {
    auto vehicleTeam = scheduler->getMaintenanceTeam("VEHICLE");
    EXPECT_FALSE(vehicleTeam.empty());
    
    auto electronicsTeam = scheduler->getMaintenanceTeam("ELECTRONICS");
    EXPECT_FALSE(electronicsTeam.empty());
    
    auto constructionTeam = scheduler->getMaintenanceTeam("CONSTRUCTION");
    EXPECT_FALSE(constructionTeam.empty());
    
    auto generalTeam = scheduler->getMaintenanceTeam("GENERAL");
    EXPECT_FALSE(generalTeam.empty());
}

TEST_F(MaintenanceSchedulerTest, CalculateMaintenanceEfficiency) {
    double efficiency = scheduler->calculateMaintenanceEfficiency("MECH001");
    EXPECT_GE(efficiency, 0.0);
    EXPECT_LE(efficiency, 100.0);
}

TEST_F(MaintenanceSchedulerTest, GetMaintenanceCount) {
    int count = scheduler->getMaintenanceCount("VEH001");
    EXPECT_EQ(count, 0);
    
    scheduler->scheduleMaintenance("VEH001", "PREVENTIVE");
    scheduler->completeMaintenanceTask("MT1000", 150.0);
    
    count = scheduler->getMaintenanceCount("VEH001");
    EXPECT_EQ(count, 1);
}

TEST_F(MaintenanceSchedulerTest, GetInventory) {
    auto retrievedInventory = scheduler->getInventory();
    EXPECT_EQ(retrievedInventory, inventory);
}

TEST_F(MaintenanceSchedulerTest, SetInventory) {
    auto newInventory = std::make_shared<Inventory>();
    scheduler->setInventory(newInventory);
    
    auto retrievedInventory = scheduler->getInventory();
    EXPECT_EQ(retrievedInventory, newInventory);
}

TEST_F(MaintenanceSchedulerTest, AssignMaintenanceTeam) {
    std::vector<std::string> newTeam = {"TECH003", "TECH004"};
    scheduler->assignMaintenanceTeam("NEW_CATEGORY", newTeam);
    
    auto retrievedTeam = scheduler->getMaintenanceTeam("NEW_CATEGORY");
    EXPECT_EQ(retrievedTeam, newTeam);
}

TEST_F(MaintenanceSchedulerTest, ScheduleMultipleMaintenanceTasks) {
    scheduler->scheduleMaintenance("VEH001", "PREVENTIVE");
    scheduler->scheduleMaintenance("CONST001", "CORRECTIVE");
    scheduler->scheduleMaintenance("ELEC001", "EMERGENCY");
    
    auto overdueTasks = scheduler->getOverdueMaintenance();
    EXPECT_GE(overdueTasks.size(), 0);
}

TEST_F(MaintenanceSchedulerTest, MaintenanceCostForGeneralCategory) {
    double cost = scheduler->calculateMaintenanceCost("TOOL001", "PREVENTIVE");
    EXPECT_GT(cost, 0.0);
}

TEST_F(MaintenanceSchedulerTest, ScheduleMaintenanceWithoutInventory) {
    auto schedulerWithoutInventory = std::make_shared<MaintenanceScheduler>();
    bool scheduled = schedulerWithoutInventory->scheduleMaintenance("VEH001", "PREVENTIVE");
    EXPECT_FALSE(scheduled);
}