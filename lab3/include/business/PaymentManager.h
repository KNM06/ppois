#ifndef PAYMENTMANAGER_H
#define PAYMENTMANAGER_H

#include <memory>
#include <vector>
#include <unordered_map>
#include <chrono>
#include "../../include/finance/Payment.h"
#include "../../include/finance/RentalContract.h"

class PaymentManager {
private:
    std::vector<std::shared_ptr<Payment>> payments;
    std::unordered_map<std::string, std::vector<std::shared_ptr<Payment>>> tenantPayments;
    std::unordered_map<std::string, double> tenantBalances;
    double lateFeeRate;
    int gracePeriodDays;
    std::string defaultPaymentMethod;

public:
    PaymentManager(double lateFee, int gracePeriod, const std::string& defaultMethod);
    
    PaymentManager(const PaymentManager& other);
    PaymentManager& operator=(const PaymentManager& other);
    PaymentManager(PaymentManager&& other) noexcept;
    PaymentManager& operator=(PaymentManager&& other) noexcept;
    ~PaymentManager();

    double getLateFeeRate() const;
    int getGracePeriodDays() const;
    std::string getDefaultPaymentMethod() const;

    void setLateFeeRate(double newRate);
    void setGracePeriodDays(int newDays);
    void setDefaultPaymentMethod(const std::string& newMethod);

    bool processPayment(std::shared_ptr<Payment> payment);
    bool recordRentPayment(std::shared_ptr<Tenant> tenant,
                          std::shared_ptr<RentalContract> contract,
                          double amount, const std::chrono::system_clock::time_point& paymentDate,
                          const std::string& paymentMethod, const std::string& period);
    double calculateLateFee(double amount, int daysLate) const;
    double getTenantBalance(const std::string& tenantId) const;
    std::vector<std::shared_ptr<Payment>> getPaymentsByTenant(const std::string& tenantId) const;
    std::vector<std::shared_ptr<Payment>> getOverduePayments(const std::chrono::system_clock::time_point& currentDate) const;
    double calculateTotalRevenue(const std::chrono::system_clock::time_point& startDate,
                                const std::chrono::system_clock::time_point& endDate) const;
    bool hasTenantMissedPayments(const std::string& tenantId, int maxAllowed) const;
    double calculateCollectionRate(const std::chrono::system_clock::time_point& startDate,
                                  const std::chrono::system_clock::time_point& endDate) const;
};

#endif