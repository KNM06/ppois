#include <gtest/gtest.h>
#include <memory>
#include "../../include/core/Property.h"
#include "../../include/core/Address.h"
#include "../../include/core/PropertyStatus.h"
#include "../../include/core/PropertyType.h"

class PropertyTest : public ::testing::Test {
protected:
    void SetUp() override {
        address = std::make_unique<Address>("123 Main St", "Springfield", "IL", "62701");
        status = std::make_unique<PropertyStatus>("AVAILABLE", "Available for rent", true, false, false,
                                                 std::chrono::system_clock::now(), "", 0.0, 0);
        type = std::make_unique<PropertyType>("APT", "Apartment", "Residential", 2, 100, 800.0, 1200.0, 
                                             false, "urban", 2.5);
    }
    
    std::unique_ptr<Address> address;
    std::unique_ptr<PropertyStatus> status;
    std::unique_ptr<PropertyType> type;
};

TEST_F(PropertyTest, ConstructorAndGetters) {
    Property property("PROP001", 85.5, 1000.0, std::move(address), std::move(status), std::move(type));
    
    EXPECT_EQ(property.getId(), "PROP001");
    EXPECT_DOUBLE_EQ(property.getArea(), 85.5);
    EXPECT_DOUBLE_EQ(property.getRentalPrice(), 1000.0);
    EXPECT_NE(property.getAddress(), nullptr);
    EXPECT_NE(property.getStatus(), nullptr);
    EXPECT_NE(property.getType(), nullptr);
}

TEST_F(PropertyTest, SimpleConstructor) {
    Property property("PROP001", "123 Main St, Springfield, IL", 85.5);
    
    EXPECT_EQ(property.getId(), "PROP001");
    EXPECT_DOUBLE_EQ(property.getArea(), 85.5);
    EXPECT_NE(property.getAddress(), nullptr);
    EXPECT_NE(property.getStatus(), nullptr);
    EXPECT_NE(property.getType(), nullptr);
}

TEST_F(PropertyTest, Setters) {
    Property property("PROP001", 85.5, 1000.0, std::move(address), std::move(status), std::move(type));
    
    property.setRentalPrice(1200.0);
    EXPECT_DOUBLE_EQ(property.getRentalPrice(), 1200.0);
    
    property.updateArea(90.0);
    EXPECT_DOUBLE_EQ(property.getArea(), 90.0);
}

TEST_F(PropertyTest, CalculateRentalYield) {
    Property property("PROP001", 85.5, 1000.0, std::move(address), std::move(status), std::move(type));
    
    EXPECT_DOUBLE_EQ(property.calculateRentalYield(12000.0), 100.0);
    EXPECT_DOUBLE_EQ(property.calculateRentalYield(6000.0), 50.0);
    
    Property zeroPriceProperty("PROP002", 85.5, 0.0, std::make_unique<Address>("456 Elm"), 
                              std::make_unique<PropertyStatus>("AVAILABLE"), 
                              std::make_unique<PropertyType>("HOUSE"));
    EXPECT_DOUBLE_EQ(zeroPriceProperty.calculateRentalYield(12000.0), 0.0);
}

TEST_F(PropertyTest, IsAvailableForRent) {
    auto availableStatus = std::make_unique<PropertyStatus>("AVAILABLE", "Available", true, false, false,
                                                          std::chrono::system_clock::now(), "", 0.0, 0);
    auto rentedStatus = std::make_unique<PropertyStatus>("RENTED", "Rented", false, false, false,
                                                        std::chrono::system_clock::now(), "", 0.0, 0);
    
    Property availableProperty("PROP001", 85.5, 1000.0, std::move(address), std::move(availableStatus), std::move(type));
    EXPECT_TRUE(availableProperty.isAvailableForRent());
    
    Property rentedProperty("PROP002", 85.5, 1000.0, std::make_unique<Address>("456 Elm"), 
                          std::move(rentedStatus), std::make_unique<PropertyType>("HOUSE"));
    EXPECT_FALSE(rentedProperty.isAvailableForRent());
    
    Property nullStatusProperty("PROP003", 85.5, 1000.0, std::make_unique<Address>("789 Oak"), 
                              nullptr, std::make_unique<PropertyType>("HOUSE"));
    EXPECT_FALSE(nullStatusProperty.isAvailableForRent());
}

