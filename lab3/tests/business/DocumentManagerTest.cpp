#include <gtest/gtest.h>
#include <memory>
#include <chrono>
#include "../../include/business/DocumentManager.h"
#include "../../include/core/Document.h"
#include "../../include/core/Property.h"
#include "../../include/core/Tenant.h"
#include "../../include/core/Address.h"
#include "../../include/core/PropertyStatus.h"
#include "../../include/core/PropertyType.h"

class DocumentManagerTest : public ::testing::Test {
protected:
    void SetUp() override {
        property = std::make_shared<Property>("P1", 100.0, 1200.0,
            std::make_unique<Address>("Street", "City", "State", "12345"),
            std::make_unique<PropertyStatus>("AVAILABLE"),
            std::make_unique<PropertyType>("Apartment"));
        
        tenant = std::make_shared<Tenant>("T1", "John", "Doe", "john@example.com", 
            "1234567890", std::make_unique<Address>("Street", "City", "State", "12345"), 
            700, 5000, false, 2);
        
        auto now = std::chrono::system_clock::now();
        
        doc1 = std::make_shared<Document>("D1", "contract", "Lease Agreement", 
            "Lease contract for apartment", "/path/to/file.pdf", now,
            now + std::chrono::hours(24*365), property, tenant, "active", 
            "legal", 1024*500, "application/pdf");
        
        doc2 = std::make_shared<Document>("D2", "inspection", "Property Inspection", 
            "Annual property inspection report", "/path/to/inspection.pdf", now,
            now + std::chrono::hours(24*180), property, nullptr, "active", 
            "report", 1024*300, "application/pdf");
        
        doc3 = std::make_shared<Document>("D3", "maintenance", "Maintenance Record", 
            "Quarterly maintenance record", "/path/to/maintenance.jpg", now,
            now + std::chrono::hours(24*90), nullptr, tenant, "active", 
            "record", 1024*200, "image/jpg");
    }
    
    std::shared_ptr<Property> property;
    std::shared_ptr<Tenant> tenant;
    std::shared_ptr<Document> doc1;
    std::shared_ptr<Document> doc2;
    std::shared_ptr<Document> doc3;
};

TEST_F(DocumentManagerTest, ConstructorAndGetters) {
    DocumentManager manager(10485760, {"pdf", "jpg", "docx"}, 7, true);
    
    EXPECT_EQ(manager.getMaxFileSize(), 10485760);
    
    auto formats = manager.getAllowedFormats();
    ASSERT_EQ(formats.size(), 3);
    EXPECT_EQ(formats[0], "pdf");
    EXPECT_EQ(formats[1], "jpg");
    EXPECT_EQ(formats[2], "docx");
    
    EXPECT_EQ(manager.getRetentionPeriodYears(), 7);
    EXPECT_TRUE(manager.getAutomaticBackupEnabled());
}

TEST_F(DocumentManagerTest, Setters) {
    DocumentManager manager(10485760, {"pdf"}, 5, false);
    
    manager.setMaxFileSize(20971520);
    EXPECT_EQ(manager.getMaxFileSize(), 20971520);
    
    manager.setMaxFileSize(500);
    EXPECT_EQ(manager.getMaxFileSize(), 1024);
    
    manager.setRetentionPeriodYears(10);
    EXPECT_EQ(manager.getRetentionPeriodYears(), 10);
    
    manager.setRetentionPeriodYears(0);
    EXPECT_EQ(manager.getRetentionPeriodYears(), 1);
    
    manager.setAutomaticBackupEnabled(true);
    EXPECT_TRUE(manager.getAutomaticBackupEnabled());
    
    manager.setAutomaticBackupEnabled(false);
    EXPECT_FALSE(manager.getAutomaticBackupEnabled());
}

TEST_F(DocumentManagerTest, UploadDocumentNullPointer) {
    DocumentManager manager(10485760, {"pdf"}, 5, true);
    
    EXPECT_FALSE(manager.uploadDocument(nullptr));
}

