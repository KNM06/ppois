#include <gtest/gtest.h>
#include "../../include/exceptions/DocumentExpiredException.h"

TEST(DocumentExpiredExceptionTest, ConstructorSetsMessage) {
    std::string documentId = "D789";
    try {
        throw DocumentExpiredException(documentId);
    } catch (const DocumentExpiredException& ex) {
        std::string expected = "Document expired: " + documentId;
        EXPECT_STREQ(ex.what(), expected.c_str());
    } catch (...) {
        FAIL() << "Expected DocumentExpiredException";
    }
}
