#include <gtest/gtest.h>
#include "../../include/business/CommissionCalculator.h"
#include "../../include/core/Agent.h"
#include "../../include/core/Property.h"
#include "../../include/core/Tenant.h"
#include "../../include/core/Owner.h"
#include "../../include/core/Address.h"
#include "../../include/core/PropertyType.h"
#include "../../include/core/PropertyStatus.h"
#include "../../include/finance/RentalContract.h"
#include <memory>

class CommissionCalculatorTest : public ::testing::Test {
protected:
    void SetUp() override {
        auto agentAddress = std::make_unique<Address>("123 Main St", "City", "State", "12345");
        seniorAgent = std::make_shared<Agent>("A1", "John", "Doe", "john@example.com", "1234567890",
                                             "LIC12345", std::move(agentAddress), 5.0, "residential", 15, true,
                                             std::vector<std::string>{"Cert1", "Cert2", "Cert3", "Cert4", "Cert5"});
        
        auto agentAddress2 = std::make_unique<Address>("456 Other St", "City", "State", "67890");
        juniorAgent = std::make_shared<Agent>("A2", "Jane", "Smith", "jane@example.com", "0987654321",
                                             "LIC67890", std::move(agentAddress2), 3.0, "commercial", 3, false,
                                             std::vector<std::string>{"Cert1"});
        
        auto propertyAddress = std::make_unique<Address>("789 Elm St", "City", "State", "13579");
        auto now = std::chrono::system_clock::now();
        auto propertyStatus = std::make_unique<PropertyStatus>("AVAILABLE", "Property is available", 
                                                               true, false, false, now, "", 0.0, 0);
        auto propertyType = std::make_unique<PropertyType>("PT1", "residential", "House type", 1, 5, 50.0, 200.0,
                                                           false, "residential", 1.5);
        property = std::make_shared<Property>("P1", 120.0, 2000.0, std::move(propertyAddress),
                                             std::move(propertyStatus), std::move(propertyType));
        
        auto tenant = std::make_shared<Tenant>("T1", "Alice Smith", 700, 4000.0);
        auto ownerAddress = std::make_unique<Address>("246 Oak St", "City", "State", "97531");
        auto owner = std::make_shared<Owner>("O1", "Bob", "Owner", "bob@example.com", "9876543210",
                                            std::move(ownerAddress), "123456789", 100.0, true, "bank_transfer");
        
        auto startDate = std::chrono::system_clock::now();
        auto endDate = startDate + std::chrono::hours(24 * 365);
        contract = std::make_shared<RentalContract>("C1", property, tenant, owner,
                                                   startDate, endDate, 2000.0, 1000.0, "monthly");
    }
    
    std::shared_ptr<Agent> seniorAgent;
    std::shared_ptr<Agent> juniorAgent;
    std::shared_ptr<Property> property;
    std::shared_ptr<RentalContract> contract;
};

TEST_F(CommissionCalculatorTest, ConstructorAndGetters) {
    CommissionCalculator calc(0.05, 0.1, 0.08, 0.02, 500000.0, 0.6, "standard");
    
    EXPECT_DOUBLE_EQ(calc.getBaseCommissionRate(), 0.05);
    EXPECT_DOUBLE_EQ(calc.getLuxuryPropertyRate(), 0.1);
    EXPECT_DOUBLE_EQ(calc.getCommercialPropertyRate(), 0.08);
    EXPECT_DOUBLE_EQ(calc.getSeniorAgentBonus(), 0.02);
    EXPECT_DOUBLE_EQ(calc.getVolumeDiscountThreshold(), 500000.0);
    EXPECT_DOUBLE_EQ(calc.getTeamCommissionSplit(), 0.6);
    EXPECT_EQ(calc.getCommissionStructure(), "standard");
}

TEST_F(CommissionCalculatorTest, CopyConstructor) {
    CommissionCalculator original(0.05, 0.1, 0.08, 0.02, 500000.0, 0.6, "standard");
    CommissionCalculator copy(original);
    
    EXPECT_DOUBLE_EQ(copy.getBaseCommissionRate(), 0.05);
    EXPECT_DOUBLE_EQ(copy.getLuxuryPropertyRate(), 0.1);
    EXPECT_DOUBLE_EQ(copy.getTeamCommissionSplit(), 0.6);
}

