#include <gtest/gtest.h>
#include <memory>
#include <chrono>
#include "../../include/finance/FinancialReport.h"
#include "../../include/finance/Payment.h"
#include "../../include/finance/Invoice.h"
#include "../../include/core/Tenant.h"
#include "../../include/finance/RentalContract.h"

class FinancialReportTest : public ::testing::Test {
protected:
    void SetUp() override {
        currentTime = std::chrono::system_clock::now();
        
        tenant1 = std::make_shared<Tenant>("TEN001", "John", "Doe", "john@example.com",
                                          "1234567890", nullptr, 700.0, 5000.0, false, 1);
        tenant2 = std::make_shared<Tenant>("TEN002", "Jane", "Smith", "jane@example.com",
                                          "0987654321", nullptr, 800.0, 6000.0, false, 2);
        
        contract1 = std::make_shared<RentalContract>("CONT001", nullptr, tenant1, 1500.0,
                                                    currentTime, currentTime + std::chrono::hours(24*30));
        contract2 = std::make_shared<RentalContract>("CONT002", nullptr, tenant2, 2000.0,
                                                    currentTime, currentTime + std::chrono::hours(24*30));
        
        payment1 = std::make_shared<Payment>("PAY001", tenant1, contract1, 1500.0, currentTime,
                                            "bank_transfer", "completed", "TX001", 0.0, "rent");
        payment2 = std::make_shared<Payment>("PAY002", tenant2, contract2, 2000.0, currentTime,
                                            "credit_card", "completed", "TX002", 100.0, "rent");
        payment3 = std::make_shared<Payment>("PAY003", tenant1, contract1, 500.0, currentTime,
                                            "bank_transfer", "pending", "TX003", 0.0, "deposit");
        
        Invoice invoice1("INV001", tenant1, contract1, 1500.0, 150.0,
                                            currentTime, currentTime + std::chrono::hours(24*30),
                                            "rent", "paid", {"rent"}, 0.0, 0.0);
        Invoice invoice2("INV002", tenant2, contract2, 2000.0, 200.0,
                                            currentTime, currentTime + std::chrono::hours(24*30),
                                            "rent", "pending", {"rent"}, 0.0, 0.0);
    }
    
    std::chrono::system_clock::time_point currentTime;
    std::shared_ptr<Tenant> tenant1;
    std::shared_ptr<Tenant> tenant2;
    std::shared_ptr<RentalContract> contract1;
    std::shared_ptr<RentalContract> contract2;
    std::shared_ptr<Payment> payment1;
    std::shared_ptr<Payment> payment2;
    std::shared_ptr<Payment> payment3;
    std::shared_ptr<Invoice> invoice1;
    std::shared_ptr<Invoice> invoice2;
};

TEST_F(FinancialReportTest, ConstructorAndGetters) {
    std::vector<std::shared_ptr<Payment>> payments = {payment1, payment2};
    std::vector<std::shared_ptr<Invoice>> invoices = {invoice1, invoice2};
    
    auto periodStart = currentTime - std::chrono::hours(24*30);
    auto periodEnd = currentTime;
    
    FinancialReport report("REP001", "monthly", currentTime, periodStart, periodEnd,
                          3500.0, 1500.0, 2000.0, payments, invoices, "USD", false);
    
    EXPECT_EQ(report.getReportId(), "REP001");
    EXPECT_EQ(report.getReportType(), "monthly");
    EXPECT_EQ(report.getReportDate(), currentTime);
    EXPECT_EQ(report.getPeriodStart(), periodStart);
    EXPECT_EQ(report.getPeriodEnd(), periodEnd);
    EXPECT_DOUBLE_EQ(report.getTotalRevenue(), 3500.0);
    EXPECT_DOUBLE_EQ(report.getTotalExpenses(), 1500.0);
    EXPECT_DOUBLE_EQ(report.getNetIncome(), 2000.0);
    EXPECT_EQ(report.getPayments().size(), 2);
    EXPECT_EQ(report.getInvoices().size(), 2);
    EXPECT_EQ(report.getCurrency(), "USD");
    EXPECT_FALSE(report.getIsFinalized());
}

TEST_F(FinancialReportTest, ConstructorWithEmptyVectors) {
    std::vector<std::shared_ptr<Payment>> payments;
    std::vector<std::shared_ptr<Invoice>> invoices;
    
    auto periodStart = currentTime - std::chrono::hours(24*30);
    auto periodEnd = currentTime;
    
    FinancialReport report("REP001", "monthly", currentTime, periodStart, periodEnd,
                          0.0, 0.0, 0.0, payments, invoices, "EUR", true);
    
    EXPECT_EQ(report.getReportId(), "REP001");
    EXPECT_EQ(report.getReportType(), "monthly");
    EXPECT_EQ(report.getReportDate(), currentTime);
    EXPECT_EQ(report.getPeriodStart(), periodStart);
    EXPECT_EQ(report.getPeriodEnd(), periodEnd);
    EXPECT_DOUBLE_EQ(report.getTotalRevenue(), 0.0);
    EXPECT_DOUBLE_EQ(report.getTotalExpenses(), 0.0);
    EXPECT_DOUBLE_EQ(report.getNetIncome(), 0.0);
    EXPECT_TRUE(report.getPayments().empty());
    EXPECT_TRUE(report.getInvoices().empty());
    EXPECT_EQ(report.getCurrency(), "EUR");
    EXPECT_TRUE(report.getIsFinalized());
}

