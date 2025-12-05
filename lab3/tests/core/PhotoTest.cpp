#include <gtest/gtest.h>
#include "../../include/core/Photo.h"
#include "../../include/core/Property.h"
#include <chrono>

class PhotoTest : public ::testing::Test {
protected:
    void SetUp() override {
        property = std::make_shared<Property>("p1", "123 Main St", 100.0);
    }
    
    std::shared_ptr<Property> property;
    std::chrono::system_clock::time_point currentTime = std::chrono::system_clock::now();
};

TEST_F(PhotoTest, ConstructorAndGetters) {
    auto takenDate = currentTime - std::chrono::hours(24 * 30);
    Photo photo("ph1", property, "path/to/photo.jpg", "Beautiful living room",
                takenDate, "1920x1080", 2097152, true, "Living Room",
                "Wide", true, "John Doe");
    
    EXPECT_EQ(photo.getPhotoId(), "ph1");
    EXPECT_EQ(photo.getProperty(), property.get());
    EXPECT_EQ(photo.getFilePath(), "path/to/photo.jpg");
    EXPECT_EQ(photo.getCaption(), "Beautiful living room");
    EXPECT_EQ(photo.getTakenDate(), takenDate);
    EXPECT_EQ(photo.getResolution(), "1920x1080");
    EXPECT_EQ(photo.getFileSize(), 2097152);
    EXPECT_TRUE(photo.getIsPrimary());
    EXPECT_EQ(photo.getRoomType(), "Living Room");
    EXPECT_EQ(photo.getAngle(), "Wide");
    EXPECT_TRUE(photo.getIsApproved());
    EXPECT_EQ(photo.getPhotographer(), "John Doe");
}

TEST_F(PhotoTest, Setters) {
    Photo photo("ph1", property, "path.jpg", "Caption", currentTime,
                "1920x1080", 1000000, false, "Kitchen", "Close", false, "");
    
    photo.setIsPrimary(true);
    EXPECT_TRUE(photo.getIsPrimary());
    
    photo.setIsApproved(true);
    EXPECT_TRUE(photo.getIsApproved());
    
    photo.setCaption("New Caption Text");
    EXPECT_EQ(photo.getCaption(), "New Caption Text");
}

TEST_F(PhotoTest, IsHighResolution) {
    Photo lowResPhoto("ph1", property, "path.jpg", "", currentTime,
                     "640x480", 1000000, false, "Living Room", "Wide", true, "");
    EXPECT_FALSE(lowResPhoto.isHighResolution());
    
    Photo highResPhoto1("ph2", property, "path.jpg", "", currentTime,
                       "3840x2160", 1000000, false, "Living Room", "Wide", true, "");
    EXPECT_TRUE(highResPhoto1.isHighResolution());
    
    Photo highResPhoto2("ph3", property, "path.jpg", "", currentTime,
                       "4K", 1000000, false, "Living Room", "Wide", true, "");
    EXPECT_TRUE(highResPhoto2.isHighResolution());
    
    Photo highResPhoto3("ph4", property, "path.jpg", "", currentTime,
                       "2160x3840", 1000000, false, "Living Room", "Wide", true, "");
    EXPECT_TRUE(highResPhoto3.isHighResolution());
}

TEST_F(PhotoTest, CalculateFileSizeMB) {
    Photo photo1("ph1", property, "path.jpg", "", currentTime,
                "1920x1080", 1048576, false, "Living Room", "Wide", true, "");
    EXPECT_EQ(photo1.calculateFileSizeMB(), 1);
    
    Photo photo2("ph2", property, "path.jpg", "", currentTime,
                "1920x1080", 5242880, false, "Living Room", "Wide", true, "");
    EXPECT_EQ(photo2.calculateFileSizeMB(), 5);
    
    Photo photo3("ph3", property, "path.jpg", "", currentTime,
                "1920x1080", 0, false, "Living Room", "Wide", true, "");
    EXPECT_EQ(photo3.calculateFileSizeMB(), 0);
}