TEST_F(CommissionCalculatorTest, CopyAssignmentOperator) {
    CommissionCalculator original(0.06, 0.12, 0.09, 0.03, 600000.0, 0.7, "premium");
    CommissionCalculator assigned(0.01, 0.01, 0.01, 0.01, 100000.0, 0.1, "basic");
    
    assigned = original;
    
    EXPECT_DOUBLE_EQ(assigned.getBaseCommissionRate(), 0.06);
    EXPECT_DOUBLE_EQ(assigned.getLuxuryPropertyRate(), 0.12);
    EXPECT_DOUBLE_EQ(assigned.getTeamCommissionSplit(), 0.7);
    EXPECT_EQ(assigned.getCommissionStructure(), "premium");
}

TEST_F(CommissionCalculatorTest, MoveConstructor) {
    CommissionCalculator original(0.07, 0.13, 0.1, 0.04, 700000.0, 0.8, "vip");
    CommissionCalculator moved(std::move(original));
    
    EXPECT_DOUBLE_EQ(moved.getBaseCommissionRate(), 0.07);
    EXPECT_DOUBLE_EQ(moved.getLuxuryPropertyRate(), 0.13);
    EXPECT_DOUBLE_EQ(moved.getTeamCommissionSplit(), 0.8);
    EXPECT_EQ(moved.getCommissionStructure(), "vip");
}

TEST_F(CommissionCalculatorTest, MoveAssignmentOperator) {
    CommissionCalculator original(0.08, 0.14, 0.11, 0.05, 800000.0, 0.9, "executive");
    CommissionCalculator moved(0.01, 0.01, 0.01, 0.01, 100000.0, 0.1, "basic");
    
    moved = std::move(original);
    
    EXPECT_DOUBLE_EQ(moved.getBaseCommissionRate(), 0.08);
    EXPECT_DOUBLE_EQ(moved.getLuxuryPropertyRate(), 0.14);
    EXPECT_DOUBLE_EQ(moved.getTeamCommissionSplit(), 0.9);
    EXPECT_EQ(moved.getCommissionStructure(), "executive");
}

TEST_F(CommissionCalculatorTest, SetBaseCommissionRate) {
    CommissionCalculator calc(0.05, 0.1, 0.08, 0.02, 500000.0, 0.6, "standard");
    
    calc.setBaseCommissionRate(0.1);
    EXPECT_DOUBLE_EQ(calc.getBaseCommissionRate(), 0.1);
}

TEST_F(CommissionCalculatorTest, SetBaseCommissionRateNegative) {
    CommissionCalculator calc(0.05, 0.1, 0.08, 0.02, 500000.0, 0.6, "standard");
    
    calc.setBaseCommissionRate(-0.1);
    EXPECT_DOUBLE_EQ(calc.getBaseCommissionRate(), 0.0);
}

TEST_F(CommissionCalculatorTest, SetLuxuryPropertyRate) {
    CommissionCalculator calc(0.05, 0.1, 0.08, 0.02, 500000.0, 0.6, "standard");
    
    calc.setLuxuryPropertyRate(0.15);
    EXPECT_DOUBLE_EQ(calc.getLuxuryPropertyRate(), 0.15);
}

TEST_F(CommissionCalculatorTest, SetCommercialPropertyRate) {
    CommissionCalculator calc(0.05, 0.1, 0.08, 0.02, 500000.0, 0.6, "standard");
    
    calc.setCommercialPropertyRate(0.12);
    EXPECT_DOUBLE_EQ(calc.getCommercialPropertyRate(), 0.12);
}

TEST_F(CommissionCalculatorTest, SetSeniorAgentBonus) {
    CommissionCalculator calc(0.05, 0.1, 0.08, 0.02, 500000.0, 0.6, "standard");
    
    calc.setSeniorAgentBonus(0.05);
    EXPECT_DOUBLE_EQ(calc.getSeniorAgentBonus(), 0.05);
}

TEST_F(CommissionCalculatorTest, CalculateCommissionWithContract) {
    CommissionCalculator calc(0.05, 0.1, 0.08, 0.02, 500000.0, 0.6, "standard");
    
    double commission = calc.calculateCommission(contract, seniorAgent);
    double expected = contract->getMonthlyRent() * 12 * 0.05 * calc.calculatePerformanceMultiplier(seniorAgent);
    
    EXPECT_DOUBLE_EQ(commission, expected);
}

TEST_F(CommissionCalculatorTest, CalculateCommissionWithNullContract) {
    CommissionCalculator calc(0.05, 0.1, 0.08, 0.02, 500000.0, 0.6, "standard");
    
    double commission = calc.calculateCommission(nullptr, seniorAgent);
    EXPECT_DOUBLE_EQ(commission, 0.0);
}