TEST_F(DocumentManagerTest, UploadDocumentInvalidFormat) {
    DocumentManager manager(10485760, {"pdf", "jpg"}, 5, true);
    
    auto invalidDoc = std::make_shared<Document>("D1", "contract", "Test", "Test",
        "/path/to/file.exe", std::chrono::system_clock::now(),
        std::chrono::system_clock::now() + std::chrono::hours(24*365),
        property, tenant, "active", "legal", 1024*100, "application/exe");
    
    EXPECT_FALSE(manager.uploadDocument(invalidDoc));
}

TEST_F(DocumentManagerTest, UploadDocumentExceedsSizeLimit) {
    DocumentManager manager(1024*1024, {"pdf"}, 5, true);
    
    auto largeDoc = std::make_shared<Document>("D1", "contract", "Test", "Test",
        "/path/to/file.pdf", std::chrono::system_clock::now(),
        std::chrono::system_clock::now() + std::chrono::hours(24*365),
        property, tenant, "active", "legal", 2*1024*1024, "application/pdf");
    
    EXPECT_FALSE(manager.uploadDocument(largeDoc));
}

TEST_F(DocumentManagerTest, UploadMultipleDocuments) {
    DocumentManager manager(10485760, {"pdf", "jpg"}, 5, true);
    
    EXPECT_TRUE(manager.uploadDocument(doc1));
    EXPECT_TRUE(manager.uploadDocument(doc2));
    EXPECT_TRUE(manager.uploadDocument(doc3));
    
    EXPECT_EQ(manager.getDocumentsByProperty("P1").size(), 2);
    EXPECT_EQ(manager.getDocumentsByTenant("T1").size(), 2);
}

TEST_F(DocumentManagerTest, DeleteDocumentNonExistent) {
    DocumentManager manager(10485760, {"pdf"}, 5, true);
    
    EXPECT_FALSE(manager.deleteDocument("NON_EXISTENT"));
}

TEST_F(DocumentManagerTest, DeleteDocumentAndVerifyCleanup) {
    DocumentManager manager(10485760, {"pdf", "jpg"}, 5, true);
    
    manager.uploadDocument(doc1);
    manager.uploadDocument(doc2);
    manager.uploadDocument(doc3);
    
    EXPECT_EQ(manager.getDocumentsByProperty("P1").size(), 2);
    EXPECT_EQ(manager.getDocumentsByTenant("T1").size(), 2);
    
    EXPECT_TRUE(manager.deleteDocument("D1"));
    
    EXPECT_EQ(manager.getDocumentsByProperty("P1").size(), 2);
    EXPECT_EQ(manager.getDocumentsByTenant("T1").size(), 2);
}

TEST_F(DocumentManagerTest, GetDocumentsByPropertyNonExistent) {
    DocumentManager manager(10485760, {"pdf"}, 5, true);
    
    auto documents = manager.getDocumentsByProperty("NON_EXISTENT");
    EXPECT_TRUE(documents.empty());
}

TEST_F(DocumentManagerTest, GetDocumentsByTenantNonExistent) {
    DocumentManager manager(10485760, {"pdf"}, 5, true);
    
    auto documents = manager.getDocumentsByTenant("NON_EXISTENT");
    EXPECT_TRUE(documents.empty());
}

TEST_F(DocumentManagerTest, GetDocumentsByPropertyOnly) {
    DocumentManager manager(10485760, {"pdf", "jpg"}, 5, true);
    
    manager.uploadDocument(doc2);
    
    auto propertyDocs = manager.getDocumentsByProperty("P1");
    EXPECT_EQ(propertyDocs.size(), 1);
    
    auto tenantDocs = manager.getDocumentsByTenant("T1");
    EXPECT_TRUE(tenantDocs.empty());
}

TEST_F(DocumentManagerTest, GetDocumentsByTenantOnly) {
    DocumentManager manager(10485760, {"pdf", "jpg"}, 5, true);
    
    manager.uploadDocument(doc3);
    
    auto propertyDocs = manager.getDocumentsByProperty("P1");
    EXPECT_TRUE(propertyDocs.empty());
    
    auto tenantDocs = manager.getDocumentsByTenant("T1");
    EXPECT_EQ(tenantDocs.size(), 1);
}

