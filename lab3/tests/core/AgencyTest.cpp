#include <gtest/gtest.h>
#include "../../include/core/Agency.h"
#include "../../include/core/Agent.h"
#include "../../include/core/Address.h"
#include <memory>
#include <vector>

class AgencyTest : public ::testing::Test {
protected:
    std::unique_ptr<Address> createAddress() {
        return std::make_unique<Address>("123 Agency St", "Metropolis", "NY", 
                                        "10001", "USA", 40.7128, -74.0060);
    }
    
    std::unique_ptr<Address> createAgentAddress() {
        return std::make_unique<Address>("456 Agent Ave", "Metropolis", "NY",
                                        "10002", "USA", 40.7138, -74.0070);
    }
    
    std::shared_ptr<Agent> createAgent(const std::string& id, double commission, 
                                      const std::string& specialization, bool active) {
        return std::make_shared<Agent>(id, "Agent", "Smith", "agent" + id + "@example.com",
                                      "555-010" + id, "LIC" + id + "456",
                                      createAgentAddress(), commission, specialization,
                                      5, active, std::vector<std::string>{"Cert1"});
    }
    
    std::unique_ptr<Agency> createBasicAgency() {
        std::vector<std::shared_ptr<Agent>> agents = {
            createAgent("A1", 5.0, "residential", true),
            createAgent("A2", 7.0, "commercial", true)
        };
        
        return std::make_unique<Agency>("AG1", "Premium Realty", "REG123456789",
                                       createAddress(), "555-123-4567",
                                       "info@premiumrealty.com", "www.premiumrealty.com",
                                       2010, 2, 100000.0, agents,
                                       std::vector<std::string>{"Metropolis", "Gotham"});
    }
};

TEST_F(AgencyTest, ConstructorEdgeCases) {
    Agency agency("", "", "", nullptr, "", "", "", -2024, -5, -100000.0,
                 std::vector<std::shared_ptr<Agent>>(), std::vector<std::string>());
    
    EXPECT_EQ(agency.getAgencyId(), "");
    EXPECT_EQ(agency.getName(), "");
    EXPECT_EQ(agency.getRegistrationNumber(), "");
    EXPECT_EQ(agency.getPhoneNumber(), "");
    EXPECT_EQ(agency.getEmail(), "");
    EXPECT_EQ(agency.getWebsite(), "");
    EXPECT_EQ(agency.getFoundingYear(), -2024);
    EXPECT_EQ(agency.getNumberOfAgents(), -5);
    EXPECT_DOUBLE_EQ(agency.getTotalCommissionEarned(), -100000.0);
    EXPECT_EQ(agency.getAgents().size(), 0);
    EXPECT_EQ(agency.getServiceAreas().size(), 0);
    EXPECT_EQ(agency.getHeadquarters(), nullptr);
}

TEST_F(AgencyTest, CopyConstructor) {
    auto agency1 = createBasicAgency();
    agency1->setPhoneNumber("555-987-6543");
    agency1->setEmail("new@example.com");
    agency1->updateTotalCommission(50000.0);
    
    Agency agency2(*agency1);
    
    EXPECT_EQ(agency2.getAgencyId(), "AG1");
    EXPECT_EQ(agency2.getName(), "Premium Realty");
    EXPECT_EQ(agency2.getRegistrationNumber(), "REG123456789");
    EXPECT_EQ(agency2.getPhoneNumber(), "555-987-6543");
    EXPECT_EQ(agency2.getEmail(), "new@example.com");
    EXPECT_EQ(agency2.getWebsite(), "www.premiumrealty.com");
    EXPECT_EQ(agency2.getFoundingYear(), 2010);
    EXPECT_EQ(agency2.getNumberOfAgents(), 2);
    EXPECT_DOUBLE_EQ(agency2.getTotalCommissionEarned(), 150000.0);
    EXPECT_EQ(agency2.getAgents().size(), 2);
    EXPECT_EQ(agency2.getServiceAreas().size(), 2);
    EXPECT_NE(agency2.getHeadquarters(), nullptr);
}

