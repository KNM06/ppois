#include "../../include/core/Agency.h"
#include <algorithm>
#include <cmath>

Agency::Agency(const std::string& id, const std::string& name, const std::string& regNumber,
               std::unique_ptr<Address> headquarters, const std::string& phone,
               const std::string& email, const std::string& website, int foundingYear,
               int agentCount, double totalCommission, const std::vector<std::shared_ptr<Agent>>& agents,
               const std::vector<std::string>& serviceAreas)
    : agencyId(id), name(name), registrationNumber(regNumber),
      headquarters(std::move(headquarters)), phoneNumber(phone), email(email),
      website(website), foundingYear(foundingYear), numberOfAgents(agentCount),
      totalCommissionEarned(totalCommission), agents(agents), serviceAreas(serviceAreas) {}

Agency::Agency(const Agency& other)
    : agencyId(other.agencyId), name(other.name), registrationNumber(other.registrationNumber),
      headquarters(other.headquarters ? std::make_unique<Address>(*other.headquarters) : nullptr),
      phoneNumber(other.phoneNumber), email(other.email), website(other.website),
      foundingYear(other.foundingYear), numberOfAgents(other.numberOfAgents),
      totalCommissionEarned(other.totalCommissionEarned), agents(other.agents),
      serviceAreas(other.serviceAreas) {}

Agency& Agency::operator=(const Agency& other) {
    if (this != &other) {
        agencyId = other.agencyId;
        name = other.name;
        registrationNumber = other.registrationNumber;
        headquarters = other.headquarters ? std::make_unique<Address>(*other.headquarters) : nullptr;
        phoneNumber = other.phoneNumber;
        email = other.email;
        website = other.website;
        foundingYear = other.foundingYear;
        numberOfAgents = other.numberOfAgents;
        totalCommissionEarned = other.totalCommissionEarned;
        agents = other.agents;
        serviceAreas = other.serviceAreas;
    }
    return *this;
}

Agency::Agency(Agency&& other) noexcept
    : agencyId(std::move(other.agencyId)), name(std::move(other.name)),
      registrationNumber(std::move(other.registrationNumber)),
      headquarters(std::move(other.headquarters)), phoneNumber(std::move(other.phoneNumber)),
      email(std::move(other.email)), website(std::move(other.website)),
      foundingYear(other.foundingYear), numberOfAgents(other.numberOfAgents),
      totalCommissionEarned(other.totalCommissionEarned), agents(std::move(other.agents)),
      serviceAreas(std::move(other.serviceAreas)) {}

Agency& Agency::operator=(Agency&& other) noexcept {
    if (this != &other) {
        agencyId = std::move(other.agencyId);
        name = std::move(other.name);
        registrationNumber = std::move(other.registrationNumber);
        headquarters = std::move(other.headquarters);
        phoneNumber = std::move(other.phoneNumber);
        email = std::move(other.email);
        website = std::move(other.website);
        foundingYear = other.foundingYear;
        numberOfAgents = other.numberOfAgents;
        totalCommissionEarned = other.totalCommissionEarned;
        agents = std::move(other.agents);
        serviceAreas = std::move(other.serviceAreas);
    }
    return *this;
}

Agency::~Agency() {}

std::string Agency::getAgencyId() const { return agencyId; }
std::string Agency::getName() const { return name; }
std::string Agency::getRegistrationNumber() const { return registrationNumber; }
Address* Agency::getHeadquarters() const { return headquarters.get(); }
std::string Agency::getPhoneNumber() const { return phoneNumber; }
std::string Agency::getEmail() const { return email; }
std::string Agency::getWebsite() const { return website; }
int Agency::getFoundingYear() const { return foundingYear; }
int Agency::getNumberOfAgents() const { return numberOfAgents; }
double Agency::getTotalCommissionEarned() const { return totalCommissionEarned; }
std::vector<std::shared_ptr<Agent>> Agency::getAgents() const { return agents; }
std::vector<std::string> Agency::getServiceAreas() const { return serviceAreas; }

void Agency::setPhoneNumber(const std::string& newPhone) {
    phoneNumber = newPhone;
}

void Agency::setEmail(const std::string& newEmail) {
    email = newEmail;
}

void Agency::setWebsite(const std::string& newWebsite) {
    website = newWebsite;
}

void Agency::addAgent(std::shared_ptr<Agent> agent) {
    agents.push_back(agent);
    numberOfAgents = agents.size();
}

void Agency::removeAgent(const std::string& agentId) {
    agents.erase(std::remove_if(agents.begin(), agents.end(),
        [&](const std::shared_ptr<Agent>& agent) {
            return agent->getAgentId() == agentId;
        }), agents.end());
    numberOfAgents = agents.size();
}

void Agency::updateTotalCommission(double additionalCommission) {
    totalCommissionEarned += additionalCommission;
}

bool Agency::isRegistered() const {
    return !registrationNumber.empty() && registrationNumber.length() >= 6;
}

int Agency::calculateAgencyAge(int currentYear) const {
    return currentYear - foundingYear;
}

double Agency::calculateAverageAgentCommission() const {
    if (agents.empty()) return 0.0;
    return totalCommissionEarned / agents.size();
}

bool Agency::servesArea(const std::string& area) const {
    for (const auto& serviceArea : serviceAreas) {
        if (serviceArea.find(area) != std::string::npos) {
            return true;
        }
    }
    return false;
}

bool Agency::hasSpecializedAgents(const std::string& specialization) const {
    for (const auto& agent : agents) {
        if (agent->isSpecializedIn(specialization)) {
            return true;
        }
    }
    return false;
}

int Agency::countActiveAgents() const {
    int count = 0;
    for (const auto& agent : agents) {
        if (agent->getIsActive()) {
            count++;
        }
    }
    return count;
}

double Agency::calculateMarketShare(double totalMarketCommission) const {
    if (totalMarketCommission <= 0) return 0.0;
    return (totalCommissionEarned / totalMarketCommission) * 100.0;
}

bool Agency::isPremiumAgency() const {
    return calculateAgencyAge(2024) >= 10 && 
           totalCommissionEarned > 1000000.0 && 
           countActiveAgents() >= 10;
}

std::vector<std::shared_ptr<Agent>> Agency::getTopPerformingAgents(double minCommission) const {
    std::vector<std::shared_ptr<Agent>> topAgents;
    for (const auto& agent : agents) {
        if (agent->getCommissionRate() >= minCommission && agent->getIsActive()) {
            topAgents.push_back(agent);
        }
    }
    return topAgents;
}