TEST_F(DocumentManagerTest, GetExpiredDocuments) {
    DocumentManager manager(10485760, {"pdf"}, 5, true);
    
    auto now = std::chrono::system_clock::now();
    auto pastDate = now - std::chrono::hours(24*400);
    
    auto expiredDoc = std::make_shared<Document>("D4", "old", "Expired", "Expired document",
        "/path/to/expired.pdf", pastDate, pastDate + std::chrono::hours(24*30),
        property, tenant, "expired", "legal", 1024*100, "application/pdf");
    
    manager.uploadDocument(expiredDoc);
    manager.uploadDocument(doc1);
    
    auto expiredDocs = manager.getExpiredDocuments(now);
    EXPECT_EQ(expiredDocs.size(), 1);
    EXPECT_EQ(expiredDocs[0]->getDocumentId(), "D4");
}

TEST_F(DocumentManagerTest, GetExpiredDocumentsNone) {
    DocumentManager manager(10485760, {"pdf"}, 5, true);
    
    manager.uploadDocument(doc1);
    manager.uploadDocument(doc2);
    
    auto now = std::chrono::system_clock::now();
    auto expiredDocs = manager.getExpiredDocuments(now);
    
    EXPECT_TRUE(expiredDocs.empty());
}

TEST_F(DocumentManagerTest, IsFileFormatAllowed) {
    DocumentManager manager(10485760, {"pdf", "jpg", "png"}, 5, true);
    
    EXPECT_TRUE(manager.isFileFormatAllowed("application/pdf"));
    EXPECT_TRUE(manager.isFileFormatAllowed("image/jpg"));
    EXPECT_TRUE(manager.isFileFormatAllowed("image/png"));
    EXPECT_TRUE(manager.isFileFormatAllowed("pdf"));
    EXPECT_TRUE(manager.isFileFormatAllowed("jpg"));
    
    EXPECT_FALSE(manager.isFileFormatAllowed("application/exe"));
    EXPECT_FALSE(manager.isFileFormatAllowed("text/plain"));
    EXPECT_FALSE(manager.isFileFormatAllowed(""));
    
    EXPECT_TRUE(manager.isFileFormatAllowed("application/vnd.pdf"));
}

TEST_F(DocumentManagerTest, IsFileSizeWithinLimit) {
    DocumentManager manager(10485760, {"pdf"}, 5, true);
    
    EXPECT_TRUE(manager.isFileSizeWithinLimit(10485760));
    EXPECT_TRUE(manager.isFileSizeWithinLimit(5242880));
    EXPECT_TRUE(manager.isFileSizeWithinLimit(0));
    
    EXPECT_FALSE(manager.isFileSizeWithinLimit(10485761));
    EXPECT_FALSE(manager.isFileSizeWithinLimit(20971520));
    
    EXPECT_TRUE(manager.isFileSizeWithinLimit(-1000));
}

TEST_F(DocumentManagerTest, SearchDocuments) {
    DocumentManager manager(10485760, {"pdf", "jpg"}, 5, true);
    
    manager.uploadDocument(doc1);
    manager.uploadDocument(doc2);
    manager.uploadDocument(doc3);
    
    auto leaseResults = manager.searchDocuments("Lease");
    EXPECT_EQ(leaseResults.size(), 1);
    EXPECT_EQ(leaseResults[0]->getDocumentId(), "D1");
    
    auto propertyResults = manager.searchDocuments("Property");
    EXPECT_EQ(propertyResults.size(), 1);
    EXPECT_EQ(propertyResults[0]->getDocumentId(), "D2");
    
    auto maintenanceResults = manager.searchDocuments("maintenance");
    EXPECT_EQ(maintenanceResults.size(), 1);
    EXPECT_EQ(maintenanceResults[0]->getDocumentId(), "D3");
    
    auto contractResults = manager.searchDocuments("contract");
    EXPECT_EQ(contractResults.size(), 1);
    
    auto emptyResults = manager.searchDocuments("nonexistent");
    EXPECT_TRUE(emptyResults.empty());
    
    auto caseSensitiveResults = manager.searchDocuments("LEASE");
    EXPECT_EQ(caseSensitiveResults.size(), 0);
    
    auto partialResults = manager.searchDocuments("agree");
    EXPECT_EQ(partialResults.size(), 0);
}

