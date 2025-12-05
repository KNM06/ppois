#include <gtest/gtest.h>
#include "../../include/core/Agent.h"
#include "../../include/core/Address.h"
#include <memory>
#include <vector>

class AgentTest : public ::testing::Test {
protected:
    std::unique_ptr<Address> createAddress() {
        return std::make_unique<Address>("123 Business Ave", "Metropolis", "NY", 
                                        "10001", "USA", 40.7128, -74.0060);
    }
    
    std::vector<std::string> createCertifications() {
        return {"CRS", "ABR", "GRI", "SRS"};
    }
    
    std::unique_ptr<Agent> createBasicAgent() {
        return std::make_unique<Agent>("AG1", "John", "Doe", "john@example.com",
                                      "555-123-4567", "LIC12345678", createAddress(),
                                      5.0, "residential,commercial", 8, true,
                                      createCertifications());
    }
};

TEST_F(AgentTest, ConstructorEdgeCases) {
    Agent agent("", "", "", "", "", "", nullptr, -5.0, "", -10, false, std::vector<std::string>());
    
    EXPECT_EQ(agent.getAgentId(), "");
    EXPECT_EQ(agent.getFirstName(), "");
    EXPECT_EQ(agent.getLastName(), "");
    EXPECT_EQ(agent.getEmail(), "");
    EXPECT_EQ(agent.getPhoneNumber(), "");
    EXPECT_EQ(agent.getLicenseNumber(), "");
    EXPECT_EQ(agent.getOfficeAddress(), nullptr);
    EXPECT_DOUBLE_EQ(agent.getCommissionRate(), -5.0);
    EXPECT_EQ(agent.getSpecialization(), "");
    EXPECT_EQ(agent.getYearsOfExperience(), -10);
    EXPECT_FALSE(agent.getIsActive());
    EXPECT_EQ(agent.getCertifications().size(), 0);
}

TEST_F(AgentTest, CopyConstructor) {
    auto agent1 = createBasicAgent();
    agent1->setEmail("updated@example.com");
    agent1->setCommissionRate(6.0);
    agent1->addCertification("NewCert");
    
    Agent agent2(*agent1);
    
    EXPECT_EQ(agent2.getAgentId(), "AG1");
    EXPECT_EQ(agent2.getFirstName(), "John");
    EXPECT_EQ(agent2.getLastName(), "Doe");
    EXPECT_EQ(agent2.getEmail(), "updated@example.com");
    EXPECT_EQ(agent2.getPhoneNumber(), "555-123-4567");
    EXPECT_EQ(agent2.getLicenseNumber(), "LIC12345678");
    EXPECT_NE(agent2.getOfficeAddress(), nullptr);
    EXPECT_DOUBLE_EQ(agent2.getCommissionRate(), 6.0);
    EXPECT_EQ(agent2.getSpecialization(), "residential,commercial");
    EXPECT_EQ(agent2.getYearsOfExperience(), 8);
    EXPECT_TRUE(agent2.getIsActive());
    EXPECT_EQ(agent2.getCertifications().size(), 5);
}

TEST_F(AgentTest, CopyConstructorWithNullAddress) {
    Agent agent1("AG1", "John", "Doe", "john@example.com", "555-123-4567",
                "LIC12345678", nullptr, 5.0, "residential", 8, true,
                createCertifications());
    
    Agent agent2(agent1);
    
    EXPECT_EQ(agent2.getAgentId(), "AG1");
    EXPECT_EQ(agent2.getFirstName(), "John");
    EXPECT_EQ(agent2.getLastName(), "Doe");
    EXPECT_EQ(agent2.getEmail(), "john@example.com");
    EXPECT_EQ(agent2.getPhoneNumber(), "555-123-4567");
    EXPECT_EQ(agent2.getLicenseNumber(), "LIC12345678");
    EXPECT_EQ(agent2.getOfficeAddress(), nullptr);
    EXPECT_DOUBLE_EQ(agent2.getCommissionRate(), 5.0);
    EXPECT_EQ(agent2.getSpecialization(), "residential");
    EXPECT_EQ(agent2.getYearsOfExperience(), 8);
    EXPECT_TRUE(agent2.getIsActive());
    EXPECT_EQ(agent2.getCertifications().size(), 4);
}

