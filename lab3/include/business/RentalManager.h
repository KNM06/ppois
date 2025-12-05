#ifndef RENTALMANAGER_H
#define RENTALMANAGER_H

#include <memory>
#include <vector>
#include <unordered_map>
#include "../../include/core/Property.h"
#include "../../include/core/Tenant.h"
#include "../../include/finance/RentalContract.h"

class RentalManager {
private:
    std::vector<std::shared_ptr<Property>> properties;
    std::vector<std::shared_ptr<Tenant>> tenants;
    std::vector<std::shared_ptr<RentalContract>> contracts;
    std::unordered_map<std::string, std::shared_ptr<RentalContract>> propertyToContractMap;
    double managementFeePercentage;
    int leaseRenewalNoticeDays;
    friend class RentalManagerTest;


public:
    RentalManager(double managementFee, int renewalNoticeDays);
    
    RentalManager(const RentalManager& other);
    RentalManager& operator=(const RentalManager& other);
    RentalManager(RentalManager&& other) noexcept;
    RentalManager& operator=(RentalManager&& other) noexcept;
    ~RentalManager();

    double getManagementFeePercentage() const;
    int getLeaseRenewalNoticeDays() const;

    void setManagementFeePercentage(double newFee);
    void setLeaseRenewalNoticeDays(int newDays);

    void addProperty(std::shared_ptr<Property> property);
    void addTenant(std::shared_ptr<Tenant> tenant);
    bool createRentalContract(std::shared_ptr<Property> property,
                             std::shared_ptr<Tenant> tenant,
                             std::shared_ptr<Owner> owner,
                             const std::chrono::system_clock::time_point& startDate,
                             const std::chrono::system_clock::time_point& endDate,
                             double monthlyRent, double securityDeposit,
                             const std::string& paymentTerms);
    bool terminateRentalContract(const std::string& contractId);
    std::vector<std::shared_ptr<Property>> getAvailableProperties() const;
    std::vector<std::shared_ptr<Tenant>> getActiveTenants() const;
    double calculateTotalMonthlyRevenue() const;
    double calculateManagementFees(double grossRevenue) const;
    bool isPropertyAvailable(const std::string& propertyId) const;
    std::shared_ptr<RentalContract> findContractByProperty(const std::string& propertyId) const;
    std::vector<std::shared_ptr<RentalContract>> getExpiringContracts(int daysThreshold) const;
    bool canTenantAffordProperty(std::shared_ptr<Tenant> tenant, std::shared_ptr<Property> property) const;
    double calculateVacancyRate() const;
};

#endif