#include "../../include/business/RentalManager.h"
#include <chrono>
#include <algorithm>

RentalManager::RentalManager(double managementFee, int renewalNoticeDays)
    : managementFeePercentage(managementFee), leaseRenewalNoticeDays(renewalNoticeDays) {}

RentalManager::RentalManager(const RentalManager& other)
    : properties(other.properties), tenants(other.tenants),
      contracts(other.contracts), propertyToContractMap(other.propertyToContractMap),
      managementFeePercentage(other.managementFeePercentage),
      leaseRenewalNoticeDays(other.leaseRenewalNoticeDays) {}

RentalManager& RentalManager::operator=(const RentalManager& other) {
    if (this != &other) {
        properties = other.properties;
        tenants = other.tenants;
        contracts = other.contracts;
        propertyToContractMap = other.propertyToContractMap;
        managementFeePercentage = other.managementFeePercentage;
        leaseRenewalNoticeDays = other.leaseRenewalNoticeDays;
    }
    return *this;
}

RentalManager::RentalManager(RentalManager&& other) noexcept
    : properties(std::move(other.properties)), tenants(std::move(other.tenants)),
      contracts(std::move(other.contracts)), propertyToContractMap(std::move(other.propertyToContractMap)),
      managementFeePercentage(other.managementFeePercentage),
      leaseRenewalNoticeDays(other.leaseRenewalNoticeDays) {}

RentalManager& RentalManager::operator=(RentalManager&& other) noexcept {
    if (this != &other) {
        properties = std::move(other.properties);
        tenants = std::move(other.tenants);
        contracts = std::move(other.contracts);
        propertyToContractMap = std::move(other.propertyToContractMap);
        managementFeePercentage = other.managementFeePercentage;
        leaseRenewalNoticeDays = other.leaseRenewalNoticeDays;
    }
    return *this;
}

RentalManager::~RentalManager() {}

double RentalManager::getManagementFeePercentage() const { return managementFeePercentage; }
int RentalManager::getLeaseRenewalNoticeDays() const { return leaseRenewalNoticeDays; }

void RentalManager::setManagementFeePercentage(double newFee) {
    managementFeePercentage = std::max(0.0, std::min(100.0, newFee));
}

void RentalManager::setLeaseRenewalNoticeDays(int newDays) {
    leaseRenewalNoticeDays = std::max(0, newDays);
}

void RentalManager::addProperty(std::shared_ptr<Property> property) {
    properties.push_back(property);
}

void RentalManager::addTenant(std::shared_ptr<Tenant> tenant) {
    tenants.push_back(tenant);
}

bool RentalManager::createRentalContract(std::shared_ptr<Property> property,
                                        std::shared_ptr<Tenant> tenant,
                                        std::shared_ptr<Owner> owner,
                                        const std::chrono::system_clock::time_point& startDate,
                                        const std::chrono::system_clock::time_point& endDate,
                                        double monthlyRent, double securityDeposit,
                                        const std::string& paymentTerms) {
    if (!property || !tenant || !owner) return false;
    if (!isPropertyAvailable(property->getId())) return false;
    
    auto contract = std::make_shared<RentalContract>(
        "CONT" + std::to_string(contracts.size() + 1),
        property, tenant, owner, startDate, endDate,
        monthlyRent, securityDeposit, paymentTerms
    );
    
    contracts.push_back(contract);
    propertyToContractMap[property->getId()] = contract;
    return true;
}

bool RentalManager::terminateRentalContract(const std::string& contractId) {
    auto it = std::find_if(contracts.begin(), contracts.end(),
        [&](const std::shared_ptr<RentalContract>& contract) {
            return contract->getContractId() == contractId;
        });
    
    if (it != contracts.end()) {
        (*it)->terminateContract();
        propertyToContractMap.erase((*it)->getProperty()->getId());
        return true;
    }
    return false;
}

std::vector<std::shared_ptr<Property>> RentalManager::getAvailableProperties() const {
    std::vector<std::shared_ptr<Property>> available;
    for (const auto& property : properties) {
        if (isPropertyAvailable(property->getId())) {
            available.push_back(property);
        }
    }
    return available;
}

std::vector<std::shared_ptr<Tenant>> RentalManager::getActiveTenants() const {
    std::vector<std::shared_ptr<Tenant>> active;
    for (const auto& contract : contracts) {
        if (contract->isContractValid()) {
            auto tenant = contract->getTenantSharedPtr();
            if (tenant) {
                active.push_back(tenant);
            }
        }
    }
    return active;
}

double RentalManager::calculateTotalMonthlyRevenue() const {
    double total = 0.0;
    for (const auto& contract : contracts) {
        if (contract->isContractValid()) {
            total += contract->getMonthlyRent();
        }
    }
    return total;
}

double RentalManager::calculateManagementFees(double grossRevenue) const {
    return grossRevenue * (managementFeePercentage / 100.0);
}

bool RentalManager::isPropertyAvailable(const std::string& propertyId) const {
    auto it = propertyToContractMap.find(propertyId);
    if (it == propertyToContractMap.end()) return true;
    return !it->second->isContractValid();
}

std::shared_ptr<RentalContract> RentalManager::findContractByProperty(const std::string& propertyId) const {
    auto it = propertyToContractMap.find(propertyId);
    return it != propertyToContractMap.end() ? it->second : nullptr;
}

std::vector<std::shared_ptr<RentalContract>> RentalManager::getExpiringContracts(int daysThreshold) const {
    std::vector<std::shared_ptr<RentalContract>> expiring;
    auto now = std::chrono::system_clock::now();
    
    for (const auto& contract : contracts) {
        if (contract->isContractValid() && contract->getRemainingDays() <= daysThreshold) {
            expiring.push_back(contract);
        }
    }
    return expiring;
}

bool RentalManager::canTenantAffordProperty(std::shared_ptr<Tenant> tenant, std::shared_ptr<Property> property) const {
    if (!tenant || !property) return false;
    double maxAffordable = tenant->calculateMaxAffordableRent();
    return property->getRentalPrice() <= maxAffordable;
}

double RentalManager::calculateVacancyRate() const {
    if (properties.empty()) return 0.0;
    int occupiedCount = 0;
    for (const auto& property : properties) {
        if (!isPropertyAvailable(property->getId())) {
            occupiedCount++;
        }
    }
    return (1.0 - (static_cast<double>(occupiedCount) / properties.size())) * 100.0;
}