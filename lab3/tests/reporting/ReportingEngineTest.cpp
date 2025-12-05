#include <gtest/gtest.h>
#include <chrono>
#include <vector>
#include <memory>
#include "../../include/reporting/ReportingEngine.h"
#include "../../include/core/Property.h"
#include "../../include/finance/RentalContract.h"
#include "../../include/finance/Payment.h"
#include "../../include/core/Address.h"
#include "../../include/core/Tenant.h"
#include "../../include/core/Owner.h"

class ReportingEngineTest : public ::testing::Test {
protected:
    std::shared_ptr<Property> property1;
    std::shared_ptr<Property> property2;
    std::shared_ptr<RentalContract> contract1;
    std::shared_ptr<RentalContract> contract2;
    std::shared_ptr<Payment> payment1;
    std::shared_ptr<Payment> payment2;
    std::shared_ptr<Tenant> tenant1;
    std::shared_ptr<Tenant> tenant2;
    std::shared_ptr<Owner> owner1;
    ReportingEngine* engine;
    std::chrono::system_clock::time_point now;

    void SetUp() override {
        now = std::chrono::system_clock::now();
        
        auto address1 = std::make_unique<Address>("Street1", "CityA", "StateA", "12345", "CountryX", 40.0, -74.0);
        auto address2 = std::make_unique<Address>("Street2", "CityB", "StateB", "54321", "CountryX", 41.0, -75.0);
        
        property1 = std::make_shared<Property>("p1", 50.0, 1000.0,
            std::move(address1), nullptr, nullptr);
        property2 = std::make_shared<Property>("p2", 80.0, 2000.0,
            std::move(address2), nullptr, nullptr);
            
        auto tenantAddress1 = std::make_unique<Address>("Tenant St1", "CityA", "StateA", "12346", "CountryX", 40.1, -74.1);
        auto tenantAddress2 = std::make_unique<Address>("Tenant St2", "CityB", "StateB", "54322", "CountryX", 41.1, -75.1);
        auto ownerAddress = std::make_unique<Address>("Owner St", "CityC", "StateC", "54323", "CountryX", 42.0, -76.0);
        
        tenant1 = std::make_shared<Tenant>("t1", "John", "Doe", "john@test.com", "1234567890",
                                          std::move(tenantAddress1), 750.0, 5000.0, false, 2);
        tenant2 = std::make_shared<Tenant>("t2", "Jane", "Smith", "jane@test.com", "0987654321",
                                          std::move(tenantAddress2), 800.0, 6000.0, true, 1);
        owner1 = std::make_shared<Owner>("o1", "Bob", "Johnson", "bob@test.com", "5555555555",
                                        std::move(ownerAddress), "123456789", 100.0, true, "bank_transfer");

        contract1 = std::make_shared<RentalContract>("c1", property1, tenant1, owner1,
                                                    now - std::chrono::hours(24*30),
                                                    now + std::chrono::hours(24*60), 1000.0, 1000.0, "monthly");
        contract2 = std::make_shared<RentalContract>("c2", property2, tenant2, owner1,
                                                    now - std::chrono::hours(24*90),
                                                    now - std::chrono::hours(24*1), 2000.0, 2000.0, "monthly");

        payment1 = std::make_shared<Payment>("pay1", tenant1, contract1, 1000.0,
                                            now - std::chrono::hours(24*10), "bank_transfer",
                                            "completed", "txn123", 0.0, "January 2024");
        payment2 = std::make_shared<Payment>("pay2", tenant2, contract2, 500.0,
                                            now - std::chrono::hours(24*5), "credit_card",
                                            "pending", "txn124", 0.0, "February 2024");

        engine = new ReportingEngine("PDF", true, 12);
    }

    void TearDown() override {
        delete engine;
    }
};

TEST_F(ReportingEngineTest, ConstructorAndGetters) {
    EXPECT_EQ(engine->getReportFormat(), "PDF");
    EXPECT_TRUE(engine->getIncludeCharts());
    EXPECT_EQ(engine->getDataRetentionMonths(), 12);
}

TEST_F(ReportingEngineTest, CopyAndMoveConstructors) {
    ReportingEngine copyEngine(*engine);
    EXPECT_EQ(copyEngine.getReportFormat(), "PDF");
    
    ReportingEngine movedEngine(std::move(copyEngine));
    EXPECT_EQ(movedEngine.getReportFormat(), "PDF");
}

TEST_F(ReportingEngineTest, AssignmentOperators) {
    ReportingEngine copy("PDF", false, 1);
    copy = *engine;
    EXPECT_EQ(copy.getReportFormat(), "PDF");
    
    ReportingEngine moved("TXT", false, 1);
    moved = std::move(copy);
    EXPECT_EQ(moved.getReportFormat(), "PDF");
}

TEST_F(ReportingEngineTest, Setters) {
    engine->setReportFormat("CSV");
    EXPECT_EQ(engine->getReportFormat(), "CSV");
    
    engine->setIncludeCharts(false);
    EXPECT_FALSE(engine->getIncludeCharts());
    
    engine->setDataRetentionMonths(24);
    EXPECT_EQ(engine->getDataRetentionMonths(), 24);
    
    engine->setDataRetentionMonths(0);
    EXPECT_EQ(engine->getDataRetentionMonths(), 1);
}

TEST_F(ReportingEngineTest, FinancialReportAndCollectionRate) {
    double revenue = engine->generateFinancialReport(now - std::chrono::hours(24*30), now);
    EXPECT_GE(revenue, 0.0);
    
    double collectionRate = engine->calculateCollectionRate(now - std::chrono::hours(24*30), now);
    EXPECT_GE(collectionRate, 0.0);
    EXPECT_LE(collectionRate, 100.0);
}

TEST_F(ReportingEngineTest, VacancyRateAndTopPerformingProperties) {
    double vacancyRate = engine->calculateVacancyRate(now);
    EXPECT_GE(vacancyRate, 0.0);
    EXPECT_LE(vacancyRate, 100.0);
    
    auto topProperties = engine->getTopPerformingProperties(1);
    EXPECT_TRUE(topProperties.empty());
}

TEST_F(ReportingEngineTest, PortfolioAndROI) {
    std::string portfolioSummary = engine->generatePortfolioSummary();
    EXPECT_FALSE(portfolioSummary.empty());
    
    double roi = engine->calculateROI(now - std::chrono::hours(24*30), now);
    EXPECT_GE(roi, 0.0);
}

TEST_F(ReportingEngineTest, ExpiringContractsAndAverageRent) {
    auto expiringContracts = engine->getExpiringContractsReport(30);
    EXPECT_TRUE(expiringContracts.empty());
    
    double avgRent = engine->calculateAverageRentByLocation("CityA");
    EXPECT_GE(avgRent, 0.0);
}

TEST_F(ReportingEngineTest, TaxReportGeneration) {
    std::string report = engine->generateTaxReport(2025);
    EXPECT_FALSE(report.empty());
}

TEST_F(ReportingEngineTest, MaintenanceCosts) {
    double cost = engine->calculateMaintenanceCosts(now - std::chrono::hours(24*30), now);
    EXPECT_DOUBLE_EQ(cost, 0.0);
}