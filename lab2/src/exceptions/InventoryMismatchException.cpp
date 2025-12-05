#include "../../include/exceptions/InventoryMismatchException.h"

InventoryMismatchException::InventoryMismatchException(const std::string& message)
    : std::runtime_error(message) {}

InventoryMismatchException::~InventoryMismatchException() = default;