TEST_F(AgentTest, CopyAssignment) {
    auto agent1 = createBasicAgent();
    agent1->setEmail("updated@example.com");
    agent1->setCommissionRate(6.0);
    
    auto agent2 = std::make_unique<Agent>("AG2", "Jane", "Smith", "jane@example.com",
                                         "555-987-6543", "LIC87654321", createAddress(),
                                         4.0, "luxury", 5, false,
                                         std::vector<std::string>{"Cert1"});
    
    *agent2 = *agent1;
    
    EXPECT_EQ(agent2->getAgentId(), "AG1");
    EXPECT_EQ(agent2->getFirstName(), "John");
    EXPECT_EQ(agent2->getLastName(), "Doe");
    EXPECT_EQ(agent2->getEmail(), "updated@example.com");
    EXPECT_EQ(agent2->getPhoneNumber(), "555-123-4567");
    EXPECT_EQ(agent2->getLicenseNumber(), "LIC12345678");
    EXPECT_NE(agent2->getOfficeAddress(), nullptr);
    EXPECT_DOUBLE_EQ(agent2->getCommissionRate(), 6.0);
    EXPECT_EQ(agent2->getSpecialization(), "residential,commercial");
    EXPECT_EQ(agent2->getYearsOfExperience(), 8);
    EXPECT_TRUE(agent2->getIsActive());
    EXPECT_EQ(agent2->getCertifications().size(), 4);
}

TEST_F(AgentTest, MoveConstructor) {
    auto agent1 = createBasicAgent();
    Agent agent2(std::move(*agent1));
    
    EXPECT_EQ(agent2.getAgentId(), "AG1");
    EXPECT_EQ(agent2.getFirstName(), "John");
    EXPECT_EQ(agent2.getLastName(), "Doe");
    EXPECT_EQ(agent2.getEmail(), "john@example.com");
    EXPECT_EQ(agent2.getPhoneNumber(), "555-123-4567");
    EXPECT_EQ(agent2.getLicenseNumber(), "LIC12345678");
    EXPECT_NE(agent2.getOfficeAddress(), nullptr);
    EXPECT_DOUBLE_EQ(agent2.getCommissionRate(), 5.0);
    EXPECT_EQ(agent2.getSpecialization(), "residential,commercial");
    EXPECT_EQ(agent2.getYearsOfExperience(), 8);
    EXPECT_TRUE(agent2.getIsActive());
    EXPECT_EQ(agent2.getCertifications().size(), 4);
}

TEST_F(AgentTest, MoveAssignment) {
    auto agent1 = createBasicAgent();
    auto agent2 = std::make_unique<Agent>("AG2", "Jane", "Smith", "jane@example.com",
                                         "555-987-6543", "LIC87654321", createAddress(),
                                         4.0, "luxury", 5, false,
                                         std::vector<std::string>{"Cert1"});
    
    *agent2 = std::move(*agent1);
    
    EXPECT_EQ(agent2->getAgentId(), "AG1");
    EXPECT_EQ(agent2->getFirstName(), "John");
    EXPECT_EQ(agent2->getLastName(), "Doe");
    EXPECT_EQ(agent2->getEmail(), "john@example.com");
    EXPECT_EQ(agent2->getPhoneNumber(), "555-123-4567");
    EXPECT_EQ(agent2->getLicenseNumber(), "LIC12345678");
    EXPECT_NE(agent2->getOfficeAddress(), nullptr);
    EXPECT_DOUBLE_EQ(agent2->getCommissionRate(), 5.0);
    EXPECT_EQ(agent2->getSpecialization(), "residential,commercial");
    EXPECT_EQ(agent2->getYearsOfExperience(), 8);
    EXPECT_TRUE(agent2->getIsActive());
    EXPECT_EQ(agent2->getCertifications().size(), 4);
}