TEST_F(FinancialReportTest, SetTotalRevenue) {
    std::vector<std::shared_ptr<Payment>> payments;
    std::vector<std::shared_ptr<Invoice>> invoices;
    
    FinancialReport report("REP001", "monthly", currentTime, currentTime, currentTime,
                          1000.0, 500.0, 500.0, payments, invoices, "USD", false);
    
    report.setTotalRevenue(2000.0);
    EXPECT_DOUBLE_EQ(report.getTotalRevenue(), 2000.0);
    EXPECT_DOUBLE_EQ(report.getNetIncome(), 1500.0);
    
    report.setTotalRevenue(0.0);
    EXPECT_DOUBLE_EQ(report.getTotalRevenue(), 0.0);
    EXPECT_DOUBLE_EQ(report.getNetIncome(), -500.0);
    
    report.setTotalRevenue(-1000.0);
    EXPECT_DOUBLE_EQ(report.getTotalRevenue(), 0.0);
    EXPECT_DOUBLE_EQ(report.getNetIncome(), -500.0);
    
    report.setTotalRevenue(1500.75);
    EXPECT_DOUBLE_EQ(report.getTotalRevenue(), 1500.75);
    EXPECT_DOUBLE_EQ(report.getNetIncome(), 1000.75);
}

TEST_F(FinancialReportTest, SetTotalExpenses) {
    std::vector<std::shared_ptr<Payment>> payments;
    std::vector<std::shared_ptr<Invoice>> invoices;
    
    FinancialReport report("REP001", "monthly", currentTime, currentTime, currentTime,
                          1000.0, 500.0, 500.0, payments, invoices, "USD", false);
    
    report.setTotalExpenses(300.0);
    EXPECT_DOUBLE_EQ(report.getTotalExpenses(), 300.0);
    EXPECT_DOUBLE_EQ(report.getNetIncome(), 700.0);
    
    report.setTotalExpenses(0.0);
    EXPECT_DOUBLE_EQ(report.getTotalExpenses(), 0.0);
    EXPECT_DOUBLE_EQ(report.getNetIncome(), 1000.0);
    
    report.setTotalExpenses(-200.0);
    EXPECT_DOUBLE_EQ(report.getTotalExpenses(), 0.0);
    EXPECT_DOUBLE_EQ(report.getNetIncome(), 1000.0);
    
    report.setTotalExpenses(750.25);
    EXPECT_DOUBLE_EQ(report.getTotalExpenses(), 750.25);
    EXPECT_DOUBLE_EQ(report.getNetIncome(), 249.75);
}

TEST_F(FinancialReportTest, SetIsFinalized) {
    std::vector<std::shared_ptr<Payment>> payments;
    std::vector<std::shared_ptr<Invoice>> invoices;
    
    FinancialReport report("REP001", "monthly", currentTime, currentTime, currentTime,
                          1000.0, 500.0, 500.0, payments, invoices, "USD", false);
    
    EXPECT_FALSE(report.getIsFinalized());
    
    report.setIsFinalized(true);
    EXPECT_TRUE(report.getIsFinalized());
    
    report.setIsFinalized(false);
    EXPECT_FALSE(report.getIsFinalized());
}

TEST_F(FinancialReportTest, CalculateProfitMargin) {
    std::vector<std::shared_ptr<Payment>> payments;
    std::vector<std::shared_ptr<Invoice>> invoices;
    
    FinancialReport report1("REP001", "monthly", currentTime, currentTime, currentTime,
                           1000.0, 500.0, 500.0, payments, invoices, "USD", false);
    EXPECT_DOUBLE_EQ(report1.calculateProfitMargin(), 50.0);
    
    FinancialReport report2("REP002", "monthly", currentTime, currentTime, currentTime,
                           2000.0, 1500.0, 500.0, payments, invoices, "USD", false);
    EXPECT_DOUBLE_EQ(report2.calculateProfitMargin(), 25.0);
    
    FinancialReport report3("REP003", "monthly", currentTime, currentTime, currentTime,
                           1000.0, 1000.0, 0.0, payments, invoices, "USD", false);
    EXPECT_DOUBLE_EQ(report3.calculateProfitMargin(), 0.0);
    
    FinancialReport report4("REP004", "monthly", currentTime, currentTime, currentTime,
                           1000.0, 1200.0, -200.0, payments, invoices, "USD", false);
    EXPECT_DOUBLE_EQ(report4.calculateProfitMargin(), -20.0);
    
    FinancialReport report5("REP005", "monthly", currentTime, currentTime, currentTime,
                           0.0, 500.0, -500.0, payments, invoices, "USD", false);
    EXPECT_DOUBLE_EQ(report5.calculateProfitMargin(), 0.0);
    
    FinancialReport report6("REP006", "monthly", currentTime, currentTime, currentTime,
                           0.0, 0.0, 0.0, payments, invoices, "USD", false);
    EXPECT_DOUBLE_EQ(report6.calculateProfitMargin(), 0.0);
    
    FinancialReport report7("REP007", "monthly", currentTime, currentTime, currentTime,
                           -1000.0, 500.0, -1500.0, payments, invoices, "USD", false);
    EXPECT_DOUBLE_EQ(report7.calculateProfitMargin(), 0.0);
}

