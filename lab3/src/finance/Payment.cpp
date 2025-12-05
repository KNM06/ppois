#include "../../include/finance/Payment.h"
#include <chrono>
#include <algorithm>

Payment::Payment(const std::string& id, std::shared_ptr<Tenant> ten,
                 std::shared_ptr<RentalContract> cont, double amt,
                 const std::chrono::system_clock::time_point& date,
                 const std::string& method, const std::string& status,
                 const std::string& transaction, double fee, const std::string& period)
    : paymentId(id), tenant(ten), contract(cont), amount(amt),
      paymentDate(date), paymentMethod(method), status(status),
      transactionId(transaction), lateFee(fee), paymentPeriod(period) {}

Payment::Payment(const Payment& other)
    : paymentId(other.paymentId), tenant(other.tenant), contract(other.contract),
      amount(other.amount), paymentDate(other.paymentDate),
      paymentMethod(other.paymentMethod), status(other.status),
      transactionId(other.transactionId), lateFee(other.lateFee),
      paymentPeriod(other.paymentPeriod) {}
Payment::Payment(const std::string& id, const std::string& tenantId, double amt,
                 const std::string& status, const std::chrono::system_clock::time_point& date,
                 bool isRentPayment)
    : paymentId(id),
      tenant(nullptr),
      contract(nullptr),
      amount(amt),
      paymentDate(date),
      paymentMethod("unknown"),
      status(status),
      transactionId(""),
      lateFee(0.0),
      paymentPeriod(isRentPayment ? "rent" : "other") {}

Payment& Payment::operator=(const Payment& other) {
    if (this != &other) {
        paymentId = other.paymentId;
        tenant = other.tenant;
        contract = other.contract;
        amount = other.amount;
        paymentDate = other.paymentDate;
        paymentMethod = other.paymentMethod;
        status = other.status;
        transactionId = other.transactionId;
        lateFee = other.lateFee;
        paymentPeriod = other.paymentPeriod;
    }
    return *this;
}

Payment::Payment(Payment&& other) noexcept
    : paymentId(std::move(other.paymentId)), tenant(std::move(other.tenant)),
      contract(std::move(other.contract)), amount(other.amount),
      paymentDate(other.paymentDate), paymentMethod(std::move(other.paymentMethod)),
      status(std::move(other.status)), transactionId(std::move(other.transactionId)),
      lateFee(other.lateFee), paymentPeriod(std::move(other.paymentPeriod)) {}

Payment& Payment::operator=(Payment&& other) noexcept {
    if (this != &other) {
        paymentId = std::move(other.paymentId);
        tenant = std::move(other.tenant);
        contract = std::move(other.contract);
        amount = other.amount;
        paymentDate = other.paymentDate;
        paymentMethod = std::move(other.paymentMethod);
        status = std::move(other.status);
        transactionId = std::move(other.transactionId);
        lateFee = other.lateFee;
        paymentPeriod = std::move(other.paymentPeriod);
    }
    return *this;
}

Payment::~Payment() {}

std::string Payment::getPaymentId() const { return paymentId; }
Tenant* Payment::getTenant() const { return tenant.get(); }
RentalContract* Payment::getContract() const { return contract.get(); }
double Payment::getAmount() const { return amount; }
std::chrono::system_clock::time_point Payment::getPaymentDate() const { return paymentDate; }
std::string Payment::getPaymentMethod() const { return paymentMethod; }
std::string Payment::getStatus() const { return status; }
std::string Payment::getTransactionId() const { return transactionId; }
double Payment::getLateFee() const { return lateFee; }
std::string Payment::getPaymentPeriod() const { return paymentPeriod; }

void Payment::setStatus(const std::string& newStatus) {
    status = newStatus;
}

void Payment::setTransactionId(const std::string& newTransactionId) {
    transactionId = newTransactionId;
}

void Payment::updateLateFee(double newFee) {
    lateFee = std::max(0.0, newFee);
}

bool Payment::isPaymentOnTime(const std::chrono::system_clock::time_point& dueDate) const {
    return paymentDate <= dueDate;
}

double Payment::calculateTotalAmountWithFees() const {
    return amount + lateFee;
}

bool Payment::isRentPayment() const {
    return paymentPeriod.find("rent") != std::string::npos ||
           paymentPeriod.find("Rent") != std::string::npos;
}

bool Payment::isSecurityDeposit() const {
    return paymentPeriod.find("deposit") != std::string::npos ||
           paymentPeriod.find("Deposit") != std::string::npos;
}

bool Payment::requiresBankTransfer() const {
    return amount > 5000.0 || paymentMethod == "bank_transfer";
}

bool Payment::canBeRefunded() const {
    return status == "completed" && 
           (isSecurityDeposit() || paymentPeriod.find("advance") != std::string::npos);
}

double Payment::calculateRefundAmount() const {
    if (!canBeRefunded()) return 0.0;
    
    if (isSecurityDeposit()) {
        return amount * 0.8;
    }
    return amount;
}

bool Payment::isOverdue(const std::chrono::system_clock::time_point& currentDate) const {
    if (status == "completed" || status == "refunded") return false;
    
    auto duration = std::chrono::duration_cast<std::chrono::hours>(currentDate - paymentDate);
    int daysOverdue = static_cast<int>(duration.count() / 24.0);
    return daysOverdue > 5;
}