TEST_F(AgencyTest, CopyConstructorWithNullAddress) {
    Agency agency1("AG1", "Test Agency", "REG123", nullptr, "555-1234", 
                  "test@example.com", "www.test.com", 2020, 0, 0.0,
                  std::vector<std::shared_ptr<Agent>>(), std::vector<std::string>());
    
    Agency agency2(agency1);
    
    EXPECT_EQ(agency2.getAgencyId(), "AG1");
    EXPECT_EQ(agency2.getName(), "Test Agency");
    EXPECT_EQ(agency2.getRegistrationNumber(), "REG123");
    EXPECT_EQ(agency2.getHeadquarters(), nullptr);
    EXPECT_EQ(agency2.getPhoneNumber(), "555-1234");
    EXPECT_EQ(agency2.getEmail(), "test@example.com");
    EXPECT_EQ(agency2.getWebsite(), "www.test.com");
}

TEST_F(AgencyTest, CopyAssignment) {
    auto agency1 = createBasicAgency();
    agency1->setPhoneNumber("555-987-6543");
    agency1->setEmail("new@example.com");
    
    auto agency2 = std::make_unique<Agency>("AG2", "Old Agency", "REG987", 
                                           createAddress(), "555-0000",
                                           "old@example.com", "www.old.com",
                                           2015, 0, 0.0,
                                           std::vector<std::shared_ptr<Agent>>(),
                                           std::vector<std::string>());
    
    *agency2 = *agency1;
    
    EXPECT_EQ(agency2->getAgencyId(), "AG1");
    EXPECT_EQ(agency2->getName(), "Premium Realty");
    EXPECT_EQ(agency2->getRegistrationNumber(), "REG123456789");
    EXPECT_EQ(agency2->getPhoneNumber(), "555-987-6543");
    EXPECT_EQ(agency2->getEmail(), "new@example.com");
    EXPECT_EQ(agency2->getWebsite(), "www.premiumrealty.com");
    EXPECT_EQ(agency2->getFoundingYear(), 2010);
    EXPECT_EQ(agency2->getNumberOfAgents(), 2);
    EXPECT_DOUBLE_EQ(agency2->getTotalCommissionEarned(), 100000.0);
    EXPECT_EQ(agency2->getAgents().size(), 2);
    EXPECT_EQ(agency2->getServiceAreas().size(), 2);
    EXPECT_NE(agency2->getHeadquarters(), nullptr);
}

TEST_F(AgencyTest, MoveConstructor) {
    auto agency1 = createBasicAgency();
    Agency agency2(std::move(*agency1));
    
    EXPECT_EQ(agency2.getAgencyId(), "AG1");
    EXPECT_EQ(agency2.getName(), "Premium Realty");
    EXPECT_EQ(agency2.getRegistrationNumber(), "REG123456789");
    EXPECT_EQ(agency2.getPhoneNumber(), "555-123-4567");
    EXPECT_EQ(agency2.getEmail(), "info@premiumrealty.com");
    EXPECT_EQ(agency2.getWebsite(), "www.premiumrealty.com");
    EXPECT_EQ(agency2.getFoundingYear(), 2010);
    EXPECT_EQ(agency2.getNumberOfAgents(), 2);
    EXPECT_DOUBLE_EQ(agency2.getTotalCommissionEarned(), 100000.0);
    EXPECT_EQ(agency2.getAgents().size(), 2);
    EXPECT_EQ(agency2.getServiceAreas().size(), 2);
    EXPECT_NE(agency2.getHeadquarters(), nullptr);
}