TEST_F(FinancialReportTest, IsQuarterlyReport) {
    std::vector<std::shared_ptr<Payment>> payments;
    std::vector<std::shared_ptr<Invoice>> invoices;
    
    auto periodStart = currentTime - std::chrono::hours(24*90);
    auto periodEnd = currentTime;
    
    FinancialReport report1("REP001", "quarterly", currentTime, periodStart, periodEnd,
                           1000.0, 500.0, 500.0, payments, invoices, "USD", false);
    EXPECT_TRUE(report1.isQuarterlyReport());
    
    auto periodStart2 = currentTime - std::chrono::hours(24*85);
    FinancialReport report2("REP002", "quarterly", currentTime, periodStart2, periodEnd,
                           1000.0, 500.0, 500.0, payments, invoices, "USD", false);
    EXPECT_TRUE(report2.isQuarterlyReport());
    
    auto periodStart3 = currentTime - std::chrono::hours(24*95);
    FinancialReport report3("REP003", "quarterly", currentTime, periodStart3, periodEnd,
                           1000.0, 500.0, 500.0, payments, invoices, "USD", false);
    EXPECT_TRUE(report3.isQuarterlyReport());
    
    auto periodStart4 = currentTime - std::chrono::hours(24*84);
    FinancialReport report4("REP004", "quarterly", currentTime, periodStart4, periodEnd,
                           1000.0, 500.0, 500.0, payments, invoices, "USD", false);
    EXPECT_FALSE(report4.isQuarterlyReport());
    
    auto periodStart5 = currentTime - std::chrono::hours(24*96);
    FinancialReport report5("REP005", "quarterly", currentTime, periodStart5, periodEnd,
                           1000.0, 500.0, 500.0, payments, invoices, "USD", false);
    EXPECT_FALSE(report5.isQuarterlyReport());
    
    auto periodStart6 = currentTime - std::chrono::hours(24*30);
    FinancialReport report6("REP006", "monthly", currentTime, periodStart6, periodEnd,
                           1000.0, 500.0, 500.0, payments, invoices, "USD", false);
    EXPECT_FALSE(report6.isQuarterlyReport());
}

TEST_F(FinancialReportTest, IsAnnualReport) {
    std::vector<std::shared_ptr<Payment>> payments;
    std::vector<std::shared_ptr<Invoice>> invoices;
    
    auto periodStart = currentTime - std::chrono::hours(24*365);
    auto periodEnd = currentTime;
    
    FinancialReport report1("REP001", "annual", currentTime, periodStart, periodEnd,
                           10000.0, 5000.0, 5000.0, payments, invoices, "USD", false);
    EXPECT_TRUE(report1.isAnnualReport());
    
    auto periodStart2 = currentTime - std::chrono::hours(24*360);
    FinancialReport report2("REP002", "annual", currentTime, periodStart2, periodEnd,
                           10000.0, 5000.0, 5000.0, payments, invoices, "USD", false);
    EXPECT_TRUE(report2.isAnnualReport());
    
    auto periodStart3 = currentTime - std::chrono::hours(24*370);
    FinancialReport report3("REP003", "annual", currentTime, periodStart3, periodEnd,
                           10000.0, 5000.0, 5000.0, payments, invoices, "USD", false);
    EXPECT_TRUE(report3.isAnnualReport());
    
    auto periodStart4 = currentTime - std::chrono::hours(24*359);
    FinancialReport report4("REP004", "annual", currentTime, periodStart4, periodEnd,
                           10000.0, 5000.0, 5000.0, payments, invoices, "USD", false);
    EXPECT_FALSE(report4.isAnnualReport());
    
    auto periodStart5 = currentTime - std::chrono::hours(24*371);
    FinancialReport report5("REP005", "annual", currentTime, periodStart5, periodEnd,
                           10000.0, 5000.0, 5000.0, payments, invoices, "USD", false);
    EXPECT_FALSE(report5.isAnnualReport());
    
    auto periodStart6 = currentTime - std::chrono::hours(24*30);
    FinancialReport report6("REP006", "monthly", currentTime, periodStart6, periodEnd,
                           1000.0, 500.0, 500.0, payments, invoices, "USD", false);
    EXPECT_FALSE(report6.isAnnualReport());
}

