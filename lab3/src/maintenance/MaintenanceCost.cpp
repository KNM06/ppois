#include "../../include/maintenance/MaintenanceCost.h"
#include <algorithm>

MaintenanceCost::MaintenanceCost(const std::string& id, std::shared_ptr<MaintenanceRequest> request,
                                 double labor, double material, double equipment, double emergencySurcharge,
                                 const std::chrono::system_clock::time_point& costDate,
                                 const std::string& paymentStatus, const std::string& vendor,
                                 const std::string& category, bool reimbursable)
    : costId(id), maintenanceRequest(request), laborCost(labor), materialCost(material),
      equipmentCost(equipment), emergencySurcharge(emergencySurcharge), costDate(costDate),
      paymentStatus(paymentStatus), vendor(vendor), costCategory(category),
      isReimbursable(reimbursable) {}

MaintenanceCost::MaintenanceCost(const MaintenanceCost& other)
    : costId(other.costId), maintenanceRequest(other.maintenanceRequest),
      laborCost(other.laborCost), materialCost(other.materialCost),
      equipmentCost(other.equipmentCost), emergencySurcharge(other.emergencySurcharge),
      costDate(other.costDate), paymentStatus(other.paymentStatus),
      vendor(other.vendor), costCategory(other.costCategory),
      isReimbursable(other.isReimbursable) {}

MaintenanceCost& MaintenanceCost::operator=(const MaintenanceCost& other) {
    if (this != &other) {
        costId = other.costId;
        maintenanceRequest = other.maintenanceRequest;
        laborCost = other.laborCost;
        materialCost = other.materialCost;
        equipmentCost = other.equipmentCost;
        emergencySurcharge = other.emergencySurcharge;
        costDate = other.costDate;
        paymentStatus = other.paymentStatus;
        vendor = other.vendor;
        costCategory = other.costCategory;
        isReimbursable = other.isReimbursable;
    }
    return *this;
}

MaintenanceCost::MaintenanceCost(MaintenanceCost&& other) noexcept
    : costId(std::move(other.costId)), maintenanceRequest(std::move(other.maintenanceRequest)),
      laborCost(other.laborCost), materialCost(other.materialCost),
      equipmentCost(other.equipmentCost), emergencySurcharge(other.emergencySurcharge),
      costDate(other.costDate), paymentStatus(std::move(other.paymentStatus)),
      vendor(std::move(other.vendor)), costCategory(std::move(other.costCategory)),
      isReimbursable(other.isReimbursable) {}

MaintenanceCost& MaintenanceCost::operator=(MaintenanceCost&& other) noexcept {
    if (this != &other) {
        costId = std::move(other.costId);
        maintenanceRequest = std::move(other.maintenanceRequest);
        laborCost = other.laborCost;
        materialCost = other.materialCost;
        equipmentCost = other.equipmentCost;
        emergencySurcharge = other.emergencySurcharge;
        costDate = other.costDate;
        paymentStatus = std::move(other.paymentStatus);
        vendor = std::move(other.vendor);
        costCategory = std::move(other.costCategory);
        isReimbursable = other.isReimbursable;
    }
    return *this;
}

MaintenanceCost::~MaintenanceCost() {}

std::string MaintenanceCost::getCostId() const { return costId; }
MaintenanceRequest* MaintenanceCost::getMaintenanceRequest() const { return maintenanceRequest.get(); }
double MaintenanceCost::getLaborCost() const { return laborCost; }
double MaintenanceCost::getMaterialCost() const { return materialCost; }
double MaintenanceCost::getEquipmentCost() const { return equipmentCost; }
double MaintenanceCost::getEmergencySurcharge() const { return emergencySurcharge; }
std::chrono::system_clock::time_point MaintenanceCost::getCostDate() const { return costDate; }
std::string MaintenanceCost::getPaymentStatus() const { return paymentStatus; }
std::string MaintenanceCost::getVendor() const { return vendor; }
std::string MaintenanceCost::getCostCategory() const { return costCategory; }
bool MaintenanceCost::getIsReimbursable() const { return isReimbursable; }

void MaintenanceCost::setLaborCost(double newCost) {
    laborCost = std::max(0.0, newCost);
}

void MaintenanceCost::setMaterialCost(double newCost) {
    materialCost = std::max(0.0, newCost);
}

void MaintenanceCost::setPaymentStatus(const std::string& newStatus) {
    paymentStatus = newStatus;
}

double MaintenanceCost::calculateTotalCost() const {
    return laborCost + materialCost + equipmentCost + emergencySurcharge;
}

bool MaintenanceCost::isEmergencyRepair() const {
    return emergencySurcharge > 0.0;
}

double MaintenanceCost::calculateTaxAmount(double taxRate) const {
    return calculateTotalCost() * (taxRate / 100.0);
}

bool MaintenanceCost::isOverBudget(double budget) const {
    return calculateTotalCost() > budget;
}

double MaintenanceCost::calculateVendorDiscount() const {
    if (vendor.find("preferred") != std::string::npos) {
        return calculateTotalCost() * 0.1;
    }
    return 0.0;
}

bool MaintenanceCost::requiresOwnerApproval() const {
    return calculateTotalCost() > 1000.0 || isEmergencyRepair();
}

double MaintenanceCost::calculateDepreciation() const {
    if (costCategory == "capital_improvement") {
        return calculateTotalCost() / 10.0;
    }
    return 0.0;
}

bool MaintenanceCost::isCapitalExpenditure() const {
    return costCategory == "capital_improvement" || calculateTotalCost() > 2500.0;
}

std::string MaintenanceCost::getCostPriority() const {
    if (isEmergencyRepair()) return "high";
    if (calculateTotalCost() > 500.0) return "medium";
    return "low";
}