#include <gtest/gtest.h>
#include <memory>
#include <chrono>
#include "../../include/business/OwnerManager.h"
#include "../../include/core/Owner.h"
#include "../../include/core/Property.h"
#include "../../include/core/Address.h"
#include "../../include/finance/Payment.h"
#include "../../include/core/PropertyStatus.h"
#include "../../include/core/PropertyType.h"

class OwnerManagerTest : public ::testing::Test {
protected:
    void SetUp() override {
        owner1 = std::make_shared<Owner>("OWN001", "John", "Doe", "john@example.com", 
            "1234567890", std::make_unique<Address>("123 Main St", "City", "State", "12345"),
            "TAX001", 60.0, true, "bank_transfer");
        
        owner2 = std::make_shared<Owner>("OWN002", "Jane", "Smith", "jane@example.com", 
            "0987654321", std::make_unique<Address>("456 Oak Ave", "City", "State", "67890"),
            "TAX002", 40.0, false, "paypal");
        
        owner3 = std::make_shared<Owner>("OWN003", "Bob", "Johnson", "bob@example.com", 
            "5555555555", std::make_unique<Address>("789 Pine Rd", "City", "State", "13579"),
            "TAX003", 100.0, true, "direct_deposit");
        
        property1 = std::make_shared<Property>("PROP001", 120.0, 1500.0,
            std::make_unique<Address>("100 Prop St", "City", "State", "11111"),
            std::make_unique<PropertyStatus>("AVAILABLE"),
            std::make_unique<PropertyType>("Apartment"));
        
        property2 = std::make_shared<Property>("PROP002", 200.0, 2500.0,
            std::make_unique<Address>("200 Prop St", "City", "State", "22222"),
            std::make_unique<PropertyStatus>("RENTED"),
            std::make_unique<PropertyType>("House"));
        
        property3 = std::make_shared<Property>("PROP003", 180.0, 2000.0,
            std::make_unique<Address>("300 Prop St", "City", "State", "33333"),
            std::make_unique<PropertyStatus>("AVAILABLE"),
            std::make_unique<PropertyType>("Condo"));
        
        property4 = std::make_shared<Property>("PROP004", 150.0, 1800.0,
            std::make_unique<Address>("400 Prop St", "City", "State", "44444"),
            std::make_unique<PropertyStatus>("MAINTENANCE"),
            std::make_unique<PropertyType>("Townhouse"));
        
        auto now = std::chrono::system_clock::now();
        
        payment1 = std::make_shared<Payment>("PAY001", nullptr, nullptr, 1000.0, now,
            "bank_transfer", "completed", "TX001", 50.0, "rent");
        
        payment2 = std::make_shared<Payment>("PAY002", nullptr, nullptr, 500.0, now,
            "paypal", "pending", "TX002", 0.0, "deposit");
        
        payment3 = std::make_shared<Payment>("PAY003", nullptr, nullptr, 1500.0, now,
            "direct_deposit", "completed", "TX003", 75.0, "rent");
        
        payment4 = std::make_shared<Payment>("PAY004", nullptr, nullptr, 200.0, now,
            "bank_transfer", "failed", "TX004", 10.0, "fee");
    }
    
    std::shared_ptr<Owner> owner1;
    std::shared_ptr<Owner> owner2;
    std::shared_ptr<Owner> owner3;
    std::shared_ptr<Property> property1;
    std::shared_ptr<Property> property2;
    std::shared_ptr<Property> property3;
    std::shared_ptr<Property> property4;
    std::shared_ptr<Payment> payment1;
    std::shared_ptr<Payment> payment2;
    std::shared_ptr<Payment> payment3;
    std::shared_ptr<Payment> payment4;
};

TEST_F(OwnerManagerTest, ConstructorAndGetters) {
    OwnerManager manager(12.5, 45);
    
    EXPECT_DOUBLE_EQ(manager.getManagementFeePercentage(), 12.5);
    EXPECT_EQ(manager.getPaymentFrequencyDays(), 45);
}

