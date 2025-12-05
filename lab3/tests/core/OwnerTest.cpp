#include <gtest/gtest.h>
#include "../../include/core/Owner.h"
#include "../../include/core/Address.h"
#include <memory>

class OwnerTest : public ::testing::Test {
protected:
    std::unique_ptr<Address> createAddress() {
        return std::make_unique<Address>("123 Main St", "Springfield", "IL", 
                                        "62704", "USA", 39.7817, -89.6501);
    }
    
    std::unique_ptr<Owner> createBasicOwner() {
        return std::make_unique<Owner>("O1", "John", "Doe", "john@example.com",
                                      "123-456-7890", createAddress(),
                                      "123456789", 75.0, true, "Bank Transfer");
    }
};

TEST_F(OwnerTest, DefaultConstructor) {
    Owner owner;
    
    EXPECT_EQ(owner.getOwnerId(), "");
    EXPECT_EQ(owner.getFirstName(), "");
    EXPECT_EQ(owner.getLastName(), "");
    EXPECT_EQ(owner.getEmail(), "");
    EXPECT_EQ(owner.getPhoneNumber(), "");
    EXPECT_EQ(owner.getTaxId(), "");
    EXPECT_DOUBLE_EQ(owner.getOwnershipPercentage(), 0.0);
    EXPECT_FALSE(owner.getIsPrimaryOwner());
    EXPECT_EQ(owner.getPreferredPaymentMethod(), "");
    EXPECT_EQ(owner.getMailingAddress(), nullptr);
}

TEST_F(OwnerTest, MinimalConstructor) {
    Owner owner("O1", "Jane", "Smith", "jane@example.com");
    
    EXPECT_EQ(owner.getOwnerId(), "O1");
    EXPECT_EQ(owner.getFirstName(), "Jane");
    EXPECT_EQ(owner.getLastName(), "Smith");
    EXPECT_EQ(owner.getEmail(), "jane@example.com");
    EXPECT_EQ(owner.getPhoneNumber(), "");
    EXPECT_EQ(owner.getTaxId(), "");
    EXPECT_DOUBLE_EQ(owner.getOwnershipPercentage(), 0.0);
    EXPECT_FALSE(owner.getIsPrimaryOwner());
    EXPECT_EQ(owner.getPreferredPaymentMethod(), "");
    EXPECT_EQ(owner.getMailingAddress(), nullptr);
}

TEST_F(OwnerTest, FullConstructorEdgeCases) {
    Owner owner1("", "", "", "", "", nullptr, "", -50.0, false, "");
    
    EXPECT_EQ(owner1.getOwnerId(), "");
    EXPECT_EQ(owner1.getFirstName(), "");
    EXPECT_EQ(owner1.getLastName(), "");
    EXPECT_EQ(owner1.getEmail(), "");
    EXPECT_EQ(owner1.getPhoneNumber(), "");
    EXPECT_EQ(owner1.getTaxId(), "");
    EXPECT_DOUBLE_EQ(owner1.getOwnershipPercentage(), -50.0);
    EXPECT_FALSE(owner1.getIsPrimaryOwner());
    EXPECT_EQ(owner1.getPreferredPaymentMethod(), "");
    EXPECT_EQ(owner1.getMailingAddress(), nullptr);
}

TEST_F(OwnerTest, CopyConstructor) {
    auto owner1 = createBasicOwner();
    owner1->setEmail("updated@example.com");
    owner1->updateOwnershipPercentage(80.0);
    
    Owner owner2(*owner1);
    
    EXPECT_EQ(owner2.getOwnerId(), "O1");
    EXPECT_EQ(owner2.getFirstName(), "John");
    EXPECT_EQ(owner2.getLastName(), "Doe");
    EXPECT_EQ(owner2.getEmail(), "updated@example.com");
    EXPECT_EQ(owner2.getPhoneNumber(), "123-456-7890");
    EXPECT_EQ(owner2.getTaxId(), "123456789");
    EXPECT_DOUBLE_EQ(owner2.getOwnershipPercentage(), 80.0);
    EXPECT_TRUE(owner2.getIsPrimaryOwner());
    EXPECT_EQ(owner2.getPreferredPaymentMethod(), "Bank Transfer");
    EXPECT_NE(owner2.getMailingAddress(), nullptr);
}

