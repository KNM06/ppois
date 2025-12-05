#include "../../include/business/PaymentManager.h"
#include <chrono>
#include <algorithm>
#include <cmath>

PaymentManager::PaymentManager(double lateFee, int gracePeriod, const std::string& defaultMethod)
    : lateFeeRate(lateFee), gracePeriodDays(gracePeriod), defaultPaymentMethod(defaultMethod) {}

PaymentManager::PaymentManager(const PaymentManager& other)
    : payments(other.payments), tenantPayments(other.tenantPayments),
      tenantBalances(other.tenantBalances), lateFeeRate(other.lateFeeRate),
      gracePeriodDays(other.gracePeriodDays), defaultPaymentMethod(other.defaultPaymentMethod) {}

PaymentManager& PaymentManager::operator=(const PaymentManager& other) {
    if (this != &other) {
        payments = other.payments;
        tenantPayments = other.tenantPayments;
        tenantBalances = other.tenantBalances;
        lateFeeRate = other.lateFeeRate;
        gracePeriodDays = other.gracePeriodDays;
        defaultPaymentMethod = other.defaultPaymentMethod;
    }
    return *this;
}

PaymentManager::PaymentManager(PaymentManager&& other) noexcept
    : payments(std::move(other.payments)), tenantPayments(std::move(other.tenantPayments)),
      tenantBalances(std::move(other.tenantBalances)), lateFeeRate(other.lateFeeRate),
      gracePeriodDays(other.gracePeriodDays), defaultPaymentMethod(std::move(other.defaultPaymentMethod)) {}

PaymentManager& PaymentManager::operator=(PaymentManager&& other) noexcept {
    if (this != &other) {
        payments = std::move(other.payments);
        tenantPayments = std::move(other.tenantPayments);
        tenantBalances = std::move(other.tenantBalances);
        lateFeeRate = other.lateFeeRate;
        gracePeriodDays = other.gracePeriodDays;
        defaultPaymentMethod = std::move(other.defaultPaymentMethod);
    }
    return *this;
}

PaymentManager::~PaymentManager() {}

double PaymentManager::getLateFeeRate() const { return lateFeeRate; }
int PaymentManager::getGracePeriodDays() const { return gracePeriodDays; }
std::string PaymentManager::getDefaultPaymentMethod() const { return defaultPaymentMethod; }

void PaymentManager::setLateFeeRate(double newRate) {
    lateFeeRate = std::max(0.0, newRate);
}

void PaymentManager::setGracePeriodDays(int newDays) {
    gracePeriodDays = std::max(0, newDays);
}

void PaymentManager::setDefaultPaymentMethod(const std::string& newMethod) {
    defaultPaymentMethod = newMethod;
}

bool PaymentManager::processPayment(std::shared_ptr<Payment> payment) {
    if (!payment || !payment->getTenant()) return false;
    
    payments.push_back(payment);
    std::string tenantId = payment->getTenant()->getTenantId();
    tenantPayments[tenantId].push_back(payment);
    
    if (payment->getStatus() == "completed") {
        tenantBalances[tenantId] -= payment->getAmount();
    }
    
    return true;
}

bool PaymentManager::recordRentPayment(std::shared_ptr<Tenant> tenant,
                                      std::shared_ptr<RentalContract> contract,
                                      double amount, const std::chrono::system_clock::time_point& paymentDate,
                                      const std::string& paymentMethod, const std::string& period) {
    if (!tenant || !contract) return false;
    
    auto payment = std::make_shared<Payment>(
        "PAY" + std::to_string(payments.size() + 1),
        tenant, contract, amount, paymentDate,
        paymentMethod.empty() ? defaultPaymentMethod : paymentMethod,
        "completed", "", 0.0, period
    );
    
    return processPayment(payment);
}

double PaymentManager::calculateLateFee(double amount, int daysLate) const {
    if (daysLate <= gracePeriodDays) return 0.0;
    int effectiveLateDays = daysLate - gracePeriodDays;
    return amount * lateFeeRate * effectiveLateDays;
}

double PaymentManager::getTenantBalance(const std::string& tenantId) const {
    auto it = tenantBalances.find(tenantId);
    return it != tenantBalances.end() ? it->second : 0.0;
}

std::vector<std::shared_ptr<Payment>> PaymentManager::getPaymentsByTenant(const std::string& tenantId) const {
    auto it = tenantPayments.find(tenantId);
    return it != tenantPayments.end() ? it->second : std::vector<std::shared_ptr<Payment>>();
}

std::vector<std::shared_ptr<Payment>> PaymentManager::getOverduePayments(const std::chrono::system_clock::time_point& currentDate) const {
    std::vector<std::shared_ptr<Payment>> overdue;
    
    for (const auto& payment : payments) {
        if (payment->isOverdue(currentDate)) {
            overdue.push_back(payment);
        }
    }
    
    return overdue;
}

double PaymentManager::calculateTotalRevenue(const std::chrono::system_clock::time_point& startDate,
                                            const std::chrono::system_clock::time_point& endDate) const {
    double total = 0.0;
    
    for (const auto& payment : payments) {
        if (payment->getStatus() == "completed" &&
            payment->getPaymentDate() >= startDate &&
            payment->getPaymentDate() <= endDate) {
            total += payment->getAmount();
        }
    }
    
    return total;
}

bool PaymentManager::hasTenantMissedPayments(const std::string& tenantId, int maxAllowed) const {
    auto tenantPayments = getPaymentsByTenant(tenantId);
    int missedCount = 0;
    
    for (const auto& payment : tenantPayments) {
        if (payment->isOverdue(std::chrono::system_clock::now()) &&
            payment->getStatus() != "completed") {
            missedCount++;
        }
    }
    
    return missedCount > maxAllowed;
}

double PaymentManager::calculateCollectionRate(const std::chrono::system_clock::time_point& startDate,
                                              const std::chrono::system_clock::time_point& endDate) const {
    int totalPayments = 0;
    int collectedPayments = 0;
    
    for (const auto& payment : payments) {
        if (payment->getPaymentDate() >= startDate && payment->getPaymentDate() <= endDate) {
            totalPayments++;
            if (payment->getStatus() == "completed") {
                collectedPayments++;
            }
        }
    }
    
    if (totalPayments == 0) return 100.0;
    return (static_cast<double>(collectedPayments) / totalPayments) * 100.0;
}

