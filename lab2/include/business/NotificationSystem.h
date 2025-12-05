#ifndef NOTIFICATIONSYSTEM_H
#define NOTIFICATIONSYSTEM_H

#include "../core/Customer.h"
#include "../business/MaintenanceScheduler.h"
#include "../business/ReservationSystem.h"
#include <memory>
#include <vector>
#include <string>

class NotificationSystem {
private:
    std::shared_ptr<MaintenanceScheduler> maintenanceScheduler;
    std::shared_ptr<ReservationSystem> reservationSystem;

public:
    NotificationSystem(std::shared_ptr<MaintenanceScheduler> maintSched,
                      std::shared_ptr<ReservationSystem> resSys);
    
    void sendMaintenanceReminder(const std::string& itemId) const;
    void sendReservationConfirmation(const std::string& customerId, const std::string& reservationId) const;
    void sendRentalReminder(const std::string& customerId, const std::string& itemId) const;
    void sendPromotionalOffer(const std::string& customerId, const std::string& offerType) const;
    std::vector<std::string> getPendingNotifications() const;
    void processOverdueMaintenanceAlerts() const;
};

#endif