TEST_F(AgentTest, SetCommissionRateEdgeCases) {
    auto agent = createBasicAgent();
    
    agent->setCommissionRate(-10.0);
    EXPECT_DOUBLE_EQ(agent->getCommissionRate(), 0.0);
    
    agent->setCommissionRate(0.0);
    EXPECT_DOUBLE_EQ(agent->getCommissionRate(), 0.0);
    
    agent->setCommissionRate(50.0);
    EXPECT_DOUBLE_EQ(agent->getCommissionRate(), 50.0);
    
    agent->setCommissionRate(100.0);
    EXPECT_DOUBLE_EQ(agent->getCommissionRate(), 100.0);
    
    agent->setCommissionRate(150.0);
    EXPECT_DOUBLE_EQ(agent->getCommissionRate(), 100.0);
    
    agent->setCommissionRate(75.5);
    EXPECT_DOUBLE_EQ(agent->getCommissionRate(), 75.5);
}

TEST_F(AgentTest, UpdateExperienceEdgeCases) {
    auto agent = createBasicAgent();
    
    agent->updateExperience(-10);
    EXPECT_EQ(agent->getYearsOfExperience(), 0);
    
    agent->updateExperience(0);
    EXPECT_EQ(agent->getYearsOfExperience(), 0);
    
    agent->updateExperience(5);
    EXPECT_EQ(agent->getYearsOfExperience(), 5);
    
    agent->updateExperience(30);
    EXPECT_EQ(agent->getYearsOfExperience(), 30);
    
    agent->updateExperience(100);
    EXPECT_EQ(agent->getYearsOfExperience(), 100);
}

TEST_F(AgentTest, AddCertificationEdgeCases) {
    auto agent = createBasicAgent();
    
    size_t initialSize = agent->getCertifications().size();
    
    agent->addCertification("");
    EXPECT_EQ(agent->getCertifications().size(), initialSize + 1);
    EXPECT_EQ(agent->getCertifications()[initialSize], "");
    
    agent->addCertification("  ");
    EXPECT_EQ(agent->getCertifications().size(), initialSize + 2);
    EXPECT_EQ(agent->getCertifications()[initialSize + 1], "  ");
    
    agent->addCertification("New Certification");
    EXPECT_EQ(agent->getCertifications().size(), initialSize + 3);
    EXPECT_EQ(agent->getCertifications()[initialSize + 2], "New Certification");
}

TEST_F(AgentTest, GetFullNameEdgeCases) {
    Agent agent1("AG1", "", "", "test@example.com", "555-1234", "LIC123", nullptr,
                5.0, "residential", 5, true, std::vector<std::string>());
    EXPECT_EQ(agent1.getFullName(), " ");
    
    Agent agent2("AG2", "John", "", "test@example.com", "555-1234", "LIC123", nullptr,
                5.0, "residential", 5, true, std::vector<std::string>());
    EXPECT_EQ(agent2.getFullName(), "John ");
    
    Agent agent3("AG3", "", "Doe", "test@example.com", "555-1234", "LIC123", nullptr,
                5.0, "residential", 5, true, std::vector<std::string>());
    EXPECT_EQ(agent3.getFullName(), " Doe");
    
    Agent agent4("AG4", "  ", "  ", "test@example.com", "555-1234", "LIC123", nullptr,
                5.0, "residential", 5, true, std::vector<std::string>());
    EXPECT_EQ(agent4.getFullName(), "     ");
}

TEST_F(AgentTest, IsLicenseValidEdgeCases) {
    Agent agent1("AG1", "John", "Doe", "test@example.com", "555-1234", "LIC123456", nullptr,
                5.0, "residential", 5, true, std::vector<std::string>());
    EXPECT_TRUE(agent1.isLicenseValid());
    
    Agent agent2("AG2", "Jane", "Smith", "test@example.com", "555-1234", "LIC12345", nullptr,
                5.0, "residential", 5, true, std::vector<std::string>());
    EXPECT_TRUE(agent2.isLicenseValid());
    
    Agent agent3("AG3", "Bob", "Johnson", "test@example.com", "555-1234", "", nullptr,
                5.0, "residential", 5, true, std::vector<std::string>());
    EXPECT_FALSE(agent3.isLicenseValid());
    
    Agent agent4("AG4", "Alice", "Brown", "test@example.com", "555-1234", "LIC12345678", nullptr,
                5.0, "residential", 5, false, std::vector<std::string>());
    EXPECT_FALSE(agent4.isLicenseValid());
    
    Agent agent5("AG5", "Charlie", "Davis", "test@example.com", "555-1234", "  ", nullptr,
                5.0, "residential", 5, true, std::vector<std::string>());
    EXPECT_FALSE(agent5.isLicenseValid());
}

