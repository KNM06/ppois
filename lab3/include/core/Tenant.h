#ifndef TENANT_H
#define TENANT_H

#include <string>
#include <memory>
#include <vector>
#include "../../include/core/Address.h"

class Tenant {
private:
    std::string tenantId;
    std::string firstName;
    std::string lastName;
    std::string email;
    std::string phoneNumber;
    std::unique_ptr<Address> currentAddress;
    double creditScore;
    double monthlyIncome;
    bool hasPets;
    int numberOfOccupants;

public:
    Tenant();
    Tenant(const std::string& id, const std::string& fName, const std::string& lName,
       const std::string& email, const std::string& phone,
       std::unique_ptr<Address> address);
    Tenant(const std::string& id, const std::string& fName, const std::string& lName,
           const std::string& email, const std::string& phone,
           std::unique_ptr<Address> address, double credit, double income,
           bool pets, int occupants);
    Tenant(const std::string& id, const std::string& fullName);
    Tenant(const std::string& id, const std::string& fullName, double credit, double income);
    Tenant(const std::string& id, const std::string& fName,
       const std::string& lName, const std::string& email);

    Tenant(const Tenant& other);
    Tenant& operator=(const Tenant& other);
    Tenant(Tenant&& other) noexcept;
    Tenant& operator=(Tenant&& other) noexcept;
    ~Tenant();

    std::string getTenantId() const;
    std::string getFirstName() const;
    std::string getLastName() const;
    std::string getEmail() const;
    std::string getPhoneNumber() const;
    Address* getCurrentAddress() const;
    double getCreditScore() const;
    double getMonthlyIncome() const;
    bool getHasPets() const;
    int getNumberOfOccupants() const;

    void setEmail(const std::string& newEmail);
    void setPhoneNumber(const std::string& newPhone);
    void updateCreditScore(double newScore);
    void updateIncome(double newIncome);

    std::string getFullName() const;
    bool meetsIncomeRequirements(double minRentToIncomeRatio) const;
    bool isCreditworthy(double minCreditScore) const;
    double calculateMaxAffordableRent() const;
    bool canAccommodatePets(bool propertyAllowsPets) const;
    bool isSuitableForPropertySize(int propertyBedrooms) const;
};

#endif