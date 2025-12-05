#ifndef INVALIDLEASETERMEXCEPTION_H
#define INVALIDLEASETERMEXCEPTION_H

#include "RentalException.h"
#include <string>

class InvalidLeaseTermException : public RentalException {
public:
    explicit InvalidLeaseTermException(int termMonths);
    virtual ~InvalidLeaseTermException() = default;
};

#endif