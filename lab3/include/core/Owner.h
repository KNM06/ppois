#ifndef OWNER_H
#define OWNER_H

#include <string>
#include <memory>
#include <vector>
#include "../../include/core/Address.h"

class Owner {
private:
    std::string ownerId;
    std::string firstName;
    std::string lastName;
    std::string email;
    std::string phoneNumber;
    std::unique_ptr<Address> mailingAddress;
    std::string taxId;
    double ownershipPercentage;
    bool isPrimaryOwner;
    std::string preferredPaymentMethod;


public:
    Owner();
    Owner(const std::string& id, const std::string& fName, const std::string& lName,
          const std::string& email, const std::string& phone,
          std::unique_ptr<Address> address, const std::string& taxId,
          double ownership, bool primary, const std::string& paymentMethod);
    Owner(const std::string& id, const std::string& fName,
      const std::string& lName, const std::string& email);

    Owner(const Owner& other);
    Owner& operator=(const Owner& other);
    Owner(Owner&& other) noexcept;
    Owner& operator=(Owner&& other) noexcept;
    ~Owner();

    std::string getOwnerId() const;
    std::string getFirstName() const;
    std::string getLastName() const;
    std::string getEmail() const;
    std::string getPhoneNumber() const;
    Address* getMailingAddress() const;
    std::string getTaxId() const;
    double getOwnershipPercentage() const;
    bool getIsPrimaryOwner() const;
    std::string getPreferredPaymentMethod() const;

    void setEmail(const std::string& newEmail);
    void setPhoneNumber(const std::string& newPhone);
    void updateOwnershipPercentage(double newPercentage);
    void setPreferredPaymentMethod(const std::string& newMethod);

    std::string getFullName() const;
    bool isValidTaxId() const;
    double calculateNetPayment(double grossAmount, double managementFee) const;
    bool requiresTaxDocumentation() const;
    bool canAuthorizeMaintenance(double costLimit) const;
    bool isContactInformationComplete() const;
};

#endif