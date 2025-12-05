#include <gtest/gtest.h>
#include "../../include/core/FloorPlan.h"
#include "../../include/core/Property.h"
#include <memory>

class FloorPlanTest : public ::testing::Test {
protected:
    std::shared_ptr<Property> createProperty() {
        return std::make_shared<Property>("P1", "123 Main St", 200.0);
    }
    
    std::unique_ptr<FloorPlan> createBasicFloorPlan() {
        return std::make_unique<FloorPlan>("FP1", createProperty(),
                                          "Main Floor Plan", "plan.dwg",
                                          0.5, "dwg", 1, 150.0, 4, 2,
                                          "North", true, "John Architect");
    }
};

TEST_F(FloorPlanTest, ConstructorEdgeCases) {
    FloorPlan plan1("", nullptr, "", "", -1.0, "", -5, -100.0, 
                   -2, -1, "", false, "");
    
    EXPECT_EQ(plan1.getFloorPlanId(), "");
    EXPECT_EQ(plan1.getProperty(), nullptr);
    EXPECT_EQ(plan1.getPlanName(), "");
    EXPECT_EQ(plan1.getFilePath(), "");
    EXPECT_DOUBLE_EQ(plan1.getScale(), -1.0);
    EXPECT_EQ(plan1.getFormat(), "");
    EXPECT_EQ(plan1.getFloorLevel(), -5);
    EXPECT_DOUBLE_EQ(plan1.getTotalArea(), -100.0);
    EXPECT_EQ(plan1.getNumberOfRooms(), -2);
    EXPECT_EQ(plan1.getNumberOfBathrooms(), -1);
    EXPECT_EQ(plan1.getOrientation(), "");
    EXPECT_FALSE(plan1.getHasMeasurements());
    EXPECT_EQ(plan1.getArchitect(), "");
}

TEST_F(FloorPlanTest, CopyConstructor) {
    auto plan1 = createBasicFloorPlan();
    plan1->setPlanName("Updated Name");
    plan1->setTotalArea(200.0);
    plan1->setHasMeasurements(false);
    
    FloorPlan plan2(*plan1);
    
    EXPECT_EQ(plan2.getFloorPlanId(), "FP1");
    EXPECT_NE(plan2.getProperty(), nullptr);
    EXPECT_EQ(plan2.getPlanName(), "Updated Name");
    EXPECT_EQ(plan2.getFilePath(), "plan.dwg");
    EXPECT_DOUBLE_EQ(plan2.getScale(), 0.5);
    EXPECT_EQ(plan2.getFormat(), "dwg");
    EXPECT_EQ(plan2.getFloorLevel(), 1);
    EXPECT_DOUBLE_EQ(plan2.getTotalArea(), 200.0);
    EXPECT_EQ(plan2.getNumberOfRooms(), 4);
    EXPECT_EQ(plan2.getNumberOfBathrooms(), 2);
    EXPECT_EQ(plan2.getOrientation(), "North");
    EXPECT_FALSE(plan2.getHasMeasurements());
    EXPECT_EQ(plan2.getArchitect(), "John Architect");
}

TEST_F(FloorPlanTest, CopyAssignment) {
    auto plan1 = createBasicFloorPlan();
    plan1->setPlanName("Updated Name");
    plan1->setTotalArea(200.0);
    
    auto plan2 = std::make_unique<FloorPlan>("FP2", nullptr, "Old Name", 
                                            "old.pdf", 1.5, "pdf", 2, 100.0,
                                            2, 1, "South", false, "Old Architect");
    
    *plan2 = *plan1;
    
    EXPECT_EQ(plan2->getFloorPlanId(), "FP1");
    EXPECT_NE(plan2->getProperty(), nullptr);
    EXPECT_EQ(plan2->getPlanName(), "Updated Name");
    EXPECT_EQ(plan2->getFilePath(), "plan.dwg");
    EXPECT_DOUBLE_EQ(plan2->getScale(), 0.5);
    EXPECT_EQ(plan2->getFormat(), "dwg");
    EXPECT_EQ(plan2->getFloorLevel(), 1);
    EXPECT_DOUBLE_EQ(plan2->getTotalArea(), 200.0);
    EXPECT_EQ(plan2->getNumberOfRooms(), 4);
    EXPECT_EQ(plan2->getNumberOfBathrooms(), 2);
    EXPECT_EQ(plan2->getOrientation(), "North");
    EXPECT_TRUE(plan2->getHasMeasurements());
    EXPECT_EQ(plan2->getArchitect(), "John Architect");
}

