#include <gtest/gtest.h>
#include "../../include/finance/Invoice.h"
#include "../../include/core/Tenant.h"
#include "../../include/core/Address.h"
#include "../../include/finance/RentalContract.h"
#include "../../include/core/Property.h"
#include "../../include/core/PropertyStatus.h"
#include "../../include/core/PropertyType.h"

class InvoiceTest : public ::testing::Test {
protected:
    void SetUp() override {
        auto tenantAddress = std::make_unique<Address>("456 Tenant St", "New York", "NY", "10002", "USA", 40.7130, -74.0050);
        tenant = std::make_shared<Tenant>("T001", "John", "Doe", "john.doe@email.com", "555-0101", 
                                         std::move(tenantAddress), 750.0, 8000.0, false, 2);
        
        auto propertyAddress = std::make_unique<Address>("123 Main St", "New York", "NY", "10001", "USA", 40.7128, -74.0060);
        auto propertyStatus = std::make_unique<PropertyStatus>("AVAILABLE", "Available", true, false, false,
                                                              std::chrono::system_clock::now(), "", 0.0, 0);
        auto propertyType = std::make_unique<PropertyType>("APT", "residential", "Apartment", 1, 4, 30.0, 200.0, false, "residential", 1.2);
        auto property = std::make_shared<Property>("PROP001", 85.5, 2500.0, std::move(propertyAddress), 
                                                  std::move(propertyStatus), std::move(propertyType));
        
        auto ownerAddress = std::make_unique<Address>("789 Owner Ave", "New York", "NY", "10003", "USA", 40.7140, -74.0040);
        auto owner = std::make_shared<Owner>("O001", "Robert", "Johnson", "robert.johnson@email.com", "555-0201", 
                                            std::move(ownerAddress), "123456789", 100.0, true, "direct_deposit");
        
        auto startDate = std::chrono::system_clock::now();
        auto endDate = startDate + std::chrono::hours(365 * 24);
        contract = std::make_shared<RentalContract>("CNT001", property, tenant, owner, startDate, endDate, 
                                                   2500.0, 5000.0, "Net 30 days");
        
        issueDate = std::chrono::system_clock::now();
        dueDate = issueDate + std::chrono::hours(24 * 30);
        lineItems = {"Base Rent", "Water Utility", "Trash Collection"};
    }
    
    std::shared_ptr<Tenant> tenant;
    std::shared_ptr<RentalContract> contract;
    std::chrono::system_clock::time_point issueDate;
    std::chrono::system_clock::time_point dueDate;
    std::vector<std::string> lineItems;
};

TEST_F(InvoiceTest, ConstructorAndGetters) {
    Invoice invoice("INV001", tenant, contract, 2500.0, 200.0, issueDate, dueDate, 
                   "rent", "pending", lineItems, 0.0, 0.0);
    
    EXPECT_EQ(invoice.getInvoiceId(), "INV001");
    EXPECT_EQ(invoice.getTenant(), tenant.get());
    EXPECT_EQ(invoice.getContract(), contract.get());
    EXPECT_DOUBLE_EQ(invoice.getAmount(), 2500.0);
    EXPECT_DOUBLE_EQ(invoice.getTaxAmount(), 200.0);
    EXPECT_EQ(invoice.getIssueDate(), issueDate);
    EXPECT_EQ(invoice.getDueDate(), dueDate);
    EXPECT_EQ(invoice.getInvoiceType(), "rent");
    EXPECT_EQ(invoice.getStatus(), "pending");
    EXPECT_EQ(invoice.getLineItems().size(), 3);
    EXPECT_DOUBLE_EQ(invoice.getLateFee(), 0.0);
    EXPECT_DOUBLE_EQ(invoice.getDiscount(), 0.0);
}

TEST_F(InvoiceTest, TotalAmountCalculation) {
    Invoice invoice("INV001", tenant, contract, 2500.0, 200.0, issueDate, dueDate, 
                   "rent", "pending", lineItems, 50.0, 100.0);
    
    double totalAmount = invoice.calculateTotalAmount();
    EXPECT_DOUBLE_EQ(totalAmount, 2650.0);
}

TEST_F(InvoiceTest, OverdueStatus) {
    auto pastDueDate = std::chrono::system_clock::now() - std::chrono::hours(24 * 5);
    
    Invoice overdueInvoice("INV001", tenant, contract, 2500.0, 200.0, issueDate, pastDueDate, 
                          "rent", "pending", lineItems, 50.0, 0.0);
    
    Invoice paidInvoice("INV002", tenant, contract, 2500.0, 200.0, issueDate, pastDueDate, 
                       "rent", "paid", lineItems, 0.0, 0.0);
    
    Invoice futureInvoice("INV003", tenant, contract, 2500.0, 200.0, issueDate, dueDate, 
                         "rent", "pending", lineItems, 0.0, 0.0);
    
    EXPECT_TRUE(overdueInvoice.isOverdue(std::chrono::system_clock::now()));
    EXPECT_FALSE(paidInvoice.isOverdue(std::chrono::system_clock::now()));
    EXPECT_FALSE(futureInvoice.isOverdue(std::chrono::system_clock::now()));
}