TEST_F(FinancialReportTest, CalculateExpenseRatio) {
    std::vector<std::shared_ptr<Payment>> payments;
    std::vector<std::shared_ptr<Invoice>> invoices;
    
    FinancialReport report1("REP001", "monthly", currentTime, currentTime, currentTime,
                           1000.0, 500.0, 500.0, payments, invoices, "USD", false);
    EXPECT_DOUBLE_EQ(report1.calculateExpenseRatio(), 50.0);
    
    FinancialReport report2("REP002", "monthly", currentTime, currentTime, currentTime,
                           2000.0, 1500.0, 500.0, payments, invoices, "USD", false);
    EXPECT_DOUBLE_EQ(report2.calculateExpenseRatio(), 75.0);
    
    FinancialReport report3("REP003", "monthly", currentTime, currentTime, currentTime,
                           1000.0, 0.0, 1000.0, payments, invoices, "USD", false);
    EXPECT_DOUBLE_EQ(report3.calculateExpenseRatio(), 0.0);
    
    FinancialReport report4("REP004", "monthly", currentTime, currentTime, currentTime,
                           1000.0, 1000.0, 0.0, payments, invoices, "USD", false);
    EXPECT_DOUBLE_EQ(report4.calculateExpenseRatio(), 100.0);
    
    FinancialReport report5("REP005", "monthly", currentTime, currentTime, currentTime,
                           1000.0, 1200.0, -200.0, payments, invoices, "USD", false);
    EXPECT_DOUBLE_EQ(report5.calculateExpenseRatio(), 120.0);
    
    FinancialReport report6("REP006", "monthly", currentTime, currentTime, currentTime,
                           0.0, 500.0, -500.0, payments, invoices, "USD", false);
    EXPECT_DOUBLE_EQ(report6.calculateExpenseRatio(), 0.0);
    
    FinancialReport report7("REP007", "monthly", currentTime, currentTime, currentTime,
                           0.0, 0.0, 0.0, payments, invoices, "USD", false);
    EXPECT_DOUBLE_EQ(report7.calculateExpenseRatio(), 0.0);
    
    FinancialReport report8("REP008", "monthly", currentTime, currentTime, currentTime,
                           -1000.0, 500.0, -1500.0, payments, invoices, "USD", false);
    EXPECT_DOUBLE_EQ(report8.calculateExpenseRatio(), 0.0);
}

TEST_F(FinancialReportTest, HasPositiveCashFlow) {
    std::vector<std::shared_ptr<Payment>> payments;
    std::vector<std::shared_ptr<Invoice>> invoices;
    
    FinancialReport report1("REP001", "monthly", currentTime, currentTime, currentTime,
                           1000.0, 500.0, 500.0, payments, invoices, "USD", false);
    EXPECT_TRUE(report1.hasPositiveCashFlow());
    
    FinancialReport report2("REP002", "monthly", currentTime, currentTime, currentTime,
                           1000.0, 1000.0, 0.0, payments, invoices, "USD", false);
    EXPECT_FALSE(report2.hasPositiveCashFlow());
    
    FinancialReport report3("REP003", "monthly", currentTime, currentTime, currentTime,
                           1000.0, 1200.0, -200.0, payments, invoices, "USD", false);
    EXPECT_FALSE(report3.hasPositiveCashFlow());
    
    FinancialReport report4("REP004", "monthly", currentTime, currentTime, currentTime,
                           0.0, 0.0, 0.0, payments, invoices, "USD", false);
    EXPECT_FALSE(report4.hasPositiveCashFlow());
    
    FinancialReport report5("REP005", "monthly", currentTime, currentTime, currentTime,
                           2000.0, 0.0, 2000.0, payments, invoices, "USD", false);
    EXPECT_TRUE(report5.hasPositiveCashFlow());
}

TEST_F(FinancialReportTest, CalculateAverageTransaction) {
    std::vector<std::shared_ptr<Payment>> payments = {payment1, payment2, payment3};
    std::vector<std::shared_ptr<Invoice>> invoices = {invoice1, invoice2};
    
    FinancialReport report1("REP001", "monthly", currentTime, currentTime, currentTime,
                           4000.0, 1500.0, 2500.0, payments, invoices, "USD", false);
    
    EXPECT_DOUBLE_EQ(report1.calculateAverageTransaction(), 4000.0 / 5);
    
    std::vector<std::shared_ptr<Payment>> payments2;
    std::vector<std::shared_ptr<Invoice>> invoices2;
    
    FinancialReport report2("REP002", "monthly", currentTime, currentTime, currentTime,
                           1000.0, 500.0, 500.0, payments2, invoices2, "USD", false);
    
    EXPECT_DOUBLE_EQ(report2.calculateAverageTransaction(), 0.0);
    
    std::vector<std::shared_ptr<Payment>> payments3 = {payment1};
    
    FinancialReport report3("REP003", "monthly", currentTime, currentTime, currentTime,
                           1500.0, 500.0, 1000.0, payments3, invoices2, "USD", false);
    
    EXPECT_DOUBLE_EQ(report3.calculateAverageTransaction(), 1500.0);
    
    std::vector<std::shared_ptr<Invoice>> invoices3 = {invoice1};
    
    FinancialReport report4("REP004", "monthly", currentTime, currentTime, currentTime,
                           1500.0, 500.0, 1000.0, payments2, invoices3, "USD", false);
    
    EXPECT_DOUBLE_EQ(report4.calculateAverageTransaction(), 1500.0);
    
    FinancialReport report5("REP005", "monthly", currentTime, currentTime, currentTime,
                           0.0, 0.0, 0.0, payments, invoices, "USD", false);
    
    EXPECT_DOUBLE_EQ(report5.calculateAverageTransaction(), 0.0);
}

