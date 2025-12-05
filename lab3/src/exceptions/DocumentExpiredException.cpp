#include "../../include/exceptions/DocumentExpiredException.h"

DocumentExpiredException::DocumentExpiredException(const std::string& documentId)
    : RentalException("Document expired: " + documentId) {}