TEST_F(OwnerTest, CopyConstructorWithNullAddress) {
    Owner owner1("O1", "John", "Doe", "john@example.com",
                "123-456-7890", nullptr, "123456789", 75.0, true, "Bank Transfer");
    
    Owner owner2(owner1);
    
    EXPECT_EQ(owner2.getOwnerId(), "O1");
    EXPECT_EQ(owner2.getFirstName(), "John");
    EXPECT_EQ(owner2.getLastName(), "Doe");
    EXPECT_EQ(owner2.getEmail(), "john@example.com");
    EXPECT_EQ(owner2.getPhoneNumber(), "123-456-7890");
    EXPECT_EQ(owner2.getTaxId(), "123456789");
    EXPECT_DOUBLE_EQ(owner2.getOwnershipPercentage(), 75.0);
    EXPECT_TRUE(owner2.getIsPrimaryOwner());
    EXPECT_EQ(owner2.getPreferredPaymentMethod(), "Bank Transfer");
    EXPECT_EQ(owner2.getMailingAddress(), nullptr);
}

TEST_F(OwnerTest, CopyAssignment) {
    auto owner1 = createBasicOwner();
    owner1->setEmail("updated@example.com");
    owner1->updateOwnershipPercentage(80.0);
    
    auto owner2 = std::make_unique<Owner>("O2", "Jane", "Smith", "jane@example.com",
                                         "987-654-3210", createAddress(),
                                         "987654321", 50.0, false, "Cash");
    
    *owner2 = *owner1;
    
    EXPECT_EQ(owner2->getOwnerId(), "O1");
    EXPECT_EQ(owner2->getFirstName(), "John");
    EXPECT_EQ(owner2->getLastName(), "Doe");
    EXPECT_EQ(owner2->getEmail(), "updated@example.com");
    EXPECT_EQ(owner2->getPhoneNumber(), "123-456-7890");
    EXPECT_EQ(owner2->getTaxId(), "123456789");
    EXPECT_DOUBLE_EQ(owner2->getOwnershipPercentage(), 80.0);
    EXPECT_TRUE(owner2->getIsPrimaryOwner());
    EXPECT_EQ(owner2->getPreferredPaymentMethod(), "Bank Transfer");
    EXPECT_NE(owner2->getMailingAddress(), nullptr);
}

TEST_F(OwnerTest, MoveConstructor) {
    auto owner1 = createBasicOwner();
    Owner owner2(std::move(*owner1));
    
    EXPECT_EQ(owner2.getOwnerId(), "O1");
    EXPECT_EQ(owner2.getFirstName(), "John");
    EXPECT_EQ(owner2.getLastName(), "Doe");
    EXPECT_EQ(owner2.getEmail(), "john@example.com");
    EXPECT_EQ(owner2.getPhoneNumber(), "123-456-7890");
    EXPECT_EQ(owner2.getTaxId(), "123456789");
    EXPECT_DOUBLE_EQ(owner2.getOwnershipPercentage(), 75.0);
    EXPECT_TRUE(owner2.getIsPrimaryOwner());
    EXPECT_EQ(owner2.getPreferredPaymentMethod(), "Bank Transfer");
    EXPECT_NE(owner2.getMailingAddress(), nullptr);
}

TEST_F(OwnerTest, MoveAssignment) {
    auto owner1 = createBasicOwner();
    auto owner2 = std::make_unique<Owner>("O2", "Jane", "Smith", "jane@example.com",
                                         "987-654-3210", createAddress(),
                                         "987654321", 50.0, false, "Cash");
    
    *owner2 = std::move(*owner1);
    
    EXPECT_EQ(owner2->getOwnerId(), "O1");
    EXPECT_EQ(owner2->getFirstName(), "John");
    EXPECT_EQ(owner2->getLastName(), "Doe");
    EXPECT_EQ(owner2->getEmail(), "john@example.com");
    EXPECT_EQ(owner2->getPhoneNumber(), "123-456-7890");
    EXPECT_EQ(owner2->getTaxId(), "123456789");
    EXPECT_DOUBLE_EQ(owner2->getOwnershipPercentage(), 75.0);
    EXPECT_TRUE(owner2->getIsPrimaryOwner());
    EXPECT_EQ(owner2->getPreferredPaymentMethod(), "Bank Transfer");
    EXPECT_NE(owner2->getMailingAddress(), nullptr);
}

TEST_F(OwnerTest, UpdateOwnershipPercentageEdgeCases) {
    auto owner = createBasicOwner();
    
    owner->updateOwnershipPercentage(-50.0);
    EXPECT_DOUBLE_EQ(owner->getOwnershipPercentage(), 0.0);
    
    owner->updateOwnershipPercentage(0.0);
    EXPECT_DOUBLE_EQ(owner->getOwnershipPercentage(), 0.0);
    
    owner->updateOwnershipPercentage(50.0);
    EXPECT_DOUBLE_EQ(owner->getOwnershipPercentage(), 50.0);
    
    owner->updateOwnershipPercentage(100.0);
    EXPECT_DOUBLE_EQ(owner->getOwnershipPercentage(), 100.0);
    
    owner->updateOwnershipPercentage(150.0);
    EXPECT_DOUBLE_EQ(owner->getOwnershipPercentage(), 100.0);
}

