#include <gtest/gtest.h>
#include "../../include/core/Tenant.h"
#include "../../include/core/Address.h"
#include <memory>

class TenantTest : public ::testing::Test {
protected:
    std::unique_ptr<Address> createAddress() {
        return std::make_unique<Address>("123 Main St", "Springfield", "IL", 
                                        "62704", "USA", 39.7817, -89.6501);
    }
    
    std::unique_ptr<Tenant> createBasicTenant() {
        return std::make_unique<Tenant>("T1", "John", "Doe", "john@example.com",
                                       "123-456-7890", createAddress(),
                                       750.0, 5000.0, false, 2);
    }
};

TEST_F(TenantTest, DefaultConstructor) {
    Tenant tenant;
    
    EXPECT_EQ(tenant.getTenantId(), "");
    EXPECT_EQ(tenant.getFirstName(), "");
    EXPECT_EQ(tenant.getLastName(), "");
    EXPECT_EQ(tenant.getEmail(), "");
    EXPECT_EQ(tenant.getPhoneNumber(), "");
    EXPECT_EQ(tenant.getCurrentAddress(), nullptr);
    EXPECT_DOUBLE_EQ(tenant.getCreditScore(), 0.0);
    EXPECT_DOUBLE_EQ(tenant.getMonthlyIncome(), 0.0);
    EXPECT_FALSE(tenant.getHasPets());
    EXPECT_EQ(tenant.getNumberOfOccupants(), 1);
}

TEST_F(TenantTest, MinimalConstructorFourParams) {
    Tenant tenant("T1", "John", "Doe", "john@example.com");
    
    EXPECT_EQ(tenant.getTenantId(), "T1");
    EXPECT_EQ(tenant.getFirstName(), "John");
    EXPECT_EQ(tenant.getLastName(), "Doe");
    EXPECT_EQ(tenant.getEmail(), "john@example.com");
    EXPECT_EQ(tenant.getPhoneNumber(), "");
    EXPECT_EQ(tenant.getCurrentAddress(), nullptr);
    EXPECT_DOUBLE_EQ(tenant.getCreditScore(), 0.0);
    EXPECT_DOUBLE_EQ(tenant.getMonthlyIncome(), 0.0);
    EXPECT_FALSE(tenant.getHasPets());
    EXPECT_EQ(tenant.getNumberOfOccupants(), 1);
}

TEST_F(TenantTest, ConstructorSixParams) {
    Tenant tenant("T1", "John", "Doe", "john@example.com", "123-456-7890", createAddress());
    
    EXPECT_EQ(tenant.getTenantId(), "T1");
    EXPECT_EQ(tenant.getFirstName(), "John");
    EXPECT_EQ(tenant.getLastName(), "Doe");
    EXPECT_EQ(tenant.getEmail(), "john@example.com");
    EXPECT_EQ(tenant.getPhoneNumber(), "123-456-7890");
    EXPECT_NE(tenant.getCurrentAddress(), nullptr);
    EXPECT_DOUBLE_EQ(tenant.getCreditScore(), 0.0);
    EXPECT_DOUBLE_EQ(tenant.getMonthlyIncome(), 0.0);
    EXPECT_FALSE(tenant.getHasPets());
    EXPECT_EQ(tenant.getNumberOfOccupants(), 1);
}

TEST_F(TenantTest, ConstructorFullNameTwoParams) {
    Tenant tenant("T1", "John Doe");
    
    EXPECT_EQ(tenant.getTenantId(), "T1");
    EXPECT_EQ(tenant.getFirstName(), "John Doe");
    EXPECT_EQ(tenant.getLastName(), "");
    EXPECT_EQ(tenant.getEmail(), "");
    EXPECT_EQ(tenant.getPhoneNumber(), "");
    EXPECT_EQ(tenant.getCurrentAddress(), nullptr);
    EXPECT_DOUBLE_EQ(tenant.getCreditScore(), 0.0);
    EXPECT_DOUBLE_EQ(tenant.getMonthlyIncome(), 0.0);
    EXPECT_FALSE(tenant.getHasPets());
    EXPECT_EQ(tenant.getNumberOfOccupants(), 1);
}