TEST_F(PhotoTest, IsRecent) {
    auto recentDate = currentTime - std::chrono::hours(24 * 10);
    auto oldDate = currentTime - std::chrono::hours(24 * 100);
    
    Photo recentPhoto("ph1", property, "path.jpg", "", recentDate,
                     "1920x1080", 1000000, false, "Living Room", "Wide", true, "");
    EXPECT_TRUE(recentPhoto.isRecent(currentTime));
    
    Photo oldPhoto("ph2", property, "path.jpg", "", oldDate,
                  "1920x1080", 1000000, false, "Living Room", "Wide", true, "");
    EXPECT_FALSE(oldPhoto.isRecent(currentTime));
    
    Photo edgeCasePhoto("ph3", property, "path.jpg", "", 
                       currentTime - std::chrono::hours(24 * 89),
                       "1920x1080", 1000000, false, "Living Room", "Wide", true, "");
    EXPECT_TRUE(edgeCasePhoto.isRecent(currentTime));
}

TEST_F(PhotoTest, IsExteriorShot) {
    Photo exterior1("ph1", property, "path.jpg", "", currentTime,
                   "1920x1080", 1000000, false, "exterior", "Wide", true, "");
    EXPECT_TRUE(exterior1.isExteriorShot());
    
    Photo exterior2("ph2", property, "path.jpg", "", currentTime,
                   "1920x1080", 1000000, false, "front", "Wide", true, "");
    EXPECT_TRUE(exterior2.isExteriorShot());
    
    Photo exterior3("ph3", property, "path.jpg", "", currentTime,
                   "1920x1080", 1000000, false, "back", "Wide", true, "");
    EXPECT_TRUE(exterior3.isExteriorShot());
    
    Photo exterior4("ph4", property, "path.jpg", "", currentTime,
                   "1920x1080", 1000000, false, "garden", "Wide", true, "");
    EXPECT_TRUE(exterior4.isExteriorShot());
    
    Photo exterior5("ph5", property, "path.jpg", "", currentTime,
                   "1920x1080", 1000000, false, "pool", "Wide", true, "");
    EXPECT_TRUE(exterior5.isExteriorShot());
    
    Photo interior("ph6", property, "path.jpg", "", currentTime,
                  "1920x1080", 1000000, false, "Living Room", "Wide", true, "");
    EXPECT_FALSE(interior.isExteriorShot());
    
    Photo aerial("ph7", property, "path.jpg", "", currentTime,
                "1920x1080", 1000000, false, "aerial", "Wide", true, "");
    EXPECT_FALSE(aerial.isExteriorShot());
    
    Photo drone("ph8", property, "path.jpg", "", currentTime,
               "1920x1080", 1000000, false, "drone", "Wide", true, "");
    EXPECT_FALSE(drone.isExteriorShot());
}

TEST_F(PhotoTest, IsInteriorShot) {
    Photo interior1("ph1", property, "path.jpg", "", currentTime,
                   "1920x1080", 1000000, false, "Living Room", "Wide", true, "");
    EXPECT_TRUE(interior1.isInteriorShot());
    
    Photo interior2("ph2", property, "path.jpg", "", currentTime,
                   "1920x1080", 1000000, false, "Kitchen", "Wide", true, "");
    EXPECT_TRUE(interior2.isInteriorShot());
    
    Photo interior3("ph3", property, "path.jpg", "", currentTime,
                   "1920x1080", 1000000, false, "Bedroom", "Wide", true, "");
    EXPECT_TRUE(interior3.isInteriorShot());
    
    Photo interior4("ph4", property, "path.jpg", "", currentTime,
                   "1920x1080", 1000000, false, "Bathroom", "Wide", true, "");
    EXPECT_TRUE(interior4.isInteriorShot());
    
    Photo exterior("ph5", property, "path.jpg", "", currentTime,
                  "1920x1080", 1000000, false, "exterior", "Wide", true, "");
    EXPECT_FALSE(exterior.isInteriorShot());
    
    Photo aerial("ph6", property, "path.jpg", "", currentTime,
                "1920x1080", 1000000, false, "aerial", "Wide", true, "");
    EXPECT_FALSE(aerial.isInteriorShot());
    
    Photo drone("ph7", property, "path.jpg", "", currentTime,
               "1920x1080", 1000000, false, "drone", "Wide", true, "");
    EXPECT_FALSE(drone.isInteriorShot());
}