TEST_F(AgencyTest, MoveAssignment) {
    auto agency1 = createBasicAgency();
    auto agency2 = std::make_unique<Agency>("AG2", "Old Agency", "REG987", 
                                           createAddress(), "555-0000",
                                           "old@example.com", "www.old.com",
                                           2015, 0, 0.0,
                                           std::vector<std::shared_ptr<Agent>>(),
                                           std::vector<std::string>());
    
    *agency2 = std::move(*agency1);
    
    EXPECT_EQ(agency2->getAgencyId(), "AG1");
    EXPECT_EQ(agency2->getName(), "Premium Realty");
    EXPECT_EQ(agency2->getRegistrationNumber(), "REG123456789");
    EXPECT_EQ(agency2->getPhoneNumber(), "555-123-4567");
    EXPECT_EQ(agency2->getEmail(), "info@premiumrealty.com");
    EXPECT_EQ(agency2->getWebsite(), "www.premiumrealty.com");
    EXPECT_EQ(agency2->getFoundingYear(), 2010);
    EXPECT_EQ(agency2->getNumberOfAgents(), 2);
    EXPECT_DOUBLE_EQ(agency2->getTotalCommissionEarned(), 100000.0);
    EXPECT_EQ(agency2->getAgents().size(), 2);
    EXPECT_EQ(agency2->getServiceAreas().size(), 2);
    EXPECT_NE(agency2->getHeadquarters(), nullptr);
}

TEST_F(AgencyTest, AddAgent) {
    auto agency = createBasicAgency();
    auto newAgent = createAgent("A3", 6.0, "luxury", true);
    
    size_t initialSize = agency->getAgents().size();
    agency->addAgent(newAgent);
    
    EXPECT_EQ(agency->getAgents().size(), initialSize + 1);
    EXPECT_EQ(agency->getNumberOfAgents(), initialSize + 1);
    EXPECT_TRUE(std::find(agency->getAgents().begin(), agency->getAgents().end(), newAgent) != agency->getAgents().end());
}

TEST_F(AgencyTest, RemoveAgentExisting) {
    auto agency = createBasicAgency();
    auto agentToRemove = agency->getAgents()[0];
    std::string agentId = agentToRemove->getAgentId();
    
    size_t initialSize = agency->getAgents().size();
    agency->removeAgent(agentId);
    
    EXPECT_EQ(agency->getAgents().size(), initialSize - 1);
    EXPECT_EQ(agency->getNumberOfAgents(), initialSize - 1);
    EXPECT_FALSE(std::find(agency->getAgents().begin(), agency->getAgents().end(), agentToRemove) == agency->getAgents().end());
}

TEST_F(AgencyTest, RemoveAgentNonExistent) {
    auto agency = createBasicAgency();
    
    size_t initialSize = agency->getAgents().size();
    agency->removeAgent("NON_EXISTENT_ID");
    
    EXPECT_EQ(agency->getAgents().size(), initialSize);
    EXPECT_EQ(agency->getNumberOfAgents(), initialSize);
}

TEST_F(AgencyTest, UpdateTotalCommission) {
    auto agency = createBasicAgency();
    
    double initialCommission = agency->getTotalCommissionEarned();
    agency->updateTotalCommission(25000.0);
    
    EXPECT_DOUBLE_EQ(agency->getTotalCommissionEarned(), initialCommission + 25000.0);
    
    agency->updateTotalCommission(-5000.0);
    EXPECT_DOUBLE_EQ(agency->getTotalCommissionEarned(), initialCommission + 20000.0);
}

TEST_F(AgencyTest, IsRegisteredEdgeCases) {
    Agency agency1("AG1", "Agency", "REG123", nullptr, "", "", "", 2020, 0, 0.0,
                  std::vector<std::shared_ptr<Agent>>(), std::vector<std::string>());
    EXPECT_TRUE(agency1.isRegistered());
    
    Agency agency2("AG2", "Agency", "REG12", nullptr, "", "", "", 2020, 0, 0.0,
                  std::vector<std::shared_ptr<Agent>>(), std::vector<std::string>());
    EXPECT_FALSE(agency2.isRegistered());
    
    Agency agency3("AG3", "Agency", "", nullptr, "", "", "", 2020, 0, 0.0,
                  std::vector<std::shared_ptr<Agent>>(), std::vector<std::string>());
    EXPECT_FALSE(agency3.isRegistered());
    
    Agency agency4("AG4", "Agency", "REG123456", nullptr, "", "", "", 2020, 0, 0.0,
                  std::vector<std::shared_ptr<Agent>>(), std::vector<std::string>());
    EXPECT_TRUE(agency4.isRegistered());
}