TEST_F(TenantTest, ConstructorFullNameFourParams) {
    Tenant tenant("T1", "John Doe", 750.0, 5000.0);
    
    EXPECT_EQ(tenant.getTenantId(), "T1");
    EXPECT_EQ(tenant.getFirstName(), "John Doe");
    EXPECT_EQ(tenant.getLastName(), "");
    EXPECT_EQ(tenant.getEmail(), "");
    EXPECT_EQ(tenant.getPhoneNumber(), "");
    EXPECT_EQ(tenant.getCurrentAddress(), nullptr);
    EXPECT_DOUBLE_EQ(tenant.getCreditScore(), 750.0);
    EXPECT_DOUBLE_EQ(tenant.getMonthlyIncome(), 5000.0);
    EXPECT_FALSE(tenant.getHasPets());
    EXPECT_EQ(tenant.getNumberOfOccupants(), 1);
}

TEST_F(TenantTest, FullConstructorEdgeCases) {
    Tenant tenant("", "", "", "", "", nullptr, -100.0, -5000.0, true, -2);
    
    EXPECT_EQ(tenant.getTenantId(), "");
    EXPECT_EQ(tenant.getFirstName(), "");
    EXPECT_EQ(tenant.getLastName(), "");
    EXPECT_EQ(tenant.getEmail(), "");
    EXPECT_EQ(tenant.getPhoneNumber(), "");
    EXPECT_EQ(tenant.getCurrentAddress(), nullptr);
    EXPECT_DOUBLE_EQ(tenant.getCreditScore(), -100.0);
    EXPECT_DOUBLE_EQ(tenant.getMonthlyIncome(), -5000.0);
    EXPECT_TRUE(tenant.getHasPets());
    EXPECT_EQ(tenant.getNumberOfOccupants(), -2);
}

TEST_F(TenantTest, CopyConstructor) {
    auto tenant1 = createBasicTenant();
    tenant1->setEmail("updated@example.com");
    tenant1->updateCreditScore(780.0);
    
    Tenant tenant2(*tenant1);
    
    EXPECT_EQ(tenant2.getTenantId(), "T1");
    EXPECT_EQ(tenant2.getFirstName(), "John");
    EXPECT_EQ(tenant2.getLastName(), "Doe");
    EXPECT_EQ(tenant2.getEmail(), "updated@example.com");
    EXPECT_EQ(tenant2.getPhoneNumber(), "123-456-7890");
    EXPECT_NE(tenant2.getCurrentAddress(), nullptr);
    EXPECT_DOUBLE_EQ(tenant2.getCreditScore(), 780.0);
    EXPECT_DOUBLE_EQ(tenant2.getMonthlyIncome(), 5000.0);
    EXPECT_FALSE(tenant2.getHasPets());
    EXPECT_EQ(tenant2.getNumberOfOccupants(), 2);
}

TEST_F(TenantTest, CopyConstructorWithNullAddress) {
    Tenant tenant1("T1", "John", "Doe", "john@example.com", "123-456-7890", nullptr,
                  750.0, 5000.0, false, 2);
    
    Tenant tenant2(tenant1);
    
    EXPECT_EQ(tenant2.getTenantId(), "T1");
    EXPECT_EQ(tenant2.getFirstName(), "John");
    EXPECT_EQ(tenant2.getLastName(), "Doe");
    EXPECT_EQ(tenant2.getEmail(), "john@example.com");
    EXPECT_EQ(tenant2.getPhoneNumber(), "123-456-7890");
    EXPECT_EQ(tenant2.getCurrentAddress(), nullptr);
    EXPECT_DOUBLE_EQ(tenant2.getCreditScore(), 750.0);
    EXPECT_DOUBLE_EQ(tenant2.getMonthlyIncome(), 5000.0);
    EXPECT_FALSE(tenant2.getHasPets());
    EXPECT_EQ(tenant2.getNumberOfOccupants(), 2);
}

TEST_F(TenantTest, CopyAssignment) {
    auto tenant1 = createBasicTenant();
    tenant1->setEmail("updated@example.com");
    tenant1->updateCreditScore(780.0);
    
    auto tenant2 = std::make_unique<Tenant>("T2", "Jane", "Smith", "jane@example.com",
                                           "987-654-3210", createAddress(),
                                           650.0, 4000.0, true, 1);
    
    *tenant2 = *tenant1;
    
    EXPECT_EQ(tenant2->getTenantId(), "T1");
    EXPECT_EQ(tenant2->getFirstName(), "John");
    EXPECT_EQ(tenant2->getLastName(), "Doe");
    EXPECT_EQ(tenant2->getEmail(), "updated@example.com");
    EXPECT_EQ(tenant2->getPhoneNumber(), "123-456-7890");
    EXPECT_NE(tenant2->getCurrentAddress(), nullptr);
    EXPECT_DOUBLE_EQ(tenant2->getCreditScore(), 780.0);
    EXPECT_DOUBLE_EQ(tenant2->getMonthlyIncome(), 5000.0);
    EXPECT_FALSE(tenant2->getHasPets());
    EXPECT_EQ(tenant2->getNumberOfOccupants(), 2);
}

