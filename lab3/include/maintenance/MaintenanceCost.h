#ifndef MAINTENANCECOST_H
#define MAINTENANCECOST_H

#include <string>
#include <memory>
#include <chrono>
#include "../../include/maintenance/MaintenanceRequest.h"

class MaintenanceCost {
private:
    std::string costId;
    std::shared_ptr<MaintenanceRequest> maintenanceRequest;
    double laborCost;
    double materialCost;
    double equipmentCost;
    double emergencySurcharge;
    std::chrono::system_clock::time_point costDate;
    std::string paymentStatus;
    std::string vendor;
    std::string costCategory;
    bool isReimbursable;

public:
    MaintenanceCost(const std::string& id, std::shared_ptr<MaintenanceRequest> request,
                   double labor, double material, double equipment, double emergencySurcharge,
                   const std::chrono::system_clock::time_point& costDate,
                   const std::string& paymentStatus, const std::string& vendor,
                   const std::string& category, bool reimbursable);
    
    MaintenanceCost(const MaintenanceCost& other);
    MaintenanceCost& operator=(const MaintenanceCost& other);
    MaintenanceCost(MaintenanceCost&& other) noexcept;
    MaintenanceCost& operator=(MaintenanceCost&& other) noexcept;
    ~MaintenanceCost();

    std::string getCostId() const;
    MaintenanceRequest* getMaintenanceRequest() const;
    double getLaborCost() const;
    double getMaterialCost() const;
    double getEquipmentCost() const;
    double getEmergencySurcharge() const;
    std::chrono::system_clock::time_point getCostDate() const;
    std::string getPaymentStatus() const;
    std::string getVendor() const;
    std::string getCostCategory() const;
    bool getIsReimbursable() const;

    void setLaborCost(double newCost);
    void setMaterialCost(double newCost);
    void setPaymentStatus(const std::string& newStatus);

    double calculateTotalCost() const;
    bool isEmergencyRepair() const;
    double calculateTaxAmount(double taxRate) const;
    bool isOverBudget(double budget) const;
    double calculateVendorDiscount() const;
    bool requiresOwnerApproval() const;
    double calculateDepreciation() const;
    bool isCapitalExpenditure() const;
    std::string getCostPriority() const;
};

#endif