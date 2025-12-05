#include <gtest/gtest.h>
#include <memory>
#include <chrono>

#include "../../include/maintenance/MaintenanceRequest.h"
#include "../../include/core/Property.h"
#include "../../include/core/Tenant.h"
#include "../../include/core/Address.h"
#include "../../include/core/PropertyStatus.h"
#include "../../include/core/PropertyType.h"

std::shared_ptr<Property> createProperty() {
    auto addr = std::make_unique<Address>("City", "Street", "123", "00000");
    auto status = std::make_unique<PropertyStatus>("AVAILABLE");
    auto type = std::make_unique<PropertyType>("HOUSE");

    return std::make_shared<Property>(
        "P1", 120.0, 2000.0,
        std::move(addr), std::move(status), std::move(type)
    );
}

std::shared_ptr<Tenant> createTenant() {
    auto addr = std::make_unique<Address>("City", "Road", "45", "10000");

    return std::make_shared<Tenant>(
        "T1", "Alice", "Brown", "alice@mail.com", "555-111",
        std::move(addr), 720, 6000, false, 3
    );
}

MaintenanceRequest createRequestRaw() {
    auto property = createProperty();
    auto tenant = createTenant();
    auto now = std::chrono::system_clock::now();

    return MaintenanceRequest(
        "R1", property, tenant, "Leak in bathroom",
        "plumbing", "high",
        now, now,
        "scheduled", 500.0, 450.0, "vendorX"
    );
}

std::shared_ptr<MaintenanceRequest> createRequestShared() {
    return std::make_shared<MaintenanceRequest>(createRequestRaw());
}

TEST(MaintenanceRequestTest, ConstructorAndGetters) {
    auto property = createProperty();
    auto tenant = createTenant();
    auto now = std::chrono::system_clock::now();

    MaintenanceRequest req(
        "R1", property, tenant, "Leak in bathroom",
        "plumbing", "high",
        now, now,
        "scheduled", 500, 450, "vendorX"
    );

    EXPECT_EQ(req.getRequestId(), "R1");
    EXPECT_EQ(req.getProperty(), property);
    EXPECT_EQ(req.getTenant(), tenant.get());
    EXPECT_EQ(req.getIssueDescription(), "Leak in bathroom");
    EXPECT_EQ(req.getCategory(), "plumbing");
    EXPECT_EQ(req.getPriority(), "high");
    EXPECT_EQ(req.getRequestDate(), now);
    EXPECT_EQ(req.getScheduledDate(), now);
    EXPECT_EQ(req.getStatus(), "scheduled");
    EXPECT_DOUBLE_EQ(req.getEstimatedCost(), 500);
    EXPECT_DOUBLE_EQ(req.getActualCost(), 450);
    EXPECT_EQ(req.getAssignedVendor(), "vendorX");
}

TEST(MaintenanceRequestTest, CopyConstructor) {
    MaintenanceRequest original = createRequestRaw();
    MaintenanceRequest copy(original);

    EXPECT_EQ(copy.getRequestId(), original.getRequestId());
    EXPECT_EQ(copy.getCategory(), original.getCategory());
    EXPECT_DOUBLE_EQ(copy.getEstimatedCost(), original.getEstimatedCost());
}

TEST(MaintenanceRequestTest, CopyAssignment) {
    MaintenanceRequest req1 = createRequestRaw();
    MaintenanceRequest req2 = createRequestRaw();

    req2 = req1;

    EXPECT_EQ(req2.getRequestId(), req1.getRequestId());
    EXPECT_EQ(req2.getStatus(), req1.getStatus());
}

TEST(MaintenanceRequestTest, MoveConstructor) {
    MaintenanceRequest original = createRequestRaw();
    std::string id = original.getRequestId();

    MaintenanceRequest moved(std::move(original));

    EXPECT_EQ(moved.getRequestId(), id);
}

TEST(MaintenanceRequestTest, MoveAssignment) {
    MaintenanceRequest req1 = createRequestRaw();
    MaintenanceRequest req2 = createRequestRaw();

    std::string id = req1.getRequestId();
    req2 = std::move(req1);

    EXPECT_EQ(req2.getRequestId(), id);
}

TEST(MaintenanceRequestTest, SetterMethods) {
    MaintenanceRequest req = createRequestRaw();

    req.setStatus("completed");
    req.setActualCost(-50);
    req.setAssignedVendor("Vendor123");

    EXPECT_EQ(req.getStatus(), "completed");
    EXPECT_DOUBLE_EQ(req.getActualCost(), 0);
    EXPECT_EQ(req.getAssignedVendor(), "Vendor123");
}

