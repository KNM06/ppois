#ifndef PROPERTYSTATUS_H
#define PROPERTYSTATUS_H

#include <string>
#include <memory>
#include <chrono>

class PropertyStatus {
private:
    std::string statusCode;
    std::string description;
    bool isAvailable;
    bool isUnderMaintenance;
    bool isRented;
    std::chrono::system_clock::time_point statusDate;
    std::string statusReason;
    double discountPercentage;
    int daysOnMarket;

public:
    PropertyStatus();
    PropertyStatus(const std::string& code);
    PropertyStatus(const std::string& code, const std::string& desc,
                  bool available, bool maintenance, bool rented,
                  const std::chrono::system_clock::time_point& date,
                  const std::string& reason, double discount, int daysOnMarket);
    
    PropertyStatus(const PropertyStatus& other);
    PropertyStatus& operator=(const PropertyStatus& other);
    PropertyStatus(PropertyStatus&& other) noexcept;
    PropertyStatus& operator=(PropertyStatus&& other) noexcept;
    ~PropertyStatus();

    std::string getStatusCode() const;
    std::string getDescription() const;
    bool getIsAvailable() const;
    bool getIsUnderMaintenance() const;
    bool getIsRented() const;
    std::chrono::system_clock::time_point getStatusDate() const;
    std::string getStatusReason() const;
    double getDiscountPercentage() const;
    int getDaysOnMarket() const;

    void setStatusCode(const std::string& newCode);
    void setDescription(const std::string& newDesc);
    void updateStatus(bool available, bool maintenance, bool rented);
    void setDiscountPercentage(double discount);

    bool canShowProperty() const;
    bool requiresPriceAdjustment() const;
    double calculateDiscountedPrice(double originalPrice) const;
    bool isLongTermVacancy(int thresholdDays) const;
    bool shouldIncreaseMarketing() const;
    bool isEligibleForPromotion() const;
    bool needsMaintenanceCheck() const;
    std::string getMarketAnalysis() const;
};

#endif