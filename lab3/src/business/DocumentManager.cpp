#include "../../include/business/DocumentManager.h"
#include <chrono>
#include <algorithm>

DocumentManager::DocumentManager(long maxSize, const std::vector<std::string>& formats,
                                 int retentionPeriod, bool autoBackup)
    : maxFileSize(maxSize), allowedFormats(formats),
      retentionPeriodYears(retentionPeriod), automaticBackupEnabled(autoBackup) {}

DocumentManager::DocumentManager(const DocumentManager& other)
    : documents(other.documents), propertyDocuments(other.propertyDocuments),
      tenantDocuments(other.tenantDocuments), maxFileSize(other.maxFileSize),
      allowedFormats(other.allowedFormats), retentionPeriodYears(other.retentionPeriodYears),
      automaticBackupEnabled(other.automaticBackupEnabled) {}

DocumentManager& DocumentManager::operator=(const DocumentManager& other) {
    if (this != &other) {
        documents = other.documents;
        propertyDocuments = other.propertyDocuments;
        tenantDocuments = other.tenantDocuments;
        maxFileSize = other.maxFileSize;
        allowedFormats = other.allowedFormats;
        retentionPeriodYears = other.retentionPeriodYears;
        automaticBackupEnabled = other.automaticBackupEnabled;
    }
    return *this;
}

DocumentManager::DocumentManager(DocumentManager&& other) noexcept
    : documents(std::move(other.documents)), propertyDocuments(std::move(other.propertyDocuments)),
      tenantDocuments(std::move(other.tenantDocuments)), maxFileSize(other.maxFileSize),
      allowedFormats(std::move(other.allowedFormats)), retentionPeriodYears(other.retentionPeriodYears),
      automaticBackupEnabled(other.automaticBackupEnabled) {}

DocumentManager& DocumentManager::operator=(DocumentManager&& other) noexcept {
    if (this != &other) {
        documents = std::move(other.documents);
        propertyDocuments = std::move(other.propertyDocuments);
        tenantDocuments = std::move(other.tenantDocuments);
        maxFileSize = other.maxFileSize;
        allowedFormats = std::move(other.allowedFormats);
        retentionPeriodYears = other.retentionPeriodYears;
        automaticBackupEnabled = other.automaticBackupEnabled;
    }
    return *this;
}

DocumentManager::~DocumentManager() {}

long DocumentManager::getMaxFileSize() const { return maxFileSize; }
std::vector<std::string> DocumentManager::getAllowedFormats() const { return allowedFormats; }
int DocumentManager::getRetentionPeriodYears() const { return retentionPeriodYears; }
bool DocumentManager::getAutomaticBackupEnabled() const { return automaticBackupEnabled; }

void DocumentManager::setMaxFileSize(long newSize) {
    maxFileSize = std::max(1024L, newSize);
}

void DocumentManager::setRetentionPeriodYears(int newPeriod) {
    retentionPeriodYears = std::max(1, newPeriod);
}

void DocumentManager::setAutomaticBackupEnabled(bool enabled) {
    automaticBackupEnabled = enabled;
}

bool DocumentManager::uploadDocument(std::shared_ptr<Document> document) {
    if (!document) return false;
    
    if (!isFileFormatAllowed(document->getMimeType())) return false;
    if (!isFileSizeWithinLimit(document->getFileSize())) return false;
    
    documents.push_back(document);
    
    if (document->getRelatedProperty()) {
        propertyDocuments[document->getRelatedProperty()->getId()].push_back(document);
    }
    
    if (document->getRelatedTenant()) {
        tenantDocuments[document->getRelatedTenant()->getTenantId()].push_back(document);
    }
    
    return true;
}

bool DocumentManager::deleteDocument(const std::string& documentId) {
    auto it = std::find_if(documents.begin(), documents.end(),
        [&](const std::shared_ptr<Document>& doc) {
            return doc->getDocumentId() == documentId;
        });
    
    if (it != documents.end()) {
        documents.erase(it);
        return true;
    }
    return false;
}

std::vector<std::shared_ptr<Document>> DocumentManager::getDocumentsByProperty(const std::string& propertyId) const {
    auto it = propertyDocuments.find(propertyId);
    return it != propertyDocuments.end() ? it->second : std::vector<std::shared_ptr<Document>>();
}

std::vector<std::shared_ptr<Document>> DocumentManager::getDocumentsByTenant(const std::string& tenantId) const {
    auto it = tenantDocuments.find(tenantId);
    return it != tenantDocuments.end() ? it->second : std::vector<std::shared_ptr<Document>>();
}

std::vector<std::shared_ptr<Document>> DocumentManager::getExpiredDocuments(
    const std::chrono::system_clock::time_point& currentDate) const {
    
    std::vector<std::shared_ptr<Document>> expired;
    
    for (const auto& document : documents) {
        if (document->isExpired(currentDate)) {
            expired.push_back(document);
        }
    }
    
    return expired;
}

bool DocumentManager::isFileFormatAllowed(const std::string& format) const {
    for (const auto& allowedFormat : allowedFormats) {
        if (format.find(allowedFormat) != std::string::npos) {
            return true;
        }
    }
    return false;
}

bool DocumentManager::isFileSizeWithinLimit(long fileSize) const {
    return fileSize <= maxFileSize;
}

std::vector<std::shared_ptr<Document>> DocumentManager::searchDocuments(const std::string& keyword) const {
    std::vector<std::shared_ptr<Document>> results;
    
    for (const auto& document : documents) {
        if (document->getTitle().find(keyword) != std::string::npos ||
            document->getDescription().find(keyword) != std::string::npos ||
            document->getDocumentType().find(keyword) != std::string::npos) {
            results.push_back(document);
        }
    }
    
    return results;
}

double DocumentManager::calculateTotalStorageUsed() const {
    long totalSize = 0;
    for (const auto& document : documents) {
        totalSize += document->getFileSize();
    }
    return static_cast<double>(totalSize) / (1024 * 1024 * 1024);
}

bool DocumentManager::requiresRenewal(const std::string& documentId,
                                     const std::chrono::system_clock::time_point& currentDate) const {
    for (const auto& document : documents) {
        if (document->getDocumentId() == documentId) {
            return document->requiresRenewal();
        }
    }
    return false;
}

std::vector<std::shared_ptr<Document>> DocumentManager::getLegalDocuments() const {
    std::vector<std::shared_ptr<Document>> legalDocs;
    
    for (const auto& document : documents) {
        if (document->isLegalDocument()) {
            legalDocs.push_back(document);
        }
    }
    
    return legalDocs;
}