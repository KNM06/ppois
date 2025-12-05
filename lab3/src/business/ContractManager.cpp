#include "../../include/business/ContractManager.h"
#include <chrono>
#include <algorithm>

ContractManager::ContractManager(int renewalNotice, double depositMultiplier, int maxLeaseTerm,
                                 const std::vector<std::string>& clauses)
    : autoRenewalNoticeDays(renewalNotice), securityDepositMultiplier(depositMultiplier),
      maxLeaseTermMonths(maxLeaseTerm), standardClauses(clauses) {}

ContractManager::ContractManager(const ContractManager& other)
    : contracts(other.contracts), propertyContractMap(other.propertyContractMap),
      tenantContractHistory(other.tenantContractHistory),
      autoRenewalNoticeDays(other.autoRenewalNoticeDays),
      securityDepositMultiplier(other.securityDepositMultiplier),
      maxLeaseTermMonths(other.maxLeaseTermMonths), standardClauses(other.standardClauses) {}

ContractManager& ContractManager::operator=(const ContractManager& other) {
    if (this != &other) {
        contracts = other.contracts;
        propertyContractMap = other.propertyContractMap;
        tenantContractHistory = other.tenantContractHistory;
        autoRenewalNoticeDays = other.autoRenewalNoticeDays;
        securityDepositMultiplier = other.securityDepositMultiplier;
        maxLeaseTermMonths = other.maxLeaseTermMonths;
        standardClauses = other.standardClauses;
    }
    return *this;
}

ContractManager::ContractManager(ContractManager&& other) noexcept
    : contracts(std::move(other.contracts)), propertyContractMap(std::move(other.propertyContractMap)),
      tenantContractHistory(std::move(other.tenantContractHistory)),
      autoRenewalNoticeDays(other.autoRenewalNoticeDays),
      securityDepositMultiplier(other.securityDepositMultiplier),
      maxLeaseTermMonths(other.maxLeaseTermMonths), standardClauses(std::move(other.standardClauses)) {}

ContractManager& ContractManager::operator=(ContractManager&& other) noexcept {
    if (this != &other) {
        contracts = std::move(other.contracts);
        propertyContractMap = std::move(other.propertyContractMap);
        tenantContractHistory = std::move(other.tenantContractHistory);
        autoRenewalNoticeDays = other.autoRenewalNoticeDays;
        securityDepositMultiplier = other.securityDepositMultiplier;
        maxLeaseTermMonths = other.maxLeaseTermMonths;
        standardClauses = std::move(other.standardClauses);
    }
    return *this;
}

ContractManager::~ContractManager() {}

int ContractManager::getAutoRenewalNoticeDays() const { return autoRenewalNoticeDays; }
double ContractManager::getSecurityDepositMultiplier() const { return securityDepositMultiplier; }
int ContractManager::getMaxLeaseTermMonths() const { return maxLeaseTermMonths; }
std::vector<std::string> ContractManager::getStandardClauses() const { return standardClauses; }

void ContractManager::setAutoRenewalNoticeDays(int newDays) {
    autoRenewalNoticeDays = std::max(0, newDays);
}

void ContractManager::setSecurityDepositMultiplier(double newMultiplier) {
    securityDepositMultiplier = std::max(1.0, newMultiplier);
}

void ContractManager::setMaxLeaseTermMonths(int newMax) {
    maxLeaseTermMonths = std::max(1, newMax);
}

void ContractManager::addStandardClause(const std::string& clause) {
    standardClauses.push_back(clause);
}

bool ContractManager::createRentalContract(std::shared_ptr<Property> property,
                                          std::shared_ptr<Tenant> tenant,
                                          std::shared_ptr<Owner> owner,
                                          const std::chrono::system_clock::time_point& startDate,
                                          int leaseTermMonths,
                                          double monthlyRent,
                                          const std::string& paymentTerms) {
    if (!property || !tenant || !owner) return false;
    
    if (!isLeaseTermValid(leaseTermMonths)) return false;
    
    auto existingContract = findContractByProperty(property->getId());
    if (existingContract && existingContract->isContractValid()) return false;
    
    auto endDate = startDate + std::chrono::hours(leaseTermMonths * 30 * 24);
    double securityDeposit = calculateRequiredSecurityDeposit(monthlyRent);
    
    auto contract = std::make_shared<RentalContract>(
        "CNTR" + std::to_string(contracts.size() + 1),
        property, tenant, owner, startDate, endDate,
        monthlyRent, securityDeposit, paymentTerms
    );
    
    contracts.push_back(contract);
    propertyContractMap[property->getId()] = contract;
    tenantContractHistory[tenant->getTenantId()].push_back(contract);
    
    return true;
}

