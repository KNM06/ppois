#ifndef AGENCY_H
#define AGENCY_H

#include <string>
#include <memory>
#include <vector>
#include "../../include/core/Address.h"
#include "../../include/core/Agent.h"

class Agency {
private:
    std::string agencyId;
    std::string name;
    std::string registrationNumber;
    std::unique_ptr<Address> headquarters;
    std::string phoneNumber;
    std::string email;
    std::string website;
    int foundingYear;
    int numberOfAgents;
    double totalCommissionEarned;
    std::vector<std::shared_ptr<Agent>> agents;
    std::vector<std::string> serviceAreas;

public:
    Agency(const std::string& id, const std::string& name, const std::string& regNumber,
           std::unique_ptr<Address> headquarters, const std::string& phone,
           const std::string& email, const std::string& website, int foundingYear,
           int agentCount, double totalCommission, const std::vector<std::shared_ptr<Agent>>& agents,
           const std::vector<std::string>& serviceAreas);
    
    Agency(const Agency& other);
    Agency& operator=(const Agency& other);
    Agency(Agency&& other) noexcept;
    Agency& operator=(Agency&& other) noexcept;
    ~Agency();

    std::string getAgencyId() const;
    std::string getName() const;
    std::string getRegistrationNumber() const;
    Address* getHeadquarters() const;
    std::string getPhoneNumber() const;
    std::string getEmail() const;
    std::string getWebsite() const;
    int getFoundingYear() const;
    int getNumberOfAgents() const;
    double getTotalCommissionEarned() const;
    std::vector<std::shared_ptr<Agent>> getAgents() const;
    std::vector<std::string> getServiceAreas() const;

    void setPhoneNumber(const std::string& newPhone);
    void setEmail(const std::string& newEmail);
    void setWebsite(const std::string& newWebsite);
    void addAgent(std::shared_ptr<Agent> agent);
    void removeAgent(const std::string& agentId);
    void updateTotalCommission(double additionalCommission);

    bool isRegistered() const;
    int calculateAgencyAge(int currentYear) const;
    double calculateAverageAgentCommission() const;
    bool servesArea(const std::string& area) const;
    bool hasSpecializedAgents(const std::string& specialization) const;
    int countActiveAgents() const;
    double calculateMarketShare(double totalMarketCommission) const;
    bool isPremiumAgency() const;
    std::vector<std::shared_ptr<Agent>> getTopPerformingAgents(double minCommission) const;
};

#endif