TEST_F(FinancialReportTest, GetFinancialHealth) {
    std::vector<std::shared_ptr<Payment>> payments;
    std::vector<std::shared_ptr<Invoice>> invoices;
    
    FinancialReport excellent("REP001", "monthly", currentTime, currentTime, currentTime,
                             1000.0, 400.0, 600.0, payments, invoices, "USD", false);
    EXPECT_EQ(excellent.getFinancialHealth(), "excellent");
    
    FinancialReport good("REP002", "monthly", currentTime, currentTime, currentTime,
                        1000.0, 500.0, 500.0, payments, invoices, "USD", false);
    EXPECT_EQ(good.getFinancialHealth(), "excellent");
    
    FinancialReport fair("REP003", "monthly", currentTime, currentTime, currentTime,
                        1000.0, 900.0, 100.0, payments, invoices, "USD", false);
    EXPECT_EQ(fair.getFinancialHealth(), "fair");
    
    FinancialReport breakEven("REP004", "monthly", currentTime, currentTime, currentTime,
                             1000.0, 1000.0, 0.0, payments, invoices, "USD", false);
    EXPECT_EQ(breakEven.getFinancialHealth(), "break_even");
    
    FinancialReport concerning("REP005", "monthly", currentTime, currentTime, currentTime,
                              1000.0, 1100.0, -100.0, payments, invoices, "USD", false);
    EXPECT_EQ(concerning.getFinancialHealth(), "concerning");
    
    FinancialReport largeLoss("REP006", "monthly", currentTime, currentTime, currentTime,
                             1000.0, 2000.0, -1000.0, payments, invoices, "USD", false);
    EXPECT_EQ(largeLoss.getFinancialHealth(), "concerning");
    
    FinancialReport zeroRevenue("REP007", "monthly", currentTime, currentTime, currentTime,
                               0.0, 0.0, 0.0, payments, invoices, "USD", false);
    EXPECT_EQ(zeroRevenue.getFinancialHealth(), "break_even");
    
    FinancialReport negativeRevenue("REP008", "monthly", currentTime, currentTime, currentTime,
                                   -1000.0, 500.0, -1500.0, payments, invoices, "USD", false);
    EXPECT_EQ(negativeRevenue.getFinancialHealth(), "break_even");
}

TEST_F(FinancialReportTest, CalculateTaxLiability) {
    std::vector<std::shared_ptr<Payment>> payments;
    std::vector<std::shared_ptr<Invoice>> invoices;
    
    FinancialReport profitable("REP001", "monthly", currentTime, currentTime, currentTime,
                              1000.0, 500.0, 500.0, payments, invoices, "USD", false);
    
    EXPECT_DOUBLE_EQ(profitable.calculateTaxLiability(25.0), 125.0);
    EXPECT_DOUBLE_EQ(profitable.calculateTaxLiability(10.0), 50.0);
    EXPECT_DOUBLE_EQ(profitable.calculateTaxLiability(0.0), 0.0);
    EXPECT_DOUBLE_EQ(profitable.calculateTaxLiability(100.0), 500.0);
    EXPECT_DOUBLE_EQ(profitable.calculateTaxLiability(-10.0), -50.0);
    EXPECT_DOUBLE_EQ(profitable.calculateTaxLiability(15.5), 77.5);
    
    FinancialReport breakeven("REP002", "monthly", currentTime, currentTime, currentTime,
                             1000.0, 1000.0, 0.0, payments, invoices, "USD", false);
    
    EXPECT_DOUBLE_EQ(breakeven.calculateTaxLiability(25.0), 0.0);
    
    FinancialReport loss("REP003", "monthly", currentTime, currentTime, currentTime,
                        1000.0, 1200.0, -200.0, payments, invoices, "USD", false);
    
    EXPECT_DOUBLE_EQ(loss.calculateTaxLiability(25.0), 0.0);
    
    FinancialReport zeroNetIncome("REP004", "monthly", currentTime, currentTime, currentTime,
                                 0.0, 0.0, 0.0, payments, invoices, "USD", false);
    
    EXPECT_DOUBLE_EQ(zeroNetIncome.calculateTaxLiability(25.0), 0.0);
}

