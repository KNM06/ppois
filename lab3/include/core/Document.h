#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <string>
#include <memory>
#include <chrono>
#include "../../include/core/Property.h"
#include "../../include/core/Tenant.h"

class Document {
private:
    std::string documentId;
    std::string documentType;
    std::string title;
    std::string description;
    std::string filePath;
    std::chrono::system_clock::time_point uploadDate;
    std::chrono::system_clock::time_point expirationDate;
    std::shared_ptr<Property> relatedProperty;
    std::shared_ptr<Tenant> relatedTenant;
    std::string status;
    std::string category;
    long fileSize;
    std::string mimeType;

public:
    Document(const std::string& id, const std::string& type, const std::string& title,
             const std::string& description, const std::string& filePath,
             const std::chrono::system_clock::time_point& upload,
             const std::chrono::system_clock::time_point& expiration,
             std::shared_ptr<Property> property, std::shared_ptr<Tenant> tenant,
             const std::string& status, const std::string& category,
             long fileSize, const std::string& mimeType);
    
    Document(const Document& other);
    Document& operator=(const Document& other);
    Document(Document&& other) noexcept;
    Document& operator=(Document&& other) noexcept;
    ~Document();

    std::string getDocumentId() const;
    std::string getDocumentType() const;
    std::string getTitle() const;
    std::string getDescription() const;
    std::string getFilePath() const;
    std::chrono::system_clock::time_point getUploadDate() const;
    std::chrono::system_clock::time_point getExpirationDate() const;
    Property* getRelatedProperty() const;
    Tenant* getRelatedTenant() const;
    std::string getStatus() const;
    std::string getCategory() const;
    long getFileSize() const;
    std::string getMimeType() const;

    void setStatus(const std::string& newStatus);
    void setExpirationDate(const std::chrono::system_clock::time_point& newDate);
    void setFilePath(const std::string& newPath);

    bool isExpired(const std::chrono::system_clock::time_point& currentDate) const;
    bool isLegalDocument() const;
    bool isFinancialDocument() const;
    bool requiresSignature() const;
    long calculateFileSizeMB() const;
    bool isImageDocument() const;
    bool isPdfDocument() const;
    int getDaysUntilExpiration(const std::chrono::system_clock::time_point& currentDate) const;
    bool requiresRenewal() const;
    bool canBeSharedExternally() const;
    std::string getDocumentClassification() const;
};

#endif