TEST_F(OwnerTest, GetFullNameEdgeCases) {
    Owner owner1("O1", "", "", "test@example.com");
    EXPECT_EQ(owner1.getFullName(), " ");
    
    Owner owner2("O2", "John", "", "test@example.com");
    EXPECT_EQ(owner2.getFullName(), "John ");
    
    Owner owner3("O3", "", "Doe", "test@example.com");
    EXPECT_EQ(owner3.getFullName(), " Doe");
    
    Owner owner4("O4", "  ", "  ", "test@example.com");
    EXPECT_EQ(owner4.getFullName(), "     ");
}

TEST_F(OwnerTest, IsValidTaxIdEdgeCases) {
    Owner owner1("O1", "John", "Doe", "test@example.com",
                "123-456-7890", createAddress(), "123456789", 75.0, true, "Bank");
    EXPECT_TRUE(owner1.isValidTaxId());
    
    Owner owner2("O2", "Jane", "Smith", "test@example.com",
                "123-456-7890", createAddress(), "12345", 75.0, true, "Bank");
    EXPECT_FALSE(owner2.isValidTaxId());
    
    Owner owner3("O3", "Bob", "Johnson", "test@example.com",
                "123-456-7890", createAddress(), "1234567890", 75.0, true, "Bank");
    EXPECT_FALSE(owner3.isValidTaxId());
    
    Owner owner4("O4", "Alice", "Brown", "test@example.com",
                "123-456-7890", createAddress(), "123AB6789", 75.0, true, "Bank");
    EXPECT_FALSE(owner4.isValidTaxId());
    
    Owner owner5("O5", "Charlie", "Davis", "test@example.com",
                "123-456-7890", createAddress(), "123-56789", 75.0, true, "Bank");
    EXPECT_FALSE(owner5.isValidTaxId());
    
    Owner owner6("O6", "Eve", "Wilson", "test@example.com",
                "123-456-7890", createAddress(), "", 75.0, true, "Bank");
    EXPECT_FALSE(owner6.isValidTaxId());
    
    Owner owner7("O7", "Frank", "Moore", "test@example.com",
                "123-456-7890", createAddress(), "   ", 75.0, true, "Bank");
    EXPECT_FALSE(owner7.isValidTaxId());
}

TEST_F(OwnerTest, CalculateNetPaymentEdgeCases) {
    auto owner = createBasicOwner();
    owner->updateOwnershipPercentage(75.0);
    
    double net1 = owner->calculateNetPayment(1000.0, 10.0);
    double expected1 = (1000.0 - (1000.0 * 0.10)) * 0.75;
    EXPECT_DOUBLE_EQ(net1, expected1);
    
    double net2 = owner->calculateNetPayment(0.0, 10.0);
    EXPECT_DOUBLE_EQ(net2, 0.0);
    
    double net3 = owner->calculateNetPayment(-1000.0, 10.0);
    double expected3 = (-1000.0 - (-1000.0 * 0.10)) * 0.75;
    EXPECT_DOUBLE_EQ(net3, expected3);
    
    double net4 = owner->calculateNetPayment(1000.0, 0.0);
    double expected4 = 1000.0 * 0.75;
    EXPECT_DOUBLE_EQ(net4, expected4);
    
    double net5 = owner->calculateNetPayment(1000.0, 100.0);
    double expected5 = (1000.0 - (1000.0 * 1.0)) * 0.75;
    EXPECT_DOUBLE_EQ(net5, expected5);
    
    double net6 = owner->calculateNetPayment(1000.0, -10.0);
    double expected6 = (1000.0 - (1000.0 * -0.10)) * 0.75;
    EXPECT_DOUBLE_EQ(net6, expected6);
    
    Owner owner2("O2", "Jane", "Smith", "test@example.com",
                "123-456-7890", createAddress(), "123456789", 0.0, true, "Bank");
    double net7 = owner2.calculateNetPayment(1000.0, 10.0);
    EXPECT_DOUBLE_EQ(net7, 0.0);
    
    Owner owner3("O3", "Bob", "Johnson", "test@example.com",
                "123-456-7890", createAddress(), "123456789", -50.0, true, "Bank");
    double net8 = owner3.calculateNetPayment(1000.0, 10.0);
    EXPECT_DOUBLE_EQ(net8, -450.0);
}