TEST_F(CommissionCalculatorTest, CalculateCommissionWithNullAgent) {
    CommissionCalculator calc(0.05, 0.1, 0.08, 0.02, 500000.0, 0.6, "standard");
    
    double commission = calc.calculateCommission(contract, nullptr);
    EXPECT_DOUBLE_EQ(commission, 0.0);
}

TEST_F(CommissionCalculatorTest, CalculateCommissionWithBothNull) {
    CommissionCalculator calc(0.05, 0.1, 0.08, 0.02, 500000.0, 0.6, "standard");
    
    double commission = calc.calculateCommission(nullptr, nullptr);
    EXPECT_DOUBLE_EQ(commission, 0.0);
}

TEST_F(CommissionCalculatorTest, CalculateLuxuryCommission) {
    CommissionCalculator calc(0.05, 0.1, 0.08, 0.02, 500000.0, 0.6, "standard");
    
    double commission = calc.calculateLuxuryCommission(1000000.0);
    EXPECT_DOUBLE_EQ(commission, 1000000.0 * 0.1);
}

TEST_F(CommissionCalculatorTest, CalculateCommercialCommission) {
    CommissionCalculator calc(0.05, 0.1, 0.08, 0.02, 500000.0, 0.6, "standard");
    
    double commission = calc.calculateCommercialCommission(120000.0);
    EXPECT_DOUBLE_EQ(commission, 120000.0 * 0.08);
}

TEST_F(CommissionCalculatorTest, CalculateSeniorAgentBonusForSeniorAgent) {
    CommissionCalculator calc(0.05, 0.1, 0.08, 0.02, 500000.0, 0.6, "standard");
    
    double bonus = calc.calculateSeniorAgentBonus(10000.0, seniorAgent);
    EXPECT_DOUBLE_EQ(bonus, 10000.0 * 0.02);
}

TEST_F(CommissionCalculatorTest, CalculateSeniorAgentBonusForJuniorAgent) {
    CommissionCalculator calc(0.05, 0.1, 0.08, 0.02, 500000.0, 0.6, "standard");
    
    double bonus = calc.calculateSeniorAgentBonus(10000.0, juniorAgent);
    EXPECT_DOUBLE_EQ(bonus, 0.0);
}

TEST_F(CommissionCalculatorTest, CalculateSeniorAgentBonusWithNullAgent) {
    CommissionCalculator calc(0.05, 0.1, 0.08, 0.02, 500000.0, 0.6, "standard");
    
    double bonus = calc.calculateSeniorAgentBonus(10000.0, nullptr);
    EXPECT_DOUBLE_EQ(bonus, 0.0);
}

TEST_F(CommissionCalculatorTest, CalculateVolumeDiscountBelowThreshold) {
    CommissionCalculator calc(0.05, 0.1, 0.08, 0.02, 500000.0, 0.6, "standard");
    
    double discounted = calc.calculateVolumeDiscount(20000.0, 400000.0);
    EXPECT_DOUBLE_EQ(discounted, 20000.0);
}

TEST_F(CommissionCalculatorTest, CalculateVolumeDiscountAtHighLevel) {
    CommissionCalculator calc(0.05, 0.1, 0.08, 0.02, 500000.0, 0.6, "standard");
    
    double discounted = calc.calculateVolumeDiscount(20000.0, 1000000.0);
    EXPECT_DOUBLE_EQ(discounted, 20000.0 * 0.90);
}

TEST_F(CommissionCalculatorTest, CalculateVolumeDiscountAtMidLevel) {
    CommissionCalculator calc(0.05, 0.1, 0.08, 0.02, 500000.0, 0.6, "standard");
    
    double discounted = calc.calculateVolumeDiscount(20000.0, 750000.0);
    EXPECT_DOUBLE_EQ(discounted, 20000.0 * 0.95);
}

TEST_F(CommissionCalculatorTest, CalculateTeamCommissionSingleMember) {
    CommissionCalculator calc(0.05, 0.1, 0.08, 0.02, 500000.0, 0.6, "standard");
    
    double teamCommission = calc.calculateTeamCommission(10000.0, 1);
    EXPECT_DOUBLE_EQ(teamCommission, 10000.0);
}

TEST_F(CommissionCalculatorTest, CalculateTeamCommissionMultipleMembers) {
    CommissionCalculator calc(0.05, 0.1, 0.08, 0.02, 500000.0, 0.6, "standard");
    
    double teamCommission = calc.calculateTeamCommission(10000.0, 3);
    EXPECT_DOUBLE_EQ(teamCommission, 10000.0 * 0.6);
}