TEST_F(PhotoTest, CanBeUsedInMarketing) {
    auto recentDate = currentTime - std::chrono::hours(24 * 10);
    auto oldDate = currentTime - std::chrono::hours(24 * 100);
    
    Photo goodPhoto("ph1", property, "path.jpg", "", recentDate,
                   "3840x2160", 1000000, true, "Living Room", "Wide", true, "John");
    EXPECT_TRUE(goodPhoto.canBeUsedInMarketing());
    
    Photo notApproved("ph2", property, "path.jpg", "", recentDate,
                     "3840x2160", 1000000, true, "Living Room", "Wide", false, "John");
    EXPECT_FALSE(notApproved.canBeUsedInMarketing());
    
    Photo lowRes("ph3", property, "path.jpg", "", recentDate,
                "640x480", 1000000, true, "Living Room", "Wide", true, "John");
    EXPECT_FALSE(lowRes.canBeUsedInMarketing());
    
    Photo oldPhoto("ph4", property, "path.jpg", "", oldDate,
                  "3840x2160", 1000000, true, "Living Room", "Wide", true, "John");
    EXPECT_FALSE(oldPhoto.canBeUsedInMarketing());
    
    Photo allBad("ph5", property, "path.jpg", "", oldDate,
                "640x480", 1000000, true, "Living Room", "Wide", false, "John");
    EXPECT_FALSE(allBad.canBeUsedInMarketing());
}

TEST_F(PhotoTest, CalculateAspectRatio) {
    Photo photo1("ph1", property, "path.jpg", "", currentTime,
                "1920x1080", 1000000, false, "Living Room", "Wide", true, "");
    EXPECT_NEAR(photo1.calculateAspectRatio(), 1920.0/1080.0, 0.001);
    
    Photo photo2("ph2", property, "path.jpg", "", currentTime,
                "3840x2160", 1000000, false, "Living Room", "Wide", true, "");
    EXPECT_NEAR(photo2.calculateAspectRatio(), 3840.0/2160.0, 0.001);
    
    Photo photo3("ph3", property, "path.jpg", "", currentTime,
                "800x600", 1000000, false, "Living Room", "Wide", true, "");
    EXPECT_NEAR(photo3.calculateAspectRatio(), 800.0/600.0, 0.001);
    
    Photo photo4("ph4", property, "path.jpg", "", currentTime,
                "1920x0", 1000000, false, "Living Room", "Wide", true, "");
    EXPECT_DOUBLE_EQ(photo4.calculateAspectRatio(), 0.0);
    
    Photo photo5("ph5", property, "path.jpg", "", currentTime,
                "invalid", 1000000, false, "Living Room", "Wide", true, "");
    EXPECT_DOUBLE_EQ(photo5.calculateAspectRatio(), 0.0);
    
    Photo photo7("ph7", property, "path.jpg", "", currentTime,
                "", 1000000, false, "Living Room", "Wide", true, "");
    EXPECT_DOUBLE_EQ(photo7.calculateAspectRatio(), 0.0);
}

TEST_F(PhotoTest, RequiresEditing) {
    Photo goodPhoto("ph1", property, "path.jpg", "", currentTime,
                   "3840x2160", 5242880, true, "Living Room", "Wide", true, "");
    EXPECT_FALSE(goodPhoto.requiresEditing());
    
    Photo notApproved("ph2", property, "path.jpg", "", currentTime,
                     "3840x2160", 5242880, true, "Living Room", "Wide", false, "");
    EXPECT_TRUE(notApproved.requiresEditing());
    
    Photo largeFile("ph3", property, "path.jpg", "", currentTime,
                   "3840x2160", 11534336, true, "Living Room", "Wide", true, "");
    EXPECT_TRUE(largeFile.requiresEditing());
    
    Photo lowRes("ph4", property, "path.jpg", "", currentTime,
                "640x480", 5242880, true, "Living Room", "Wide", true, "");
    EXPECT_TRUE(lowRes.requiresEditing());
    
    Photo multipleIssues("ph5", property, "path.jpg", "", currentTime,
                        "640x480", 11534336, true, "Living Room", "Wide", false, "");
    EXPECT_TRUE(multipleIssues.requiresEditing());
}

