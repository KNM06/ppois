#ifndef DOCUMENTMANAGER_H
#define DOCUMENTMANAGER_H

#include <memory>
#include <vector>
#include <unordered_map>
#include "../../include/core/Document.h"
#include "../../include/core/Property.h"
#include "../../include/core/Tenant.h"

class DocumentManager {
private:
    std::vector<std::shared_ptr<Document>> documents;
    std::unordered_map<std::string, std::vector<std::shared_ptr<Document>>> propertyDocuments;
    std::unordered_map<std::string, std::vector<std::shared_ptr<Document>>> tenantDocuments;
    long maxFileSize;
    std::vector<std::string> allowedFormats;
    int retentionPeriodYears;
    bool automaticBackupEnabled;

public:
    DocumentManager(long maxSize, const std::vector<std::string>& formats,
                   int retentionPeriod, bool autoBackup);
    
    DocumentManager(const DocumentManager& other);
    DocumentManager& operator=(const DocumentManager& other);
    DocumentManager(DocumentManager&& other) noexcept;
    DocumentManager& operator=(DocumentManager&& other) noexcept;
    ~DocumentManager();

    long getMaxFileSize() const;
    std::vector<std::string> getAllowedFormats() const;
    int getRetentionPeriodYears() const;
    bool getAutomaticBackupEnabled() const;

    void setMaxFileSize(long newSize);
    void setRetentionPeriodYears(int newPeriod);
    void setAutomaticBackupEnabled(bool enabled);

    bool uploadDocument(std::shared_ptr<Document> document);
    bool deleteDocument(const std::string& documentId);
    std::vector<std::shared_ptr<Document>> getDocumentsByProperty(const std::string& propertyId) const;
    std::vector<std::shared_ptr<Document>> getDocumentsByTenant(const std::string& tenantId) const;
    std::vector<std::shared_ptr<Document>> getExpiredDocuments(
        const std::chrono::system_clock::time_point& currentDate) const;
    bool isFileFormatAllowed(const std::string& format) const;
    bool isFileSizeWithinLimit(long fileSize) const;
    std::vector<std::shared_ptr<Document>> searchDocuments(const std::string& keyword) const;
    double calculateTotalStorageUsed() const;
    bool requiresRenewal(const std::string& documentId,
                        const std::chrono::system_clock::time_point& currentDate) const;
    std::vector<std::shared_ptr<Document>> getLegalDocuments() const;
};

#endif