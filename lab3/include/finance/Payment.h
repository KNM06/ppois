#ifndef PAYMENT_H
#define PAYMENT_H

#include <string>
#include <memory>
#include <chrono>
#include "../../include/core/Tenant.h"
#include "../../include/finance/RentalContract.h"

class Payment {
private:
    std::string paymentId;
    std::shared_ptr<Tenant> tenant;
    std::shared_ptr<RentalContract> contract;
    double amount;
    std::chrono::system_clock::time_point paymentDate;
    std::string paymentMethod;
    std::string status;
    std::string transactionId;
    double lateFee;
    std::string paymentPeriod;

public:
    Payment(const std::string& id, std::shared_ptr<Tenant> ten,
            std::shared_ptr<RentalContract> cont, double amt,
            const std::chrono::system_clock::time_point& date,
            const std::string& method, const std::string& status,
            const std::string& transaction, double fee, const std::string& period);
    
    Payment(const Payment& other);
    Payment(const std::string& id, const std::string& tenantId, double amt,
        const std::string& status, const std::chrono::system_clock::time_point& date,
        bool isRentPayment);

    Payment& operator=(const Payment& other);
    Payment(Payment&& other) noexcept;
    Payment& operator=(Payment&& other) noexcept;
    ~Payment();

    std::string getPaymentId() const;
    Tenant* getTenant() const;
    RentalContract* getContract() const;
    double getAmount() const;
    std::chrono::system_clock::time_point getPaymentDate() const;
    std::string getPaymentMethod() const;
    std::string getStatus() const;
    std::string getTransactionId() const;
    double getLateFee() const;
    std::string getPaymentPeriod() const;

    void setStatus(const std::string& newStatus);
    void setTransactionId(const std::string& newTransactionId);
    void updateLateFee(double newFee);

    bool isPaymentOnTime(const std::chrono::system_clock::time_point& dueDate) const;
    double calculateTotalAmountWithFees() const;
    bool isRentPayment() const;
    bool isSecurityDeposit() const;
    bool requiresBankTransfer() const;
    bool canBeRefunded() const;
    double calculateRefundAmount() const;
    bool isOverdue(const std::chrono::system_clock::time_point& currentDate) const;
};

#endif