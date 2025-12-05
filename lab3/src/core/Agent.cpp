#include "../../include/core/Agent.h"
#include <algorithm>
#include <cctype>

Agent::Agent(const std::string& id, const std::string& fName, const std::string& lName,
             const std::string& email, const std::string& phone, const std::string& license,
             std::unique_ptr<Address> office, double commission, const std::string& specialization,
             int experience, bool active, const std::vector<std::string>& certs)
    : agentId(id), firstName(fName), lastName(lName), email(email),
      phoneNumber(phone), licenseNumber(license), officeAddress(std::move(office)),
      commissionRate(commission), specialization(specialization),
      yearsOfExperience(experience), isActive(active), certifications(certs) {}

Agent::Agent(const Agent& other)
    : agentId(other.agentId), firstName(other.firstName), lastName(other.lastName),
      email(other.email), phoneNumber(other.phoneNumber), licenseNumber(other.licenseNumber),
      officeAddress(other.officeAddress ? std::make_unique<Address>(*other.officeAddress) : nullptr),
      commissionRate(other.commissionRate), specialization(other.specialization),
      yearsOfExperience(other.yearsOfExperience), isActive(other.isActive),
      certifications(other.certifications) {}

Agent& Agent::operator=(const Agent& other) {
    if (this != &other) {
        agentId = other.agentId;
        firstName = other.firstName;
        lastName = other.lastName;
        email = other.email;
        phoneNumber = other.phoneNumber;
        licenseNumber = other.licenseNumber;
        officeAddress = other.officeAddress ? std::make_unique<Address>(*other.officeAddress) : nullptr;
        commissionRate = other.commissionRate;
        specialization = other.specialization;
        yearsOfExperience = other.yearsOfExperience;
        isActive = other.isActive;
        certifications = other.certifications;
    }
    return *this;
}

Agent::Agent(Agent&& other) noexcept
    : agentId(std::move(other.agentId)), firstName(std::move(other.firstName)),
      lastName(std::move(other.lastName)), email(std::move(other.email)),
      phoneNumber(std::move(other.phoneNumber)), licenseNumber(std::move(other.licenseNumber)),
      officeAddress(std::move(other.officeAddress)), commissionRate(other.commissionRate),
      specialization(std::move(other.specialization)), yearsOfExperience(other.yearsOfExperience),
      isActive(other.isActive), certifications(std::move(other.certifications)) {}

Agent& Agent::operator=(Agent&& other) noexcept {
    if (this != &other) {
        agentId = std::move(other.agentId);
        firstName = std::move(other.firstName);
        lastName = std::move(other.lastName);
        email = std::move(other.email);
        phoneNumber = std::move(other.phoneNumber);
        licenseNumber = std::move(other.licenseNumber);
        officeAddress = std::move(other.officeAddress);
        commissionRate = other.commissionRate;
        specialization = std::move(other.specialization);
        yearsOfExperience = other.yearsOfExperience;
        isActive = other.isActive;
        certifications = std::move(other.certifications);
    }
    return *this;
}

Agent::~Agent() {}

std::string Agent::getAgentId() const { return agentId; }
std::string Agent::getFirstName() const { return firstName; }
std::string Agent::getLastName() const { return lastName; }
std::string Agent::getEmail() const { return email; }
std::string Agent::getPhoneNumber() const { return phoneNumber; }
std::string Agent::getLicenseNumber() const { return licenseNumber; }
Address* Agent::getOfficeAddress() const { return officeAddress.get(); }
double Agent::getCommissionRate() const { return commissionRate; }
std::string Agent::getSpecialization() const { return specialization; }
int Agent::getYearsOfExperience() const { return yearsOfExperience; }
bool Agent::getIsActive() const { return isActive; }
std::vector<std::string> Agent::getCertifications() const { return certifications; }

void Agent::setEmail(const std::string& newEmail) {
    email = newEmail;
}

void Agent::setPhoneNumber(const std::string& newPhone) {
    phoneNumber = newPhone;
}

void Agent::setCommissionRate(double newRate) {
    commissionRate = std::max(0.0, std::min(100.0, newRate));
}

void Agent::updateExperience(int newExperience) {
    yearsOfExperience = std::max(0, newExperience);
}

void Agent::addCertification(const std::string& certification) {
    certifications.push_back(certification);
}

std::string Agent::getFullName() const {
    return firstName + " " + lastName;
}

bool Agent::isLicenseValid() const {
    return licenseNumber.length() >= 8 && isActive;
}

double Agent::calculateExpectedCommission(double propertyValue) const {
    return propertyValue * (commissionRate / 100.0);
}

bool Agent::isSpecializedIn(const std::string& propertyType) const {
    return specialization.find(propertyType) != std::string::npos;
}

bool Agent::canHandleCommercialProperties() const {
    return isSpecializedIn("commercial") || isSpecializedIn("office") || isSpecializedIn("retail");
}

bool Agent::canHandleLuxuryProperties() const {
    return isSpecializedIn("luxury") || isSpecializedIn("high_end") || yearsOfExperience >= 10;
}

int Agent::getCertificationCount() const {
    return certifications.size();
}

bool Agent::meetsExperienceRequirements(int requiredYears) const {
    return yearsOfExperience >= requiredYears;
}

double Agent::calculateSeniorityBonus() const {
    if (yearsOfExperience >= 20) return commissionRate * 0.1;
    if (yearsOfExperience >= 10) return commissionRate * 0.05;
    if (yearsOfExperience >= 5) return commissionRate * 0.02;
    return 0.0;
}