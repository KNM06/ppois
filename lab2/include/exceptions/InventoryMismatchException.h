#ifndef INVENTORYMISMATCHEXCEPTION_H
#define INVENTORYMISMATCHEXCEPTION_H

#include <stdexcept>
#include <string>

class InventoryMismatchException : public std::runtime_error {
public:
    explicit InventoryMismatchException(const std::string& message);
    virtual ~InventoryMismatchException();
};

#endif