#include "../../include/exceptions/PropertyNotAvailableException.h"

PropertyNotAvailableException::PropertyNotAvailableException(const std::string& propertyId)
    : RentalException("Property " + propertyId + " is not available for rent") {}