TEST_F(TenantTest, MoveConstructor) {
    auto tenant1 = createBasicTenant();
    Tenant tenant2(std::move(*tenant1));
    
    EXPECT_EQ(tenant2.getTenantId(), "T1");
    EXPECT_EQ(tenant2.getFirstName(), "John");
    EXPECT_EQ(tenant2.getLastName(), "Doe");
    EXPECT_EQ(tenant2.getEmail(), "john@example.com");
    EXPECT_EQ(tenant2.getPhoneNumber(), "123-456-7890");
    EXPECT_NE(tenant2.getCurrentAddress(), nullptr);
    EXPECT_DOUBLE_EQ(tenant2.getCreditScore(), 750.0);
    EXPECT_DOUBLE_EQ(tenant2.getMonthlyIncome(), 5000.0);
    EXPECT_FALSE(tenant2.getHasPets());
    EXPECT_EQ(tenant2.getNumberOfOccupants(), 2);
}

TEST_F(TenantTest, MoveAssignment) {
    auto tenant1 = createBasicTenant();
    auto tenant2 = std::make_unique<Tenant>("T2", "Jane", "Smith", "jane@example.com",
                                           "987-654-3210", createAddress(),
                                           650.0, 4000.0, true, 1);
    
    *tenant2 = std::move(*tenant1);
    
    EXPECT_EQ(tenant2->getTenantId(), "T1");
    EXPECT_EQ(tenant2->getFirstName(), "John");
    EXPECT_EQ(tenant2->getLastName(), "Doe");
    EXPECT_EQ(tenant2->getEmail(), "john@example.com");
    EXPECT_EQ(tenant2->getPhoneNumber(), "123-456-7890");
    EXPECT_NE(tenant2->getCurrentAddress(), nullptr);
    EXPECT_DOUBLE_EQ(tenant2->getCreditScore(), 750.0);
    EXPECT_DOUBLE_EQ(tenant2->getMonthlyIncome(), 5000.0);
    EXPECT_FALSE(tenant2->getHasPets());
    EXPECT_EQ(tenant2->getNumberOfOccupants(), 2);
}

TEST_F(TenantTest, UpdateCreditScoreEdgeCases) {
    auto tenant = createBasicTenant();
    
    tenant->updateCreditScore(-100.0);
    EXPECT_DOUBLE_EQ(tenant->getCreditScore(), 300.0);
    
    tenant->updateCreditScore(0.0);
    EXPECT_DOUBLE_EQ(tenant->getCreditScore(), 300.0);
    
    tenant->updateCreditScore(300.0);
    EXPECT_DOUBLE_EQ(tenant->getCreditScore(), 300.0);
    
    tenant->updateCreditScore(500.0);
    EXPECT_DOUBLE_EQ(tenant->getCreditScore(), 500.0);
    
    tenant->updateCreditScore(850.0);
    EXPECT_DOUBLE_EQ(tenant->getCreditScore(), 850.0);
    
    tenant->updateCreditScore(900.0);
    EXPECT_DOUBLE_EQ(tenant->getCreditScore(), 850.0);
    
    tenant->updateCreditScore(1000.0);
    EXPECT_DOUBLE_EQ(tenant->getCreditScore(), 850.0);
}

TEST_F(TenantTest, UpdateIncomeEdgeCases) {
    auto tenant = createBasicTenant();
    
    tenant->updateIncome(-5000.0);
    EXPECT_DOUBLE_EQ(tenant->getMonthlyIncome(), 0.0);
    
    tenant->updateIncome(0.0);
    EXPECT_DOUBLE_EQ(tenant->getMonthlyIncome(), 0.0);
    
    tenant->updateIncome(2500.0);
    EXPECT_DOUBLE_EQ(tenant->getMonthlyIncome(), 2500.0);
    
    tenant->updateIncome(10000.0);
    EXPECT_DOUBLE_EQ(tenant->getMonthlyIncome(), 10000.0);
}