TEST(MaintenanceRequestTest, EmergencyRequestLogic) {
    auto now = std::chrono::system_clock::now();
    auto property = createProperty();
    auto tenant = createTenant();

    MaintenanceRequest req1(
        "R1", property, tenant, "Test", "repair",
        "emergency", now, now, "scheduled", 100, 50, "v"
    );

    MaintenanceRequest req2(
        "R2", property, tenant, "Test", "repair",
        "high", now, now, "scheduled", 100, 50, "v"
    );

    MaintenanceRequest req3(
        "R3", property, tenant, "Test", "repair",
        "low", now, now, "scheduled", 100, 50, "v"
    );

    EXPECT_TRUE(req1.isEmergencyRequest());
    EXPECT_TRUE(req2.isEmergencyRequest());
    EXPECT_FALSE(req3.isEmergencyRequest());
}

TEST(MaintenanceRequestTest, SchedulingLogic) {
    MaintenanceRequest req = createRequestRaw();

    EXPECT_TRUE(req.isScheduled());

    req.setStatus("in_progress");
    EXPECT_TRUE(req.isScheduled());

    req.setStatus("completed");
    EXPECT_FALSE(req.isScheduled());
}

TEST(MaintenanceRequestTest, CompletedLogic) {
    MaintenanceRequest req = createRequestRaw();

    EXPECT_FALSE(req.isCompleted());

    req.setStatus("completed");
    EXPECT_TRUE(req.isCompleted());
}

TEST(MaintenanceRequestTest, CostVarianceCalculation) {
    MaintenanceRequest req = createRequestRaw();
    EXPECT_DOUBLE_EQ(req.calculateCostVariance(), req.getActualCost() - req.getEstimatedCost());
}

TEST(MaintenanceRequestTest, DaysSinceRequestCalculation) {
    auto property = createProperty();
    auto tenant = createTenant();

    auto now = std::chrono::system_clock::now();
    auto earlier = now - std::chrono::hours(72);

    MaintenanceRequest req(
        "R1", property, tenant, "Test", "cat", "low",
        earlier, earlier, "scheduled", 100, 50, "v"
    );

    int days = req.getDaysSinceRequest(now);
    EXPECT_EQ(days, 3);
}

TEST(MaintenanceRequestTest, RequiresImmediateAttentionLogic) {
    auto property = createProperty();
    auto tenant = createTenant();
    auto now = std::chrono::system_clock::now();

    MaintenanceRequest emergencyReq(
        "R1", property, tenant, "Test", "cat", "emergency",
        now, now, "scheduled", 100, 50, "v"
    );

    EXPECT_TRUE(emergencyReq.requiresImmediateAttention());

    MaintenanceRequest slowReq(
        "R2", property, tenant, "Test", "cat", "low",
        now - std::chrono::hours(24 * 10),
        now, "scheduled", 100, 50, "v"
    );

    EXPECT_TRUE(slowReq.requiresImmediateAttention());
}

TEST(MaintenanceRequestTest, WithinBudgetLogic) {
    auto property = createProperty();
    auto tenant = createTenant();

    MaintenanceRequest req(
        "R1", property, tenant, "Leak in bathroom",
        "plumbing", "high",
        std::chrono::system_clock::now(),
        std::chrono::system_clock::now(),
        "scheduled", 100, 100, "vendorX"
    );

    EXPECT_TRUE(req.isWithinBudget());

    MaintenanceRequest over(
        "R2", property, tenant,
        "Leak in bathroom", "plumbing", "high",
        std::chrono::system_clock::now(),
        std::chrono::system_clock::now(),
        "scheduled", 100, 500, "vendorX"
    );

    EXPECT_FALSE(over.isWithinBudget());
}


TEST(MaintenanceRequestTest, CanBeDeferredLogic) {
    auto property = createProperty();
    auto tenant = createTenant();
    auto now = std::chrono::system_clock::now();

    MaintenanceRequest req1("R1", property, tenant, "Test", "plumbing", "low", now, now, "scheduled", 100, 50, "v");
    MaintenanceRequest req2("R2", property, tenant, "Test", "electrical", "low", now, now, "scheduled", 100, 50, "v");
    MaintenanceRequest req3("R3", property, tenant, "Test", "heating", "low", now, now, "scheduled", 100, 50, "v");
    MaintenanceRequest req4("R4", property, tenant, "Test", "security", "low", now, now, "scheduled", 100, 50, "v");
    MaintenanceRequest req5("R5", property, tenant, "Test", "general", "low", now, now, "scheduled", 100, 50, "v");

    EXPECT_FALSE(req1.canBeDeferred());
    EXPECT_FALSE(req2.canBeDeferred());
    EXPECT_FALSE(req3.canBeDeferred());
    EXPECT_FALSE(req4.canBeDeferred());
    EXPECT_TRUE(req5.canBeDeferred());
}