TEST_F(CommissionCalculatorTest, CalculateNetCommission) {
    CommissionCalculator calc(0.05, 0.1, 0.08, 0.02, 500000.0, 0.6, "standard");
    
    double netCommission = calc.calculateNetCommission(10000.0, 10.0);
    EXPECT_DOUBLE_EQ(netCommission, 9000.0);
}

TEST_F(CommissionCalculatorTest, CalculateNetCommissionZeroTax) {
    CommissionCalculator calc(0.05, 0.1, 0.08, 0.02, 500000.0, 0.6, "standard");
    
    double netCommission = calc.calculateNetCommission(10000.0, 0.0);
    EXPECT_DOUBLE_EQ(netCommission, 10000.0);
}

TEST_F(CommissionCalculatorTest, QualifiesForBonusByDealSize) {
    CommissionCalculator calc(0.05, 0.1, 0.08, 0.02, 500000.0, 0.6, "standard");
    
    EXPECT_TRUE(calc.qualifiesForBonus(juniorAgent, 600000.0));
}

TEST_F(CommissionCalculatorTest, QualifiesForBonusByExperience) {
    CommissionCalculator calc(0.05, 0.1, 0.08, 0.02, 500000.0, 0.6, "standard");
    
    EXPECT_TRUE(calc.qualifiesForBonus(seniorAgent, 400000.0));
}

TEST_F(CommissionCalculatorTest, DoesNotQualifyForBonus) {
    CommissionCalculator calc(0.05, 0.1, 0.08, 0.02, 500000.0, 0.6, "standard");
    
    EXPECT_FALSE(calc.qualifiesForBonus(juniorAgent, 400000.0));
}

TEST_F(CommissionCalculatorTest, QualifiesForBonusWithNullAgent) {
    CommissionCalculator calc(0.05, 0.1, 0.08, 0.02, 500000.0, 0.6, "standard");
    
    EXPECT_FALSE(calc.qualifiesForBonus(nullptr, 1000000.0));
}

TEST_F(CommissionCalculatorTest, CalculatePerformanceMultiplierForSeniorAgent) {
    CommissionCalculator calc(0.05, 0.1, 0.08, 0.02, 500000.0, 0.6, "standard");
    
    double multiplier = calc.calculatePerformanceMultiplier(seniorAgent);
    EXPECT_DOUBLE_EQ(multiplier, 1.2);
}

TEST_F(CommissionCalculatorTest, CalculatePerformanceMultiplierForJuniorAgent) {
    CommissionCalculator calc(0.05, 0.1, 0.08, 0.02, 500000.0, 0.6, "standard");
    
    double multiplier = calc.calculatePerformanceMultiplier(juniorAgent);
    EXPECT_DOUBLE_EQ(multiplier, 1.0);
}

TEST_F(CommissionCalculatorTest, CalculatePerformanceMultiplierWithNullAgent) {
    CommissionCalculator calc(0.05, 0.1, 0.08, 0.02, 500000.0, 0.6, "standard");
    
    double multiplier = calc.calculatePerformanceMultiplier(nullptr);
    EXPECT_DOUBLE_EQ(multiplier, 1.0);
}

TEST_F(CommissionCalculatorTest, CalculatePerformanceMultiplierForAgentWith5YearsExp) {
    auto agentAddress = std::make_unique<Address>("123 Main St", "City", "State", "12345");
    auto agent5Years = std::make_shared<Agent>("A3", "Mike", "Jones", "mike@example.com", "1234567890",
                                              "LIC33333", std::move(agentAddress), 4.0, "residential", 5, false,
                                              std::vector<std::string>{"Cert1", "Cert2"});
    
    CommissionCalculator calc(0.05, 0.1, 0.08, 0.02, 500000.0, 0.6, "standard");
    
    double multiplier = calc.calculatePerformanceMultiplier(agent5Years);
    EXPECT_DOUBLE_EQ(multiplier, 1.05);
}

TEST_F(CommissionCalculatorTest, CalculatePerformanceMultiplierForAgentWith10YearsExp) {
    auto agentAddress = std::make_unique<Address>("123 Main St", "City", "State", "12345");
    auto agent10Years = std::make_shared<Agent>("A4", "Sarah", "Connor", "sarah@example.com", "1234567890",
                                               "LIC44444", std::move(agentAddress), 4.5, "residential", 10, false,
                                               std::vector<std::string>{"Cert1", "Cert2", "Cert3"});
    
    CommissionCalculator calc(0.05, 0.1, 0.08, 0.02, 500000.0, 0.6, "standard");
    
    double multiplier = calc.calculatePerformanceMultiplier(agent10Years);
    EXPECT_DOUBLE_EQ(multiplier, 1.15);
}