TEST_F(PhotoTest, GetPhotoQuality) {
    Photo lowQuality("ph1", property, "path.jpg", "", currentTime,
                    "640x480", 1048576, false, "Living Room", "Wide", false, "");
    EXPECT_EQ(lowQuality.getPhotoQuality(), "low");
    
    Photo premiumQuality("ph5", property, "path.jpg", "", currentTime,
                        "3840x2160", 5242880, true, "Living Room", "Wide", true, "");
    EXPECT_EQ(premiumQuality.getPhotoQuality(), "premium");
    
    Photo highResNotApproved("ph6", property, "path.jpg", "", currentTime,
                            "3840x2160", 5242880, true, "Living Room", "Wide", false, "");
    EXPECT_EQ(highResNotApproved.getPhotoQuality(), "high");
}

TEST_F(PhotoTest, CopyConstructor) {
    Photo original("ph1", property, "path.jpg", "Original Caption", currentTime,
                  "1920x1080", 1000000, true, "Living Room", "Wide", true, "Photographer");
    original.setIsPrimary(false);
    
    Photo copy(original);
    
    EXPECT_EQ(copy.getPhotoId(), "ph1");
    EXPECT_EQ(copy.getFilePath(), "path.jpg");
    EXPECT_EQ(copy.getCaption(), "Original Caption");
    EXPECT_EQ(copy.getResolution(), "1920x1080");
    EXPECT_EQ(copy.getFileSize(), 1000000);
    EXPECT_FALSE(copy.getIsPrimary());
    EXPECT_EQ(copy.getRoomType(), "Living Room");
    EXPECT_EQ(copy.getAngle(), "Wide");
    EXPECT_TRUE(copy.getIsApproved());
    EXPECT_EQ(copy.getPhotographer(), "Photographer");
    EXPECT_EQ(copy.getProperty(), property.get());
}

TEST_F(PhotoTest, CopyAssignment) {
    Photo original("ph1", property, "path1.jpg", "Caption 1", currentTime,
                  "1920x1080", 1000000, true, "Living Room", "Wide", true, "Photographer 1");
    
    auto property2 = std::make_shared<Property>("p2", "456 Elm St", 150.0);
    Photo copy("ph2", property2, "path2.jpg", "Caption 2", currentTime - std::chrono::hours(24),
              "640x480", 500000, false, "Kitchen", "Close", false, "Photographer 2");
    
    copy = original;
    
    EXPECT_EQ(copy.getPhotoId(), "ph1");
    EXPECT_EQ(copy.getFilePath(), "path1.jpg");
    EXPECT_EQ(copy.getCaption(), "Caption 1");
    EXPECT_EQ(copy.getResolution(), "1920x1080");
    EXPECT_EQ(copy.getFileSize(), 1000000);
    EXPECT_TRUE(copy.getIsPrimary());
    EXPECT_EQ(copy.getRoomType(), "Living Room");
    EXPECT_EQ(copy.getAngle(), "Wide");
    EXPECT_TRUE(copy.getIsApproved());
    EXPECT_EQ(copy.getPhotographer(), "Photographer 1");
    EXPECT_EQ(copy.getProperty(), property.get());
}

TEST_F(PhotoTest, MoveConstructor) {
    Photo original("ph1", property, "path.jpg", "Caption", currentTime,
                  "1920x1080", 1000000, true, "Living Room", "Wide", true, "Photographer");
    
    Photo moved(std::move(original));
    
    EXPECT_EQ(moved.getPhotoId(), "ph1");
    EXPECT_EQ(moved.getFilePath(), "path.jpg");
    EXPECT_EQ(moved.getCaption(), "Caption");
    EXPECT_EQ(moved.getResolution(), "1920x1080");
    EXPECT_EQ(moved.getFileSize(), 1000000);
    EXPECT_TRUE(moved.getIsPrimary());
    EXPECT_EQ(moved.getRoomType(), "Living Room");
    EXPECT_EQ(moved.getAngle(), "Wide");
    EXPECT_TRUE(moved.getIsApproved());
    EXPECT_EQ(moved.getPhotographer(), "Photographer");
    EXPECT_EQ(moved.getProperty(), property.get());
}

