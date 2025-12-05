#include "../../include/exceptions/CustomerBlacklistedException.h"

CustomerBlacklistedException::CustomerBlacklistedException(const std::string& message)
    : std::runtime_error(message) {}