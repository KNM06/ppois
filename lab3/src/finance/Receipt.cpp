#include "../../include/finance/Receipt.h"
#include <chrono>
#include <algorithm>

Receipt::Receipt(const std::string& id, std::shared_ptr<Payment> payment,
                 const std::chrono::system_clock::time_point& issueDate,
                 const std::string& receiptNumber, double amount,
                 const std::string& paymentMethod, const std::string& receivedFrom,
                 const std::string& receivedBy, const std::string& description,
                 const std::string& status, const std::string& taxDetails)
    : receiptId(id), payment(payment), issueDate(issueDate), receiptNumber(receiptNumber),
      amount(amount), paymentMethod(paymentMethod), receivedFrom(receivedFrom),
      receivedBy(receivedBy), description(description), status(status),
      taxDetails(taxDetails) {}

Receipt::Receipt(const Receipt& other)
    : receiptId(other.receiptId), payment(other.payment), issueDate(other.issueDate),
      receiptNumber(other.receiptNumber), amount(other.amount),
      paymentMethod(other.paymentMethod), receivedFrom(other.receivedFrom),
      receivedBy(other.receivedBy), description(other.description),
      status(other.status), taxDetails(other.taxDetails) {}

Receipt& Receipt::operator=(const Receipt& other) {
    if (this != &other) {
        receiptId = other.receiptId;
        payment = other.payment;
        issueDate = other.issueDate;
        receiptNumber = other.receiptNumber;
        amount = other.amount;
        paymentMethod = other.paymentMethod;
        receivedFrom = other.receivedFrom;
        receivedBy = other.receivedBy;
        description = other.description;
        status = other.status;
        taxDetails = other.taxDetails;
    }
    return *this;
}

Receipt::Receipt(Receipt&& other) noexcept
    : receiptId(std::move(other.receiptId)), payment(std::move(other.payment)),
      issueDate(other.issueDate), receiptNumber(std::move(other.receiptNumber)),
      amount(other.amount), paymentMethod(std::move(other.paymentMethod)),
      receivedFrom(std::move(other.receivedFrom)), receivedBy(std::move(other.receivedBy)),
      description(std::move(other.description)), status(std::move(other.status)),
      taxDetails(std::move(other.taxDetails)) {}

Receipt& Receipt::operator=(Receipt&& other) noexcept {
    if (this != &other) {
        receiptId = std::move(other.receiptId);
        payment = std::move(other.payment);
        issueDate = other.issueDate;
        receiptNumber = std::move(other.receiptNumber);
        amount = other.amount;
        paymentMethod = std::move(other.paymentMethod);
        receivedFrom = std::move(other.receivedFrom);
        receivedBy = std::move(other.receivedBy);
        description = std::move(other.description);
        status = std::move(other.status);
        taxDetails = std::move(other.taxDetails);
    }
    return *this;
}

Receipt::~Receipt() {}

std::string Receipt::getReceiptId() const { return receiptId; }
Payment* Receipt::getPayment() const { return payment.get(); }
std::chrono::system_clock::time_point Receipt::getIssueDate() const { return issueDate; }
std::string Receipt::getReceiptNumber() const { return receiptNumber; }
double Receipt::getAmount() const { return amount; }
std::string Receipt::getPaymentMethod() const { return paymentMethod; }
std::string Receipt::getReceivedFrom() const { return receivedFrom; }
std::string Receipt::getReceivedBy() const { return receivedBy; }
std::string Receipt::getDescription() const { return description; }
std::string Receipt::getStatus() const { return status; }
std::string Receipt::getTaxDetails() const { return taxDetails; }

void Receipt::setStatus(const std::string& newStatus) {
    status = newStatus;
}

void Receipt::setDescription(const std::string& newDescription) {
    description = newDescription;
}

bool Receipt::isDigitalReceipt() const {
    return receiptNumber.find("DIG") != std::string::npos || paymentMethod == "electronic";
}

bool Receipt::isValidForTax() const {
    return amount > 0.0 && !taxDetails.empty() && status == "issued";
}

double Receipt::calculateTaxAmount() const {
    if (taxDetails.find("10%") != std::string::npos) return amount * 0.10;
    if (taxDetails.find("15%") != std::string::npos) return amount * 0.15;
    if (taxDetails.find("20%") != std::string::npos) return amount * 0.20;
    return 0.0;
}

bool Receipt::isRentReceipt() const {
    return description.find("rent") != std::string::npos || 
           description.find("Rent") != std::string::npos;
}

bool Receipt::isSecurityDepositReceipt() const {
    return description.find("deposit") != std::string::npos || 
           description.find("security") != std::string::npos;
}

std::string Receipt::getReceiptType() const {
    if (isRentReceipt()) return "rent";
    if (isSecurityDepositReceipt()) return "security_deposit";
    if (description.find("maintenance") != std::string::npos) return "maintenance";
    if (description.find("fee") != std::string::npos) return "fee";
    return "other";
}

bool Receipt::requiresArchiving() const {
    return amount >= 1000.0 || isRentReceipt() || isValidForTax();
}

int Receipt::getDaysSinceIssue(const std::chrono::system_clock::time_point& currentDate) const {
    auto duration = std::chrono::duration_cast<std::chrono::hours>(currentDate - issueDate);
    return static_cast<int>(duration.count() / 24.0);
}

std::string Receipt::getFinancialYear() const {
    auto time = std::chrono::system_clock::to_time_t(issueDate);
    std::tm* timeinfo = std::localtime(&time);
    int year = timeinfo->tm_year + 1900;
    int month = timeinfo->tm_mon + 1;
    
    if (month < 4) {
        return std::to_string(year - 1) + "-" + std::to_string(year);
    } else {
        return std::to_string(year) + "-" + std::to_string(year + 1);
    }
}