TEST_F(OwnerManagerTest, SetManagementFeePercentage) {
    OwnerManager manager(10.0, 30);
    
    manager.setManagementFeePercentage(15.0);
    EXPECT_DOUBLE_EQ(manager.getManagementFeePercentage(), 15.0);
    
    manager.setManagementFeePercentage(0.0);
    EXPECT_DOUBLE_EQ(manager.getManagementFeePercentage(), 0.0);
    
    manager.setManagementFeePercentage(100.0);
    EXPECT_DOUBLE_EQ(manager.getManagementFeePercentage(), 100.0);
    
    manager.setManagementFeePercentage(-10.0);
    EXPECT_DOUBLE_EQ(manager.getManagementFeePercentage(), 0.0);
    
    manager.setManagementFeePercentage(150.0);
    EXPECT_DOUBLE_EQ(manager.getManagementFeePercentage(), 100.0);
    
    manager.setManagementFeePercentage(25.5);
    EXPECT_DOUBLE_EQ(manager.getManagementFeePercentage(), 25.5);
}

TEST_F(OwnerManagerTest, SetPaymentFrequencyDays) {
    OwnerManager manager(10.0, 30);
    
    manager.setPaymentFrequencyDays(60);
    EXPECT_EQ(manager.getPaymentFrequencyDays(), 60);
    
    manager.setPaymentFrequencyDays(7);
    EXPECT_EQ(manager.getPaymentFrequencyDays(), 7);
    
    manager.setPaymentFrequencyDays(365);
    EXPECT_EQ(manager.getPaymentFrequencyDays(), 365);
    
    manager.setPaymentFrequencyDays(6);
    EXPECT_EQ(manager.getPaymentFrequencyDays(), 7);
    
    manager.setPaymentFrequencyDays(0);
    EXPECT_EQ(manager.getPaymentFrequencyDays(), 7);
    
    manager.setPaymentFrequencyDays(-10);
    EXPECT_EQ(manager.getPaymentFrequencyDays(), 7);
}

TEST_F(OwnerManagerTest, RegisterOwner) {
    OwnerManager manager(10.0, 30);
    
    EXPECT_TRUE(manager.registerOwner(owner1));
    EXPECT_TRUE(manager.registerOwner(owner2));
    EXPECT_TRUE(manager.registerOwner(owner3));
    
    EXPECT_FALSE(manager.registerOwner(nullptr));
}

TEST_F(OwnerManagerTest, RegisterDuplicateOwner) {
    OwnerManager manager(10.0, 30);
    
    EXPECT_TRUE(manager.registerOwner(owner1));
    EXPECT_TRUE(manager.registerOwner(owner1));
}

TEST_F(OwnerManagerTest, AssignPropertyToOwner) {
    OwnerManager manager(10.0, 30);
    
    manager.registerOwner(owner1);
    
    EXPECT_TRUE(manager.assignPropertyToOwner(owner1, property1));
    EXPECT_TRUE(manager.assignPropertyToOwner(owner1, property2));
    EXPECT_TRUE(manager.assignPropertyToOwner(owner1, property3));
    
    EXPECT_FALSE(manager.assignPropertyToOwner(nullptr, property1));
    EXPECT_FALSE(manager.assignPropertyToOwner(owner1, nullptr));
    EXPECT_TRUE(manager.assignPropertyToOwner(owner2, property1));
}

TEST_F(OwnerManagerTest, CalculateOwnerPayment) {
    OwnerManager manager(10.0, 30);
    
    EXPECT_DOUBLE_EQ(manager.calculateOwnerPayment(nullptr, 1000.0), 0.0);
    
    EXPECT_DOUBLE_EQ(manager.calculateOwnerPayment(owner1, 1000.0), 540.0);
    EXPECT_DOUBLE_EQ(manager.calculateOwnerPayment(owner2, 1000.0), 360.0);
    EXPECT_DOUBLE_EQ(manager.calculateOwnerPayment(owner3, 1000.0), 900.0);
    
    manager.setManagementFeePercentage(20.0);
    EXPECT_DOUBLE_EQ(manager.calculateOwnerPayment(owner1, 1000.0), 480.0);
    
    EXPECT_DOUBLE_EQ(manager.calculateOwnerPayment(owner1, 0.0), 0.0);
    EXPECT_DOUBLE_EQ(manager.calculateOwnerPayment(owner1, -1000.0), -480.0);
}

TEST_F(OwnerManagerTest, GetPropertiesByOwner) {
    OwnerManager manager(10.0, 30);
    
    manager.registerOwner(owner1);
    manager.registerOwner(owner2);
    
    manager.assignPropertyToOwner(owner1, property1);
    manager.assignPropertyToOwner(owner1, property2);
    manager.assignPropertyToOwner(owner2, property3);
    
    auto owner1Props = manager.getPropertiesByOwner("OWN001");
    EXPECT_EQ(owner1Props.size(), 2);
    EXPECT_EQ(owner1Props[0]->getId(), "PROP001");
    EXPECT_EQ(owner1Props[1]->getId(), "PROP002");
    
    auto owner2Props = manager.getPropertiesByOwner("OWN002");
    EXPECT_EQ(owner2Props.size(), 1);
    EXPECT_EQ(owner2Props[0]->getId(), "PROP003");
    
    auto nonExistentProps = manager.getPropertiesByOwner("NON_EXISTENT");
    EXPECT_TRUE(nonExistentProps.empty());
}