TEST_F(AgentTest, CalculateExpectedCommissionEdgeCases) {
    auto agent = createBasicAgent();
    agent->setCommissionRate(5.0);
    
    double commission1 = agent->calculateExpectedCommission(0.0);
    EXPECT_DOUBLE_EQ(commission1, 0.0);
    
    double commission2 = agent->calculateExpectedCommission(100000.0);
    EXPECT_DOUBLE_EQ(commission2, 100000.0 * 0.05);
    
    double commission3 = agent->calculateExpectedCommission(-100000.0);
    EXPECT_DOUBLE_EQ(commission3, -100000.0 * 0.05);
    
    agent->setCommissionRate(0.0);
    double commission4 = agent->calculateExpectedCommission(100000.0);
    EXPECT_DOUBLE_EQ(commission4, 0.0);
    
    agent->setCommissionRate(-5.0);
    double commission5 = agent->calculateExpectedCommission(100000.0);
    EXPECT_DOUBLE_EQ(commission5, 0.0);
}

TEST_F(AgentTest, IsSpecializedInEdgeCases) {
    Agent agent1("AG1", "John", "Doe", "test@example.com", "555-1234", "LIC123", nullptr,
                5.0, "residential,commercial,luxury", 5, true, std::vector<std::string>());
    
    EXPECT_TRUE(agent1.isSpecializedIn("residential"));
    EXPECT_TRUE(agent1.isSpecializedIn("commercial"));
    EXPECT_TRUE(agent1.isSpecializedIn("luxury"));
    EXPECT_TRUE(agent1.isSpecializedIn("cial")); 
    EXPECT_FALSE(agent1.isSpecializedIn("industrial"));
    EXPECT_TRUE(agent1.isSpecializedIn(""));
    EXPECT_TRUE(agent1.isSpecializedIn("res"));
    
    Agent agent2("AG2", "Jane", "Smith", "test@example.com", "555-1234", "LIC123", nullptr,
                5.0, "", 5, true, std::vector<std::string>());
    
    EXPECT_FALSE(agent2.isSpecializedIn("any"));
    
    Agent agent3("AG3", "Bob", "Johnson", "test@example.com", "555-1234", "LIC123", nullptr,
                5.0, "  ", 5, true, std::vector<std::string>());
    
    EXPECT_FALSE(agent3.isSpecializedIn("any"));
}

TEST_F(AgentTest, CanHandleCommercialPropertiesAllCases) {
    Agent agent1("AG1", "John", "Doe", "test@example.com", "555-1234", "LIC123", nullptr,
                5.0, "commercial", 5, true, std::vector<std::string>());
    EXPECT_TRUE(agent1.canHandleCommercialProperties());
    
    Agent agent2("AG2", "Jane", "Smith", "test@example.com", "555-1234", "LIC123", nullptr,
                5.0, "office", 5, true, std::vector<std::string>());
    EXPECT_TRUE(agent2.canHandleCommercialProperties());
    
    Agent agent3("AG3", "Bob", "Johnson", "test@example.com", "555-1234", "LIC123", nullptr,
                5.0, "retail", 5, true, std::vector<std::string>());
    EXPECT_TRUE(agent3.canHandleCommercialProperties());
    
    Agent agent4("AG4", "Alice", "Brown", "test@example.com", "555-1234", "LIC123", nullptr,
                5.0, "commercial_office_retail", 5, true, std::vector<std::string>());
    EXPECT_TRUE(agent4.canHandleCommercialProperties());
    
    Agent agent5("AG5", "Charlie", "Davis", "test@example.com", "555-1234", "LIC123", nullptr,
                5.0, "residential", 5, true, std::vector<std::string>());
    EXPECT_FALSE(agent5.canHandleCommercialProperties());
    
    Agent agent6("AG6", "Eve", "Wilson", "test@example.com", "555-1234", "LIC123", nullptr,
                5.0, "", 5, true, std::vector<std::string>());
    EXPECT_FALSE(agent6.canHandleCommercialProperties());
}