TEST_F(FloorPlanTest, MoveConstructor) {
    auto plan1 = createBasicFloorPlan();
    FloorPlan plan2(std::move(*plan1));
    
    EXPECT_EQ(plan2.getFloorPlanId(), "FP1");
    EXPECT_NE(plan2.getProperty(), nullptr);
    EXPECT_EQ(plan2.getPlanName(), "Main Floor Plan");
    EXPECT_EQ(plan2.getFilePath(), "plan.dwg");
    EXPECT_DOUBLE_EQ(plan2.getScale(), 0.5);
    EXPECT_EQ(plan2.getFormat(), "dwg");
    EXPECT_EQ(plan2.getFloorLevel(), 1);
    EXPECT_DOUBLE_EQ(plan2.getTotalArea(), 150.0);
    EXPECT_EQ(plan2.getNumberOfRooms(), 4);
    EXPECT_EQ(plan2.getNumberOfBathrooms(), 2);
    EXPECT_EQ(plan2.getOrientation(), "North");
    EXPECT_TRUE(plan2.getHasMeasurements());
    EXPECT_EQ(plan2.getArchitect(), "John Architect");
}

TEST_F(FloorPlanTest, MoveAssignment) {
    auto plan1 = createBasicFloorPlan();
    auto plan2 = std::make_unique<FloorPlan>("FP2", nullptr, "Old Name", 
                                            "old.pdf", 1.5, "pdf", 2, 100.0,
                                            2, 1, "South", false, "Old Architect");
    
    *plan2 = std::move(*plan1);
    
    EXPECT_EQ(plan2->getFloorPlanId(), "FP1");
    EXPECT_NE(plan2->getProperty(), nullptr);
    EXPECT_EQ(plan2->getPlanName(), "Main Floor Plan");
    EXPECT_EQ(plan2->getFilePath(), "plan.dwg");
    EXPECT_DOUBLE_EQ(plan2->getScale(), 0.5);
    EXPECT_EQ(plan2->getFormat(), "dwg");
    EXPECT_EQ(plan2->getFloorLevel(), 1);
    EXPECT_DOUBLE_EQ(plan2->getTotalArea(), 150.0);
    EXPECT_EQ(plan2->getNumberOfRooms(), 4);
    EXPECT_EQ(plan2->getNumberOfBathrooms(), 2);
    EXPECT_EQ(plan2->getOrientation(), "North");
    EXPECT_TRUE(plan2->getHasMeasurements());
    EXPECT_EQ(plan2->getArchitect(), "John Architect");
}

TEST_F(FloorPlanTest, SetTotalAreaEdgeCases) {
    auto plan = createBasicFloorPlan();
    
    plan->setTotalArea(-50.0);
    EXPECT_DOUBLE_EQ(plan->getTotalArea(), 0.0);
    
    plan->setTotalArea(0.0);
    EXPECT_DOUBLE_EQ(plan->getTotalArea(), 0.0);
    
    plan->setTotalArea(250.0);
    EXPECT_DOUBLE_EQ(plan->getTotalArea(), 250.0);
}

TEST_F(FloorPlanTest, IsToScaleEdgeCases) {
    auto plan1 = createBasicFloorPlan();
    EXPECT_TRUE(plan1->isToScale());
    
    FloorPlan plan2("FP2", createProperty(), "Plan", "file.dwg",
                    0.0, "dwg", 1, 150.0, 4, 2, "North", true, "Architect");
    EXPECT_FALSE(plan2.isToScale());
    
    FloorPlan plan3("FP3", createProperty(), "Plan", "file.dwg",
                    -0.5, "dwg", 1, 150.0, 4, 2, "North", true, "Architect");
    EXPECT_FALSE(plan3.isToScale());
    
    FloorPlan plan4("FP4", createProperty(), "Plan", "file.dwg",
                    1.0, "dwg", 1, 150.0, 4, 2, "North", true, "Architect");
    EXPECT_TRUE(plan4.isToScale());
    
    FloorPlan plan5("FP5", createProperty(), "Plan", "file.dwg",
                    1.5, "dwg", 1, 150.0, 4, 2, "North", true, "Architect");
    EXPECT_FALSE(plan5.isToScale());
}