TEST_F(OwnerManagerTest, GetPaymentHistory) {
    OwnerManager manager(10.0, 30);
    
    EXPECT_TRUE(manager.getPaymentHistory("OWN001").empty());
    
    manager.registerOwner(owner1);
    EXPECT_TRUE(manager.getPaymentHistory("OWN001").empty());
}

TEST_F(OwnerManagerTest, CalculateTotalRevenueForOwner) {
    OwnerManager manager(10.0, 30);
    
    EXPECT_DOUBLE_EQ(manager.calculateTotalRevenueForOwner("OWN001"), 0.0);
    EXPECT_DOUBLE_EQ(manager.calculateTotalRevenueForOwner("NON_EXISTENT"), 0.0);
}

TEST_F(OwnerManagerTest, IsOwnerActive) {
    OwnerManager manager(10.0, 30);
    
    manager.registerOwner(owner1);
    manager.registerOwner(owner2);
    
    EXPECT_FALSE(manager.isOwnerActive("OWN001"));
    EXPECT_FALSE(manager.isOwnerActive("OWN002"));
    EXPECT_FALSE(manager.isOwnerActive("NON_EXISTENT"));
    
    manager.assignPropertyToOwner(owner1, property1);
    EXPECT_TRUE(manager.isOwnerActive("OWN001"));
    EXPECT_FALSE(manager.isOwnerActive("OWN002"));
    
    manager.assignPropertyToOwner(owner2, property2);
    EXPECT_TRUE(manager.isOwnerActive("OWN001"));
    EXPECT_TRUE(manager.isOwnerActive("OWN002"));
    
    manager.assignPropertyToOwner(owner1, property3);
    EXPECT_TRUE(manager.isOwnerActive("OWN001"));
}

TEST_F(OwnerManagerTest, CalculateManagementFee) {
    OwnerManager manager(10.0, 30);
    
    EXPECT_DOUBLE_EQ(manager.calculateManagementFee(1000.0), 100.0);
    EXPECT_DOUBLE_EQ(manager.calculateManagementFee(500.0), 50.0);
    EXPECT_DOUBLE_EQ(manager.calculateManagementFee(0.0), 0.0);
    EXPECT_DOUBLE_EQ(manager.calculateManagementFee(-1000.0), -100.0);
    
    manager.setManagementFeePercentage(15.0);
    EXPECT_DOUBLE_EQ(manager.calculateManagementFee(1000.0), 150.0);
    
    manager.setManagementFeePercentage(0.0);
    EXPECT_DOUBLE_EQ(manager.calculateManagementFee(1000.0), 0.0);
    
    manager.setManagementFeePercentage(100.0);
    EXPECT_DOUBLE_EQ(manager.calculateManagementFee(1000.0), 1000.0);
}

TEST_F(OwnerManagerTest, GetOwnersWithMultipleProperties) {
    OwnerManager manager(10.0, 30);
    
    manager.registerOwner(owner1);
    manager.registerOwner(owner2);
    manager.registerOwner(owner3);
    
    manager.assignPropertyToOwner(owner1, property1);
    manager.assignPropertyToOwner(owner1, property2);
    manager.assignPropertyToOwner(owner1, property3);
    
    manager.assignPropertyToOwner(owner2, property4);
    
    auto multiOwners = manager.getOwnersWithMultipleProperties();
    EXPECT_EQ(multiOwners.size(), 1);
    EXPECT_EQ(multiOwners[0]->getOwnerId(), "OWN001");
    
    manager.assignPropertyToOwner(owner2, property1);
    multiOwners = manager.getOwnersWithMultipleProperties();
    EXPECT_EQ(multiOwners.size(), 2);
    
    bool foundOwner1 = false;
    bool foundOwner2 = false;
    for (const auto& owner : multiOwners) {
        if (owner->getOwnerId() == "OWN001") foundOwner1 = true;
        if (owner->getOwnerId() == "OWN002") foundOwner2 = true;
    }
    EXPECT_TRUE(foundOwner1);
    EXPECT_TRUE(foundOwner2);
}

