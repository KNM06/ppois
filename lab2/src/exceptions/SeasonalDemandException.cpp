#include "../../include/exceptions/SeasonalDemandException.h"

SeasonalDemandException::SeasonalDemandException(const std::string& message)
    : std::runtime_error(message) {}

SeasonalDemandException::~SeasonalDemandException() = default;  