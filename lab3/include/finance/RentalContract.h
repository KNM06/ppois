#ifndef RENTALCONTRACT_H
#define RENTALCONTRACT_H

#include <string>
#include <memory>
#include <chrono>
#include "../../include/core/Property.h"
#include "../../include/core/Tenant.h"
#include "../../include/core/Owner.h"

class RentalContract {
private:
    std::string contractId;
    std::shared_ptr<Property> property;
    std::shared_ptr<Tenant> tenant;
    std::shared_ptr<Owner> owner;
    std::chrono::system_clock::time_point startDate;
    std::chrono::system_clock::time_point endDate;
    double monthlyRent;
    double securityDeposit;
    std::string paymentTerms;
    bool isActive;


public:
    RentalContract(const std::string& id, std::shared_ptr<Property> prop,
                   std::shared_ptr<Tenant> ten, std::shared_ptr<Owner> own,
                   const std::chrono::system_clock::time_point& start,
                   const std::chrono::system_clock::time_point& end,
                   double rent, double deposit, const std::string& terms);
    RentalContract(const std::string& id,
               std::shared_ptr<Property> prop,
               std::shared_ptr<Tenant> ten,
               double rent,
               const std::chrono::system_clock::time_point& start,
               const std::chrono::system_clock::time_point& end);
    RentalContract(const RentalContract& other);
    RentalContract& operator=(const RentalContract& other);
    RentalContract(RentalContract&& other) noexcept;
    RentalContract& operator=(RentalContract&& other) noexcept;
    ~RentalContract();


    std::shared_ptr<Tenant> getTenantSharedPtr() const; 
    std::shared_ptr<Property> getPropertySharedPtr() const;
    std::shared_ptr<Owner> getOwnerSharedPtr() const;

    std::string getContractId() const;
    Property* getProperty() const;
    Tenant* getTenant() const;
    Owner* getOwner() const;
    std::chrono::system_clock::time_point getStartDate() const;
    std::chrono::system_clock::time_point getEndDate() const;
    double getMonthlyRent() const;
    double getSecurityDeposit() const;
    std::string getPaymentTerms() const;
    bool getIsActive() const;

    void setMonthlyRent(double newRent);
    void setPaymentTerms(const std::string& newTerms);
    void terminateContract();

    bool isContractValid() const;
    int getRemainingDays() const;
    double calculateTotalContractValue() const;
    bool isRentPaymentDue(const std::chrono::system_clock::time_point& checkDate) const;
    double calculateEarlyTerminationFee(int monthsRemaining) const;
};

#endif