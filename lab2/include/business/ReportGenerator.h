#ifndef REPORTGENERATOR_H
#define REPORTGENERATOR_H

#include "../core/Inventory.h"
#include "../core/RentalSystem.h"
#include "../business/CustomerManager.h"
#include <memory>
#include <vector>
#include <string>

class ReportGenerator {
private:
    std::shared_ptr<Inventory> inventory;
    std::shared_ptr<RentalSystem> rentalSystem;
    std::shared_ptr<CustomerManager> customerManager;

public:
    ReportGenerator(std::shared_ptr<Inventory> inv, 
                   std::shared_ptr<RentalSystem> rentalSys,
                   std::shared_ptr<CustomerManager> custMgr);
    
    std::string generateFinancialReport(const std::string& startDate, const std::string& endDate) const;
    std::string generateInventoryUtilizationReport() const;
    std::string generateCustomerRetentionReport() const;
    std::string generateMaintenanceCostReport(int months) const;
    std::vector<std::string> getTopPerformingCategories(int limit) const;
    double calculateCustomerAcquisitionCost() const;
};

#endif