TEST_F(OwnerManagerTest, GetOwnersWithMultiplePropertiesNone) {
    OwnerManager manager(10.0, 30);
    
    manager.registerOwner(owner1);
    manager.registerOwner(owner2);
    
    manager.assignPropertyToOwner(owner1, property1);
    manager.assignPropertyToOwner(owner2, property2);
    
    auto multiOwners = manager.getOwnersWithMultipleProperties();
    EXPECT_TRUE(multiOwners.empty());
}

TEST_F(OwnerManagerTest, GetOwnersWithMultiplePropertiesEmptyManager) {
    OwnerManager manager(10.0, 30);
    
    auto multiOwners = manager.getOwnersWithMultipleProperties();
    EXPECT_TRUE(multiOwners.empty());
}

TEST_F(OwnerManagerTest, CanOwnerAddMoreProperties) {
    OwnerManager manager(10.0, 30);
    
    manager.registerOwner(owner1);
    
    EXPECT_TRUE(manager.canOwnerAddMoreProperties("OWN001"));
    EXPECT_TRUE(manager.canOwnerAddMoreProperties("NON_EXISTENT"));
    
    for (int i = 0; i < 9; ++i) {
        auto prop = std::make_shared<Property>("PROP" + std::to_string(i+10), 
            100.0, 1000.0, nullptr, nullptr, nullptr);
        manager.assignPropertyToOwner(owner1, prop);
    }
    
    EXPECT_TRUE(manager.canOwnerAddMoreProperties("OWN001"));
    
    auto prop10 = std::make_shared<Property>("PROP20", 100.0, 1000.0, nullptr, nullptr, nullptr);
    manager.assignPropertyToOwner(owner1, prop10);
    
    EXPECT_FALSE(manager.canOwnerAddMoreProperties("OWN001"));
    
    manager.registerOwner(owner2);
    EXPECT_TRUE(manager.canOwnerAddMoreProperties("OWN002"));
}

TEST_F(OwnerManagerTest, CanOwnerAddMorePropertiesNoProperties) {
    OwnerManager manager(10.0, 30);
    
    manager.registerOwner(owner1);
    
    EXPECT_TRUE(manager.canOwnerAddMoreProperties("OWN001"));
    
    auto prop1 = std::make_shared<Property>("PROP1", 100.0, 1000.0, nullptr, nullptr, nullptr);
    manager.assignPropertyToOwner(owner1, prop1);
    
    EXPECT_TRUE(manager.canOwnerAddMoreProperties("OWN001"));
}

TEST_F(OwnerManagerTest, CalculateOwnerPortfolioValue) {
    OwnerManager manager(10.0, 30);
    
    manager.registerOwner(owner1);
    
    EXPECT_DOUBLE_EQ(manager.calculateOwnerPortfolioValue("OWN001"), 0.0);
    EXPECT_DOUBLE_EQ(manager.calculateOwnerPortfolioValue("NON_EXISTENT"), 0.0);
    
    manager.assignPropertyToOwner(owner1, property1);
    EXPECT_DOUBLE_EQ(manager.calculateOwnerPortfolioValue("OWN001"), 1500.0 * 100);
    
    manager.assignPropertyToOwner(owner1, property2);
    EXPECT_DOUBLE_EQ(manager.calculateOwnerPortfolioValue("OWN001"), (1500.0 + 2500.0) * 100);
    
    manager.assignPropertyToOwner(owner1, property3);
    EXPECT_DOUBLE_EQ(manager.calculateOwnerPortfolioValue("OWN001"), (1500.0 + 2500.0 + 2000.0) * 100);
    
    manager.assignPropertyToOwner(owner1, property4);
    EXPECT_DOUBLE_EQ(manager.calculateOwnerPortfolioValue("OWN001"), (1500.0 + 2500.0 + 2000.0 + 1800.0) * 100);
}

TEST_F(OwnerManagerTest, CalculateOwnerPortfolioValueWithZeroRentalPrice) {
    OwnerManager manager(10.0, 30);
    
    manager.registerOwner(owner1);
    
    auto zeroPriceProperty = std::make_shared<Property>("PROP0", 100.0, 0.0, 
        nullptr, nullptr, nullptr);
    
    manager.assignPropertyToOwner(owner1, zeroPriceProperty);
    EXPECT_DOUBLE_EQ(manager.calculateOwnerPortfolioValue("OWN001"), 0.0);
    
    auto negativePriceProperty = std::make_shared<Property>("PROPN", 100.0, -500.0, 
        nullptr, nullptr, nullptr);
    
    manager.assignPropertyToOwner(owner1, negativePriceProperty);
    EXPECT_DOUBLE_EQ(manager.calculateOwnerPortfolioValue("OWN001"), -500.0 * 100);
}