TEST_F(DocumentManagerTest, SearchDocumentsEmptyKeyword) {
    DocumentManager manager(10485760, {"pdf"}, 5, true);
    
    manager.uploadDocument(doc1);
    
    auto results = manager.searchDocuments("");
    EXPECT_EQ(results.size(), 1);
}

TEST_F(DocumentManagerTest, CalculateTotalStorageUsed) {
    DocumentManager manager(10485760, {"pdf", "jpg"}, 5, true);
    
    EXPECT_DOUBLE_EQ(manager.calculateTotalStorageUsed(), 0.0);
    
    manager.uploadDocument(doc1);
    double storage1 = manager.calculateTotalStorageUsed();
    EXPECT_GT(storage1, 0.0);
    
    manager.uploadDocument(doc2);
    double storage2 = manager.calculateTotalStorageUsed();
    EXPECT_GT(storage2, storage1);
    
    manager.uploadDocument(doc3);
    double storage3 = manager.calculateTotalStorageUsed();
    EXPECT_GT(storage3, storage2);
    
    manager.deleteDocument("D1");
    double storage4 = manager.calculateTotalStorageUsed();
    EXPECT_LT(storage4, storage3);
}

TEST_F(DocumentManagerTest, RequiresRenewal) {
    DocumentManager manager(10485760, {"pdf"}, 5, true);
    
    auto now = std::chrono::system_clock::now();
    auto nearExpiry = now - std::chrono::hours(24*350);
    
    auto renewalDoc = std::make_shared<Document>("D4", "license", "Business License", 
        "Annual business license", "/path/to/license.pdf", nearExpiry,
        nearExpiry + std::chrono::hours(24*365), property, tenant, "active", 
        "legal", 1024*100, "application/pdf");
    
    manager.uploadDocument(renewalDoc);
    manager.uploadDocument(doc1);
    
    EXPECT_TRUE(manager.requiresRenewal("D4", now));
    EXPECT_FALSE(manager.requiresRenewal("D1", now));
    EXPECT_FALSE(manager.requiresRenewal("NON_EXISTENT", now));
}

TEST_F(DocumentManagerTest, GetLegalDocuments) {
    DocumentManager manager(10485760, {"pdf", "jpg"}, 5, true);
    
    auto now = std::chrono::system_clock::now();
    
    auto legalDoc1 = std::make_shared<Document>("D4", "contract", "Legal Contract", 
        "Legal agreement", "/path/to/contract.pdf", now,
        now + std::chrono::hours(24*365), property, tenant, "active", 
        "legal", 1024*100, "application/pdf");
    
    auto nonLegalDoc = std::make_shared<Document>("D5", "photo", "Property Photo", 
        "Exterior photo", "/path/to/photo.jpg", now,
        now + std::chrono::hours(24*365), property, tenant, "active", 
        "photo", 1024*500, "image/jpg");
    
    manager.uploadDocument(legalDoc1);
    manager.uploadDocument(nonLegalDoc);
    manager.uploadDocument(doc1);
    
    auto legalDocs = manager.getLegalDocuments();
    EXPECT_EQ(legalDocs.size(), 2);
    
    bool foundLegal1 = false;
    bool foundDoc1 = false;
    for (const auto& doc : legalDocs) {
        if (doc->getDocumentId() == "D4") foundLegal1 = true;
        if (doc->getDocumentId() == "D1") foundDoc1 = true;
    }
    
    EXPECT_TRUE(foundLegal1);
    EXPECT_TRUE(foundDoc1);
}

TEST_F(DocumentManagerTest, GetLegalDocumentsNone) {
    DocumentManager manager(10485760, {"jpg"}, 5, true);
    
    auto now = std::chrono::system_clock::now();
    
    auto photoDoc = std::make_shared<Document>("D4", "photo", "Property Photo", 
        "Exterior photo", "/path/to/photo.jpg", now,
        now + std::chrono::hours(24*365), property, tenant, "active", 
        "photo", 1024*500, "image/jpg");
    
    manager.uploadDocument(photoDoc);
    
    auto legalDocs = manager.getLegalDocuments();
    EXPECT_TRUE(legalDocs.empty());
}

