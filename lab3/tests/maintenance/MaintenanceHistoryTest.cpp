#include <gtest/gtest.h>
#include <memory>
#include <chrono>

#include "../../include/maintenance/MaintenanceHistory.h"
#include "../../include/maintenance/MaintenanceRequest.h"
#include "../../include/core/Property.h"
#include "../../include/core/Tenant.h"
#include "../../include/core/Address.h"
#include "../../include/core/PropertyStatus.h"
#include "../../include/core/PropertyType.h"

std::shared_ptr<Property> createPropertyMH() {
    auto addr = std::make_unique<Address>("City", "Street", "10", "00000");
    auto status = std::make_unique<PropertyStatus>("AVAILABLE");
    auto type = std::make_unique<PropertyType>("HOUSE");

    return std::make_shared<Property>(
        "P1", 100.0, 2000.0,
        std::move(addr), std::move(status), std::move(type)
    );
}

std::shared_ptr<Tenant> createTenantMH() {
    auto addr = std::make_unique<Address>("City", "Lane", "55", "11111");

    return std::make_shared<Tenant>(
        "T1", "Jane", "Smith", "js@mail.com", "555-001",
        std::move(addr), 700, 5000, false, 2
    );
}

std::shared_ptr<MaintenanceRequest> createRequestMH(
    const std::string& id, const std::string& category, const std::string& priority
) {
    auto property = createPropertyMH();
    auto tenant = createTenantMH();
    auto now = std::chrono::system_clock::now();

    return std::make_shared<MaintenanceRequest>(
        id, property, tenant, "Test issue",
        category, priority,
        now, now,
        "scheduled", 100, 80, "vendor"
    );
}

MaintenanceHistory createHistoryBase(
    const std::vector<std::shared_ptr<MaintenanceRequest>>& reqs
) {
    auto property = createPropertyMH();
    auto now = std::chrono::system_clock::now();

    return MaintenanceHistory(
        "H1", property, reqs,
        now - std::chrono::hours(240), 
        now + std::chrono::hours(48),
        5000.0,
        static_cast<int>(reqs.size()),
        "regular",
        "good"
    );
}

TEST(MaintenanceHistoryTest, ConstructorAndGetters) {
    auto reqs = std::vector<std::shared_ptr<MaintenanceRequest>> {
        createRequestMH("R1", "plumbing", "high")
    };
    auto history = createHistoryBase(reqs);
    auto now = std::chrono::system_clock::now();

    EXPECT_EQ(history.getHistoryId(), "H1");
    EXPECT_NE(history.getProperty(), nullptr);
    EXPECT_EQ(history.getMaintenanceRequests().size(), 1);
    EXPECT_DOUBLE_EQ(history.getTotalMaintenanceCost(), 5000);
    EXPECT_EQ(history.getTotalRequests(), 1);
    EXPECT_EQ(history.getMaintenanceFrequency(), "regular");
    EXPECT_EQ(history.getOverallCondition(), "good");
}

TEST(MaintenanceHistoryTest, CopyConstructor) {
    auto history1 = createHistoryBase({});
    MaintenanceHistory history2(history1);

    EXPECT_EQ(history2.getHistoryId(), history1.getHistoryId());
    EXPECT_EQ(history2.getTotalRequests(), history1.getTotalRequests());
}

TEST(MaintenanceHistoryTest, CopyAssignment) {
    auto h1 = createHistoryBase({});
    auto h2 = createHistoryBase({});

    h2 = h1;

    EXPECT_EQ(h2.getHistoryId(), h1.getHistoryId());
}

TEST(MaintenanceHistoryTest, MoveConstructor) {
    auto h1 = createHistoryBase({});
    std::string id = h1.getHistoryId();

    MaintenanceHistory h2(std::move(h1));

    EXPECT_EQ(h2.getHistoryId(), id);
}

TEST(MaintenanceHistoryTest, MoveAssignment) {
    auto h1 = createHistoryBase({});
    auto h2 = createHistoryBase({});

    std::string id = h1.getHistoryId();
    h2 = std::move(h1);

    EXPECT_EQ(h2.getHistoryId(), id);
}

TEST(MaintenanceHistoryTest, SetterMethods) {
    auto h = createHistoryBase({});
    auto now = std::chrono::system_clock::now();

    h.setLastMaintenanceDate(now);
    h.setNextScheduledMaintenance(now + std::chrono::hours(50));
    h.setOverallCondition("fair");

    EXPECT_EQ(h.getLastMaintenanceDate(), now);
    EXPECT_EQ(h.getNextScheduledMaintenance(), now + std::chrono::hours(50));
    EXPECT_EQ(h.getOverallCondition(), "fair");
}

TEST(MaintenanceHistoryTest, CalculateAverageCost) {
    auto reqs = std::vector<std::shared_ptr<MaintenanceRequest>> {
        createRequestMH("R1", "cat", "low"),
        createRequestMH("R2", "cat", "low")
    };
    auto h = createHistoryBase(reqs);

    EXPECT_DOUBLE_EQ(h.calculateAverageMaintenanceCost(), 5000.0 / 2.0);
}