TEST_F(InvoiceTest, DaysOverdueCalculation) {
    auto pastDueDate = std::chrono::system_clock::now() - std::chrono::hours(24 * 10);
    
    Invoice invoice("INV001", tenant, contract, 2500.0, 200.0, issueDate, pastDueDate, 
                   "rent", "pending", lineItems, 100.0, 0.0);
    
    int daysOverdue = invoice.getDaysOverdue(std::chrono::system_clock::now());
    EXPECT_EQ(daysOverdue, 10);
}

TEST_F(InvoiceTest, RentInvoiceIdentification) {
    Invoice rentInvoice("INV001", tenant, contract, 2500.0, 200.0, issueDate, dueDate, 
                       "rent", "pending", lineItems, 0.0, 0.0);
    
    Invoice maintenanceInvoice("INV002", tenant, contract, 150.0, 15.0, issueDate, dueDate, 
                              "maintenance", "pending", {"Repair service"}, 0.0, 0.0);
    
    EXPECT_TRUE(rentInvoice.isRentInvoice());
    EXPECT_FALSE(maintenanceInvoice.isRentInvoice());
}

TEST_F(InvoiceTest, MaintenanceInvoiceIdentification) {
    Invoice maintenanceInvoice("INV001", tenant, contract, 150.0, 15.0, issueDate, dueDate, 
                              "maintenance", "pending", {"Repair service"}, 0.0, 0.0);
    
    Invoice rentInvoice("INV002", tenant, contract, 2500.0, 200.0, issueDate, dueDate, 
                       "rent", "pending", lineItems, 0.0, 0.0);
    
    EXPECT_TRUE(maintenanceInvoice.isMaintenanceInvoice());
    EXPECT_FALSE(rentInvoice.isMaintenanceInvoice());
}

TEST_F(InvoiceTest, NetAmountCalculation) {
    Invoice invoice("INV001", tenant, contract, 2500.0, 200.0, issueDate, dueDate, 
                   "rent", "pending", lineItems, 0.0, 150.0);
    
    double netAmount = invoice.calculateNetAmount();
    EXPECT_DOUBLE_EQ(netAmount, 2350.0);
}

TEST_F(InvoiceTest, DiscountEligibility) {
    Invoice eligibleInvoice("INV001", tenant, contract, 2500.0, 200.0, issueDate, dueDate, 
                           "rent", "pending", lineItems, 0.0, 100.0);
    
    Invoice ineligibleInvoice("INV002", tenant, contract, 2500.0, 200.0, issueDate, dueDate, 
                             "rent", "paid", lineItems, 0.0, 100.0);
    
    Invoice noDiscountInvoice("INV003", tenant, contract, 2500.0, 200.0, issueDate, dueDate, 
                             "rent", "pending", lineItems, 0.0, 0.0);
    
    EXPECT_TRUE(eligibleInvoice.hasDiscountEligibility());
    EXPECT_FALSE(ineligibleInvoice.hasDiscountEligibility());
    EXPECT_FALSE(noDiscountInvoice.hasDiscountEligibility());
}

TEST_F(InvoiceTest, TaxDocumentationRequirement) {
    Invoice rentInvoice("INV001", tenant, contract, 2500.0, 200.0, issueDate, dueDate, 
                       "rent", "pending", lineItems, 0.0, 0.0);
    
    Invoice maintenanceInvoice("INV002", tenant, contract, 150.0, 0.0, issueDate, dueDate, 
                              "maintenance", "pending", {"Repair service"}, 0.0, 0.0);
    
    Invoice rentNoTax("INV003", tenant, contract, 2500.0, 0.0, issueDate, dueDate, 
                     "rent", "pending", lineItems, 0.0, 0.0);
    
    EXPECT_TRUE(rentInvoice.requiresTaxDocumentation());
    EXPECT_FALSE(maintenanceInvoice.requiresTaxDocumentation());
    EXPECT_FALSE(rentNoTax.requiresTaxDocumentation());
}

TEST_F(InvoiceTest, PaymentTerms) {
    Invoice rentInvoice("INV001", tenant, contract, 2500.0, 200.0, issueDate, dueDate, 
                       "rent", "pending", lineItems, 0.0, 0.0);
    
    Invoice maintenanceInvoice("INV002", tenant, contract, 150.0, 0.0, issueDate, dueDate, 
                              "maintenance", "pending", {"Repair service"}, 0.0, 0.0);
    
    Invoice otherInvoice("INV003", tenant, contract, 100.0, 0.0, issueDate, dueDate, 
                        "other", "pending", {"Service fee"}, 0.0, 0.0);
    
    EXPECT_EQ(rentInvoice.getPaymentTerms(), "Net 30 days");
    EXPECT_EQ(maintenanceInvoice.getPaymentTerms(), "Net 15 days");
    EXPECT_EQ(otherInvoice.getPaymentTerms(), "Due upon receipt");
}

TEST_F(InvoiceTest, SetterMethods) {
    Invoice invoice("INV001", tenant, contract, 2500.0, 200.0, issueDate, dueDate, 
                   "rent", "pending", lineItems, 0.0, 0.0);
    
    invoice.setStatus("paid");
    invoice.setLateFee(75.0);
    invoice.setDiscount(100.0);
    
    EXPECT_EQ(invoice.getStatus(), "paid");
    EXPECT_DOUBLE_EQ(invoice.getLateFee(), 75.0);
    EXPECT_DOUBLE_EQ(invoice.getDiscount(), 100.0);
}