TEST_F(AgencyTest, CalculateAgencyAgeEdgeCases) {
    Agency agency1("AG1", "Agency", "REG123", nullptr, "", "", "", 2020, 0, 0.0,
                  std::vector<std::shared_ptr<Agent>>(), std::vector<std::string>());
    
    EXPECT_EQ(agency1.calculateAgencyAge(2025), 5);
    EXPECT_EQ(agency1.calculateAgencyAge(2020), 0);
    EXPECT_EQ(agency1.calculateAgencyAge(2019), -1);
    EXPECT_EQ(agency1.calculateAgencyAge(2000), -20);
    
    Agency agency2("AG2", "Agency", "REG123", nullptr, "", "", "", -2020, 0, 0.0,
                  std::vector<std::shared_ptr<Agent>>(), std::vector<std::string>());
    
    EXPECT_EQ(agency2.calculateAgencyAge(2025), 4045);
}

TEST_F(AgencyTest, CalculateAverageAgentCommissionEdgeCases) {
    Agency agency1("AG1", "Agency", "REG123", nullptr, "", "", "", 2020, 0, 100000.0,
                  std::vector<std::shared_ptr<Agent>>(), std::vector<std::string>());
    
    double avg1 = agency1.calculateAverageAgentCommission();
    EXPECT_DOUBLE_EQ(avg1, 0.0);
    
    std::vector<std::shared_ptr<Agent>> agents = {
        createAgent("A1", 5.0, "residential", true)
    };
    
    Agency agency2("AG2", "Agency", "REG123", nullptr, "", "", "", 2020, 1, 50000.0,
                  agents, std::vector<std::string>());
    
    double avg2 = agency2.calculateAverageAgentCommission();
    EXPECT_DOUBLE_EQ(avg2, 50000.0);
    
    std::vector<std::shared_ptr<Agent>> agents3 = {
        createAgent("A1", 5.0, "residential", true),
        createAgent("A2", 7.0, "commercial", true)
    };
    
    Agency agency3("AG3", "Agency", "REG123", nullptr, "", "", "", 2020, 2, 120000.0,
                  agents3, std::vector<std::string>());
    
    double avg3 = agency3.calculateAverageAgentCommission();
    EXPECT_DOUBLE_EQ(avg3, 60000.0);
}

TEST_F(AgencyTest, ServesAreaEdgeCases) {
    Agency agency1("AG1", "Agency", "REG123", nullptr, "", "", "", 2020, 0, 0.0,
                  std::vector<std::shared_ptr<Agent>>(), 
                  std::vector<std::string>{"Metropolis", "Gotham City", "New York"});
    
    EXPECT_TRUE(agency1.servesArea("Metropolis"));
    EXPECT_TRUE(agency1.servesArea("Gotham"));
    EXPECT_TRUE(agency1.servesArea("York"));
    EXPECT_FALSE(agency1.servesArea("Smallville"));
    EXPECT_TRUE(agency1.servesArea(""));
    
    Agency agency2("AG2", "Agency", "REG123", nullptr, "", "", "", 2020, 0, 0.0,
                  std::vector<std::shared_ptr<Agent>>(), std::vector<std::string>());
    
    EXPECT_FALSE(agency2.servesArea("Anywhere"));
    
    Agency agency3("AG3", "Agency", "REG123", nullptr, "", "", "", 2020, 0, 0.0,
                  std::vector<std::shared_ptr<Agent>>(), 
                  std::vector<std::string>{"", "  ", "Area"});
    
    EXPECT_TRUE(agency3.servesArea(""));
    EXPECT_TRUE(agency3.servesArea("Area"));
}

