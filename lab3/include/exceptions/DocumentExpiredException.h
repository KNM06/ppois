#ifndef DOCUMENTEXPIREDEXCEPTION_H
#define DOCUMENTEXPIREDEXCEPTION_H

#include "RentalException.h"
#include <string>

class DocumentExpiredException : public RentalException {
public:
    explicit DocumentExpiredException(const std::string& documentId);
    virtual ~DocumentExpiredException() = default;
};

#endif  