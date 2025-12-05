#include "../../include/finance/Commission.h"
#include <chrono>
#include <algorithm>

Commission::Commission(const std::string& id, std::shared_ptr<Agent> ag,
                       std::shared_ptr<RentalContract> cont, double rate,
                       double amount, const std::chrono::system_clock::time_point& earned,
                       const std::chrono::system_clock::time_point& payment,
                       const std::string& status, const std::string& paymentMethod,
                       double bonus, const std::string& type)
    : commissionId(id), agent(ag), contract(cont), commissionRate(rate),
      commissionAmount(amount), earnedDate(earned), paymentDate(payment),
      status(status), paymentMethod(paymentMethod), bonusAmount(bonus),
      commissionType(type) {}

Commission::Commission(const Commission& other)
    : commissionId(other.commissionId), agent(other.agent), contract(other.contract),
      commissionRate(other.commissionRate), commissionAmount(other.commissionAmount),
      earnedDate(other.earnedDate), paymentDate(other.paymentDate),
      status(other.status), paymentMethod(other.paymentMethod),
      bonusAmount(other.bonusAmount), commissionType(other.commissionType) {}

Commission& Commission::operator=(const Commission& other) {
    if (this != &other) {
        commissionId = other.commissionId;
        agent = other.agent;
        contract = other.contract;
        commissionRate = other.commissionRate;
        commissionAmount = other.commissionAmount;
        earnedDate = other.earnedDate;
        paymentDate = other.paymentDate;
        status = other.status;
        paymentMethod = other.paymentMethod;
        bonusAmount = other.bonusAmount;
        commissionType = other.commissionType;
    }
    return *this;
}

Commission::Commission(Commission&& other) noexcept
    : commissionId(std::move(other.commissionId)), agent(std::move(other.agent)),
      contract(std::move(other.contract)), commissionRate(other.commissionRate),
      commissionAmount(other.commissionAmount), earnedDate(other.earnedDate),
      paymentDate(other.paymentDate), status(std::move(other.status)),
      paymentMethod(std::move(other.paymentMethod)), bonusAmount(other.bonusAmount),
      commissionType(std::move(other.commissionType)) {}

Commission& Commission::operator=(Commission&& other) noexcept {
    if (this != &other) {
        commissionId = std::move(other.commissionId);
        agent = std::move(other.agent);
        contract = std::move(other.contract);
        commissionRate = other.commissionRate;
        commissionAmount = other.commissionAmount;
        earnedDate = other.earnedDate;
        paymentDate = other.paymentDate;
        status = std::move(other.status);
        paymentMethod = std::move(other.paymentMethod);
        bonusAmount = other.bonusAmount;
        commissionType = std::move(other.commissionType);
    }
    return *this;
}

Commission::~Commission() {}

std::string Commission::getCommissionId() const { return commissionId; }
Agent* Commission::getAgent() const { return agent.get(); }
RentalContract* Commission::getContract() const { return contract.get(); }
double Commission::getCommissionRate() const { return commissionRate; }
double Commission::getCommissionAmount() const { return commissionAmount; }
std::chrono::system_clock::time_point Commission::getEarnedDate() const { return earnedDate; }
std::chrono::system_clock::time_point Commission::getPaymentDate() const { return paymentDate; }
std::string Commission::getStatus() const { return status; }
std::string Commission::getPaymentMethod() const { return paymentMethod; }
double Commission::getBonusAmount() const { return bonusAmount; }
std::string Commission::getCommissionType() const { return commissionType; }

void Commission::setStatus(const std::string& newStatus) {
    status = newStatus;
}

void Commission::setPaymentDate(const std::chrono::system_clock::time_point& newDate) {
    paymentDate = newDate;
}

void Commission::setBonusAmount(double newBonus) {
    bonusAmount = std::max(0.0, newBonus);
}

double Commission::calculateTotalCommission() const {
    return commissionAmount + bonusAmount;
}

bool Commission::isPaid() const {
    return status == "paid" || status == "processed";
}

bool Commission::isOverdue(const std::chrono::system_clock::time_point& currentDate) const {
    if (isPaid()) return false;
    auto duration = std::chrono::duration_cast<std::chrono::hours>(currentDate - earnedDate);
    int daysSinceEarned = static_cast<int>(duration.count() / 24.0);
    return daysSinceEarned > 30;
}

double Commission::calculateTaxWithholding(double taxRate) const {
    return calculateTotalCommission() * (taxRate / 100.0);
}

bool Commission::isLeasingCommission() const {
    return commissionType == "leasing" || commissionType.find("lease") != std::string::npos;
}

bool Commission::isManagementCommission() const {
    return commissionType == "management" || commissionType.find("management") != std::string::npos;
}

double Commission::calculateNetCommission(double taxRate) const {
    double total = calculateTotalCommission();
    double tax = calculateTaxWithholding(taxRate);
    return total - tax;
}

bool Commission::qualifiesForBonus() const {
    return commissionAmount > 5000.0 || 
           (contract && contract->getMonthlyRent() > 3000.0);
}

bool Commission::requiresManagerApproval() const {
    return commissionAmount > 10000.0 || bonusAmount > 2000.0;
}

std::string Commission::getCommissionTier() const {
    double total = calculateTotalCommission();
    if (total < 1000) return "standard";
    if (total < 5000) return "premium";
    if (total < 15000) return "elite";
    return "executive";
}