TEST_F(FloorPlanTest, CalculateRoomDensityEdgeCases) {
    FloorPlan plan1("FP1", createProperty(), "Plan", "file.dwg",
                    0.5, "dwg", 1, 0.0, 4, 2, "North", true, "Architect");
    
    double density1 = plan1.calculateRoomDensity();
    EXPECT_DOUBLE_EQ(density1, 0.0);
    
    FloorPlan plan2("FP2", createProperty(), "Plan", "file.dwg",
                    0.5, "dwg", 1, -100.0, 4, 2, "North", true, "Architect");
    
    double density2 = plan2.calculateRoomDensity();
    EXPECT_DOUBLE_EQ(density2, 0.0);
    
    FloorPlan plan3("FP3", createProperty(), "Plan", "file.dwg",
                    0.5, "dwg", 1, 100.0, 0, 2, "North", true, "Architect");
    
    double density3 = plan3.calculateRoomDensity();
    EXPECT_DOUBLE_EQ(density3, 0.0);
    
    FloorPlan plan4("FP4", createProperty(), "Plan", "file.dwg",
                    0.5, "dwg", 1, 200.0, 10, 2, "North", true, "Architect");
    
    double density4 = plan4.calculateRoomDensity();
    EXPECT_DOUBLE_EQ(density4, 10.0 / 200.0);
}

TEST_F(FloorPlanTest, IsDetailedEdgeCases) {
    auto plan1 = createBasicFloorPlan();
    EXPECT_TRUE(plan1->isDetailed());
    
    FloorPlan plan2("FP2", createProperty(), "Plan", "file.dwg",
                    0.5, "dwg", 1, 150.0, 4, 2, "North", false, "Architect");
    EXPECT_FALSE(plan2.isDetailed());
    
    FloorPlan plan3("FP3", createProperty(), "Plan", "file.dwg",
                    0.0, "dwg", 1, 150.0, 4, 2, "North", true, "Architect");
    EXPECT_FALSE(plan3.isDetailed());
    
    FloorPlan plan4("FP4", createProperty(), "Plan", "file.dwg",
                    0.5, "dwg", 1, 150.0, 4, 2, "North", true, "");
    EXPECT_FALSE(plan4.isDetailed());
    
    FloorPlan plan5("FP5", createProperty(), "Plan", "file.dwg",
                    0.5, "dwg", 1, 150.0, 4, 2, "North", false, "");
    EXPECT_FALSE(plan5.isDetailed());
}

TEST_F(FloorPlanTest, CalculateEfficiencyRatioEdgeCases) {
    FloorPlan plan1("FP1", createProperty(), "Plan", "file.dwg",
                    0.5, "dwg", 1, 0.0, 4, 2, "North", true, "Architect");
    
    double ratio1 = plan1.calculateEfficiencyRatio();
    EXPECT_DOUBLE_EQ(ratio1, 0.0);
    
    FloorPlan plan2("FP2", createProperty(), "Plan", "file.dwg",
                    0.5, "dwg", 1, -100.0, 4, 2, "North", true, "Architect");
    
    double ratio2 = plan2.calculateEfficiencyRatio();
    EXPECT_DOUBLE_EQ(ratio2, 0.0);
    
    FloorPlan plan3("FP3", createProperty(), "Plan", "file.dwg",
                    0.5, "dwg", 1, 60.0, 4, 2, "North", true, "Architect");
    
    double ratio3 = plan3.calculateEfficiencyRatio();
    double livingArea3 = 60.0 - (2 * 6.0);
    double expected3 = (livingArea3 / 60.0) * 100.0;
    EXPECT_DOUBLE_EQ(ratio3, expected3);
    
    FloorPlan plan4("FP4", createProperty(), "Plan", "file.dwg",
                    0.5, "dwg", 1, 120.0, 4, 0, "North", true, "Architect");
    
    double ratio4 = plan4.calculateEfficiencyRatio();
    EXPECT_DOUBLE_EQ(ratio4, 100.0);
    
    FloorPlan plan5("FP5", createProperty(), "Plan", "file.dwg",
                    0.5, "dwg", 1, 12.0, 4, 3, "North", true, "Architect");
    
    double ratio5 = plan5.calculateEfficiencyRatio();
    EXPECT_DOUBLE_EQ(ratio5, -50);
}

