#include "../../include/exceptions/InspectionFailedException.h"

InspectionFailedException::InspectionFailedException(const std::string& propertyId)
    : RentalException("Property inspection failed: " + propertyId) {}