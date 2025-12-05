#include <gtest/gtest.h>
#include "../../include/exceptions/TenantNotQualifiedException.h"

TEST(TenantNotQualifiedExceptionTest, ConstructorSetsMessage) {
    std::string tenantId = "TEN123";
    try {
        throw TenantNotQualifiedException(tenantId);
    } catch (const TenantNotQualifiedException& ex) {
        EXPECT_STREQ(ex.what(), ("Tenant " + tenantId + " does not meet qualification requirements").c_str());
    } catch (...) {
        FAIL() << "Expected TenantNotQualifiedException";
    }
}
