#include "../../include/finance/SecurityDeposit.h"
#include <chrono>
#include <algorithm>
#include <cmath>

SecurityDeposit::SecurityDeposit(const std::string& id, std::shared_ptr<Tenant> ten,
                                 std::shared_ptr<RentalContract> cont, double amt,
                                 const std::chrono::system_clock::time_point& depositDate,
                                 const std::chrono::system_clock::time_point& returnDate,
                                 const std::string& status, double deductions,
                                 const std::string& reason, double interest,
                                 const std::string& account)
    : depositId(id), tenant(ten), contract(cont), amount(amt),
      depositDate(depositDate), expectedReturnDate(returnDate), status(status),
      deductions(deductions), deductionReason(reason), interestEarned(interest),
      holdingAccount(account) {}

SecurityDeposit::SecurityDeposit(const SecurityDeposit& other)
    : depositId(other.depositId), tenant(other.tenant), contract(other.contract),
      amount(other.amount), depositDate(other.depositDate),
      expectedReturnDate(other.expectedReturnDate), status(other.status),
      deductions(other.deductions), deductionReason(other.deductionReason),
      interestEarned(other.interestEarned), holdingAccount(other.holdingAccount) {}

SecurityDeposit& SecurityDeposit::operator=(const SecurityDeposit& other) {
    if (this != &other) {
        depositId = other.depositId;
        tenant = other.tenant;
        contract = other.contract;
        amount = other.amount;
        depositDate = other.depositDate;
        expectedReturnDate = other.expectedReturnDate;
        status = other.status;
        deductions = other.deductions;
        deductionReason = other.deductionReason;
        interestEarned = other.interestEarned;
        holdingAccount = other.holdingAccount;
    }
    return *this;
}

SecurityDeposit::SecurityDeposit(SecurityDeposit&& other) noexcept
    : depositId(std::move(other.depositId)), tenant(std::move(other.tenant)),
      contract(std::move(other.contract)), amount(other.amount),
      depositDate(other.depositDate), expectedReturnDate(other.expectedReturnDate),
      status(std::move(other.status)), deductions(other.deductions),
      deductionReason(std::move(other.deductionReason)), interestEarned(other.interestEarned),
      holdingAccount(std::move(other.holdingAccount)) {}

SecurityDeposit& SecurityDeposit::operator=(SecurityDeposit&& other) noexcept {
    if (this != &other) {
        depositId = std::move(other.depositId);
        tenant = std::move(other.tenant);
        contract = std::move(other.contract);
        amount = other.amount;
        depositDate = other.depositDate;
        expectedReturnDate = other.expectedReturnDate;
        status = std::move(other.status);
        deductions = other.deductions;
        deductionReason = std::move(other.deductionReason);
        interestEarned = other.interestEarned;
        holdingAccount = std::move(other.holdingAccount);
    }
    return *this;
}

SecurityDeposit::~SecurityDeposit() {}

std::string SecurityDeposit::getDepositId() const { return depositId; }
Tenant* SecurityDeposit::getTenant() const { return tenant.get(); }
RentalContract* SecurityDeposit::getContract() const { return contract.get(); }
double SecurityDeposit::getAmount() const { return amount; }
std::chrono::system_clock::time_point SecurityDeposit::getDepositDate() const { return depositDate; }
std::chrono::system_clock::time_point SecurityDeposit::getExpectedReturnDate() const { return expectedReturnDate; }
std::string SecurityDeposit::getStatus() const { return status; }
double SecurityDeposit::getDeductions() const { return deductions; }
std::string SecurityDeposit::getDeductionReason() const { return deductionReason; }
double SecurityDeposit::getInterestEarned() const { return interestEarned; }
std::string SecurityDeposit::getHoldingAccount() const { return holdingAccount; }

void SecurityDeposit::setStatus(const std::string& newStatus) {
    status = newStatus;
}

void SecurityDeposit::setDeductions(double newDeductions, const std::string& reason) {
    deductions = std::max(0.0, std::min(amount, newDeductions));
    deductionReason = reason;
}

void SecurityDeposit::setInterestEarned(double newInterest) {
    interestEarned = std::max(0.0, newInterest);
}

double SecurityDeposit::calculateReturnAmount() const {
    return amount - deductions + interestEarned;
}

bool SecurityDeposit::isRefundable() const {
    return status != "forfeited" && status != "applied_to_rent";
}

bool SecurityDeposit::isFullyRefundable() const {
    return isRefundable() && deductions == 0.0;
}

double SecurityDeposit::calculateInterest(double annualRate, const std::chrono::system_clock::time_point& currentDate) const {
    auto duration = std::chrono::duration_cast<std::chrono::hours>(currentDate - depositDate);
    double years = duration.count() / (24.0 * 365.0);
    return amount * annualRate * years;
}

bool SecurityDeposit::canBeReleased(const std::chrono::system_clock::time_point& currentDate) const {
    if (status != "held") return false;
    if (contract && !contract->getIsActive()) return true;
    return currentDate >= expectedReturnDate;
}

bool SecurityDeposit::hasDamageDeductions() const {
    return deductions > 0.0 && 
           (deductionReason.find("damage") != std::string::npos ||
            deductionReason.find("repair") != std::string::npos);
}

double SecurityDeposit::calculateDeductionPercentage() const {
    if (amount <= 0) return 0.0;
    return (deductions / amount) * 100.0;
}

bool SecurityDeposit::requiresLegalReview() const {
    return deductions > amount * 0.5 || 
           deductionReason.find("legal") != std::string::npos;
}

bool SecurityDeposit::isHeldInEscrow() const {
    return !holdingAccount.empty() && holdingAccount != "general_account";
}