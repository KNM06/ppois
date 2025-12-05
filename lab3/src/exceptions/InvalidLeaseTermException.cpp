#include "../../include/exceptions/InvalidLeaseTermException.h"

InvalidLeaseTermException::InvalidLeaseTermException(int termMonths)
    : RentalException("Invalid lease term: " + std::to_string(termMonths) + " months") {}