#ifndef INVOICE_H
#define INVOICE_H

#include <string>
#include <memory>
#include <chrono>
#include <vector>
#include "../../include/core/Tenant.h"
#include "../../include/finance/RentalContract.h"

class Invoice {
private:
    std::string invoiceId;
    std::shared_ptr<Tenant> tenant;
    std::shared_ptr<RentalContract> contract;
    double amount;
    double taxAmount;
    std::chrono::system_clock::time_point issueDate;
    std::chrono::system_clock::time_point dueDate;
    std::string invoiceType;
    std::string status;
    std::vector<std::string> lineItems;
    double lateFee;
    double discount;

public:
    Invoice(const std::string& id, std::shared_ptr<Tenant> ten,
            std::shared_ptr<RentalContract> cont, double amt,
            double tax, const std::chrono::system_clock::time_point& issue,
            const std::chrono::system_clock::time_point& due,
            const std::string& type, const std::string& status,
            const std::vector<std::string>& items, double fee, double disc);
    
    Invoice(const Invoice& other);
    Invoice& operator=(const Invoice& other);
    Invoice(Invoice&& other) noexcept;
    Invoice& operator=(Invoice&& other) noexcept;
    ~Invoice();

    std::string getInvoiceId() const;
    Tenant* getTenant() const;
    RentalContract* getContract() const;
    double getAmount() const;
    double getTaxAmount() const;
    std::chrono::system_clock::time_point getIssueDate() const;
    std::chrono::system_clock::time_point getDueDate() const;
    std::string getInvoiceType() const;
    std::string getStatus() const;
    std::vector<std::string> getLineItems() const;
    double getLateFee() const;
    double getDiscount() const;

    void setStatus(const std::string& newStatus);
    void setLateFee(double newFee);
    void setDiscount(double newDiscount);

    double calculateTotalAmount() const;
    bool isOverdue(const std::chrono::system_clock::time_point& currentDate) const;
    int getDaysOverdue(const std::chrono::system_clock::time_point& currentDate) const;
    bool isRentInvoice() const;
    bool isMaintenanceInvoice() const;
    double calculateNetAmount() const;
    bool hasDiscountEligibility() const;
    bool requiresTaxDocumentation() const;
    std::string getPaymentTerms() const;
};

#endif