TEST_F(AgencyTest, HasSpecializedAgentsEdgeCases) {
    std::vector<std::shared_ptr<Agent>> agents = {
        createAgent("A1", 5.0, "residential,commercial", true),
        createAgent("A2", 7.0, "luxury,high_end", true),
        createAgent("A3", 4.0, "industrial", false)
    };
    
    Agency agency("AG1", "Agency", "REG123", nullptr, "", "", "", 2020, 3, 0.0,
                 agents, std::vector<std::string>());
    
    EXPECT_TRUE(agency.hasSpecializedAgents("residential"));
    EXPECT_TRUE(agency.hasSpecializedAgents("commercial"));
    EXPECT_TRUE(agency.hasSpecializedAgents("luxury"));
    EXPECT_TRUE(agency.hasSpecializedAgents("high_end"));
    EXPECT_TRUE(agency.hasSpecializedAgents("industrial"));
    EXPECT_FALSE(agency.hasSpecializedAgents("agricultural"));
    EXPECT_TRUE(agency.hasSpecializedAgents(""));
    
    Agency agency2("AG2", "Agency", "REG123", nullptr, "", "", "", 2020, 0, 0.0,
                  std::vector<std::shared_ptr<Agent>>(), std::vector<std::string>());
    
    EXPECT_FALSE(agency2.hasSpecializedAgents("any"));
}

TEST_F(AgencyTest, CountActiveAgentsEdgeCases) {
    std::vector<std::shared_ptr<Agent>> agents = {
        createAgent("A1", 5.0, "residential", true),
        createAgent("A2", 7.0, "commercial", true),
        createAgent("A3", 6.0, "luxury", false),
        createAgent("A4", 4.0, "industrial", false)
    };
    
    Agency agency("AG1", "Agency", "REG123", nullptr, "", "", "", 2020, 4, 0.0,
                 agents, std::vector<std::string>());
    
    EXPECT_EQ(agency.countActiveAgents(), 2);
    
    Agency agency2("AG2", "Agency", "REG123", nullptr, "", "", "", 2020, 0, 0.0,
                  std::vector<std::shared_ptr<Agent>>(), std::vector<std::string>());
    
    EXPECT_EQ(agency2.countActiveAgents(), 0);
    
    std::vector<std::shared_ptr<Agent>> agents3 = {
        createAgent("A1", 5.0, "residential", false),
        createAgent("A2", 7.0, "commercial", false)
    };
    
    Agency agency3("AG3", "Agency", "REG123", nullptr, "", "", "", 2020, 2, 0.0,
                  agents3, std::vector<std::string>());
    
    EXPECT_EQ(agency3.countActiveAgents(), 0);
}

TEST_F(AgencyTest, CalculateMarketShareEdgeCases) {
    Agency agency("AG1", "Agency", "REG123", nullptr, "", "", "", 2020, 0, 500000.0,
                 std::vector<std::shared_ptr<Agent>>(), std::vector<std::string>());
    
    double share1 = agency.calculateMarketShare(1000000.0);
    EXPECT_DOUBLE_EQ(share1, 50.0);
    
    double share2 = agency.calculateMarketShare(0.0);
    EXPECT_DOUBLE_EQ(share2, 0.0);
    
    double share3 = agency.calculateMarketShare(-1000000.0);
    EXPECT_DOUBLE_EQ(share3, 0.0);
    
    double share4 = agency.calculateMarketShare(2000000.0);
    EXPECT_DOUBLE_EQ(share4, 25.0);
    
    double share5 = agency.calculateMarketShare(500000.0);
    EXPECT_DOUBLE_EQ(share5, 100.0);
    
    Agency agency2("AG2", "Agency", "REG123", nullptr, "", "", "", 2020, 0, -500000.0,
                  std::vector<std::shared_ptr<Agent>>(), std::vector<std::string>());
    
    double share6 = agency2.calculateMarketShare(1000000.0);
    EXPECT_DOUBLE_EQ(share6, -50.0);
}

