#include "../../include/business/NotificationSystem.h"
#include <chrono>
#include <algorithm>

NotificationSystem::NotificationSystem(bool email, bool sms, bool push, int maxNotifications)
    : emailEnabled(email), smsEnabled(sms), pushEnabled(push),
      maxNotificationsPerDay(maxNotifications),
      lastNotificationReset(std::chrono::system_clock::now()) {}

NotificationSystem::NotificationSystem(const NotificationSystem& other)
    : sentNotifications(other.sentNotifications), userNotifications(other.userNotifications),
      emailEnabled(other.emailEnabled), smsEnabled(other.smsEnabled),
      pushEnabled(other.pushEnabled), maxNotificationsPerDay(other.maxNotificationsPerDay),
      lastNotificationReset(other.lastNotificationReset) {}

NotificationSystem& NotificationSystem::operator=(const NotificationSystem& other) {
    if (this != &other) {
        sentNotifications = other.sentNotifications;
        userNotifications = other.userNotifications;
        emailEnabled = other.emailEnabled;
        smsEnabled = other.smsEnabled;
        pushEnabled = other.pushEnabled;
        maxNotificationsPerDay = other.maxNotificationsPerDay;
        lastNotificationReset = other.lastNotificationReset;
    }
    return *this;
}

NotificationSystem::NotificationSystem(NotificationSystem&& other) noexcept
    : sentNotifications(std::move(other.sentNotifications)),
      userNotifications(std::move(other.userNotifications)),
      emailEnabled(other.emailEnabled), smsEnabled(other.smsEnabled),
      pushEnabled(other.pushEnabled), maxNotificationsPerDay(other.maxNotificationsPerDay),
      lastNotificationReset(other.lastNotificationReset) {}

NotificationSystem& NotificationSystem::operator=(NotificationSystem&& other) noexcept {
    if (this != &other) {
        sentNotifications = std::move(other.sentNotifications);
        userNotifications = std::move(other.userNotifications);
        emailEnabled = other.emailEnabled;
        smsEnabled = other.smsEnabled;
        pushEnabled = other.pushEnabled;
        maxNotificationsPerDay = other.maxNotificationsPerDay;
        lastNotificationReset = other.lastNotificationReset;
    }
    return *this;
}

NotificationSystem::~NotificationSystem() {}

bool NotificationSystem::getEmailEnabled() const { return emailEnabled; }
bool NotificationSystem::getSmsEnabled() const { return smsEnabled; }
bool NotificationSystem::getPushEnabled() const { return pushEnabled; }
int NotificationSystem::getMaxNotificationsPerDay() const { return maxNotificationsPerDay; }
std::chrono::system_clock::time_point NotificationSystem::getLastNotificationReset() const { return lastNotificationReset; }

void NotificationSystem::setEmailEnabled(bool enabled) {
    emailEnabled = enabled;
}

void NotificationSystem::setSmsEnabled(bool enabled) {
    smsEnabled = enabled;
}

void NotificationSystem::setPushEnabled(bool enabled) {
    pushEnabled = enabled;
}

void NotificationSystem::setMaxNotificationsPerDay(int max) {
    maxNotificationsPerDay = std::max(1, max);
}

bool NotificationSystem::sendRentReminder(std::shared_ptr<Tenant> tenant, std::shared_ptr<RentalContract> contract,
                                         const std::chrono::system_clock::time_point& dueDate) {
    if (!tenant || !contract) return false;
    
    std::string notificationId = "RENT_REMINDER_" + tenant->getTenantId() + "_" + 
                                std::to_string(std::chrono::system_clock::to_time_t(dueDate));
    
    if (!canSendNotification(tenant->getTenantId())) return false;
    
    sentNotifications.push_back(notificationId);
    userNotifications[tenant->getTenantId()].push_back("Rent due reminder for contract " + 
                                                      contract->getContractId());
    
    return true;
}

bool NotificationSystem::sendMaintenanceUpdate(std::shared_ptr<Tenant> tenant, const std::string& maintenanceStatus,
                                              const std::string& details) {
    if (!tenant) return false;
    
    std::string notificationId = "MAINT_UPDATE_" + tenant->getTenantId() + "_" + 
                                std::to_string(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));
    
    if (!canSendNotification(tenant->getTenantId())) return false;
    
    sentNotifications.push_back(notificationId);
    userNotifications[tenant->getTenantId()].push_back("Maintenance update: " + maintenanceStatus + " - " + details);
    
    return true;
}