TEST(MaintenanceHistoryTest, MaintenanceOverdue) {
    auto history = createHistoryBase({});
    auto now = std::chrono::system_clock::now();

    EXPECT_FALSE(history.isMaintenanceOverdue(now));

    EXPECT_TRUE(history.isMaintenanceOverdue(now + std::chrono::hours(1000)));
}

TEST(MaintenanceHistoryTest, DaysSinceLastMaintenance) {
    auto now = std::chrono::system_clock::now();

    MaintenanceHistory h(
        "H", createPropertyMH(), {},
        now - std::chrono::hours(72),
        now, 0, 0, "reg", "good"
    );

    EXPECT_EQ(h.getDaysSinceLastMaintenance(now), 3);
}

TEST(MaintenanceHistoryTest, CostPerSqFt) {
    auto h = createHistoryBase({});
    EXPECT_DOUBLE_EQ(h.calculateMaintenanceCostPerSqFt(), 5000.0 / 100.0);
}

TEST(MaintenanceHistoryTest, HasFrequentIssues) {
    auto h1 = createHistoryBase({});
    EXPECT_FALSE(h1.hasFrequentIssues());

    MaintenanceHistory h2(
        "H", createPropertyMH(), {}, 
        std::chrono::system_clock::now(), 
        std::chrono::system_clock::now(),
        100, 11, "regular", "good"
    );
    EXPECT_TRUE(h2.hasFrequentIssues());

    MaintenanceHistory h3(
        "H", createPropertyMH(), {}, 
        std::chrono::system_clock::now(), 
        std::chrono::system_clock::now(),
        100, 5, "frequent", "good"
    );
    EXPECT_TRUE(h3.hasFrequentIssues());
}

TEST(MaintenanceHistoryTest, EmergencyRequests) {
    auto req1 = createRequestMH("R1", "plumbing", "high");
    auto req2 = createRequestMH("R2", "routine", "low");

    auto h = createHistoryBase({req1, req2});

    auto emergencies = h.getEmergencyRequests();

    EXPECT_EQ(emergencies.size(), 1);
    EXPECT_EQ(emergencies[0]->getRequestId(), "R1");
}

TEST(MaintenanceHistoryTest, PreventiveMaintenanceRatio) {
    auto r1 = createRequestMH("R1", "preventive", "low");
    auto r2 = createRequestMH("R2", "routine", "low");
    auto r3 = createRequestMH("R3", "plumbing", "low");

    auto h = createHistoryBase({r1, r2, r3});

    EXPECT_DOUBLE_EQ(h.calculatePreventiveMaintenanceRatio(), (2.0 / 3.0) * 100.0);
}

TEST(MaintenanceHistoryTest, MaintenanceEfficiencyLevels) {
    auto property = createPropertyMH();

    MaintenanceHistory excellent(
        "H", property, 
        {createRequestMH("R1", "preventive", "low")},
        std::chrono::system_clock::now(),
        std::chrono::system_clock::now(),
        100, 1,
        "regular", "good"
    );
    EXPECT_EQ(excellent.getMaintenanceEfficiency(), "excellent");

    MaintenanceHistory good(
        "H2", property, 
        {createRequestMH("R1", "preventive", "low")},
        std::chrono::system_clock::now(),
        std::chrono::system_clock::now(),
        400, 1,
        "regular", "good"
    );
    EXPECT_EQ(good.getMaintenanceEfficiency(), "good");

    MaintenanceHistory fair(
        "H3", property, 
        {createRequestMH("R1", "preventive", "low")},
        std::chrono::system_clock::now(),
        std::chrono::system_clock::now(),
        900, 1,
        "regular", "good"
    );
    EXPECT_EQ(fair.getMaintenanceEfficiency(), "fair");

    MaintenanceHistory bad(
        "H4", property, 
        {createRequestMH("R1", "plumbing", "low")},
        std::chrono::system_clock::now(),
        std::chrono::system_clock::now(),
        20000, 1,
        "regular", "poor"
    );
    EXPECT_EQ(bad.getMaintenanceEfficiency(), "needs_improvement");
}

TEST(MaintenanceHistoryTest, RequiresMajorRenovation) {
    auto base = createHistoryBase({});
    EXPECT_FALSE(base.requiresMajorRenovation());

    MaintenanceHistory poor(
        "H2", createPropertyMH(), {},
        std::chrono::system_clock::now(), 
        std::chrono::system_clock::now(),
        0, 0, "reg", "poor"
    );
    EXPECT_TRUE(poor.requiresMajorRenovation());

    MaintenanceHistory highCost(
        "H3", createPropertyMH(), {}, 
        std::chrono::system_clock::now(),
        std::chrono::system_clock::now(),
        60000, 0, "reg", "good"
    );
    EXPECT_TRUE(highCost.requiresMajorRenovation());
}