TEST_F(AgentTest, CanHandleLuxuryPropertiesAllCases) {
    Agent agent1("AG1", "John", "Doe", "test@example.com", "555-1234", "LIC123", nullptr,
                5.0, "luxury", 5, true, std::vector<std::string>());
    EXPECT_TRUE(agent1.canHandleLuxuryProperties());
    
    Agent agent2("AG2", "Jane", "Smith", "test@example.com", "555-1234", "LIC123", nullptr,
                5.0, "high_end", 5, true, std::vector<std::string>());
    EXPECT_TRUE(agent2.canHandleLuxuryProperties());
    
    Agent agent3("AG3", "Bob", "Johnson", "test@example.com", "555-1234", "LIC123", nullptr,
                5.0, "residential", 15, true, std::vector<std::string>());
    EXPECT_TRUE(agent3.canHandleLuxuryProperties());
    
    Agent agent4("AG4", "Alice", "Brown", "test@example.com", "555-1234", "LIC123", nullptr,
                5.0, "luxury_high_end", 8, true, std::vector<std::string>());
    EXPECT_TRUE(agent4.canHandleLuxuryProperties());
    
    Agent agent5("AG5", "Charlie", "Davis", "test@example.com", "555-1234", "LIC123", nullptr,
                5.0, "residential", 8, true, std::vector<std::string>());
    EXPECT_FALSE(agent5.canHandleLuxuryProperties());
    
    Agent agent6("AG6", "Eve", "Wilson", "test@example.com", "555-1234", "LIC123", nullptr,
                5.0, "", 5, true, std::vector<std::string>());
    EXPECT_FALSE(agent6.canHandleLuxuryProperties());
}

TEST_F(AgentTest, GetCertificationCountEdgeCases) {
    Agent agent1("AG1", "John", "Doe", "test@example.com", "555-1234", "LIC123", nullptr,
                5.0, "residential", 5, true, std::vector<std::string>());
    EXPECT_EQ(agent1.getCertificationCount(), 0);
    
    Agent agent2("AG2", "Jane", "Smith", "test@example.com", "555-1234", "LIC123", nullptr,
                5.0, "residential", 5, true, std::vector<std::string>{"Cert1"});
    EXPECT_EQ(agent2.getCertificationCount(), 1);
    
    Agent agent3("AG3", "Bob", "Johnson", "test@example.com", "555-1234", "LIC123", nullptr,
                5.0, "residential", 5, true, std::vector<std::string>{"Cert1", "Cert2", "Cert3"});
    EXPECT_EQ(agent3.getCertificationCount(), 3);
    
    Agent agent4("AG4", "Alice", "Brown", "test@example.com", "555-1234", "LIC123", nullptr,
                5.0, "residential", 5, true, std::vector<std::string>{"", "  "});
    EXPECT_EQ(agent4.getCertificationCount(), 2);
}

TEST_F(AgentTest, MeetsExperienceRequirementsEdgeCases) {
    Agent agent1("AG1", "John", "Doe", "test@example.com", "555-1234", "LIC123", nullptr,
                5.0, "residential", 0, true, std::vector<std::string>());
    
    EXPECT_TRUE(agent1.meetsExperienceRequirements(0));
    EXPECT_FALSE(agent1.meetsExperienceRequirements(1));
    
    Agent agent2("AG2", "Jane", "Smith", "test@example.com", "555-1234", "LIC123", nullptr,
                5.0, "residential", 5, true, std::vector<std::string>());
    
    EXPECT_TRUE(agent2.meetsExperienceRequirements(5));
    EXPECT_TRUE(agent2.meetsExperienceRequirements(4));
    EXPECT_FALSE(agent2.meetsExperienceRequirements(6));
    
    Agent agent3("AG3", "Bob", "Johnson", "test@example.com", "555-1234", "LIC123", nullptr,
                5.0, "residential", -5, true, std::vector<std::string>());
    
    EXPECT_TRUE(agent3.meetsExperienceRequirements(-10));
    EXPECT_FALSE(agent3.meetsExperienceRequirements(-4));
    
    Agent agent4("AG4", "Alice", "Brown", "test@example.com", "555-1234", "LIC123", nullptr,
                5.0, "residential", 10, true, std::vector<std::string>());
    
    EXPECT_TRUE(agent4.meetsExperienceRequirements(-5));
    EXPECT_TRUE(agent4.meetsExperienceRequirements(10));
}

