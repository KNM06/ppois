#ifndef NOTIFICATIONSYSTEM_H
#define NOTIFICATIONSYSTEM_H

#include <string>
#include <memory>
#include <vector>
#include <chrono>
#include <unordered_map>
#include "../../include/core/Tenant.h"
#include "../../include/core/Owner.h"
#include "../../include/finance/RentalContract.h"

class NotificationSystem {
private:
    std::vector<std::string> sentNotifications;
    std::unordered_map<std::string, std::vector<std::string>> userNotifications; // Оставляем, но исправляем использование
    bool emailEnabled;
    bool smsEnabled;
    bool pushEnabled;
    int maxNotificationsPerDay;
    std::chrono::system_clock::time_point lastNotificationReset;

public:
    NotificationSystem(bool email, bool sms, bool push, int maxNotifications);
    
    NotificationSystem(const NotificationSystem& other);
    NotificationSystem& operator=(const NotificationSystem& other);
    NotificationSystem(NotificationSystem&& other) noexcept;
    NotificationSystem& operator=(NotificationSystem&& other) noexcept;
    ~NotificationSystem();

    bool getEmailEnabled() const;
    bool getSmsEnabled() const;
    bool getPushEnabled() const;
    int getMaxNotificationsPerDay() const;
    std::chrono::system_clock::time_point getLastNotificationReset() const;

    void setEmailEnabled(bool enabled);
    void setSmsEnabled(bool enabled);
    void setPushEnabled(bool enabled);
    void setMaxNotificationsPerDay(int max);

    bool sendRentReminder(std::shared_ptr<Tenant> tenant, std::shared_ptr<RentalContract> contract,
                         const std::chrono::system_clock::time_point& dueDate);
    bool sendMaintenanceUpdate(std::shared_ptr<Tenant> tenant, const std::string& maintenanceStatus,
                              const std::string& details);
    bool sendContractExpirationAlert(std::shared_ptr<Owner> owner, std::shared_ptr<RentalContract> contract,
                                    int daysUntilExpiration);
    bool sendPaymentConfirmation(std::shared_ptr<Tenant> tenant, double amount,
                                const std::chrono::system_clock::time_point& paymentDate);
    bool sendSecurityDepositRefund(std::shared_ptr<Tenant> tenant, double refundAmount);
    std::vector<std::string> getNotificationsForUser(const std::string& userId) const;
    int getDailyNotificationCount() const;
    bool canSendNotification(const std::string& userId) const;
    void clearOldNotifications(int daysOld);
    bool sendEmergencyAlert(std::shared_ptr<Tenant> tenant, const std::string& emergencyType,
                           const std::string& instructions);
    bool sendInspectionNotice(std::shared_ptr<Tenant> tenant, 
                             const std::chrono::system_clock::time_point& inspectionDate);
    int getUserNotificationCount(const std::string& userId) const;
    bool hasUserReceivedNotification(const std::string& userId, const std::string& notificationType) const;
};

#endif