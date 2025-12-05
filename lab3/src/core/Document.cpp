#include "../../include/core/Document.h"
#include <chrono>
#include <algorithm>

Document::Document(const std::string& id, const std::string& type, const std::string& title,
                   const std::string& description, const std::string& filePath,
                   const std::chrono::system_clock::time_point& upload,
                   const std::chrono::system_clock::time_point& expiration,
                   std::shared_ptr<Property> property, std::shared_ptr<Tenant> tenant,
                   const std::string& status, const std::string& category,
                   long fileSize, const std::string& mimeType)
    : documentId(id), documentType(type), title(title), description(description),
      filePath(filePath), uploadDate(upload), expirationDate(expiration),
      relatedProperty(property), relatedTenant(tenant), status(status),
      category(category), fileSize(fileSize), mimeType(mimeType) {}

Document::Document(const Document& other)
    : documentId(other.documentId), documentType(other.documentType),
      title(other.title), description(other.description), filePath(other.filePath),
      uploadDate(other.uploadDate), expirationDate(other.expirationDate),
      relatedProperty(other.relatedProperty), relatedTenant(other.relatedTenant),
      status(other.status), category(other.category), fileSize(other.fileSize),
      mimeType(other.mimeType) {}

Document& Document::operator=(const Document& other) {
    if (this != &other) {
        documentId = other.documentId;
        documentType = other.documentType;
        title = other.title;
        description = other.description;
        filePath = other.filePath;
        uploadDate = other.uploadDate;
        expirationDate = other.expirationDate;
        relatedProperty = other.relatedProperty;
        relatedTenant = other.relatedTenant;
        status = other.status;
        category = other.category;
        fileSize = other.fileSize;
        mimeType = other.mimeType;
    }
    return *this;
}

Document::Document(Document&& other) noexcept
    : documentId(std::move(other.documentId)), documentType(std::move(other.documentType)),
      title(std::move(other.title)), description(std::move(other.description)),
      filePath(std::move(other.filePath)), uploadDate(other.uploadDate),
      expirationDate(other.expirationDate), relatedProperty(std::move(other.relatedProperty)),
      relatedTenant(std::move(other.relatedTenant)), status(std::move(other.status)),
      category(std::move(other.category)), fileSize(other.fileSize),
      mimeType(std::move(other.mimeType)) {}

Document& Document::operator=(Document&& other) noexcept {
    if (this != &other) {
        documentId = std::move(other.documentId);
        documentType = std::move(other.documentType);
        title = std::move(other.title);
        description = std::move(other.description);
        filePath = std::move(other.filePath);
        uploadDate = other.uploadDate;
        expirationDate = other.expirationDate;
        relatedProperty = std::move(other.relatedProperty);
        relatedTenant = std::move(other.relatedTenant);
        status = std::move(other.status);
        category = std::move(other.category);
        fileSize = other.fileSize;
        mimeType = std::move(other.mimeType);
    }
    return *this;
}

Document::~Document() {}

std::string Document::getDocumentId() const { return documentId; }
std::string Document::getDocumentType() const { return documentType; }
std::string Document::getTitle() const { return title; }
std::string Document::getDescription() const { return description; }
std::string Document::getFilePath() const { return filePath; }
std::chrono::system_clock::time_point Document::getUploadDate() const { return uploadDate; }
std::chrono::system_clock::time_point Document::getExpirationDate() const { return expirationDate; }
Property* Document::getRelatedProperty() const { return relatedProperty.get(); }
Tenant* Document::getRelatedTenant() const { return relatedTenant.get(); }
std::string Document::getStatus() const { return status; }
std::string Document::getCategory() const { return category; }
long Document::getFileSize() const { return fileSize; }
std::string Document::getMimeType() const { return mimeType; }

void Document::setStatus(const std::string& newStatus) {
    status = newStatus;
}

void Document::setExpirationDate(const std::chrono::system_clock::time_point& newDate) {
    expirationDate = newDate;
}

void Document::setFilePath(const std::string& newPath) {
    filePath = newPath;
}

bool Document::isExpired(const std::chrono::system_clock::time_point& currentDate) const {
    return currentDate > expirationDate;
}

bool Document::isLegalDocument() const {
    return documentType == "contract" || documentType == "lease" || 
           documentType == "agreement" || category == "legal";
}

bool Document::isFinancialDocument() const {
    return documentType == "invoice" || documentType == "receipt" ||
           documentType == "payment" || category == "financial";
}

bool Document::requiresSignature() const {
    return isLegalDocument() || documentType == "approval" || documentType == "authorization";
}

long Document::calculateFileSizeMB() const {
    return fileSize / (1024 * 1024);
}

bool Document::isImageDocument() const {
    return mimeType.find("image") != std::string::npos ||
           mimeType.find("jpeg") != std::string::npos ||
           mimeType.find("png") != std::string::npos;
}

bool Document::isPdfDocument() const {
    return mimeType.find("pdf") != std::string::npos || 
           documentType.find("pdf") != std::string::npos;
}

int Document::getDaysUntilExpiration(const std::chrono::system_clock::time_point& currentDate) const {
    if (isExpired(currentDate)) return 0;
    
    auto duration = std::chrono::duration_cast<std::chrono::hours>(expirationDate - currentDate);
    return static_cast<int>(duration.count() / 24.0);
}

bool Document::requiresRenewal() const {
    return getDaysUntilExpiration(std::chrono::system_clock::now()) < 30 && isLegalDocument();
}

bool Document::canBeSharedExternally() const {
    return !isLegalDocument() && category != "confidential" && category != "internal";
}

std::string Document::getDocumentClassification() const {
    if (isLegalDocument()) return "high_priority";
    if (isFinancialDocument()) return "medium_priority";
    if (isExpired(std::chrono::system_clock::now())) return "expired";
    return "standard";
}