TEST_F(FloorPlanTest, IsGroundFloorEdgeCases) {
    FloorPlan plan1("FP1", createProperty(), "Plan", "file.dwg",
                    0.5, "dwg", 0, 150.0, 4, 2, "North", true, "Architect");
    EXPECT_TRUE(plan1.isGroundFloor());
    
    FloorPlan plan2("FP2", createProperty(), "Plan", "file.dwg",
                    0.5, "dwg", 1, 150.0, 4, 2, "North", true, "Architect");
    EXPECT_TRUE(plan2.isGroundFloor());
    
    FloorPlan plan3("FP3", createProperty(), "Plan", "file.dwg",
                    0.5, "dwg", -1, 150.0, 4, 2, "North", true, "Architect");
    EXPECT_FALSE(plan3.isGroundFloor());
    
    FloorPlan plan4("FP4", createProperty(), "Plan", "file.dwg",
                    0.5, "dwg", 2, 150.0, 4, 2, "North", true, "Architect");
    EXPECT_FALSE(plan4.isGroundFloor());
}

TEST_F(FloorPlanTest, HasMultipleBathroomsEdgeCases) {
    FloorPlan plan1("FP1", createProperty(), "Plan", "file.dwg",
                    0.5, "dwg", 1, 150.0, 4, 2, "North", true, "Architect");
    EXPECT_TRUE(plan1.hasMultipleBathrooms());
    
    FloorPlan plan2("FP2", createProperty(), "Plan", "file.dwg",
                    0.5, "dwg", 1, 150.0, 4, 1, "North", true, "Architect");
    EXPECT_FALSE(plan2.hasMultipleBathrooms());
    
    FloorPlan plan3("FP3", createProperty(), "Plan", "file.dwg",
                    0.5, "dwg", 1, 150.0, 4, 0, "North", true, "Architect");
    EXPECT_FALSE(plan3.hasMultipleBathrooms());
    
    FloorPlan plan4("FP4", createProperty(), "Plan", "file.dwg",
                    0.5, "dwg", 1, 150.0, 4, -1, "North", true, "Architect");
    EXPECT_FALSE(plan4.hasMultipleBathrooms());
    
    FloorPlan plan5("FP5", createProperty(), "Plan", "file.dwg",
                    0.5, "dwg", 1, 150.0, 4, 5, "North", true, "Architect");
    EXPECT_TRUE(plan5.hasMultipleBathrooms());
}

TEST_F(FloorPlanTest, CalculateAverageRoomSizeEdgeCases) {
    FloorPlan plan1("FP1", createProperty(), "Plan", "file.dwg",
                    0.5, "dwg", 1, 150.0, 0, 2, "North", true, "Architect");
    
    double avg1 = plan1.calculateAverageRoomSize();
    EXPECT_DOUBLE_EQ(avg1, 0.0);
    
    FloorPlan plan2("FP2", createProperty(), "Plan", "file.dwg",
                    0.5, "dwg", 1, 150.0, -2, 2, "North", true, "Architect");
    
    double avg2 = plan2.calculateAverageRoomSize();
    EXPECT_DOUBLE_EQ(avg2, 0.0);
    
    FloorPlan plan3("FP3", createProperty(), "Plan", "file.dwg",
                    0.5, "dwg", 1, 0.0, 4, 2, "North", true, "Architect");
    
    double avg3 = plan3.calculateAverageRoomSize();
    EXPECT_DOUBLE_EQ(avg3, 0.0);
    
    FloorPlan plan4("FP4", createProperty(), "Plan", "file.dwg",
                    0.5, "dwg", 1, -150.0, 4, 2, "North", true, "Architect");
    
    double avg4 = plan4.calculateAverageRoomSize();
    EXPECT_DOUBLE_EQ(avg4, -37.5);
    
    FloorPlan plan5("FP5", createProperty(), "Plan", "file.dwg",
                    0.5, "dwg", 1, 200.0, 5, 2, "North", true, "Architect");
    
    double avg5 = plan5.calculateAverageRoomSize();
    EXPECT_DOUBLE_EQ(avg5, 200.0 / 5.0);
}

