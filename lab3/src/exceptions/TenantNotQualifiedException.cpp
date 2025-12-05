#include "../../include/exceptions/TenantNotQualifiedException.h"

TenantNotQualifiedException::TenantNotQualifiedException(const std::string& tenantId)
    : RentalException("Tenant " + tenantId + " does not meet qualification requirements") {}