TEST_F(DocumentManagerTest, CopyConstructor) {
    DocumentManager original(10485760, {"pdf", "jpg"}, 7, true);
    
    original.uploadDocument(doc1);
    original.uploadDocument(doc2);
    
    DocumentManager copy(original);
    
    EXPECT_EQ(copy.getMaxFileSize(), 10485760);
    EXPECT_EQ(copy.getRetentionPeriodYears(), 7);
    EXPECT_TRUE(copy.getAutomaticBackupEnabled());
    
    auto formats = copy.getAllowedFormats();
    ASSERT_EQ(formats.size(), 2);
    EXPECT_EQ(formats[0], "pdf");
    EXPECT_EQ(formats[1], "jpg");
    
    EXPECT_EQ(copy.getDocumentsByProperty("P1").size(), 2);
    EXPECT_EQ(copy.getDocumentsByTenant("T1").size(), 1);
}

TEST_F(DocumentManagerTest, CopyAssignment) {
    DocumentManager original(10485760, {"pdf", "jpg"}, 7, true);
    original.uploadDocument(doc1);
    
    DocumentManager assigned(20971520, {"docx"}, 3, false);
    assigned.uploadDocument(doc2);
    
    assigned = original;
    
    EXPECT_EQ(assigned.getMaxFileSize(), 10485760);
    EXPECT_EQ(assigned.getRetentionPeriodYears(), 7);
    EXPECT_TRUE(assigned.getAutomaticBackupEnabled());
    
    auto formats = assigned.getAllowedFormats();
    ASSERT_EQ(formats.size(), 2);
    EXPECT_EQ(formats[0], "pdf");
    EXPECT_EQ(formats[1], "jpg");
    
    EXPECT_EQ(assigned.getDocumentsByProperty("P1").size(), 1);
    EXPECT_EQ(assigned.getDocumentsByTenant("T1").size(), 1);
}

TEST_F(DocumentManagerTest, MoveConstructor) {
    DocumentManager original(10485760, {"pdf", "jpg"}, 7, true);
    original.uploadDocument(doc1);
    
    DocumentManager moved(std::move(original));
    
    EXPECT_EQ(moved.getMaxFileSize(), 10485760);
    EXPECT_EQ(moved.getRetentionPeriodYears(), 7);
    EXPECT_TRUE(moved.getAutomaticBackupEnabled());
    
    auto formats = moved.getAllowedFormats();
    ASSERT_EQ(formats.size(), 2);
    EXPECT_EQ(formats[0], "pdf");
    EXPECT_EQ(formats[1], "jpg");
    
    EXPECT_EQ(moved.getDocumentsByProperty("P1").size(), 1);
    EXPECT_EQ(moved.getDocumentsByTenant("T1").size(), 1);
}

TEST_F(DocumentManagerTest, MoveAssignment) {
    DocumentManager original(10485760, {"pdf", "jpg"}, 7, true);
    original.uploadDocument(doc1);
    
    DocumentManager moved(20971520, {"docx"}, 3, false);
    moved.uploadDocument(doc2);
    
    moved = std::move(original);
    
    EXPECT_EQ(moved.getMaxFileSize(), 10485760);
    EXPECT_EQ(moved.getRetentionPeriodYears(), 7);
    EXPECT_TRUE(moved.getAutomaticBackupEnabled());
    
    auto formats = moved.getAllowedFormats();
    ASSERT_EQ(formats.size(), 2);
    EXPECT_EQ(formats[0], "pdf");
    EXPECT_EQ(formats[1], "jpg");
    
    EXPECT_EQ(moved.getDocumentsByProperty("P1").size(), 1);
    EXPECT_EQ(moved.getDocumentsByTenant("T1").size(), 1);
}

TEST_F(DocumentManagerTest, SelfAssignment) {
    DocumentManager manager(10485760, {"pdf"}, 5, true);
    manager.uploadDocument(doc1);
    
    manager = manager;
    
    EXPECT_EQ(manager.getMaxFileSize(), 10485760);
    EXPECT_EQ(manager.getRetentionPeriodYears(), 5);
    EXPECT_TRUE(manager.getAutomaticBackupEnabled());
    
    EXPECT_EQ(manager.getDocumentsByProperty("P1").size(), 1);
    EXPECT_EQ(manager.getDocumentsByTenant("T1").size(), 1);
}

