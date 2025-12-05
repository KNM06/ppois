#ifndef SECURITYDEPOSIT_H
#define SECURITYDEPOSIT_H

#include <string>
#include <memory>
#include <chrono>
#include "../../include/core/Tenant.h"
#include "../../include/finance/RentalContract.h"

class SecurityDeposit {
private:
    std::string depositId;
    std::shared_ptr<Tenant> tenant;
    std::shared_ptr<RentalContract> contract;
    double amount;
    std::chrono::system_clock::time_point depositDate;
    std::chrono::system_clock::time_point expectedReturnDate;
    std::string status;
    double deductions;
    std::string deductionReason;
    double interestEarned;
    std::string holdingAccount;

public:
    SecurityDeposit(const std::string& id, std::shared_ptr<Tenant> ten,
                   std::shared_ptr<RentalContract> cont, double amt,
                   const std::chrono::system_clock::time_point& depositDate,
                   const std::chrono::system_clock::time_point& returnDate,
                   const std::string& status, double deductions,
                   const std::string& reason, double interest,
                   const std::string& account);
    
    SecurityDeposit(const SecurityDeposit& other);
    SecurityDeposit& operator=(const SecurityDeposit& other);
    SecurityDeposit(SecurityDeposit&& other) noexcept;
    SecurityDeposit& operator=(SecurityDeposit&& other) noexcept;
    ~SecurityDeposit();

    std::string getDepositId() const;
    Tenant* getTenant() const;
    RentalContract* getContract() const;
    double getAmount() const;
    std::chrono::system_clock::time_point getDepositDate() const;
    std::chrono::system_clock::time_point getExpectedReturnDate() const;
    std::string getStatus() const;
    double getDeductions() const;
    std::string getDeductionReason() const;
    double getInterestEarned() const;
    std::string getHoldingAccount() const;

    void setStatus(const std::string& newStatus);
    void setDeductions(double newDeductions, const std::string& reason);
    void setInterestEarned(double newInterest);

    double calculateReturnAmount() const;
    bool isRefundable() const;
    bool isFullyRefundable() const;
    double calculateInterest(double annualRate, const std::chrono::system_clock::time_point& currentDate) const;
    bool canBeReleased(const std::chrono::system_clock::time_point& currentDate) const;
    bool hasDamageDeductions() const;
    double calculateDeductionPercentage() const;
    bool requiresLegalReview() const;
    bool isHeldInEscrow() const;
};

#endif