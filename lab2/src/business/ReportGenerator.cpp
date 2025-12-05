#include "../../include/business/ReportGenerator.h"
#include "../../include/core/Inventory.h"
#include "../../include/core/RentalSystem.h"
#include "../../include/business/CustomerManager.h"
#include <algorithm>

ReportGenerator::ReportGenerator(std::shared_ptr<Inventory> inv,
                                 std::shared_ptr<RentalSystem> rentalSys,
                                 std::shared_ptr<CustomerManager> custMgr)
    : inventory(inv), rentalSystem(rentalSys), customerManager(custMgr) {}

std::string ReportGenerator::generateFinancialReport(const std::string&, const std::string&) const {
    double revenue = rentalSystem ? rentalSystem->getTotalRevenue() : 0.0;
    double utilization = rentalSystem ? rentalSystem->calculateSystemUtilizationRate() : 0.0;

    return "Financial Report: Revenue=" + std::to_string(revenue) +
           ", Utilization=" + std::to_string(utilization);
}

std::string ReportGenerator::generateInventoryUtilizationReport() const {
    if (!inventory) return "Inventory Utilization:\n(no inventory)\n";

    auto utilization = inventory->getUtilizationByCategory();
    if (utilization.empty()) return "Inventory Utilization:\n(no data)\n";

    std::string report = "Inventory Utilization:\n";
    for (const auto& [category, rate] : utilization) {
        report += category + ": " + std::to_string(rate) + "\n";
    }

    return report;
}

std::string ReportGenerator::generateCustomerRetentionReport() const {
    if (!customerManager) return "Customer Retention: Rate=0";

    auto vip = customerManager->getVIPCustomers();
    double rate = vip.empty() ? 0.0 : static_cast<double>(vip.size()) / 10.0;

    return "Customer Retention: Rate=" + std::to_string(rate);
}

std::string ReportGenerator::generateMaintenanceCostReport(int months) const {
    if (months <= 0) return "Maintenance Cost: 0";

    double baseCost = 150.0;
    double total = baseCost * months;

    return "Maintenance Cost: " + std::to_string(total);
}

std::vector<std::string> ReportGenerator::getTopPerformingCategories(int limit) const {
    std::vector<std::string> result;
    if (!inventory || limit <= 0) return result;

    auto utilization = inventory->getUtilizationByCategory();
    if (utilization.empty()) return result;

    std::vector<std::pair<std::string, double>> data(utilization.begin(), utilization.end());

    std::sort(data.begin(), data.end(),
              [](const auto& a, const auto& b) { return a.second > b.second; });

    int count = std::min(limit, static_cast<int>(data.size()));
    for (int i = 0; i < count; ++i)
        result.push_back(data[i].first);

    return result;
}

double ReportGenerator::calculateCustomerAcquisitionCost() const {
    double marketingBudget = 5000.0;
    int estimatedCustomers = 100;

    return marketingBudget / estimatedCustomers;
}