TEST_F(AgentTest, CalculateSeniorityBonusAllCases) {
    Agent agent1("AG1", "John", "Doe", "test@example.com", "555-1234", "LIC123", nullptr,
                5.0, "residential", 0, true, std::vector<std::string>());
    EXPECT_DOUBLE_EQ(agent1.calculateSeniorityBonus(), 0.0);
    
    Agent agent2("AG2", "Jane", "Smith", "test@example.com", "555-1234", "LIC123", nullptr,
                5.0, "residential", 3, true, std::vector<std::string>());
    EXPECT_DOUBLE_EQ(agent2.calculateSeniorityBonus(), 0.0);
    
    Agent agent3("AG3", "Bob", "Johnson", "test@example.com", "555-1234", "LIC123", nullptr,
                5.0, "residential", 5, true, std::vector<std::string>());
    EXPECT_DOUBLE_EQ(agent3.calculateSeniorityBonus(), 5.0 * 0.02);
    
    Agent agent4("AG4", "Alice", "Brown", "test@example.com", "555-1234", "LIC123", nullptr,
                6.0, "residential", 9, true, std::vector<std::string>());
    EXPECT_DOUBLE_EQ(agent4.calculateSeniorityBonus(), 6.0 * 0.02);
    
    Agent agent5("AG5", "Charlie", "Davis", "test@example.com", "555-1234", "LIC123", nullptr,
                7.0, "residential", 10, true, std::vector<std::string>());
    EXPECT_DOUBLE_EQ(agent5.calculateSeniorityBonus(), 7.0 * 0.05);
    
    Agent agent6("AG6", "Eve", "Wilson", "test@example.com", "555-1234", "LIC123", nullptr,
                8.0, "residential", 19, true, std::vector<std::string>());
    EXPECT_DOUBLE_EQ(agent6.calculateSeniorityBonus(), 8.0 * 0.05);
    
    Agent agent7("AG7", "Frank", "Moore", "test@example.com", "555-1234", "LIC123", nullptr,
                10.0, "residential", 20, true, std::vector<std::string>());
    EXPECT_DOUBLE_EQ(agent7.calculateSeniorityBonus(), 10.0 * 0.1);
    
    Agent agent8("AG8", "Grace", "Taylor", "test@example.com", "555-1234", "LIC123", nullptr,
                12.0, "residential", 25, true, std::vector<std::string>());
    EXPECT_DOUBLE_EQ(agent8.calculateSeniorityBonus(), 12.0 * 0.1);
}

TEST_F(AgentTest, CalculateSeniorityBonusNegativeCommission) {
    Agent agent("AG1", "John", "Doe", "test@example.com", "555-1234", "LIC123", nullptr,
                -5.0, "residential", 10, true, std::vector<std::string>());
    
    double bonus = agent.calculateSeniorityBonus();
    EXPECT_DOUBLE_EQ(bonus, -5.0 * 0.05);
}

TEST_F(AgentTest, SetContactInfoEdgeCases) {
    auto agent = createBasicAgent();
    
    agent->setEmail("");
    EXPECT_EQ(agent->getEmail(), "");
    
    agent->setEmail("  ");
    EXPECT_EQ(agent->getEmail(), "  ");
    
    agent->setEmail("new.email-with+symbols@example-domain.com");
    EXPECT_EQ(agent->getEmail(), "new.email-with+symbols@example-domain.com");
    
    agent->setPhoneNumber("");
    EXPECT_EQ(agent->getPhoneNumber(), "");
    
    agent->setPhoneNumber("  ");
    EXPECT_EQ(agent->getPhoneNumber(), "  ");
    
    agent->setPhoneNumber("+1 (234) 567-8900 ext. 123");
    EXPECT_EQ(agent->getPhoneNumber(), "+1 (234) 567-8900 ext. 123");
}