TEST_F(DocumentManagerTest, MixedDocumentTypesAndRelationships) {
    DocumentManager manager(10485760, {"pdf", "jpg", "docx"}, 5, true);
    
    auto now = std::chrono::system_clock::now();
    
    auto property2 = std::make_shared<Property>("P2", 150.0, 1800.0,
        std::make_unique<Address>("Another St", "City", "State", "54321"),
        std::make_unique<PropertyStatus>("AVAILABLE"),
        std::make_unique<PropertyType>("House"));
    
    auto tenant2 = std::make_shared<Tenant>("T2", "Jane", "Smith", "jane@example.com", 
        "0987654321", std::make_unique<Address>("Other St", "City", "State", "67890"), 
        900, 6000, false, 1);
    
    std::vector<std::shared_ptr<Document>> allDocs;
    
    for (int i = 1; i <= 10; i++) {
        std::string id = "DOC" + std::to_string(i);
        std::shared_ptr<Property> docProperty = (i % 2 == 0) ? property : property2;
        std::shared_ptr<Tenant> docTenant = (i % 3 == 0) ? tenant : tenant2;
        
        auto doc = std::make_shared<Document>(id, "type" + std::to_string(i), 
            "Title " + std::to_string(i), "Description " + std::to_string(i),
            "/path/to/file" + std::to_string(i) + ".pdf", now,
            now + std::chrono::hours(24*365), docProperty, docTenant,
            "active", (i % 4 == 0) ? "legal" : "other", 1024*100*(i%5+1),
            "application/pdf");
        
        allDocs.push_back(doc);
        manager.uploadDocument(doc);
    }
    
    EXPECT_EQ(manager.getDocumentsByProperty("P1").size(), 5);
    EXPECT_EQ(manager.getDocumentsByProperty("P2").size(), 5);
    EXPECT_EQ(manager.getDocumentsByTenant("T1").size(), 3);
    EXPECT_EQ(manager.getDocumentsByTenant("T2").size(), 7);
    
    auto legalDocs = manager.getLegalDocuments();
    EXPECT_EQ(legalDocs.size(), 2);
    
    double storageUsed = manager.calculateTotalStorageUsed();
    EXPECT_GT(storageUsed, 0.0);
}

TEST_F(DocumentManagerTest, FormatMatchingEdgeCases) {
    DocumentManager manager(10485760, {"pdf", "jpg", "doc", "xls"}, 5, true);
    
    EXPECT_TRUE(manager.isFileFormatAllowed("application/pdf"));
    EXPECT_TRUE(manager.isFileFormatAllowed("image/jpg"));
    EXPECT_FALSE(manager.isFileFormatAllowed("application/msword"));
    EXPECT_FALSE(manager.isFileFormatAllowed("application/vnd.ms-excel"));
    
    EXPECT_FALSE(manager.isFileFormatAllowed("application/zip"));
    EXPECT_FALSE(manager.isFileFormatAllowed("text/plain"));
    
    EXPECT_TRUE(manager.isFileFormatAllowed("pdf"));
    EXPECT_TRUE(manager.isFileFormatAllowed("doc"));
}

TEST_F(DocumentManagerTest, EmptyManagerOperations) {
    DocumentManager manager(10485760, {"pdf"}, 5, true);
    
    EXPECT_TRUE(manager.getDocumentsByProperty("ANY").empty());
    EXPECT_TRUE(manager.getDocumentsByTenant("ANY").empty());
    EXPECT_TRUE(manager.getExpiredDocuments(std::chrono::system_clock::now()).empty());
    EXPECT_TRUE(manager.searchDocuments("anything").empty());
    EXPECT_TRUE(manager.getLegalDocuments().empty());
    EXPECT_DOUBLE_EQ(manager.calculateTotalStorageUsed(), 0.0);
    EXPECT_FALSE(manager.requiresRenewal("ANY", std::chrono::system_clock::now()));
    EXPECT_FALSE(manager.deleteDocument("ANY"));
}
