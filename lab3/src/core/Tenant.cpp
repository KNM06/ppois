#include "../../include/core/Tenant.h"
#include <algorithm>

Tenant::Tenant(const std::string& id, const std::string& fName, const std::string& lName,
               const std::string& email, const std::string& phone,
               std::unique_ptr<Address> address, double credit, double income,
               bool pets, int occupants)
    : tenantId(id), firstName(fName), lastName(lName), email(email),
      phoneNumber(phone), currentAddress(std::move(address)),
      creditScore(credit), monthlyIncome(income), hasPets(pets),
      numberOfOccupants(occupants) {}
Tenant::Tenant() :
    tenantId(""), firstName(""), lastName(""),
    email(""), phoneNumber(""),
    currentAddress(nullptr),
    creditScore(0.0), monthlyIncome(0.0),
    hasPets(false), numberOfOccupants(1) {}
Tenant::Tenant(const std::string& id, const std::string& fName, const std::string& lName,
       const std::string& email, const std::string& phone,
       std::unique_ptr<Address> address)
    : tenantId(id), firstName(fName), lastName(lName), email(email),
      phoneNumber(phone), currentAddress(std::move(address)),
      creditScore(0.0), monthlyIncome(0.0),
      hasPets(false), numberOfOccupants(1) {}

Tenant::Tenant(const Tenant& other)
    : tenantId(other.tenantId), firstName(other.firstName), lastName(other.lastName),
      email(other.email), phoneNumber(other.phoneNumber),
      currentAddress(other.currentAddress ? std::make_unique<Address>(*other.currentAddress) : nullptr),
      creditScore(other.creditScore), monthlyIncome(other.monthlyIncome),
      hasPets(other.hasPets), numberOfOccupants(other.numberOfOccupants) {}
Tenant::Tenant(const std::string& id, const std::string& fullName)
    : tenantId(id), firstName(fullName), lastName(""), email(""), phoneNumber(""),
      currentAddress(nullptr), creditScore(0.0), monthlyIncome(0.0),
      hasPets(false), numberOfOccupants(1) {}
Tenant::Tenant(const std::string& id, const std::string& fullName, double credit, double income)
    : tenantId(id), firstName(fullName), lastName(""),
      email(""), phoneNumber(""), currentAddress(nullptr),
      creditScore(credit), monthlyIncome(income),
      hasPets(false), numberOfOccupants(1) {}
Tenant::Tenant(const std::string& id, const std::string& fName,
               const std::string& lName, const std::string& email)
    : tenantId(id), firstName(fName), lastName(lName), email(email),
      phoneNumber(""), currentAddress(nullptr), creditScore(0.0),
      monthlyIncome(0.0), hasPets(false), numberOfOccupants(1) {}

Tenant& Tenant::operator=(const Tenant& other) {
    if (this != &other) {
        tenantId = other.tenantId;
        firstName = other.firstName;
        lastName = other.lastName;
        email = other.email;
        phoneNumber = other.phoneNumber;
        currentAddress = other.currentAddress ? std::make_unique<Address>(*other.currentAddress) : nullptr;
        creditScore = other.creditScore;
        monthlyIncome = other.monthlyIncome;
        hasPets = other.hasPets;
        numberOfOccupants = other.numberOfOccupants;
    }
    return *this;
}

Tenant::Tenant(Tenant&& other) noexcept
    : tenantId(std::move(other.tenantId)), firstName(std::move(other.firstName)),
      lastName(std::move(other.lastName)), email(std::move(other.email)),
      phoneNumber(std::move(other.phoneNumber)), currentAddress(std::move(other.currentAddress)),
      creditScore(other.creditScore), monthlyIncome(other.monthlyIncome),
      hasPets(other.hasPets), numberOfOccupants(other.numberOfOccupants) {}

Tenant& Tenant::operator=(Tenant&& other) noexcept {
    if (this != &other) {
        tenantId = std::move(other.tenantId);
        firstName = std::move(other.firstName);
        lastName = std::move(other.lastName);
        email = std::move(other.email);
        phoneNumber = std::move(other.phoneNumber);
        currentAddress = std::move(other.currentAddress);
        creditScore = other.creditScore;
        monthlyIncome = other.monthlyIncome;
        hasPets = other.hasPets;
        numberOfOccupants = other.numberOfOccupants;
    }
    return *this;
}

Tenant::~Tenant() {}

std::string Tenant::getTenantId() const { return tenantId; }
std::string Tenant::getFirstName() const { return firstName; }
std::string Tenant::getLastName() const { return lastName; }
std::string Tenant::getEmail() const { return email; }
std::string Tenant::getPhoneNumber() const { return phoneNumber; }
Address* Tenant::getCurrentAddress() const { return currentAddress.get(); }
double Tenant::getCreditScore() const { return creditScore; }
double Tenant::getMonthlyIncome() const { return monthlyIncome; }
bool Tenant::getHasPets() const { return hasPets; }
int Tenant::getNumberOfOccupants() const { return numberOfOccupants; }

void Tenant::setEmail(const std::string& newEmail) {
    email = newEmail;
}

void Tenant::setPhoneNumber(const std::string& newPhone) {
    phoneNumber = newPhone;
}

void Tenant::updateCreditScore(double newScore) {
    creditScore = std::max(300.0, std::min(850.0, newScore));
}

void Tenant::updateIncome(double newIncome) {
    monthlyIncome = std::max(0.0, newIncome);
}

std::string Tenant::getFullName() const {
    return firstName + " " + lastName;
}

bool Tenant::meetsIncomeRequirements(double minRentToIncomeRatio) const {
    if (monthlyIncome <= 0) return false;
    double maxRent = monthlyIncome * minRentToIncomeRatio;
    return maxRent > 0;
}

bool Tenant::isCreditworthy(double minCreditScore) const {
    return creditScore >= minCreditScore;
}

double Tenant::calculateMaxAffordableRent() const {
    return monthlyIncome * 0.3;
}

bool Tenant::canAccommodatePets(bool propertyAllowsPets) const {
    return !hasPets || propertyAllowsPets;
}

bool Tenant::isSuitableForPropertySize(int propertyBedrooms) const {
    int requiredBedrooms = (numberOfOccupants + 1) / 2;
    return propertyBedrooms >= requiredBedrooms;
}