bool NotificationSystem::sendContractExpirationAlert(std::shared_ptr<Owner> owner, std::shared_ptr<RentalContract> contract,
                                                    int daysUntilExpiration) {
    if (!owner || !contract) return false;
    
    std::string notificationId = "CONTRACT_EXPIRY_" + owner->getOwnerId() + "_" + 
                                contract->getContractId();
    
    if (!canSendNotification(owner->getOwnerId())) return false;
    
    sentNotifications.push_back(notificationId);
    userNotifications[owner->getOwnerId()].push_back("Contract " + contract->getContractId() + 
                                                    " expires in " + std::to_string(daysUntilExpiration) + " days");
    
    return true;
}

bool NotificationSystem::sendPaymentConfirmation(std::shared_ptr<Tenant> tenant, double amount,
                                                const std::chrono::system_clock::time_point& paymentDate) {
    if (!tenant) return false;
    
    std::string notificationId = "PAYMENT_CONFIRM_" + tenant->getTenantId() + "_" + 
                                std::to_string(std::chrono::system_clock::to_time_t(paymentDate));
    
    if (!canSendNotification(tenant->getTenantId())) return false;
    
    sentNotifications.push_back(notificationId);
    userNotifications[tenant->getTenantId()].push_back("Payment confirmed: $" + 
                                                      std::to_string(amount) + " on " + 
                                                      std::to_string(std::chrono::system_clock::to_time_t(paymentDate)));
    
    return true;
}

bool NotificationSystem::sendSecurityDepositRefund(std::shared_ptr<Tenant> tenant, double refundAmount) {
    if (!tenant) return false;
    
    std::string notificationId = "DEPOSIT_REFUND_" + tenant->getTenantId() + "_" + 
                                std::to_string(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));
    
    if (!canSendNotification(tenant->getTenantId())) return false;
    
    sentNotifications.push_back(notificationId);
    userNotifications[tenant->getTenantId()].push_back("Security deposit refund: $" + 
                                                      std::to_string(refundAmount));
    
    return true;
}

std::vector<std::string> NotificationSystem::getNotificationsForUser(const std::string& userId) const {
    auto it = userNotifications.find(userId);
    return it != userNotifications.end() ? it->second : std::vector<std::string>();
}

int NotificationSystem::getDailyNotificationCount() const {
    auto now = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::hours>(now - lastNotificationReset);
    
    if (duration.count() >= 24) {
        const_cast<NotificationSystem*>(this)->lastNotificationReset = now;
        return 0;
    }
    
    return sentNotifications.size();
}

bool NotificationSystem::canSendNotification(const std::string& userId) const {
    if (getDailyNotificationCount() >= maxNotificationsPerDay) return false;
    
    auto userNotifs = getNotificationsForUser(userId);
    return userNotifs.size() < maxNotificationsPerDay / 2;
}

void NotificationSystem::clearOldNotifications(int daysOld) {
    auto cutoffTime = std::chrono::system_clock::now() - std::chrono::hours(daysOld * 24);
    
    sentNotifications.erase(
        std::remove_if(sentNotifications.begin(), sentNotifications.end(),
            [&](const std::string& notif) {
                return notif.find("_") != std::string::npos;
            }),
        sentNotifications.end()
    );
}

bool NotificationSystem::sendEmergencyAlert(std::shared_ptr<Tenant> tenant, const std::string& emergencyType,
                                           const std::string& instructions) {
    if (!tenant) return false;
    
    std::string notificationId = "EMERGENCY_" + tenant->getTenantId() + "_" + 
                                std::to_string(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));
    
    sentNotifications.push_back(notificationId);
    userNotifications[tenant->getTenantId()].push_back("EMERGENCY: " + emergencyType + " - " + instructions);
    
    return true;
}

bool NotificationSystem::sendInspectionNotice(std::shared_ptr<Tenant> tenant, 
                                             const std::chrono::system_clock::time_point& inspectionDate) {
    if (!tenant) return false;
    
    std::string notificationId = "INSPECTION_" + tenant->getTenantId() + "_" + 
                                std::to_string(std::chrono::system_clock::to_time_t(inspectionDate));
    
    if (!canSendNotification(tenant->getTenantId())) return false;
    
    sentNotifications.push_back(notificationId);
    userNotifications[tenant->getTenantId()].push_back("Property inspection scheduled for " + 
                                                      std::to_string(std::chrono::system_clock::to_time_t(inspectionDate)));
    
    return true;
}

int NotificationSystem::getUserNotificationCount(const std::string& userId) const {
    auto it = userNotifications.find(userId);
    return it != userNotifications.end() ? it->second.size() : 0;
}

bool NotificationSystem::hasUserReceivedNotification(const std::string& userId, const std::string& notificationType) const {
    auto userNotifs = getNotificationsForUser(userId);
    for (const auto& notif : userNotifs) {
        if (notif.find(notificationType) != std::string::npos) {
            return true;
        }
    }
    return false;
}