#ifndef COMMISSION_H
#define COMMISSION_H

#include <string>
#include <memory>
#include <chrono>
#include "../../include/core/Agent.h"
#include "../../include/finance/RentalContract.h"

class Commission {
private:
    std::string commissionId;
    std::shared_ptr<Agent> agent;
    std::shared_ptr<RentalContract> contract;
    double commissionRate;
    double commissionAmount;
    std::chrono::system_clock::time_point earnedDate;
    std::chrono::system_clock::time_point paymentDate;
    std::string status;
    std::string paymentMethod;
    double bonusAmount;
    std::string commissionType;

public:
    Commission(const std::string& id, std::shared_ptr<Agent> ag,
               std::shared_ptr<RentalContract> cont, double rate,
               double amount, const std::chrono::system_clock::time_point& earned,
               const std::chrono::system_clock::time_point& payment,
               const std::string& status, const std::string& paymentMethod,
               double bonus, const std::string& type);
    
    Commission(const Commission& other);
    Commission& operator=(const Commission& other);
    Commission(Commission&& other) noexcept;
    Commission& operator=(Commission&& other) noexcept;
    ~Commission();

    std::string getCommissionId() const;
    Agent* getAgent() const;
    RentalContract* getContract() const;
    double getCommissionRate() const;
    double getCommissionAmount() const;
    std::chrono::system_clock::time_point getEarnedDate() const;
    std::chrono::system_clock::time_point getPaymentDate() const;
    std::string getStatus() const;
    std::string getPaymentMethod() const;
    double getBonusAmount() const;
    std::string getCommissionType() const;

    void setStatus(const std::string& newStatus);
    void setPaymentDate(const std::chrono::system_clock::time_point& newDate);
    void setBonusAmount(double newBonus);

    double calculateTotalCommission() const;
    bool isPaid() const;
    bool isOverdue(const std::chrono::system_clock::time_point& currentDate) const;
    double calculateTaxWithholding(double taxRate) const;
    bool isLeasingCommission() const;
    bool isManagementCommission() const;
    double calculateNetCommission(double taxRate) const;
    bool qualifiesForBonus() const;
    bool requiresManagerApproval() const;
    std::string getCommissionTier() const;
};

#endif