TEST_F(TenantTest, GetFullNameEdgeCases) {
    Tenant tenant1("T1", "", "", "test@example.com");
    EXPECT_EQ(tenant1.getFullName(), " ");
    
    Tenant tenant2("T2", "John", "", "test@example.com");
    EXPECT_EQ(tenant2.getFullName(), "John ");
    
    Tenant tenant3("T3", "", "Doe", "test@example.com");
    EXPECT_EQ(tenant3.getFullName(), " Doe");
    
    Tenant tenant4("T4", "  ", "  ", "test@example.com");
    EXPECT_EQ(tenant4.getFullName(), "     ");
    
    Tenant tenant5("T5", "John", "Doe", "test@example.com");
    EXPECT_EQ(tenant5.getFullName(), "John Doe");
}

TEST_F(TenantTest, MeetsIncomeRequirementsEdgeCases) {
    Tenant tenant1("T1", "John", "Doe", "test@example.com", "123-456-7890", createAddress(),
                  750.0, 0.0, false, 2);
    
    EXPECT_FALSE(tenant1.meetsIncomeRequirements(0.3));
    
    Tenant tenant2("T2", "Jane", "Smith", "test@example.com", "123-456-7890", createAddress(),
                  750.0, -5000.0, false, 2);
    
    EXPECT_FALSE(tenant2.meetsIncomeRequirements(0.3));
    
    Tenant tenant3("T3", "Bob", "Johnson", "test@example.com", "123-456-7890", createAddress(),
                  750.0, 5000.0, false, 2);
    
    EXPECT_FALSE(tenant3.meetsIncomeRequirements(0.0));
    EXPECT_TRUE(tenant3.meetsIncomeRequirements(0.3));
    EXPECT_TRUE(tenant3.meetsIncomeRequirements(1.0));
    
    Tenant tenant4("T4", "Alice", "Brown", "test@example.com", "123-456-7890", createAddress(),
                  750.0, 5000.0, false, 2);
    
    EXPECT_FALSE(tenant4.meetsIncomeRequirements(-0.3));
}

TEST_F(TenantTest, IsCreditworthyEdgeCases) {
    Tenant tenant1("T1", "John", "Doe", "test@example.com", "123-456-7890", createAddress(),
                  0.0, 5000.0, false, 2);
    
    EXPECT_TRUE(tenant1.isCreditworthy(0.0));
    EXPECT_FALSE(tenant1.isCreditworthy(100.0));
    
    Tenant tenant2("T2", "Jane", "Smith", "test@example.com", "123-456-7890", createAddress(),
                  -100.0, 5000.0, false, 2);
    
    EXPECT_TRUE(tenant2.isCreditworthy(-200.0));
    EXPECT_FALSE(tenant2.isCreditworthy(-50.0));
    
    Tenant tenant3("T3", "Bob", "Johnson", "test@example.com", "123-456-7890", createAddress(),
                  750.0, 5000.0, false, 2);
    
    EXPECT_TRUE(tenant3.isCreditworthy(750.0));
    EXPECT_TRUE(tenant3.isCreditworthy(749.9));
    EXPECT_FALSE(tenant3.isCreditworthy(750.1));
}

TEST_F(TenantTest, CalculateMaxAffordableRentEdgeCases) {
    Tenant tenant1("T1", "John", "Doe", "test@example.com", "123-456-7890", createAddress(),
                  750.0, 0.0, false, 2);
    
    double rent1 = tenant1.calculateMaxAffordableRent();
    EXPECT_DOUBLE_EQ(rent1, 0.0);
    
    Tenant tenant2("T2", "Jane", "Smith", "test@example.com", "123-456-7890", createAddress(),
                  750.0, -5000.0, false, 2);
    
    double rent2 = tenant2.calculateMaxAffordableRent();
    EXPECT_DOUBLE_EQ(rent2, -1500.0);
    
    Tenant tenant3("T3", "Bob", "Johnson", "test@example.com", "123-456-7890", createAddress(),
                  750.0, 5000.0, false, 2);
    
    double rent3 = tenant3.calculateMaxAffordableRent();
    EXPECT_DOUBLE_EQ(rent3, 1500.0);
    
    Tenant tenant4("T4", "Alice", "Brown", "test@example.com", "123-456-7890", createAddress(),
                  750.0, 10000.0, false, 2);
    
    double rent4 = tenant4.calculateMaxAffordableRent();
    EXPECT_DOUBLE_EQ(rent4, 3000.0);
}