TEST_F(OwnerManagerTest, CopyConstructor) {
    OwnerManager original(12.5, 45);
    
    original.registerOwner(owner1);
    original.registerOwner(owner2);
    original.assignPropertyToOwner(owner1, property1);
    original.assignPropertyToOwner(owner1, property2);
    original.assignPropertyToOwner(owner2, property3);
    
    OwnerManager copy(original);
    
    EXPECT_DOUBLE_EQ(copy.getManagementFeePercentage(), 12.5);
    EXPECT_EQ(copy.getPaymentFrequencyDays(), 45);
    
    EXPECT_EQ(copy.getPropertiesByOwner("OWN001").size(), 2);
    EXPECT_EQ(copy.getPropertiesByOwner("OWN002").size(), 1);
}

TEST_F(OwnerManagerTest, CopyAssignment) {
    OwnerManager original(12.5, 45);
    original.registerOwner(owner1);
    original.assignPropertyToOwner(owner1, property1);
    
    OwnerManager assigned(20.0, 60);
    assigned.registerOwner(owner2);
    assigned.assignPropertyToOwner(owner2, property2);
    
    assigned = original;
    
    EXPECT_DOUBLE_EQ(assigned.getManagementFeePercentage(), 12.5);
    EXPECT_EQ(assigned.getPaymentFrequencyDays(), 45);
    
    EXPECT_EQ(assigned.getPropertiesByOwner("OWN001").size(), 1);
    EXPECT_TRUE(assigned.getPropertiesByOwner("OWN002").empty());
}

TEST_F(OwnerManagerTest, MoveConstructor) {
    OwnerManager original(12.5, 45);
    original.registerOwner(owner1);
    original.assignPropertyToOwner(owner1, property1);
    
    OwnerManager moved(std::move(original));
    
    EXPECT_DOUBLE_EQ(moved.getManagementFeePercentage(), 12.5);
    EXPECT_EQ(moved.getPaymentFrequencyDays(), 45);
    
    EXPECT_EQ(moved.getPropertiesByOwner("OWN001").size(), 1);
}

TEST_F(OwnerManagerTest, MoveAssignment) {
    OwnerManager original(12.5, 45);
    original.registerOwner(owner1);
    original.assignPropertyToOwner(owner1, property1);
    
    OwnerManager moved(20.0, 60);
    moved.registerOwner(owner2);
    moved.assignPropertyToOwner(owner2, property2);
    
    moved = std::move(original);
    
    EXPECT_DOUBLE_EQ(moved.getManagementFeePercentage(), 12.5);
    EXPECT_EQ(moved.getPaymentFrequencyDays(), 45);
    
    EXPECT_EQ(moved.getPropertiesByOwner("OWN001").size(), 1);
    EXPECT_TRUE(moved.getPropertiesByOwner("OWN002").empty());
}

TEST_F(OwnerManagerTest, SelfAssignment) {
    OwnerManager manager(10.0, 30);
    manager.registerOwner(owner1);
    manager.assignPropertyToOwner(owner1, property1);
    
    manager = manager;
    
    EXPECT_DOUBLE_EQ(manager.getManagementFeePercentage(), 10.0);
    EXPECT_EQ(manager.getPaymentFrequencyDays(), 30);
    
    EXPECT_EQ(manager.getPropertiesByOwner("OWN001").size(), 1);
}

