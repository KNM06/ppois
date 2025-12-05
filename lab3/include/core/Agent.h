#ifndef AGENT_H
#define AGENT_H

#include <string>
#include <memory>
#include <vector>
#include "../../include/core/Address.h"

class Agent {
private:
    std::string agentId;
    std::string firstName;
    std::string lastName;
    std::string email;
    std::string phoneNumber;
    std::string licenseNumber;
    std::unique_ptr<Address> officeAddress;
    double commissionRate;
    std::string specialization;
    int yearsOfExperience;
    bool isActive;
    std::vector<std::string> certifications;

public:
    Agent(const std::string& id, const std::string& fName, const std::string& lName,
          const std::string& email, const std::string& phone, const std::string& license,
          std::unique_ptr<Address> office, double commission, const std::string& specialization,
          int experience, bool active, const std::vector<std::string>& certs);
    
    Agent(const Agent& other);
    Agent& operator=(const Agent& other);
    Agent(Agent&& other) noexcept;
    Agent& operator=(Agent&& other) noexcept;
    ~Agent();

    std::string getAgentId() const;
    std::string getFirstName() const;
    std::string getLastName() const;
    std::string getEmail() const;
    std::string getPhoneNumber() const;
    std::string getLicenseNumber() const;
    Address* getOfficeAddress() const;
    double getCommissionRate() const;
    std::string getSpecialization() const;
    int getYearsOfExperience() const;
    bool getIsActive() const;
    std::vector<std::string> getCertifications() const;

    void setEmail(const std::string& newEmail);
    void setPhoneNumber(const std::string& newPhone);
    void setCommissionRate(double newRate);
    void updateExperience(int newExperience);
    void addCertification(const std::string& certification);

    std::string getFullName() const;
    bool isLicenseValid() const;
    double calculateExpectedCommission(double propertyValue) const;
    bool isSpecializedIn(const std::string& propertyType) const;
    bool canHandleCommercialProperties() const;
    bool canHandleLuxuryProperties() const;
    int getCertificationCount() const;
    bool meetsExperienceRequirements(int requiredYears) const;
    double calculateSeniorityBonus() const;
};

#endif