bool ContractManager::terminateContract(const std::string& contractId, const std::string& terminationReason) {
    for (auto& contract : contracts) {
        if (contract->getContractId() == contractId) {
            contract->terminateContract();
            propertyContractMap.erase(contract->getProperty()->getId());
            return true;
        }
    }
    return false;
}

bool ContractManager::renewContract(const std::string& contractId, int renewalTermMonths, double newRent) {
    if (!isLeaseTermValid(renewalTermMonths)) return false;
    
    for (auto& contract : contracts) {
        if (contract->getContractId() == contractId && contract->getIsActive()) {
            auto newEndDate = contract->getEndDate() + std::chrono::hours(renewalTermMonths * 30 * 24);
            contract->setMonthlyRent(newRent);
            return true;
        }
    }
    return false;
}

std::shared_ptr<RentalContract> ContractManager::findContractByProperty(const std::string& propertyId) const {
    auto it = propertyContractMap.find(propertyId);
    return it != propertyContractMap.end() ? it->second : nullptr;
}

std::vector<std::shared_ptr<RentalContract>> ContractManager::getExpiringContracts(int daysThreshold) const {
    std::vector<std::shared_ptr<RentalContract>> expiring;
    auto now = std::chrono::system_clock::now();
    
    for (const auto& contract : contracts) {
        if (contract->isContractValid() && contract->getRemainingDays() <= daysThreshold) {
            expiring.push_back(contract);
        }
    }
    return expiring;
}

std::vector<std::shared_ptr<RentalContract>> ContractManager::getActiveContracts() const {
    std::vector<std::shared_ptr<RentalContract>> active;
    for (const auto& contract : contracts) {
        if (contract->isContractValid()) {
            active.push_back(contract);
        }
    }
    return active;
}

double ContractManager::calculateTotalSecurityDeposits() const {
    double total = 0.0;
    for (const auto& contract : contracts) {
        if (contract->isContractValid()) {
            total += contract->getSecurityDeposit();
        }
    }
    return total;
}

bool ContractManager::canTenantRentAnotherProperty(const std::string& tenantId) const {
    auto it = tenantContractHistory.find(tenantId);
    if (it == tenantContractHistory.end()) return true;
    
    int activeContracts = 0;
    for (const auto& contract : it->second) {
        if (contract->isContractValid()) {
            activeContracts++;
        }
    }
    return activeContracts < 2;
}

double ContractManager::calculateRequiredSecurityDeposit(double monthlyRent) const {
    return monthlyRent * securityDepositMultiplier;
}

bool ContractManager::isLeaseTermValid(int leaseTermMonths) const {
    return leaseTermMonths >= 1 && leaseTermMonths <= maxLeaseTermMonths;
}

std::vector<std::shared_ptr<RentalContract>> ContractManager::getContractsByTenant(const std::string& tenantId) const {
    auto it = tenantContractHistory.find(tenantId);
    return it != tenantContractHistory.end() ? it->second : std::vector<std::shared_ptr<RentalContract>>();
}

bool ContractManager::hasContractViolations(const std::string& tenantId) const {
    auto tenantContracts = getContractsByTenant(tenantId);
    for (const auto& contract : tenantContracts) {
        if (!contract->getIsActive() && contract->getRemainingDays() < -30) {
            return true;
        }
    }
    return false;
}

double ContractManager::calculateTotalMonthlyRent() const {
    double total = 0.0;
    for (const auto& contract : contracts) {
        if (contract->isContractValid()) {
            total += contract->getMonthlyRent();
        }
    }
    return total;
}