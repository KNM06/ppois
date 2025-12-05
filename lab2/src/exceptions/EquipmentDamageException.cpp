#include "../../include/exceptions/EquipmentDamageException.h"

EquipmentDamageException::EquipmentDamageException(const std::string& message)
    : std::runtime_error(message) {}

EquipmentDamageException::~EquipmentDamageException() = default;