TEST_F(OwnerManagerTest, MixedOperations) {
    OwnerManager manager(15.0, 30);
    
    manager.registerOwner(owner1);
    manager.registerOwner(owner2);
    manager.registerOwner(owner3);
    
    manager.assignPropertyToOwner(owner1, property1);
    manager.assignPropertyToOwner(owner1, property2);
    manager.assignPropertyToOwner(owner2, property3);
    manager.assignPropertyToOwner(owner3, property4);
    
    EXPECT_TRUE(manager.isOwnerActive("OWN001"));
    EXPECT_TRUE(manager.isOwnerActive("OWN002"));
    EXPECT_TRUE(manager.isOwnerActive("OWN003"));
    
    EXPECT_EQ(manager.getPropertiesByOwner("OWN001").size(), 2);
    EXPECT_EQ(manager.getPropertiesByOwner("OWN002").size(), 1);
    EXPECT_EQ(manager.getPropertiesByOwner("OWN003").size(), 1);
    
    auto multiOwners = manager.getOwnersWithMultipleProperties();
    EXPECT_EQ(multiOwners.size(), 1);
    EXPECT_EQ(multiOwners[0]->getOwnerId(), "OWN001");
    
    EXPECT_TRUE(manager.canOwnerAddMoreProperties("OWN001"));
    EXPECT_TRUE(manager.canOwnerAddMoreProperties("OWN002"));
    EXPECT_TRUE(manager.canOwnerAddMoreProperties("OWN003"));
    
    double portfolio1 = manager.calculateOwnerPortfolioValue("OWN001");
    double portfolio2 = manager.calculateOwnerPortfolioValue("OWN002");
    double portfolio3 = manager.calculateOwnerPortfolioValue("OWN003");
    
    EXPECT_DOUBLE_EQ(portfolio1, (1500.0 + 2500.0) * 100);
    EXPECT_DOUBLE_EQ(portfolio2, 2000.0 * 100);
    EXPECT_DOUBLE_EQ(portfolio3, 1800.0 * 100);
    
    EXPECT_DOUBLE_EQ(manager.calculateOwnerPayment(owner1, 5000.0), (5000.0 - 750.0) * 0.6);
    EXPECT_DOUBLE_EQ(manager.calculateOwnerPayment(owner2, 5000.0), (5000.0 - 750.0) * 0.4);
    EXPECT_DOUBLE_EQ(manager.calculateOwnerPayment(owner3, 5000.0), (5000.0 - 750.0) * 1.0);
}

TEST_F(OwnerManagerTest, EdgeCases) {
    OwnerManager manager(0.0, 7);
    
    EXPECT_DOUBLE_EQ(manager.getManagementFeePercentage(), 0.0);
    EXPECT_EQ(manager.getPaymentFrequencyDays(), 7);
    
    EXPECT_DOUBLE_EQ(manager.calculateManagementFee(1000.0), 0.0);
    EXPECT_DOUBLE_EQ(manager.calculateOwnerPayment(owner1, 1000.0), 600.0);
    
    manager.setManagementFeePercentage(100.0);
    EXPECT_DOUBLE_EQ(manager.calculateManagementFee(1000.0), 1000.0);
    EXPECT_DOUBLE_EQ(manager.calculateOwnerPayment(owner1, 1000.0), 0.0);
    
    auto zeroOwnershipOwner = std::make_shared<Owner>("ZERO", "Zero", "Owner", 
        "zero@example.com", "0000000000", nullptr, "TAX000", 0.0, true, "bank");
    
    manager.registerOwner(zeroOwnershipOwner);
    EXPECT_DOUBLE_EQ(manager.calculateOwnerPayment(zeroOwnershipOwner, 1000.0), 0.0);
    
    auto negativeOwnershipOwner = std::make_shared<Owner>("NEG", "Negative", "Owner", 
        "neg@example.com", "1111111111", nullptr, "TAX111", -50.0, true, "bank");
    
    manager.registerOwner(negativeOwnershipOwner);
    EXPECT_DOUBLE_EQ(manager.calculateOwnerPayment(negativeOwnershipOwner, 1000.0), 0.0);
}

TEST_F(OwnerManagerTest, OwnerWithNoProperties) {
    OwnerManager manager(10.0, 30);
    
    manager.registerOwner(owner1);
    
    EXPECT_FALSE(manager.isOwnerActive("OWN001"));
    EXPECT_TRUE(manager.getPropertiesByOwner("OWN001").empty());
    EXPECT_DOUBLE_EQ(manager.calculateOwnerPortfolioValue("OWN001"), 0.0);
    EXPECT_TRUE(manager.canOwnerAddMoreProperties("OWN001"));
}

TEST_F(OwnerManagerTest, RemoveOwnerNotSupported) {
    OwnerManager manager(10.0, 30);
    
    manager.registerOwner(owner1);
    manager.assignPropertyToOwner(owner1, property1);
    
    EXPECT_TRUE(manager.isOwnerActive("OWN001"));
    EXPECT_EQ(manager.getPropertiesByOwner("OWN001").size(), 1);
    
    manager.registerOwner(owner1);
    
    EXPECT_TRUE(manager.isOwnerActive("OWN001"));
    EXPECT_EQ(manager.getPropertiesByOwner("OWN001").size(), 1);
}