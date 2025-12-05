#include "../../include/business/TenantManager.h"
#include <algorithm>

TenantManager::TenantManager(double minCreditScore, double minIncomeRatio)
    : minimumCreditScore(minCreditScore), minimumIncomeToRentRatio(minIncomeRatio) {}

TenantManager::TenantManager(const TenantManager& other)
    : tenants(other.tenants), tenantContracts(other.tenantContracts),
      tenantPaymentHistory(other.tenantPaymentHistory),
      minimumCreditScore(other.minimumCreditScore),
      minimumIncomeToRentRatio(other.minimumIncomeToRentRatio) {}

TenantManager& TenantManager::operator=(const TenantManager& other) {
    if (this != &other) {
        tenants = other.tenants;
        tenantContracts = other.tenantContracts;
        tenantPaymentHistory = other.tenantPaymentHistory;
        minimumCreditScore = other.minimumCreditScore;
        minimumIncomeToRentRatio = other.minimumIncomeToRentRatio;
    }
    return *this;
}

TenantManager::TenantManager(TenantManager&& other) noexcept
    : tenants(std::move(other.tenants)), tenantContracts(std::move(other.tenantContracts)),
      tenantPaymentHistory(std::move(other.tenantPaymentHistory)),
      minimumCreditScore(other.minimumCreditScore),
      minimumIncomeToRentRatio(other.minimumIncomeToRentRatio) {}

TenantManager& TenantManager::operator=(TenantManager&& other) noexcept {
    if (this != &other) {
        tenants = std::move(other.tenants);
        tenantContracts = std::move(other.tenantContracts);
        tenantPaymentHistory = std::move(other.tenantPaymentHistory);
        minimumCreditScore = other.minimumCreditScore;
        minimumIncomeToRentRatio = other.minimumIncomeToRentRatio;
    }
    return *this;
}

TenantManager::~TenantManager() {}

double TenantManager::getMinimumCreditScore() const { return minimumCreditScore; }
double TenantManager::getMinimumIncomeToRentRatio() const { return minimumIncomeToRentRatio; }

void TenantManager::setMinimumCreditScore(double newScore) {
    minimumCreditScore = std::max(300.0, std::min(850.0, newScore));
}

void TenantManager::setMinimumIncomeToRentRatio(double newRatio) {
    minimumIncomeToRentRatio = std::max(0.1, newRatio);
}

bool TenantManager::registerTenant(std::shared_ptr<Tenant> tenant) {
    if (!tenant) return false;
    
    tenants.push_back(tenant);
    return true;
}

bool TenantManager::approveTenantApplication(std::shared_ptr<Tenant> tenant, double proposedRent) {
    if (!tenant) return false;
    
    bool creditApproved = tenant->getCreditScore() >= minimumCreditScore;
    bool incomeApproved = tenant->getMonthlyIncome() >= (proposedRent * minimumIncomeToRentRatio);
    bool paymentHistoryGood = hasGoodPaymentHistory(tenant->getTenantId());
    
    return creditApproved && incomeApproved && paymentHistoryGood;
}

std::vector<std::shared_ptr<Payment>> TenantManager::getTenantPaymentHistory(const std::string& tenantId) const {
    auto it = tenantPaymentHistory.find(tenantId);
    return it != tenantPaymentHistory.end() ? it->second : std::vector<std::shared_ptr<Payment>>();
}

double TenantManager::calculateTenantPaymentScore(const std::string& tenantId) const {
    auto payments = getTenantPaymentHistory(tenantId);
    if (payments.empty()) return 100.0;
    
    int onTimePayments = 0;
    for (const auto& payment : payments) {
        if (payment->isPaymentOnTime(payment->getPaymentDate())) {
            onTimePayments++;
        }
    }
    
    return (static_cast<double>(onTimePayments) / payments.size()) * 100.0;
}

bool TenantManager::hasGoodPaymentHistory(const std::string& tenantId) const {
    return calculateTenantPaymentScore(tenantId) >= 80.0;
}

std::vector<std::shared_ptr<Tenant>> TenantManager::getTenantsWithLatePayments() const {
    std::vector<std::shared_ptr<Tenant>> latePayers;
    
    for (const auto& tenant : tenants) {
        if (!hasGoodPaymentHistory(tenant->getTenantId())) {
            latePayers.push_back(tenant);
        }
    }
    
    return latePayers;
}

bool TenantManager::canTenantRenewLease(const std::string& tenantId) const {
    return hasGoodPaymentHistory(tenantId) && calculateTenantPaymentScore(tenantId) >= 90.0;
}

double TenantManager::calculateAverageRentPayment(const std::string& tenantId) const {
    auto payments = getTenantPaymentHistory(tenantId);
    if (payments.empty()) return 0.0;
    
    double totalAmount = 0.0;
    int rentPaymentCount = 0;
    
    for (const auto& payment : payments) {
        if (payment->isRentPayment() && payment->getStatus() == "completed") {
            totalAmount += payment->getAmount();
            rentPaymentCount++;
        }
    }
    
    return rentPaymentCount > 0 ? totalAmount / rentPaymentCount : 0.0;
}

std::vector<std::shared_ptr<Tenant>> TenantManager::searchTenantsByCriteria(double minCreditScore, double minIncome) const {
    std::vector<std::shared_ptr<Tenant>> matchingTenants;
    
    for (const auto& tenant : tenants) {
        if (tenant->getCreditScore() >= minCreditScore && tenant->getMonthlyIncome() >= minIncome) {
            matchingTenants.push_back(tenant);
        }
    }
    
    return matchingTenants;
}

bool TenantManager::isTenantEligibleForUpgrade(const std::string& tenantId, double newRent) const {
    auto tenantIt = std::find_if(tenants.begin(), tenants.end(),
        [&](const std::shared_ptr<Tenant>& t) { return t->getTenantId() == tenantId; });
    
    if (tenantIt == tenants.end()) return false;
    
    auto tenant = *tenantIt;
    return tenant->getMonthlyIncome() >= (newRent * minimumIncomeToRentRatio) &&
           hasGoodPaymentHistory(tenantId) &&
           calculateTenantPaymentScore(tenantId) >= 95.0;
}