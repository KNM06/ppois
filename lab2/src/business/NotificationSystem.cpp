#include "../../include/business/NotificationSystem.h"

NotificationSystem::NotificationSystem(std::shared_ptr<MaintenanceScheduler> maintSched,
                                     std::shared_ptr<ReservationSystem> resSys)
    : maintenanceScheduler(maintSched), reservationSystem(resSys) {}

void NotificationSystem::sendMaintenanceReminder(const std::string& itemId) const {
    int maintenanceCount = maintenanceScheduler->getMaintenanceCount(itemId);
    double savings = maintenanceScheduler->calculatePreventiveMaintenanceSavings(itemId);
    
    std::string message = "Maintenance reminder for item " + itemId + 
                         ". Completed maintenance: " + std::to_string(maintenanceCount) +
                         ". Estimated savings: " + std::to_string(savings);
}

void NotificationSystem::sendReservationConfirmation(const std::string& customerId, const std::string& reservationId) const {
    double seasonalFactor = reservationSystem->getSeasonalDemandFactor();
    std::string seasonStatus = seasonalFactor > 1.0 ? "peak season" : "off-season";
    
    std::string message = "Reservation " + reservationId + " confirmed for customer " + 
                         customerId + ". Current season: " + seasonStatus;
}

void NotificationSystem::sendRentalReminder(const std::string& customerId, const std::string& itemId) const {
    auto overdueMaintenance = maintenanceScheduler->getOverdueMaintenance();
    bool needsMaintenance = false;
    
    for (const auto& taskId : overdueMaintenance) {
        if (taskId.find(itemId) != std::string::npos) {
            needsMaintenance = true;
            break;
        }
    }
    
    std::string maintenanceStatus = needsMaintenance ? " (Maintenance Required)" : " (Ready for Rental)";
    std::string message = "Rental reminder for customer " + customerId + 
                         ", item " + itemId + maintenanceStatus;
}

void NotificationSystem::sendPromotionalOffer(const std::string& customerId, const std::string& offerType) const {
    double discountRate = 0.0;
    
    if (offerType == "LOYALTY") discountRate = 0.15;
    else if (offerType == "SEASONAL") discountRate = 0.10;
    else if (offerType == "FIRST_TIME") discountRate = 0.20;
    
    std::string message = "Promotional offer for customer " + customerId + 
                         ": " + offerType + " discount " + 
                         std::to_string(discountRate * 100) + "%";
}

std::vector<std::string> NotificationSystem::getPendingNotifications() const {
    std::vector<std::string> notifications;
    
    auto overdueMaintenance = maintenanceScheduler->getOverdueMaintenance();
    for (const auto& task : overdueMaintenance) {
        notifications.push_back("Overdue maintenance: " + task);
    }
    
    notifications.push_back("System check completed");
    notifications.push_back("Customer feedback review pending");
    
    return notifications;
}

void NotificationSystem::processOverdueMaintenanceAlerts() const {
    auto overdueTasks = maintenanceScheduler->getOverdueMaintenance();
    
    for (const auto& taskId : overdueTasks) {
        double downtime = maintenanceScheduler->calculateEquipmentDowntime(taskId);
        std::string alert = "ALERT: Overdue maintenance " + taskId + 
                           ". Downtime cost: " + std::to_string(downtime);
    }
}