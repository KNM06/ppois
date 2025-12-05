#include "../../include/finance/Invoice.h"
#include <chrono>
#include <algorithm>

Invoice::Invoice(const std::string& id, std::shared_ptr<Tenant> ten,
                 std::shared_ptr<RentalContract> cont, double amt,
                 double tax, const std::chrono::system_clock::time_point& issue,
                 const std::chrono::system_clock::time_point& due,
                 const std::string& type, const std::string& status,
                 const std::vector<std::string>& items, double fee, double disc)
    : invoiceId(id), tenant(ten), contract(cont), amount(amt),
      taxAmount(tax), issueDate(issue), dueDate(due), invoiceType(type),
      status(status), lineItems(items), lateFee(fee), discount(disc) {}

Invoice::Invoice(const Invoice& other)
    : invoiceId(other.invoiceId), tenant(other.tenant), contract(other.contract),
      amount(other.amount), taxAmount(other.taxAmount), issueDate(other.issueDate),
      dueDate(other.dueDate), invoiceType(other.invoiceType), status(other.status),
      lineItems(other.lineItems), lateFee(other.lateFee), discount(other.discount) {}

Invoice& Invoice::operator=(const Invoice& other) {
    if (this != &other) {
        invoiceId = other.invoiceId;
        tenant = other.tenant;
        contract = other.contract;
        amount = other.amount;
        taxAmount = other.taxAmount;
        issueDate = other.issueDate;
        dueDate = other.dueDate;
        invoiceType = other.invoiceType;
        status = other.status;
        lineItems = other.lineItems;
        lateFee = other.lateFee;
        discount = other.discount;
    }
    return *this;
}

Invoice::Invoice(Invoice&& other) noexcept
    : invoiceId(std::move(other.invoiceId)), tenant(std::move(other.tenant)),
      contract(std::move(other.contract)), amount(other.amount),
      taxAmount(other.taxAmount), issueDate(other.issueDate), dueDate(other.dueDate),
      invoiceType(std::move(other.invoiceType)), status(std::move(other.status)),
      lineItems(std::move(other.lineItems)), lateFee(other.lateFee),
      discount(other.discount) {}

Invoice& Invoice::operator=(Invoice&& other) noexcept {
    if (this != &other) {
        invoiceId = std::move(other.invoiceId);
        tenant = std::move(other.tenant);
        contract = std::move(other.contract);
        amount = other.amount;
        taxAmount = other.taxAmount;
        issueDate = other.issueDate;
        dueDate = other.dueDate;
        invoiceType = std::move(other.invoiceType);
        status = std::move(other.status);
        lineItems = std::move(other.lineItems);
        lateFee = other.lateFee;
        discount = other.discount;
    }
    return *this;
}

Invoice::~Invoice() {}

std::string Invoice::getInvoiceId() const { return invoiceId; }
Tenant* Invoice::getTenant() const { return tenant.get(); }
RentalContract* Invoice::getContract() const { return contract.get(); }
double Invoice::getAmount() const { return amount; }
double Invoice::getTaxAmount() const { return taxAmount; }
std::chrono::system_clock::time_point Invoice::getIssueDate() const { return issueDate; }
std::chrono::system_clock::time_point Invoice::getDueDate() const { return dueDate; }
std::string Invoice::getInvoiceType() const { return invoiceType; }
std::string Invoice::getStatus() const { return status; }
std::vector<std::string> Invoice::getLineItems() const { return lineItems; }
double Invoice::getLateFee() const { return lateFee; }
double Invoice::getDiscount() const { return discount; }

void Invoice::setStatus(const std::string& newStatus) {
    status = newStatus;
}

void Invoice::setLateFee(double newFee) {
    lateFee = std::max(0.0, newFee);
}

void Invoice::setDiscount(double newDiscount) {
    discount = std::max(0.0, newDiscount);
}

double Invoice::calculateTotalAmount() const {
    return amount + taxAmount + lateFee - discount;
}

bool Invoice::isOverdue(const std::chrono::system_clock::time_point& currentDate) const {
    return status != "paid" && currentDate > dueDate;
}

int Invoice::getDaysOverdue(const std::chrono::system_clock::time_point& currentDate) const {
    if (!isOverdue(currentDate)) return 0;
    auto duration = std::chrono::duration_cast<std::chrono::hours>(currentDate - dueDate);
    return static_cast<int>(duration.count() / 24.0);
}

bool Invoice::isRentInvoice() const {
    return invoiceType == "rent" || invoiceType.find("rent") != std::string::npos;
}

bool Invoice::isMaintenanceInvoice() const {
    return invoiceType == "maintenance" || invoiceType.find("maintenance") != std::string::npos;
}

double Invoice::calculateNetAmount() const {
    return amount - discount;
}

bool Invoice::hasDiscountEligibility() const {
    return discount > 0.0 && status == "pending";
}

bool Invoice::requiresTaxDocumentation() const {
    return taxAmount > 0.0 && isRentInvoice();
}

std::string Invoice::getPaymentTerms() const {
    if (isRentInvoice()) return "Net 30 days";
    if (isMaintenanceInvoice()) return "Net 15 days";
    return "Due upon receipt";
}