TEST_F(TenantTest, CanAccommodatePetsEdgeCases) {
    Tenant tenant1("T1", "John", "Doe", "test@example.com", "123-456-7890", createAddress(),
                  750.0, 5000.0, true, 2);
    
    EXPECT_TRUE(tenant1.canAccommodatePets(true));
    EXPECT_FALSE(tenant1.canAccommodatePets(false));
    
    Tenant tenant2("T2", "Jane", "Smith", "test@example.com", "123-456-7890", createAddress(),
                  750.0, 5000.0, false, 2);
    
    EXPECT_TRUE(tenant2.canAccommodatePets(true));
    EXPECT_TRUE(tenant2.canAccommodatePets(false));
}

TEST_F(TenantTest, IsSuitableForPropertySizeEdgeCases) {
    Tenant tenant1("T1", "John", "Doe", "test@example.com", "123-456-7890", createAddress(),
                  750.0, 5000.0, false, 1);
    
    EXPECT_TRUE(tenant1.isSuitableForPropertySize(1));
    EXPECT_TRUE(tenant1.isSuitableForPropertySize(2));
    EXPECT_FALSE(tenant1.isSuitableForPropertySize(0));
    EXPECT_FALSE(tenant1.isSuitableForPropertySize(-1));
    
    Tenant tenant2("T2", "Jane", "Smith", "test@example.com", "123-456-7890", createAddress(),
                  750.0, 5000.0, false, 2);
    
    EXPECT_TRUE(tenant2.isSuitableForPropertySize(1));
    EXPECT_TRUE(tenant2.isSuitableForPropertySize(2));
    EXPECT_FALSE(tenant2.isSuitableForPropertySize(0));
    
    Tenant tenant3("T3", "Bob", "Johnson", "test@example.com", "123-456-7890", createAddress(),
                  750.0, 5000.0, false, 3);
    
    EXPECT_TRUE(tenant3.isSuitableForPropertySize(2));
    EXPECT_FALSE(tenant3.isSuitableForPropertySize(1));
    EXPECT_TRUE(tenant3.isSuitableForPropertySize(3));
    
    Tenant tenant4("T4", "Alice", "Brown", "test@example.com", "123-456-7890", createAddress(),
                  750.0, 5000.0, false, 4);
    
    EXPECT_TRUE(tenant4.isSuitableForPropertySize(2));
    EXPECT_TRUE(tenant4.isSuitableForPropertySize(3));
    EXPECT_FALSE(tenant4.isSuitableForPropertySize(1));
    
    Tenant tenant5("T5", "Charlie", "Davis", "test@example.com", "123-456-7890", createAddress(),
                  750.0, 5000.0, false, 0);
    
    EXPECT_TRUE(tenant5.isSuitableForPropertySize(0));
    EXPECT_TRUE(tenant5.isSuitableForPropertySize(1));
    
    Tenant tenant6("T6", "Eve", "Wilson", "test@example.com", "123-456-7890", createAddress(),
                  750.0, 5000.0, false, -2);
    
    EXPECT_TRUE(tenant6.isSuitableForPropertySize(0));
    EXPECT_TRUE(tenant6.isSuitableForPropertySize(1));
}

TEST_F(TenantTest, SetEmailAndPhoneEdgeCases) {
    auto tenant = createBasicTenant();
    
    tenant->setEmail("");
    EXPECT_EQ(tenant->getEmail(), "");
    
    tenant->setEmail("  ");
    EXPECT_EQ(tenant->getEmail(), "  ");
    
    tenant->setEmail("new.email@example.com");
    EXPECT_EQ(tenant->getEmail(), "new.email@example.com");
    
    tenant->setPhoneNumber("");
    EXPECT_EQ(tenant->getPhoneNumber(), "");
    
    tenant->setPhoneNumber("  ");
    EXPECT_EQ(tenant->getPhoneNumber(), "  ");
    
    tenant->setPhoneNumber("+1-234-567-8900");
    EXPECT_EQ(tenant->getPhoneNumber(), "+1-234-567-8900");
}