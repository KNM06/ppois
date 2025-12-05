#include "../../include/core/Owner.h"
#include <algorithm>
#include <cctype>

Owner::Owner(const std::string& id, const std::string& fName, const std::string& lName,
             const std::string& email, const std::string& phone,
             std::unique_ptr<Address> address, const std::string& taxId,
             double ownership, bool primary, const std::string& paymentMethod)
    : ownerId(id), firstName(fName), lastName(lName), email(email),
      phoneNumber(phone), mailingAddress(std::move(address)), taxId(taxId),
      ownershipPercentage(ownership), isPrimaryOwner(primary),
      preferredPaymentMethod(paymentMethod) {}
Owner::Owner() :
    ownerId(""), firstName(""), lastName(""),
    email(""), phoneNumber(""),
    mailingAddress(nullptr),
    taxId(""), ownershipPercentage(0.0),
    isPrimaryOwner(false), preferredPaymentMethod("") {}

Owner::Owner(const Owner& other)
    : ownerId(other.ownerId), firstName(other.firstName), lastName(other.lastName),
      email(other.email), phoneNumber(other.phoneNumber),
      mailingAddress(other.mailingAddress ? std::make_unique<Address>(*other.mailingAddress) : nullptr),
      taxId(other.taxId), ownershipPercentage(other.ownershipPercentage),
      isPrimaryOwner(other.isPrimaryOwner), preferredPaymentMethod(other.preferredPaymentMethod) {}
Owner::Owner(const std::string& id, const std::string& fName,
             const std::string& lName, const std::string& email)
    : ownerId(id), firstName(fName), lastName(lName), email(email),
      phoneNumber(""), mailingAddress(nullptr), taxId(""), ownershipPercentage(0.0),
      isPrimaryOwner(false), preferredPaymentMethod("") {}

Owner& Owner::operator=(const Owner& other) {
    if (this != &other) {
        ownerId = other.ownerId;
        firstName = other.firstName;
        lastName = other.lastName;
        email = other.email;
        phoneNumber = other.phoneNumber;
        mailingAddress = other.mailingAddress ? std::make_unique<Address>(*other.mailingAddress) : nullptr;
        taxId = other.taxId;
        ownershipPercentage = other.ownershipPercentage;
        isPrimaryOwner = other.isPrimaryOwner;
        preferredPaymentMethod = other.preferredPaymentMethod;
    }
    return *this;
}

Owner::Owner(Owner&& other) noexcept
    : ownerId(std::move(other.ownerId)), firstName(std::move(other.firstName)),
      lastName(std::move(other.lastName)), email(std::move(other.email)),
      phoneNumber(std::move(other.phoneNumber)), mailingAddress(std::move(other.mailingAddress)),
      taxId(std::move(other.taxId)), ownershipPercentage(other.ownershipPercentage),
      isPrimaryOwner(other.isPrimaryOwner), preferredPaymentMethod(std::move(other.preferredPaymentMethod)) {}

Owner& Owner::operator=(Owner&& other) noexcept {
    if (this != &other) {
        ownerId = std::move(other.ownerId);
        firstName = std::move(other.firstName);
        lastName = std::move(other.lastName);
        email = std::move(other.email);
        phoneNumber = std::move(other.phoneNumber);
        mailingAddress = std::move(other.mailingAddress);
        taxId = std::move(other.taxId);
        ownershipPercentage = other.ownershipPercentage;
        isPrimaryOwner = other.isPrimaryOwner;
        preferredPaymentMethod = std::move(other.preferredPaymentMethod);
    }
    return *this;
}

Owner::~Owner() {}

std::string Owner::getOwnerId() const { return ownerId; }
std::string Owner::getFirstName() const { return firstName; }
std::string Owner::getLastName() const { return lastName; }
std::string Owner::getEmail() const { return email; }
std::string Owner::getPhoneNumber() const { return phoneNumber; }
Address* Owner::getMailingAddress() const { return mailingAddress.get(); }
std::string Owner::getTaxId() const { return taxId; }
double Owner::getOwnershipPercentage() const { return ownershipPercentage; }
bool Owner::getIsPrimaryOwner() const { return isPrimaryOwner; }
std::string Owner::getPreferredPaymentMethod() const { return preferredPaymentMethod; }

void Owner::setEmail(const std::string& newEmail) {
    email = newEmail;
}

void Owner::setPhoneNumber(const std::string& newPhone) {
    phoneNumber = newPhone;
}

void Owner::updateOwnershipPercentage(double newPercentage) {
    ownershipPercentage = std::max(0.0, std::min(100.0, newPercentage));
}

void Owner::setPreferredPaymentMethod(const std::string& newMethod) {
    preferredPaymentMethod = newMethod;
}

std::string Owner::getFullName() const {
    return firstName + " " + lastName;
}

bool Owner::isValidTaxId() const {
    if (taxId.length() != 9) return false;
    for (char c : taxId) {
        if (!std::isdigit(c)) return false;
    }
    return true;
}

double Owner::calculateNetPayment(double grossAmount, double managementFee) const {
    double feeAmount = grossAmount * (managementFee / 100.0);
    double ownerShare = (ownershipPercentage / 100.0);
    return (grossAmount - feeAmount) * ownerShare;
}

bool Owner::requiresTaxDocumentation() const {
    return ownershipPercentage > 0.0;
}

bool Owner::canAuthorizeMaintenance(double costLimit) const {
    return isPrimaryOwner || ownershipPercentage >= 50.0;
}

bool Owner::isContactInformationComplete() const {
    return !email.empty() && !phoneNumber.empty() && mailingAddress != nullptr;
}