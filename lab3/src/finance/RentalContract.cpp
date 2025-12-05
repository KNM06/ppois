#include "../../include/finance/RentalContract.h"
#include <chrono>
#include <cmath>

RentalContract::RentalContract(const std::string& id, std::shared_ptr<Property> prop,
                               std::shared_ptr<Tenant> ten, std::shared_ptr<Owner> own,
                               const std::chrono::system_clock::time_point& start,
                               const std::chrono::system_clock::time_point& end,
                               double rent, double deposit, const std::string& terms)
    : contractId(id), property(prop), tenant(ten), owner(own),
      startDate(start), endDate(end), monthlyRent(rent),
      securityDeposit(deposit), paymentTerms(terms), isActive(true) {}

RentalContract::RentalContract(const RentalContract& other)
    : contractId(other.contractId), property(other.property),
      tenant(other.tenant), owner(other.owner),
      startDate(other.startDate), endDate(other.endDate),
      monthlyRent(other.monthlyRent), securityDeposit(other.securityDeposit),
      paymentTerms(other.paymentTerms), isActive(other.isActive) {}
RentalContract::RentalContract(const std::string& id,
                               std::shared_ptr<Property> prop,
                               std::shared_ptr<Tenant> ten,
                               double rent,
                               const std::chrono::system_clock::time_point& start,
                               const std::chrono::system_clock::time_point& end)
    : contractId(id),
      property(prop),
      tenant(ten),
      owner(nullptr),
      startDate(start),
      endDate(end),
      monthlyRent(rent),
      securityDeposit(0.0),
      paymentTerms("standard"),
      isActive(true)
{}

RentalContract& RentalContract::operator=(const RentalContract& other) {
    if (this != &other) {
        contractId = other.contractId;
        property = other.property;
        tenant = other.tenant;
        owner = other.owner;
        startDate = other.startDate;
        endDate = other.endDate;
        monthlyRent = other.monthlyRent;
        securityDeposit = other.securityDeposit;
        paymentTerms = other.paymentTerms;
        isActive = other.isActive;
    }
    return *this;
}

RentalContract::RentalContract(RentalContract&& other) noexcept
    : contractId(std::move(other.contractId)), property(std::move(other.property)),
      tenant(std::move(other.tenant)), owner(std::move(other.owner)),
      startDate(other.startDate), endDate(other.endDate),
      monthlyRent(other.monthlyRent), securityDeposit(other.securityDeposit),
      paymentTerms(std::move(other.paymentTerms)), isActive(other.isActive) {}

RentalContract& RentalContract::operator=(RentalContract&& other) noexcept {
    if (this != &other) {
        contractId = std::move(other.contractId);
        property = std::move(other.property);
        tenant = std::move(other.tenant);
        owner = std::move(other.owner);
        startDate = other.startDate;
        endDate = other.endDate;
        monthlyRent = other.monthlyRent;
        securityDeposit = other.securityDeposit;
        paymentTerms = std::move(other.paymentTerms);
        isActive = other.isActive;
    }
    return *this;
}

RentalContract::~RentalContract() {}

std::string RentalContract::getContractId() const { return contractId; }
Property* RentalContract::getProperty() const { return property.get(); }
Tenant* RentalContract::getTenant() const { return tenant.get(); }
Owner* RentalContract::getOwner() const { return owner.get(); }
std::chrono::system_clock::time_point RentalContract::getStartDate() const { return startDate; }
std::chrono::system_clock::time_point RentalContract::getEndDate() const { return endDate; }
double RentalContract::getMonthlyRent() const { return monthlyRent; }
double RentalContract::getSecurityDeposit() const { return securityDeposit; }
std::string RentalContract::getPaymentTerms() const { return paymentTerms; }
bool RentalContract::getIsActive() const { return isActive; }

std::shared_ptr<Tenant> RentalContract::getTenantSharedPtr() const { return tenant; }
std::shared_ptr<Property> RentalContract::getPropertySharedPtr() const { return property; }
std::shared_ptr<Owner> RentalContract::getOwnerSharedPtr() const { return owner; }

void RentalContract::setMonthlyRent(double newRent) {
    monthlyRent = newRent;
}

void RentalContract::setPaymentTerms(const std::string& newTerms) {
    paymentTerms = newTerms;
}

void RentalContract::terminateContract() {
    isActive = false;
}

bool RentalContract::isContractValid() const {
    auto now = std::chrono::system_clock::now();
    return isActive && now >= startDate && now <= endDate;
}

int RentalContract::getRemainingDays() const {
    auto now = std::chrono::system_clock::now();
    if (now > endDate) return 0;
    
    auto duration = std::chrono::duration_cast<std::chrono::hours>(endDate - now);
    return static_cast<int>(std::ceil(duration.count() / 24.0));
}

double RentalContract::calculateTotalContractValue() const {
    auto duration = std::chrono::duration_cast<std::chrono::hours>(endDate - startDate);
    int months = static_cast<int>(std::ceil(duration.count() / (24.0 * 30.0)));
    return monthlyRent * months;
}

bool RentalContract::isRentPaymentDue(const std::chrono::system_clock::time_point& checkDate) const {
    auto duration = std::chrono::duration_cast<std::chrono::hours>(checkDate - startDate);
    int daysSinceStart = static_cast<int>(duration.count() / 24.0);
    return daysSinceStart % 30 == 0 && isContractValid();
}

double RentalContract::calculateEarlyTerminationFee(int monthsRemaining) const {
    if (monthsRemaining <= 0) return 0.0;
    return monthlyRent * monthsRemaining * 0.5;
}