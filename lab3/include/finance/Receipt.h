#ifndef RECEIPT_H
#define RECEIPT_H

#include <string>
#include <memory>
#include <chrono>
#include "../../include/finance/Payment.h"

class Receipt {
private:
    std::string receiptId;
    std::shared_ptr<Payment> payment;
    std::chrono::system_clock::time_point issueDate;
    std::string receiptNumber;
    double amount;
    std::string paymentMethod;
    std::string receivedFrom;
    std::string receivedBy;
    std::string description;
    std::string status;
    std::string taxDetails;

public:
    Receipt(const std::string& id, std::shared_ptr<Payment> payment,
            const std::chrono::system_clock::time_point& issueDate,
            const std::string& receiptNumber, double amount,
            const std::string& paymentMethod, const std::string& receivedFrom,
            const std::string& receivedBy, const std::string& description,
            const std::string& status, const std::string& taxDetails);
    
    Receipt(const Receipt& other);
    Receipt& operator=(const Receipt& other);
    Receipt(Receipt&& other) noexcept;
    Receipt& operator=(Receipt&& other) noexcept;
    ~Receipt();

    std::string getReceiptId() const;
    Payment* getPayment() const;
    std::chrono::system_clock::time_point getIssueDate() const;
    std::string getReceiptNumber() const;
    double getAmount() const;
    std::string getPaymentMethod() const;
    std::string getReceivedFrom() const;
    std::string getReceivedBy() const;
    std::string getDescription() const;
    std::string getStatus() const;
    std::string getTaxDetails() const;

    void setStatus(const std::string& newStatus);
    void setDescription(const std::string& newDescription);

    bool isDigitalReceipt() const;
    bool isValidForTax() const;
    double calculateTaxAmount() const;
    bool isRentReceipt() const;
    bool isSecurityDepositReceipt() const;
    std::string getReceiptType() const;
    bool requiresArchiving() const;
    int getDaysSinceIssue(const std::chrono::system_clock::time_point& currentDate) const;
    std::string getFinancialYear() const;
};

#endif