TEST_F(OwnerTest, RequiresTaxDocumentationEdgeCases) {
    Owner owner1("O1", "John", "Doe", "test@example.com",
                "123-456-7890", createAddress(), "123456789", 0.0, true, "Bank");
    EXPECT_FALSE(owner1.requiresTaxDocumentation());
    
    Owner owner2("O2", "Jane", "Smith", "test@example.com",
                "123-456-7890", createAddress(), "123456789", 0.1, true, "Bank");
    EXPECT_TRUE(owner2.requiresTaxDocumentation());
    
    Owner owner3("O3", "Bob", "Johnson", "test@example.com",
                "123-456-7890", createAddress(), "123456789", 50.0, true, "Bank");
    EXPECT_TRUE(owner3.requiresTaxDocumentation());
    
    Owner owner4("O4", "Alice", "Brown", "test@example.com",
                "123-456-7890", createAddress(), "123456789", 100.0, true, "Bank");
    EXPECT_TRUE(owner4.requiresTaxDocumentation());
    
    Owner owner5("O5", "Charlie", "Davis", "test@example.com",
                "123-456-7890", createAddress(), "123456789", -50.0, true, "Bank");
    EXPECT_FALSE(owner5.requiresTaxDocumentation());
}

TEST_F(OwnerTest, CanAuthorizeMaintenanceEdgeCases) {
    Owner owner1("O1", "John", "Doe", "test@example.com",
                "123-456-7890", createAddress(), "123456789", 49.9, false, "Bank");
    EXPECT_FALSE(owner1.canAuthorizeMaintenance(1000.0));
    
    Owner owner2("O2", "Jane", "Smith", "test@example.com",
                "123-456-7890", createAddress(), "123456789", 50.0, false, "Bank");
    EXPECT_TRUE(owner2.canAuthorizeMaintenance(1000.0));
    
    Owner owner3("O3", "Bob", "Johnson", "test@example.com",
                "123-456-7890", createAddress(), "123456789", 75.0, false, "Bank");
    EXPECT_TRUE(owner3.canAuthorizeMaintenance(1000.0));
    
    Owner owner4("O4", "Alice", "Brown", "test@example.com",
                "123-456-7890", createAddress(), "123456789", 25.0, true, "Bank");
    EXPECT_TRUE(owner4.canAuthorizeMaintenance(1000.0));
    
    Owner owner5("O5", "Charlie", "Davis", "test@example.com",
                "123-456-7890", createAddress(), "123456789", 0.0, true, "Bank");
    EXPECT_TRUE(owner5.canAuthorizeMaintenance(1000.0));
    
    Owner owner6("O6", "Eve", "Wilson", "test@example.com",
                "123-456-7890", createAddress(), "123456789", -50.0, false, "Bank");
    EXPECT_FALSE(owner6.canAuthorizeMaintenance(1000.0));
    
    Owner owner7("O7", "Frank", "Moore", "test@example.com",
                "123-456-7890", createAddress(), "123456789", 49.9, true, "Bank");
    EXPECT_TRUE(owner7.canAuthorizeMaintenance(1000.0));
}

TEST_F(OwnerTest, IsContactInformationCompleteEdgeCases) {
    Owner owner1("O1", "John", "Doe", "john@example.com",
                "123-456-7890", createAddress(), "123456789", 75.0, true, "Bank");
    EXPECT_TRUE(owner1.isContactInformationComplete());
    
    Owner owner2("O2", "Jane", "Smith", "", "123-456-7890", createAddress(),
                "123456789", 75.0, true, "Bank");
    EXPECT_FALSE(owner2.isContactInformationComplete());
    
    Owner owner3("O3", "Bob", "Johnson", "bob@example.com", "", createAddress(),
                "123456789", 75.0, true, "Bank");
    EXPECT_FALSE(owner3.isContactInformationComplete());
    
    Owner owner4("O4", "Alice", "Brown", "alice@example.com", "123-456-7890", nullptr,
                "123456789", 75.0, true, "Bank");
    EXPECT_FALSE(owner4.isContactInformationComplete());
    
    Owner owner5("O5", "Charlie", "Davis", "", "", createAddress(),
                "123456789", 75.0, true, "Bank");
    EXPECT_FALSE(owner5.isContactInformationComplete());
    
    Owner owner6("O6", "Eve", "Wilson", "", "123-456-7890", nullptr,
                "123456789", 75.0, true, "Bank");
    EXPECT_FALSE(owner6.isContactInformationComplete());
    
    Owner owner7("O7", "Frank", "Moore", "frank@example.com", "", nullptr,
                "123456789", 75.0, true, "Bank");
    EXPECT_FALSE(owner7.isContactInformationComplete());
    
    Owner owner8("O8", "Grace", "Taylor", "", "", nullptr,
                "123456789", 75.0, true, "Bank");
    EXPECT_FALSE(owner8.isContactInformationComplete());
}

TEST_F(OwnerTest, SetIsPrimaryOwner) {
    auto owner = createBasicOwner();
    
    EXPECT_TRUE(owner->getIsPrimaryOwner());
}

TEST_F(OwnerTest, GetFullNameWithSpaces) {
    Owner owner("O1", "  John  ", "  Doe  ", "test@example.com");
    EXPECT_EQ(owner.getFullName(), "  John     Doe  ");
}