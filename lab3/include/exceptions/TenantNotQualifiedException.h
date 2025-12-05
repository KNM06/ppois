#ifndef TENANTNOTQUALIFIEDEXCEPTION_H
#define TENANTNOTQUALIFIEDEXCEPTION_H

#include "RentalException.h"
#include <string>

class TenantNotQualifiedException : public RentalException {
public:
    explicit TenantNotQualifiedException(const std::string& tenantId);
    virtual ~TenantNotQualifiedException() = default;
};

#endif