TEST_F(CommissionCalculatorTest, CalculatePerformanceMultiplierForAgentWith3Certifications) {
    auto agentAddress = std::make_unique<Address>("123 Main St", "City", "State", "12345");
    auto agent3Certs = std::make_shared<Agent>("A5", "Tom", "Brown", "tom@example.com", "1234567890",
                                              "LIC55555", std::move(agentAddress), 3.5, "residential", 2, false,
                                              std::vector<std::string>{"Cert1", "Cert2", "Cert3"});
    
    CommissionCalculator calc(0.05, 0.1, 0.08, 0.02, 500000.0, 0.6, "standard");
    
    double multiplier = calc.calculatePerformanceMultiplier(agent3Certs);
    EXPECT_DOUBLE_EQ(multiplier, 1.05);
}

TEST_F(CommissionCalculatorTest, GetCommissionTierPlatinum) {
    CommissionCalculator calc(0.05, 0.1, 0.08, 0.02, 500000.0, 0.6, "standard");
    
    EXPECT_EQ(calc.getCommissionTier(60000.0), "platinum");
    EXPECT_EQ(calc.getCommissionTier(50001.0), "platinum");
}

TEST_F(CommissionCalculatorTest, GetCommissionTierGold) {
    CommissionCalculator calc(0.05, 0.1, 0.08, 0.02, 500000.0, 0.6, "standard");
    
    EXPECT_EQ(calc.getCommissionTier(40000.0), "gold");
    EXPECT_EQ(calc.getCommissionTier(25001.0), "gold");
}

TEST_F(CommissionCalculatorTest, GetCommissionTierSilver) {
    CommissionCalculator calc(0.05, 0.1, 0.08, 0.02, 500000.0, 0.6, "standard");
    
    EXPECT_EQ(calc.getCommissionTier(15000.0), "silver");
    EXPECT_EQ(calc.getCommissionTier(10001.0), "silver");
}

TEST_F(CommissionCalculatorTest, GetCommissionTierBronze) {
    CommissionCalculator calc(0.05, 0.1, 0.08, 0.02, 500000.0, 0.6, "standard");
    
    EXPECT_EQ(calc.getCommissionTier(8000.0), "bronze");
    EXPECT_EQ(calc.getCommissionTier(5001.0), "bronze");
}

TEST_F(CommissionCalculatorTest, GetCommissionTierStandard) {
    CommissionCalculator calc(0.05, 0.1, 0.08, 0.02, 500000.0, 0.6, "standard");
    
    EXPECT_EQ(calc.getCommissionTier(4000.0), "standard");
    EXPECT_EQ(calc.getCommissionTier(0.0), "standard");
}

TEST_F(CommissionCalculatorTest, EdgeCases) {
    CommissionCalculator calc(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, "zero");
    
    EXPECT_DOUBLE_EQ(calc.getBaseCommissionRate(), 0.0);
    EXPECT_DOUBLE_EQ(calc.calculateCommission(contract, seniorAgent), 0.0);
    EXPECT_DOUBLE_EQ(calc.calculateLuxuryCommission(1000000.0), 0.0);
    EXPECT_DOUBLE_EQ(calc.calculateTeamCommission(10000.0, 5), 0.0);
    EXPECT_EQ(calc.getCommissionTier(-1000.0), "standard");
}

TEST_F(CommissionCalculatorTest, SetRatesWithNegativeValues) {
    CommissionCalculator calc(0.05, 0.1, 0.08, 0.02, 500000.0, 0.6, "standard");
    
    calc.setBaseCommissionRate(-0.1);
    calc.setLuxuryPropertyRate(-0.2);
    calc.setCommercialPropertyRate(-0.3);
    calc.setSeniorAgentBonus(-0.4);
    
    EXPECT_DOUBLE_EQ(calc.getBaseCommissionRate(), 0.0);
    EXPECT_DOUBLE_EQ(calc.getLuxuryPropertyRate(), 0.0);
    EXPECT_DOUBLE_EQ(calc.getCommercialPropertyRate(), 0.0);
    EXPECT_DOUBLE_EQ(calc.getSeniorAgentBonus(), 0.0);
}

TEST_F(CommissionCalculatorTest, SelfAssignment) {
    CommissionCalculator calc(0.05, 0.1, 0.08, 0.02, 500000.0, 0.6, "standard");
    
    CommissionCalculator& ref = calc;
    calc = ref;
    
    EXPECT_DOUBLE_EQ(calc.getBaseCommissionRate(), 0.05);
    EXPECT_DOUBLE_EQ(calc.getLuxuryPropertyRate(), 0.1);
}