TEST_F(FinancialReportTest, RequiresAudit) {
    std::vector<std::shared_ptr<Payment>> payments;
    std::vector<std::shared_ptr<Invoice>> invoices;
    
    auto annualPeriodStart = currentTime - std::chrono::hours(24*365);
    auto annualPeriodEnd = currentTime;
    
    FinancialReport annualReport("REP001", "annual", currentTime, annualPeriodStart, annualPeriodEnd,
                                500000.0, 300000.0, 200000.0, payments, invoices, "USD", false);
    EXPECT_TRUE(annualReport.requiresAudit());
    
    FinancialReport highRevenueMonthly("REP002", "monthly", currentTime, currentTime, currentTime,
                                       1000001.0, 500000.0, 500001.0, payments, invoices, "USD", false);
    EXPECT_TRUE(highRevenueMonthly.requiresAudit());
    
    FinancialReport monthlyReport("REP003", "monthly", currentTime, currentTime, currentTime,
                                  500000.0, 300000.0, 200000.0, payments, invoices, "USD", false);
    EXPECT_FALSE(monthlyReport.requiresAudit());
    
    FinancialReport exactRevenue("REP004", "monthly", currentTime, currentTime, currentTime,
                                 1000000.0, 500000.0, 500000.0, payments, invoices, "USD", false);
    EXPECT_FALSE(exactRevenue.requiresAudit());
    
    FinancialReport quarterlyReport("REP005", "quarterly", currentTime, currentTime, currentTime,
                                    500000.0, 300000.0, 200000.0, payments, invoices, "USD", false);
    EXPECT_FALSE(quarterlyReport.requiresAudit());
    
    FinancialReport lowRevenueAnnual("REP006", "annual", currentTime, annualPeriodStart, annualPeriodEnd,
                                     500000.0, 300000.0, 200000.0, payments, invoices, "USD", false);
    EXPECT_TRUE(lowRevenueAnnual.requiresAudit());
}

TEST_F(FinancialReportTest, CopyConstructor) {
    std::vector<std::shared_ptr<Payment>> payments = {payment1, payment2};
    std::vector<std::shared_ptr<Invoice>> invoices = {invoice1, invoice2};
    
    auto periodStart = currentTime - std::chrono::hours(24*30);
    auto periodEnd = currentTime;
    
    FinancialReport original("REP001", "monthly", currentTime, periodStart, periodEnd,
                            3500.0, 1500.0, 2000.0, payments, invoices, "USD", true);
    
    FinancialReport copy(original);
    
    EXPECT_EQ(copy.getReportId(), "REP001");
    EXPECT_EQ(copy.getReportType(), "monthly");
    EXPECT_EQ(copy.getReportDate(), currentTime);
    EXPECT_EQ(copy.getPeriodStart(), periodStart);
    EXPECT_EQ(copy.getPeriodEnd(), periodEnd);
    EXPECT_DOUBLE_EQ(copy.getTotalRevenue(), 3500.0);
    EXPECT_DOUBLE_EQ(copy.getTotalExpenses(), 1500.0);
    EXPECT_DOUBLE_EQ(copy.getNetIncome(), 2000.0);
    EXPECT_EQ(copy.getPayments().size(), 2);
    EXPECT_EQ(copy.getInvoices().size(), 2);
    EXPECT_EQ(copy.getCurrency(), "USD");
    EXPECT_TRUE(copy.getIsFinalized());
}

TEST_F(FinancialReportTest, CopyAssignment) {
    std::vector<std::shared_ptr<Payment>> payments1 = {payment1, payment2};
    std::vector<std::shared_ptr<Invoice>> invoices1 = {invoice1, invoice2};
    
    auto periodStart = currentTime - std::chrono::hours(24*30);
    auto periodEnd = currentTime;
    
    FinancialReport original("REP001", "monthly", currentTime, periodStart, periodEnd,
                            3500.0, 1500.0, 2000.0, payments1, invoices1, "USD", true);
    
    std::vector<std::shared_ptr<Payment>> payments2 = {payment3};
    std::vector<std::shared_ptr<Invoice>> invoices2;
    
    FinancialReport assigned("REP002", "quarterly", currentTime - std::chrono::hours(24*100), 
                            currentTime - std::chrono::hours(24*200), currentTime - std::chrono::hours(24*100),
                            1000.0, 800.0, 200.0, payments2, invoices2, "EUR", false);
    
    assigned = original;
    
    EXPECT_EQ(assigned.getReportId(), "REP001");
    EXPECT_EQ(assigned.getReportType(), "monthly");
    EXPECT_EQ(assigned.getReportDate(), currentTime);
    EXPECT_EQ(assigned.getPeriodStart(), periodStart);
    EXPECT_EQ(assigned.getPeriodEnd(), periodEnd);
    EXPECT_DOUBLE_EQ(assigned.getTotalRevenue(), 3500.0);
    EXPECT_DOUBLE_EQ(assigned.getTotalExpenses(), 1500.0);
    EXPECT_DOUBLE_EQ(assigned.getNetIncome(), 2000.0);
    EXPECT_EQ(assigned.getPayments().size(), 2);
    EXPECT_EQ(assigned.getInvoices().size(), 2);
    EXPECT_EQ(assigned.getCurrency(), "USD");
    EXPECT_TRUE(assigned.getIsFinalized());
}

