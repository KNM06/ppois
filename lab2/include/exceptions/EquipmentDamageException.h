#ifndef EQUIPMENTDAMAGEEXCEPTION_H
#define EQUIPMENTDAMAGEEXCEPTION_H

#include <stdexcept>
#include <string>

class EquipmentDamageException : public std::runtime_error {
public:
    explicit EquipmentDamageException(const std::string& message);
    virtual ~EquipmentDamageException();
};

#endif