TEST_F(PropertyTest, CalculatePricePerSqMeter) {
    Property property("PROP001", 100.0, 1000.0, std::move(address), std::move(status), std::move(type));
    
    EXPECT_DOUBLE_EQ(property.calculatePricePerSqMeter(), 10.0);
    
    Property zeroAreaProperty("PROP002", 0.0, 1000.0, std::make_unique<Address>("456 Elm"), 
                             std::make_unique<PropertyStatus>("AVAILABLE"), 
                             std::make_unique<PropertyType>("HOUSE"));
    EXPECT_DOUBLE_EQ(zeroAreaProperty.calculatePricePerSqMeter(), 0.0);
    
    Property negativeAreaProperty("PROP003", -100.0, 1000.0, std::make_unique<Address>("789 Oak"), 
                                 std::make_unique<PropertyStatus>("AVAILABLE"), 
                                 std::make_unique<PropertyType>("HOUSE"));
    EXPECT_DOUBLE_EQ(negativeAreaProperty.calculatePricePerSqMeter(), 0.0);
}

TEST_F(PropertyTest, MeetsAreaRequirements) {
    Property property("PROP001", 100.0, 1000.0, std::move(address), std::move(status), std::move(type));
    
    EXPECT_TRUE(property.meetsAreaRequirements(50.0, 150.0));
    EXPECT_TRUE(property.meetsAreaRequirements(100.0, 100.0));
    EXPECT_FALSE(property.meetsAreaRequirements(150.0, 200.0));
    EXPECT_FALSE(property.meetsAreaRequirements(50.0, 90.0));
    EXPECT_TRUE(property.meetsAreaRequirements(90.0, 110.0));
}

TEST_F(PropertyTest, CopyConstructor) {
    Property original("PROP001", 85.5, 1000.0, std::move(address), std::move(status), std::move(type));
    original.setRentalPrice(1200.0);
    
    Property copy(original);
    
    EXPECT_EQ(copy.getId(), "PROP001");
    EXPECT_DOUBLE_EQ(copy.getArea(), 85.5);
    EXPECT_DOUBLE_EQ(copy.getRentalPrice(), 1200.0);
    EXPECT_NE(copy.getAddress(), nullptr);
    EXPECT_NE(copy.getStatus(), nullptr);
    EXPECT_NE(copy.getType(), nullptr);
}

TEST_F(PropertyTest, CopyAssignment) {
    Property original("PROP001", 85.5, 1000.0, std::move(address), std::move(status), std::move(type));
    Property copy("PROP002", 50.0, 500.0, std::make_unique<Address>("456 Elm"), 
                 std::make_unique<PropertyStatus>("RENTED"), 
                 std::make_unique<PropertyType>("HOUSE"));
    
    copy = original;
    
    EXPECT_EQ(copy.getId(), "PROP001");
    EXPECT_DOUBLE_EQ(copy.getArea(), 85.5);
    EXPECT_DOUBLE_EQ(copy.getRentalPrice(), 1000.0);
    EXPECT_NE(copy.getAddress(), nullptr);
    EXPECT_NE(copy.getStatus(), nullptr);
    EXPECT_NE(copy.getType(), nullptr);
}

TEST_F(PropertyTest, MoveConstructor) {
    Property original("PROP001", 85.5, 1000.0, std::move(address), std::move(status), std::move(type));
    
    Property moved(std::move(original));
    
    EXPECT_EQ(moved.getId(), "PROP001");
    EXPECT_DOUBLE_EQ(moved.getArea(), 85.5);
    EXPECT_DOUBLE_EQ(moved.getRentalPrice(), 1000.0);
    EXPECT_NE(moved.getAddress(), nullptr);
    EXPECT_NE(moved.getStatus(), nullptr);
    EXPECT_NE(moved.getType(), nullptr);
}

TEST_F(PropertyTest, MoveAssignment) {
    Property original("PROP001", 85.5, 1000.0, std::move(address), std::move(status), std::move(type));
    Property moved("PROP002", 50.0, 500.0, std::make_unique<Address>("456 Elm"), 
                  std::make_unique<PropertyStatus>("RENTED"), 
                  std::make_unique<PropertyType>("HOUSE"));
    
    moved = std::move(original);
    
    EXPECT_EQ(moved.getId(), "PROP001");
    EXPECT_DOUBLE_EQ(moved.getArea(), 85.5);
    EXPECT_DOUBLE_EQ(moved.getRentalPrice(), 1000.0);
    EXPECT_NE(moved.getAddress(), nullptr);
    EXPECT_NE(moved.getStatus(), nullptr);
    EXPECT_NE(moved.getType(), nullptr);
}