TEST_F(FinancialReportTest, MoveConstructor) {
    std::vector<std::shared_ptr<Payment>> payments = {payment1, payment2};
    std::vector<std::shared_ptr<Invoice>> invoices = {invoice1, invoice2};
    
    auto periodStart = currentTime - std::chrono::hours(24*30);
    auto periodEnd = currentTime;
    
    FinancialReport original("REP001", "monthly", currentTime, periodStart, periodEnd,
                            3500.0, 1500.0, 2000.0, payments, invoices, "USD", true);
    
    FinancialReport moved(std::move(original));
    
    EXPECT_EQ(moved.getReportId(), "REP001");
    EXPECT_EQ(moved.getReportType(), "monthly");
    EXPECT_EQ(moved.getReportDate(), currentTime);
    EXPECT_EQ(moved.getPeriodStart(), periodStart);
    EXPECT_EQ(moved.getPeriodEnd(), periodEnd);
    EXPECT_DOUBLE_EQ(moved.getTotalRevenue(), 3500.0);
    EXPECT_DOUBLE_EQ(moved.getTotalExpenses(), 1500.0);
    EXPECT_DOUBLE_EQ(moved.getNetIncome(), 2000.0);
    EXPECT_EQ(moved.getPayments().size(), 2);
    EXPECT_EQ(moved.getInvoices().size(), 2);
    EXPECT_EQ(moved.getCurrency(), "USD");
    EXPECT_TRUE(moved.getIsFinalized());
}

TEST_F(FinancialReportTest, MoveAssignment) {
    std::vector<std::shared_ptr<Payment>> payments1 = {payment1, payment2};
    std::vector<std::shared_ptr<Invoice>> invoices1 = {invoice1, invoice2};
    
    auto periodStart = currentTime - std::chrono::hours(24*30);
    auto periodEnd = currentTime;
    
    FinancialReport original("REP001", "monthly", currentTime, periodStart, periodEnd,
                            3500.0, 1500.0, 2000.0, payments1, invoices1, "USD", true);
    
    std::vector<std::shared_ptr<Payment>> payments2 = {payment3};
    std::vector<std::shared_ptr<Invoice>> invoices2;
    
    FinancialReport moved("REP002", "quarterly", currentTime - std::chrono::hours(24*100), 
                         currentTime - std::chrono::hours(24*200), currentTime - std::chrono::hours(24*100),
                         1000.0, 800.0, 200.0, payments2, invoices2, "EUR", false);
    
    moved = std::move(original);
    
    EXPECT_EQ(moved.getReportId(), "REP001");
    EXPECT_EQ(moved.getReportType(), "monthly");
    EXPECT_EQ(moved.getReportDate(), currentTime);
    EXPECT_EQ(moved.getPeriodStart(), periodStart);
    EXPECT_EQ(moved.getPeriodEnd(), periodEnd);
    EXPECT_DOUBLE_EQ(moved.getTotalRevenue(), 3500.0);
    EXPECT_DOUBLE_EQ(moved.getTotalExpenses(), 1500.0);
    EXPECT_DOUBLE_EQ(moved.getNetIncome(), 2000.0);
    EXPECT_EQ(moved.getPayments().size(), 2);
    EXPECT_EQ(moved.getInvoices().size(), 2);
    EXPECT_EQ(moved.getCurrency(), "USD");
    EXPECT_TRUE(moved.getIsFinalized());
}

TEST_F(FinancialReportTest, SelfAssignment) {
    std::vector<std::shared_ptr<Payment>> payments = {payment1};
    std::vector<std::shared_ptr<Invoice>> invoices = {invoice1};
    
    FinancialReport report("REP001", "monthly", currentTime, currentTime, currentTime,
                          1500.0, 500.0, 1000.0, payments, invoices, "USD", false);
    
    report.setTotalRevenue(2000.0);
    report.setIsFinalized(true);
    
    report = report;
    
    EXPECT_EQ(report.getReportId(), "REP001");
    EXPECT_EQ(report.getReportType(), "monthly");
    EXPECT_EQ(report.getReportDate(), currentTime);
    EXPECT_DOUBLE_EQ(report.getTotalRevenue(), 2000.0);
    EXPECT_DOUBLE_EQ(report.getTotalExpenses(), 500.0);
    EXPECT_DOUBLE_EQ(report.getNetIncome(), 1500.0);
    EXPECT_EQ(report.getPayments().size(), 1);
    EXPECT_EQ(report.getInvoices().size(), 1);
    EXPECT_EQ(report.getCurrency(), "USD");
    EXPECT_TRUE(report.getIsFinalized());
}