TEST_F(FloorPlanTest, IsArchitecturalPlanEdgeCases) {
    FloorPlan plan1("FP1", createProperty(), "Plan", "file.dwg",
                    0.5, "CAD", 1, 150.0, 4, 2, "North", true, "Architect");
    EXPECT_TRUE(plan1.isArchitecturalPlan());
    
    FloorPlan plan2("FP2", createProperty(), "Plan", "file.dwg",
                    0.5, "DWG", 1, 150.0, 4, 2, "North", true, "Architect");
    EXPECT_TRUE(plan2.isArchitecturalPlan());
    
    FloorPlan plan3("FP3", createProperty(), "Plan", "file.dwg",
                    0.5, "vector", 1, 150.0, 4, 2, "North", true, "Architect");
    EXPECT_TRUE(plan3.isArchitecturalPlan());
    
    FloorPlan plan4("FP4", createProperty(), "Plan", "file.dwg",
                    0.5, "pdf", 1, 150.0, 4, 2, "North", true, "Architect");
    EXPECT_FALSE(plan4.isArchitecturalPlan());
    
    FloorPlan plan5("FP5", createProperty(), "Plan", "file.dwg",
                    0.5, "jpg", 1, 150.0, 4, 2, "North", true, "Architect");
    EXPECT_FALSE(plan5.isArchitecturalPlan());
    
    FloorPlan plan6("FP6", createProperty(), "Plan", "file.dwg",
                    0.5, "Cad", 1, 150.0, 4, 2, "North", true, "Architect");
    EXPECT_FALSE(plan6.isArchitecturalPlan());
    
    FloorPlan plan7("FP7", createProperty(), "Plan", "file.dwg",
                    0.5, "", 1, 150.0, 4, 2, "North", true, "Architect");
    EXPECT_FALSE(plan7.isArchitecturalPlan());
}

TEST_F(FloorPlanTest, GetPlanComplexityAllCases) {
    FloorPlan plan1("FP1", createProperty(), "Plan", "file.dwg",
                    0.5, "dwg", 1, 150.0, 9, 2, "North", true, "Architect");
    EXPECT_EQ(plan1.getPlanComplexity(), "complex");
    
    FloorPlan plan2("FP2", createProperty(), "Plan", "file.dwg",
                    0.5, "dwg", 4, 150.0, 4, 2, "North", true, "Architect");
    EXPECT_EQ(plan2.getPlanComplexity(), "complex");
    
    FloorPlan plan3("FP3", createProperty(), "Plan", "file.dwg",
                    0.5, "dwg", 2, 150.0, 5, 2, "North", true, "Architect");
    EXPECT_EQ(plan3.getPlanComplexity(), "moderate");
    
    FloorPlan plan4("FP4", createProperty(), "Plan", "file.dwg",
                    0.5, "dwg", 3, 150.0, 4, 2, "North", true, "Architect");
    EXPECT_EQ(plan4.getPlanComplexity(), "moderate");
    
    FloorPlan plan5("FP5", createProperty(), "Plan", "file.dwg",
                    0.5, "dwg", 1, 150.0, 4, 2, "North", true, "Architect");
    EXPECT_EQ(plan5.getPlanComplexity(), "simple");
    
    FloorPlan plan6("FP6", createProperty(), "Plan", "file.dwg",
                    0.5, "dwg", 0, 150.0, 4, 2, "North", true, "Architect");
    EXPECT_EQ(plan6.getPlanComplexity(), "simple");
    
    FloorPlan plan7("FP7", createProperty(), "Plan", "file.dwg",
                    0.5, "dwg", 2, 150.0, 3, 2, "North", true, "Architect");
    EXPECT_EQ(plan7.getPlanComplexity(), "moderate");
    
    FloorPlan plan8("FP8", createProperty(), "Plan", "file.dwg",
                    0.5, "dwg", 1, 150.0, 8, 2, "North", true, "Architect");
    EXPECT_EQ(plan8.getPlanComplexity(), "moderate");
    
    FloorPlan plan9("FP9", createProperty(), "Plan", "file.dwg",
                    0.5, "dwg", 1, 150.0, 5, 2, "North", true, "Architect");
    EXPECT_EQ(plan9.getPlanComplexity(), "moderate");
}

TEST_F(FloorPlanTest, GetPlanComplexityEdgeCases) {
    FloorPlan plan1("FP1", createProperty(), "Plan", "file.dwg",
                    0.5, "dwg", -1, 150.0, 0, 2, "North", true, "Architect");
    EXPECT_EQ(plan1.getPlanComplexity(), "moderate");
    
    FloorPlan plan2("FP2", createProperty(), "Plan", "file.dwg",
                    0.5, "dwg", 1, 150.0, -5, 2, "North", true, "Architect");
    EXPECT_EQ(plan2.getPlanComplexity(), "simple");
}