TEST_F(AgencyTest, IsPremiumAgencyAllCases) {
    std::vector<std::shared_ptr<Agent>> agents1;
    for (int i = 0; i < 10; i++) {
        agents1.push_back(createAgent("A" + std::to_string(i), 5.0, "residential", true));
    }
    
    Agency agency1("AG1", "Agency", "REG123", nullptr, "", "", "", 2010, 10, 2000000.0,
                  agents1, std::vector<std::string>());
    
    EXPECT_TRUE(agency1.isPremiumAgency());
    
    Agency agency2("AG2", "Agency", "REG123", nullptr, "", "", "", 2020, 10, 2000000.0,
                  agents1, std::vector<std::string>());
    
    EXPECT_FALSE(agency2.isPremiumAgency());
    
    Agency agency3("AG3", "Agency", "REG123", nullptr, "", "", "", 2010, 10, 500000.0,
                  agents1, std::vector<std::string>());
    
    EXPECT_FALSE(agency3.isPremiumAgency());
    
    std::vector<std::shared_ptr<Agent>> agents4;
    for (int i = 0; i < 5; i++) {
        agents4.push_back(createAgent("A" + std::to_string(i), 5.0, "residential", true));
    }
    
    Agency agency4("AG4", "Agency", "REG123", nullptr, "", "", "", 2010, 5, 2000000.0,
                  agents4, std::vector<std::string>());
    
    EXPECT_FALSE(agency4.isPremiumAgency());
    
    std::vector<std::shared_ptr<Agent>> agents5;
    for (int i = 0; i < 10; i++) {
        agents5.push_back(createAgent("A" + std::to_string(i), 5.0, "residential", false));
    }
    
    Agency agency5("AG5", "Agency", "REG123", nullptr, "", "", "", 2010, 10, 2000000.0,
                  agents5, std::vector<std::string>());
    
    EXPECT_FALSE(agency5.isPremiumAgency());
}

TEST_F(AgencyTest, GetTopPerformingAgentsAllCases) {
    std::vector<std::shared_ptr<Agent>> agents = {
        createAgent("A1", 3.0, "residential", true),
        createAgent("A2", 5.0, "commercial", true),
        createAgent("A3", 7.0, "luxury", true),
        createAgent("A4", 6.0, "industrial", false),
        createAgent("A5", 8.0, "commercial", true)
    };
    
    Agency agency("AG1", "Agency", "REG123", nullptr, "", "", "", 2020, 5, 0.0,
                 agents, std::vector<std::string>());
    
    auto topAgents1 = agency.getTopPerformingAgents(5.0);
    EXPECT_EQ(topAgents1.size(), 3);
    
    auto topAgents2 = agency.getTopPerformingAgents(7.0);
    EXPECT_EQ(topAgents2.size(), 2);
    
    auto topAgents3 = agency.getTopPerformingAgents(8.0);
    EXPECT_EQ(topAgents3.size(), 1);
    
    auto topAgents4 = agency.getTopPerformingAgents(10.0);
    EXPECT_EQ(topAgents4.size(), 0);
    
    auto topAgents5 = agency.getTopPerformingAgents(0.0);
    EXPECT_EQ(topAgents5.size(), 4);
    
    auto topAgents6 = agency.getTopPerformingAgents(-5.0);
    EXPECT_EQ(topAgents6.size(), 4);
    
    Agency agency2("AG2", "Agency", "REG123", nullptr, "", "", "", 2020, 0, 0.0,
                  std::vector<std::shared_ptr<Agent>>(), std::vector<std::string>());
    
    auto topAgents7 = agency2.getTopPerformingAgents(5.0);
    EXPECT_EQ(topAgents7.size(), 0);
}

TEST_F(AgencyTest, UpdateAgentCountAfterModifications) {
    auto agency = createBasicAgency();
    
    EXPECT_EQ(agency->getAgents().size(), agency->getNumberOfAgents());
    
    auto newAgent = createAgent("A3", 6.0, "luxury", true);
    agency->addAgent(newAgent);
    
    EXPECT_EQ(agency->getAgents().size(), agency->getNumberOfAgents());
    
    agency->removeAgent("A1");
    
    EXPECT_EQ(agency->getAgents().size(), agency->getNumberOfAgents());
}

TEST_F(AgencyTest, SetContactInfoEdgeCases) {
    auto agency = createBasicAgency();
    
    agency->setPhoneNumber("");
    EXPECT_EQ(agency->getPhoneNumber(), "");
    
    agency->setEmail("  ");
    EXPECT_EQ(agency->getEmail(), "  ");
    
    agency->setWebsite("https://new-website.com/path");
    EXPECT_EQ(agency->getWebsite(), "https://new-website.com/path");
}