TEST_F(PhotoTest, MoveAssignment) {
    Photo original("ph1", property, "path1.jpg", "Caption 1", currentTime,
                  "1920x1080", 1000000, true, "Living Room", "Wide", true, "Photographer 1");
    
    auto property2 = std::make_shared<Property>("p2", "456 Elm St", 150.0);
    Photo moved("ph2", property2, "path2.jpg", "Caption 2", currentTime - std::chrono::hours(24),
               "640x480", 500000, false, "Kitchen", "Close", false, "Photographer 2");
    
    moved = std::move(original);
    
    EXPECT_EQ(moved.getPhotoId(), "ph1");
    EXPECT_EQ(moved.getFilePath(), "path1.jpg");
    EXPECT_EQ(moved.getCaption(), "Caption 1");
    EXPECT_EQ(moved.getResolution(), "1920x1080");
    EXPECT_EQ(moved.getFileSize(), 1000000);
    EXPECT_TRUE(moved.getIsPrimary());
    EXPECT_EQ(moved.getRoomType(), "Living Room");
    EXPECT_EQ(moved.getAngle(), "Wide");
    EXPECT_TRUE(moved.getIsApproved());
    EXPECT_EQ(moved.getPhotographer(), "Photographer 1");
    EXPECT_EQ(moved.getProperty(), property.get());
}

TEST_F(PhotoTest, PropertyMethods) {
    Photo photo("ph1", property, "path.jpg", "Caption", currentTime,
               "1920x1080", 1000000, true, "Living Room", "Wide", true, "Photographer");
    
    EXPECT_EQ(photo.getProperty(), property.get());
    
    Photo photoWithoutProperty("ph2", nullptr, "path.jpg", "Caption", currentTime,
                              "1920x1080", 1000000, true, "Living Room", "Wide", true, "Photographer");
    EXPECT_EQ(photoWithoutProperty.getProperty(), nullptr);
}

TEST_F(PhotoTest, EdgeCases) {
    Photo emptyStrings("ph1", property, "", "", currentTime,
                      "", 0, false, "", "", false, "");
    
    EXPECT_EQ(emptyStrings.getPhotoId(), "ph1");
    EXPECT_EQ(emptyStrings.getFilePath(), "");
    EXPECT_EQ(emptyStrings.getCaption(), "");
    EXPECT_EQ(emptyStrings.getResolution(), "");
    EXPECT_EQ(emptyStrings.getFileSize(), 0);
    EXPECT_FALSE(emptyStrings.getIsPrimary());
    EXPECT_EQ(emptyStrings.getRoomType(), "");
    EXPECT_EQ(emptyStrings.getAngle(), "");
    EXPECT_FALSE(emptyStrings.getIsApproved());
    EXPECT_EQ(emptyStrings.getPhotographer(), "");
    
    EXPECT_FALSE(emptyStrings.isHighResolution());
    EXPECT_EQ(emptyStrings.calculateFileSizeMB(), 0);
    EXPECT_DOUBLE_EQ(emptyStrings.calculateAspectRatio(), 0.0);
    EXPECT_FALSE(emptyStrings.isExteriorShot());
    EXPECT_TRUE(emptyStrings.isInteriorShot());
    EXPECT_FALSE(emptyStrings.canBeUsedInMarketing());
    EXPECT_TRUE(emptyStrings.requiresEditing());
    EXPECT_EQ(emptyStrings.getPhotoQuality(), "low");
}

TEST_F(PhotoTest, FileSizeEdgeCases) {
    Photo negativeFileSize("ph1", property, "path.jpg", "", currentTime,
                          "1920x1080", -1000, false, "Living Room", "Wide", true, "");
    EXPECT_EQ(negativeFileSize.calculateFileSizeMB(), -1000 / (1024 * 1024));
    
    Photo veryLargeFile("ph2", property, "path.jpg", "", currentTime,
                       "1920x1080", 1073741824, false, "Living Room", "Wide", true, "");
    EXPECT_EQ(veryLargeFile.calculateFileSizeMB(), 1024);
}