#ifndef INSPECTIONFAILEDEXCEPTION_H
#define INSPECTIONFAILEDEXCEPTION_H

#include "RentalException.h"
#include <string>

class InspectionFailedException : public RentalException {
public:
    explicit InspectionFailedException(const std::string& propertyId);
    virtual ~InspectionFailedException() = default;
};

#endif