TEST_F(FinancialReportTest, EdgeCases) {
    std::vector<std::shared_ptr<Payment>> payments;
    std::vector<std::shared_ptr<Invoice>> invoices;
    
    FinancialReport emptyReport("", "", std::chrono::system_clock::time_point(),
                               std::chrono::system_clock::time_point(), std::chrono::system_clock::time_point(),
                               0.0, 0.0, 0.0, payments, invoices, "", false);
    
    EXPECT_EQ(emptyReport.getReportId(), "");
    EXPECT_EQ(emptyReport.getReportType(), "");
    EXPECT_EQ(emptyReport.getCurrency(), "");
    EXPECT_DOUBLE_EQ(emptyReport.getTotalRevenue(), 0.0);
    EXPECT_DOUBLE_EQ(emptyReport.getTotalExpenses(), 0.0);
    EXPECT_DOUBLE_EQ(emptyReport.getNetIncome(), 0.0);
    EXPECT_TRUE(emptyReport.getPayments().empty());
    EXPECT_TRUE(emptyReport.getInvoices().empty());
    EXPECT_FALSE(emptyReport.getIsFinalized());
    
    EXPECT_DOUBLE_EQ(emptyReport.calculateProfitMargin(), 0.0);
    EXPECT_FALSE(emptyReport.isQuarterlyReport());
    EXPECT_FALSE(emptyReport.isAnnualReport());
    EXPECT_DOUBLE_EQ(emptyReport.calculateExpenseRatio(), 0.0);
    EXPECT_FALSE(emptyReport.hasPositiveCashFlow());
    EXPECT_DOUBLE_EQ(emptyReport.calculateAverageTransaction(), 0.0);
    EXPECT_EQ(emptyReport.getFinancialHealth(), "break_even");
    EXPECT_DOUBLE_EQ(emptyReport.calculateTaxLiability(25.0), 0.0);
    EXPECT_FALSE(emptyReport.requiresAudit());
    
    FinancialReport negativeReport("NEG", "monthly", currentTime, currentTime, currentTime,
                                  -1000.0, -500.0, -500.0, payments, invoices, "USD", false);
    
    EXPECT_DOUBLE_EQ(negativeReport.getTotalRevenue(), -1000.0);
    EXPECT_DOUBLE_EQ(negativeReport.getTotalExpenses(), -500.0);
    EXPECT_DOUBLE_EQ(negativeReport.getNetIncome(), -500.0);
}

TEST_F(FinancialReportTest, MixedMethods) {
    std::vector<std::shared_ptr<Payment>> payments = {payment1, payment2};
    std::vector<std::shared_ptr<Invoice>> invoices = {invoice1, invoice2};
    
    auto periodStart = currentTime - std::chrono::hours(24*30);
    auto periodEnd = currentTime;
    
    FinancialReport report("REP001", "monthly", currentTime, periodStart, periodEnd,
                          3500.0, 1500.0, 2000.0, payments, invoices, "USD", false);
    
    EXPECT_DOUBLE_EQ(report.calculateProfitMargin(), (2000.0 / 3500.0) * 100.0);
    EXPECT_DOUBLE_EQ(report.calculateExpenseRatio(), (1500.0 / 3500.0) * 100.0);
    EXPECT_TRUE(report.hasPositiveCashFlow());
    EXPECT_DOUBLE_EQ(report.calculateAverageTransaction(), 3500.0 / 4);
    EXPECT_EQ(report.getFinancialHealth(), "excellent");
    EXPECT_DOUBLE_EQ(report.calculateTaxLiability(20.0), 400.0);
    EXPECT_FALSE(report.requiresAudit());
    EXPECT_FALSE(report.isQuarterlyReport());
    EXPECT_FALSE(report.isAnnualReport());
    
    report.setTotalRevenue(7000.0);
    report.setTotalExpenses(3000.0);
    report.setIsFinalized(true);
    
    EXPECT_DOUBLE_EQ(report.getNetIncome(), 4000.0);
    EXPECT_DOUBLE_EQ(report.calculateProfitMargin(), (4000.0 / 7000.0) * 100.0);
    EXPECT_DOUBLE_EQ(report.calculateExpenseRatio(), (3000.0 / 7000.0) * 100.0);
    EXPECT_TRUE(report.hasPositiveCashFlow());
    EXPECT_DOUBLE_EQ(report.calculateAverageTransaction(), 7000.0 / 4);
    EXPECT_EQ(report.getFinancialHealth(), "excellent");
    EXPECT_DOUBLE_EQ(report.calculateTaxLiability(25.0), 1000.0);
    EXPECT_FALSE(report.requiresAudit());
}
