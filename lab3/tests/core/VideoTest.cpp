#include <gtest/gtest.h>
#include <memory>
#include <chrono>
#include "../../include/core/Video.h"
#include "../../include/core/Property.h"
#include "../../include/core/Address.h"
#include "../../include/core/PropertyStatus.h"
#include "../../include/core/PropertyType.h"

class VideoTest : public ::testing::Test {
protected:
    void SetUp() override {
        auto address = std::make_unique<Address>("123 Main St", "City", "State", "12345", "Country", 40.7128, -74.0060);
        auto status = std::make_unique<PropertyStatus>("AVAILABLE", "Available", true, false, false, 
                                                      std::chrono::system_clock::now(), "New listing", 0.0, 15);
        auto type = std::make_unique<PropertyType>("APARTMENT", "Apartment", "Residential", 2, 100, 
                                                  800.0, 2000.0, false, "residential", 1.0);
        
        property = std::make_shared<Property>("PROP001", 85.0, 1500.0, std::move(address), 
                                             std::move(status), std::move(type));
        
        currentTime = std::chrono::system_clock::now();
    }
    
    std::shared_ptr<Property> property;
    std::chrono::system_clock::time_point currentTime;
};

TEST_F(VideoTest, ConstructorAndGetters) {
    Video video("VID001", property, "/videos/tour.mp4", "Property Tour", 
               "Complete walkthrough of the property", currentTime, 300.0, 
               "1920x1080", 2147483648, true, "walkthrough", true, 
               "/thumbnails/tour.jpg");
    
    EXPECT_EQ(video.getVideoId(), "VID001");
    EXPECT_EQ(video.getProperty(), property.get());
    EXPECT_EQ(video.getFilePath(), "/videos/tour.mp4");
    EXPECT_EQ(video.getTitle(), "Property Tour");
    EXPECT_EQ(video.getDescription(), "Complete walkthrough of the property");
    EXPECT_EQ(video.getRecordedDate(), currentTime);
    EXPECT_DOUBLE_EQ(video.getDuration(), 300.0);
    EXPECT_EQ(video.getResolution(), "1920x1080");
    EXPECT_EQ(video.getFileSize(), 2147483648);
    EXPECT_TRUE(video.getHasAudio());
    EXPECT_EQ(video.getVideoType(), "walkthrough");
    EXPECT_TRUE(video.getIsVirtualTour());
    EXPECT_EQ(video.getThumbnailPath(), "/thumbnails/tour.jpg");
}

TEST_F(VideoTest, Setters) {
    Video video("VID001", property, "/videos/tour.mp4", "Original Title", 
               "Original Description", currentTime, 300.0, "1920x1080", 
               1073741824, true, "walkthrough", true, "/thumbnails/original.jpg");
    
    video.setTitle("Updated Title");
    EXPECT_EQ(video.getTitle(), "Updated Title");
    
    video.setDescription("Updated Description");
    EXPECT_EQ(video.getDescription(), "Updated Description");
    
    video.setThumbnailPath("/thumbnails/updated.jpg");
    EXPECT_EQ(video.getThumbnailPath(), "/thumbnails/updated.jpg");
    
    video.setTitle("");
    EXPECT_EQ(video.getTitle(), "");
    
    video.setDescription("");
    EXPECT_EQ(video.getDescription(), "");
    
    video.setThumbnailPath("");
    EXPECT_EQ(video.getThumbnailPath(), "");
}

TEST_F(VideoTest, IsHDVideo) {
    Video hdVideo("VID001", property, "/videos/hd.mp4", "HD Video", "Description",
                 currentTime, 300.0, "1920x1080", 1073741824, true, "walkthrough", 
                 false, "/thumbnails/hd.jpg");
    EXPECT_TRUE(hdVideo.isHDVideo());
    
    Video fullHDVideo("VID002", property, "/videos/fullhd.mp4", "Full HD", "Description",
                     currentTime, 300.0, "1920x1080", 2147483648, true, "walkthrough", 
                     false, "/thumbnails/fullhd.jpg");
    EXPECT_TRUE(fullHDVideo.isHDVideo());
    
    Video hd720Video("VID003", property, "/videos/720p.mp4", "720p", "Description",
                    currentTime, 300.0, "1280x720", 536870912, true, "walkthrough", 
                    false, "/thumbnails/720p.jpg");
    EXPECT_TRUE(hd720Video.isHDVideo());
    
    Video hdTextVideo("VID004", property, "/videos/hdtext.mp4", "HD Text", "Description",
                     currentTime, 300.0, "HD", 1073741824, true, "walkthrough", 
                     false, "/thumbnails/hdtext.jpg");
    EXPECT_TRUE(hdTextVideo.isHDVideo());
    
    Video sdVideo("VID005", property, "/videos/sd.mp4", "SD Video", "Description",
                 currentTime, 300.0, "640x480", 268435456, true, "walkthrough", 
                 false, "/thumbnails/sd.jpg");
    EXPECT_FALSE(sdVideo.isHDVideo());
    
    Video lowResVideo("VID006", property, "/videos/low.mp4", "Low Res", "Description",
                     currentTime, 300.0, "320x240", 134217728, true, "walkthrough", 
                     false, "/thumbnails/low.jpg");
    EXPECT_FALSE(lowResVideo.isHDVideo());
    
    Video emptyResVideo("VID007", property, "/videos/empty.mp4", "Empty", "Description",
                       currentTime, 300.0, "", 1073741824, true, "walkthrough", 
                       false, "/thumbnails/empty.jpg");
    EXPECT_FALSE(emptyResVideo.isHDVideo());
}

TEST_F(VideoTest, CalculateFileSizeGB) {
    Video smallVideo("VID001", property, "/videos/small.mp4", "Small", "Description",
                    currentTime, 300.0, "1920x1080", 1073741824, true, "walkthrough", 
                    false, "/thumbnails/small.jpg");
    EXPECT_EQ(smallVideo.calculateFileSizeGB(), 1);
    
    Video largeVideo("VID002", property, "/videos/large.mp4", "Large", "Description",
                    currentTime, 300.0, "1920x1080", 3221225472, true, "walkthrough", 
                    false, "/thumbnails/large.jpg");
    EXPECT_EQ(largeVideo.calculateFileSizeGB(), 3);
    
    Video exactGBVideo("VID003", property, "/videos/exact.mp4", "Exact", "Description",
                      currentTime, 300.0, "1920x1080", 1073741824, true, "walkthrough", 
                      false, "/thumbnails/exact.jpg");
    EXPECT_EQ(exactGBVideo.calculateFileSizeGB(), 1);
    
    Video zeroSizeVideo("VID004", property, "/videos/zero.mp4", "Zero", "Description",
                       currentTime, 300.0, "1920x1080", 0, true, "walkthrough", 
                       false, "/thumbnails/zero.jpg");
    EXPECT_EQ(zeroSizeVideo.calculateFileSizeGB(), 0);
    
    Video negativeSizeVideo("VID005", property, "/videos/negative.mp4", "Negative", "Description",
                           currentTime, 300.0, "1920x1080", -1073741824, true, "walkthrough", 
                           false, "/thumbnails/negative.jpg");
    EXPECT_EQ(negativeSizeVideo.calculateFileSizeGB(), -1);
    
    Video partialGBVideo("VID006", property, "/videos/partial.mp4", "Partial", "Description",
                        currentTime, 300.0, "1920x1080", 536870912, true, "walkthrough", 
                        false, "/thumbnails/partial.jpg");
    EXPECT_EQ(partialGBVideo.calculateFileSizeGB(), 0);
    
    Video justUnderGBVideo("VID007", property, "/videos/justunder.mp4", "Just Under", "Description",
                          currentTime, 300.0, "1920x1080", 1073741823, true, "walkthrough", 
                          false, "/thumbnails/justunder.jpg");
    EXPECT_EQ(justUnderGBVideo.calculateFileSizeGB(), 0);
}

TEST_F(VideoTest, IsRecentVideo) {
    auto recentTime = currentTime - std::chrono::hours(24 * 30);
    auto oldTime = currentTime - std::chrono::hours(24 * 200);
    
    Video recentVideo("VID001", property, "/videos/recent.mp4", "Recent", "Description",
                     recentTime, 300.0, "1920x1080", 1073741824, true, "walkthrough", 
                     false, "/thumbnails/recent.jpg");
    EXPECT_TRUE(recentVideo.isRecentVideo(currentTime));
    
    Video oldVideo("VID002", property, "/videos/old.mp4", "Old", "Description",
                  oldTime, 300.0, "1920x1080", 1073741824, true, "walkthrough", 
                  false, "/thumbnails/old.jpg");
    EXPECT_FALSE(oldVideo.isRecentVideo(currentTime));
    
    Video boundaryRecentVideo("VID003", property, "/videos/boundary.mp4", "Boundary", "Description",
                            currentTime - std::chrono::hours(24 * 179), 300.0, "1920x1080", 
                            1073741824, true, "walkthrough", false, "/thumbnails/boundary.jpg");
    EXPECT_TRUE(boundaryRecentVideo.isRecentVideo(currentTime));
    
    Video boundaryOldVideo("VID004", property, "/videos/boundary2.mp4", "Boundary2", "Description",
                          currentTime - std::chrono::hours(24 * 180), 300.0, "1920x1080", 
                          1073741824, true, "walkthrough", false, "/thumbnails/boundary2.jpg");
    EXPECT_FALSE(boundaryOldVideo.isRecentVideo(currentTime));
    
    Video justRecordedVideo("VID005", property, "/videos/just.mp4", "Just Recorded", "Description",
                           currentTime, 300.0, "1920x1080", 1073741824, true, "walkthrough", 
                           false, "/thumbnails/just.jpg");
    EXPECT_TRUE(justRecordedVideo.isRecentVideo(currentTime));
    
    Video futureVideo("VID006", property, "/videos/future.mp4", "Future", "Description",
                     currentTime + std::chrono::hours(24 * 10), 300.0, "1920x1080", 
                     1073741824, true, "walkthrough", false, "/thumbnails/future.jpg");
    EXPECT_TRUE(futureVideo.isRecentVideo(currentTime));
}

TEST_F(VideoTest, IsWalkthrough) {
    Video walkthroughVideo("VID001", property, "/videos/walkthrough.mp4", "Walkthrough", "Description",
                          currentTime, 300.0, "1920x1080", 1073741824, true, "walkthrough", 
                          false, "/thumbnails/walkthrough.jpg");
    EXPECT_TRUE(walkthroughVideo.isWalkthrough());
    
    Video tourVideo("VID002", property, "/videos/tour.mp4", "Tour", "Description",
                   currentTime, 300.0, "1920x1080", 1073741824, true, "tour", 
                   false, "/thumbnails/tour.jpg");
    EXPECT_TRUE(tourVideo.isWalkthrough());
    
    Video virtualTourVideo("VID003", property, "/videos/virtual.mp4", "Virtual", "Description",
                          currentTime, 300.0, "1920x1080", 1073741824, true, "other", 
                          true, "/thumbnails/virtual.jpg");
    EXPECT_TRUE(virtualTourVideo.isWalkthrough());
    
    Video otherVideo("VID004", property, "/videos/other.mp4", "Other", "Description",
                    currentTime, 300.0, "1920x1080", 1073741824, true, "other", 
                    false, "/thumbnails/other.jpg");
    EXPECT_FALSE(otherVideo.isWalkthrough());
    
    Video emptyTypeVideo("VID005", property, "/videos/empty.mp4", "Empty", "Description",
                        currentTime, 300.0, "1920x1080", 1073741824, true, "", 
                        false, "/thumbnails/empty.jpg");
    EXPECT_FALSE(emptyTypeVideo.isWalkthrough());
    
    Video promotionalVideo("VID006", property, "/videos/promo.mp4", "Promo", "Description",
                          currentTime, 300.0, "1920x1080", 1073741824, true, "promotional", 
                          false, "/thumbnails/promo.jpg");
    EXPECT_FALSE(promotionalVideo.isWalkthrough());
}

TEST_F(VideoTest, IsPromotional) {
    Video promotionalVideo("VID001", property, "/videos/promotional.mp4", "Promotional", "Description",
                          currentTime, 300.0, "1920x1080", 1073741824, true, "promotional", 
                          false, "/thumbnails/promotional.jpg");
    EXPECT_TRUE(promotionalVideo.isPromotional());
    
    Video marketingVideo("VID002", property, "/videos/marketing.mp4", "Marketing", "Description",
                        currentTime, 300.0, "1920x1080", 1073741824, true, "marketing", 
                        false, "/thumbnails/marketing.jpg");
    EXPECT_TRUE(marketingVideo.isPromotional());
    
    Video commercialVideo("VID003", property, "/videos/commercial.mp4", "Commercial", "Description",
                         currentTime, 300.0, "1920x1080", 1073741824, true, "commercial", 
                         false, "/thumbnails/commercial.jpg");
    EXPECT_TRUE(commercialVideo.isPromotional());
    
    Video walkthroughVideo("VID004", property, "/videos/walkthrough.mp4", "Walkthrough", "Description",
                          currentTime, 300.0, "1920x1080", 1073741824, true, "walkthrough", 
                          false, "/thumbnails/walkthrough.jpg");
    EXPECT_FALSE(walkthroughVideo.isPromotional());
    
    Video otherVideo("VID005", property, "/videos/other.mp4", "Other", "Description",
                    currentTime, 300.0, "1920x1080", 1073741824, true, "other", 
                    false, "/thumbnails/other.jpg");
    EXPECT_FALSE(otherVideo.isPromotional());
    
    Video emptyTypeVideo("VID006", property, "/videos/empty.mp4", "Empty", "Description",
                        currentTime, 300.0, "1920x1080", 1073741824, true, "", 
                        false, "/thumbnails/empty.jpg");
    EXPECT_FALSE(emptyTypeVideo.isPromotional());
}

TEST_F(VideoTest, CalculateBitrate) {
    Video video1("VID001", property, "/videos/video1.mp4", "Video 1", "Description",
                currentTime, 300.0, "1920x1080", 1073741824, true, "walkthrough", 
                false, "/thumbnails/video1.jpg");
    double expectedBitrate1 = (1073741824.0 * 8.0) / (300.0 * 1000.0);
    EXPECT_DOUBLE_EQ(video1.calculateBitrate(), expectedBitrate1);
    
    Video video2("VID002", property, "/videos/video2.mp4", "Video 2", "Description",
                currentTime, 600.0, "1920x1080", 2147483648, true, "walkthrough", 
                false, "/thumbnails/video2.jpg");
    double expectedBitrate2 = (2147483648 * 8) / (600.0 * 1000);
    EXPECT_DOUBLE_EQ(video2.calculateBitrate(), expectedBitrate2);
    
    Video zeroDurationVideo("VID003", property, "/videos/zero.mp4", "Zero Duration", "Description",
                           currentTime, 0.0, "1920x1080", 1073741824, true, "walkthrough", 
                           false, "/thumbnails/zero.jpg");
    EXPECT_DOUBLE_EQ(zeroDurationVideo.calculateBitrate(), 0.0);
    
    Video negativeDurationVideo("VID004", property, "/videos/negative.mp4", "Negative Duration", "Description",
                               currentTime, -300.0, "1920x1080", 1073741824, true, "walkthrough", 
                               false, "/thumbnails/negative.jpg");
    EXPECT_DOUBLE_EQ(negativeDurationVideo.calculateBitrate(), 0.0);
    
    Video zeroSizeVideo("VID005", property, "/videos/zerosize.mp4", "Zero Size", "Description",
                       currentTime, 300.0, "1920x1080", 0, true, "walkthrough", 
                       false, "/thumbnails/zerosize.jpg");
    EXPECT_DOUBLE_EQ(zeroSizeVideo.calculateBitrate(), 0.0);
        
    Video shortVideo("VID007", property, "/videos/short.mp4", "Short", "Description",
                    currentTime, 10.0, "1920x1080", 10485760, true, "walkthrough", 
                    false, "/thumbnails/short.jpg");
    double expectedBitrate3 = (10485760 * 8) / (10.0 * 1000);
    EXPECT_DOUBLE_EQ(shortVideo.calculateBitrate(), expectedBitrate3);
}

TEST_F(VideoTest, RequiresCompression) {
    Video largeVideo("VID001", property, "/videos/large.mp4", "Large", "Description",
                    currentTime, 300.0, "1920x1080", 3221225472, true, "walkthrough", 
                    false, "/thumbnails/large.jpg");
    EXPECT_TRUE(largeVideo.requiresCompression());
    
    Video highBitrateVideo("VID002", property, "/videos/highbitrate.mp4", "High Bitrate", "Description",
                          currentTime, 300.0, "1920x1080", 3758096384, true, "walkthrough", 
                          false, "/thumbnails/highbitrate.jpg");
    EXPECT_TRUE(highBitrateVideo.requiresCompression());
    
    Video smallVideo("VID003", property, "/videos/small.mp4", "Small", "Description",
                    currentTime, 300.0, "1920x1080", 536870912, true, "walkthrough", 
                    false, "/thumbnails/small.jpg");
    EXPECT_TRUE(smallVideo.requiresCompression());
    
    Video exactSizeVideo("VID004", property, "/videos/exact.mp4", "Exact", "Description",
                        currentTime, 300.0, "1920x1080", 1073741824, true, "walkthrough", 
                        false, "/thumbnails/exact.jpg");
    EXPECT_TRUE(exactSizeVideo.requiresCompression());
    
    Video lowBitrateVideo("VID005", property, "/videos/lowbitrate.mp4", "Low Bitrate", "Description",
                         currentTime, 600.0, "1920x1080", 1073741824, true, "walkthrough", 
                         false, "/thumbnails/lowbitrate.jpg");
    EXPECT_TRUE(lowBitrateVideo.requiresCompression());
    
    Video zeroSizeVideo("VID006", property, "/videos/zero.mp4", "Zero", "Description",
                       currentTime, 300.0, "1920x1080", 0, true, "walkthrough", 
                       false, "/thumbnails/zero.jpg");
    EXPECT_FALSE(zeroSizeVideo.requiresCompression());
    
    Video zeroDurationVideo("VID007", property, "/videos/zeroduration.mp4", "Zero Duration", "Description",
                           currentTime, 0.0, "1920x1080", 1073741824, true, "walkthrough", 
                           false, "/thumbnails/zeroduration.jpg");
    EXPECT_FALSE(zeroDurationVideo.requiresCompression());
}

TEST_F(VideoTest, CanBeStreamed) {
    Video streamableVideo("VID001", property, "/videos/streamable.mp4", "Streamable", "Description",
                         currentTime, 300.0, "1920x1080", 536870912, true, "walkthrough", 
                         false, "/thumbnails/streamable.jpg");
    EXPECT_FALSE(streamableVideo.canBeStreamed());
    
    Video longVideo("VID002", property, "/videos/long.mp4", "Long", "Description",
                   currentTime, 601.0, "1920x1080", 536870912, true, "walkthrough", 
                   false, "/thumbnails/long.jpg");
    EXPECT_FALSE(longVideo.canBeStreamed());
    
    Video highBitrateVideo("VID003", property, "/videos/highbitrate.mp4", "High Bitrate", "Description",
                          currentTime, 300.0, "1920x1080", 3221225472, true, "walkthrough", 
                          false, "/thumbnails/highbitrate.jpg");
    EXPECT_FALSE(highBitrateVideo.canBeStreamed());
    
    Video exactDurationVideo("VID004", property, "/videos/exactduration.mp4", "Exact Duration", "Description",
                            currentTime, 600.0, "1920x1080", 536870912, true, "walkthrough", 
                            false, "/thumbnails/exactduration.jpg");
    EXPECT_FALSE(exactDurationVideo.canBeStreamed());
    
    Video exactBitrateVideo("VID005", property, "/videos/exactbitrate.mp4", "Exact Bitrate", "Description",
                           currentTime, 300.0, "1920x1080", 1875000000, true, "walkthrough", 
                           false, "/thumbnails/exactbitrate.jpg");
    EXPECT_FALSE(exactBitrateVideo.canBeStreamed());
    
    Video zeroDurationVideo("VID006", property, "/videos/zeroduration.mp4", "Zero Duration", "Description",
                           currentTime, 0.0, "1920x1080", 0, true, "walkthrough", 
                           false, "/thumbnails/zeroduration.jpg");
    EXPECT_TRUE(zeroDurationVideo.canBeStreamed());
    
    Video negativeDurationVideo("VID007", property, "/videos/negativeduration.mp4", "Negative Duration", "Description",
                               currentTime, -100.0, "1920x1080", 536870912, true, "walkthrough", 
                               false, "/thumbnails/negativeduration.jpg");
    EXPECT_TRUE(negativeDurationVideo.canBeStreamed());
}

TEST_F(VideoTest, CalculateStorageCost) {
    Video video("VID001", property, "/videos/video.mp4", "Video", "Description",
               currentTime, 300.0, "1920x1080", 1073741824, true, "walkthrough", 
               false, "/thumbnails/video.jpg");
    
    EXPECT_DOUBLE_EQ(video.calculateStorageCost(0.10), 0.1);
    EXPECT_DOUBLE_EQ(video.calculateStorageCost(0.50), 0.5);
    EXPECT_DOUBLE_EQ(video.calculateStorageCost(1.0), 1.0);
    EXPECT_DOUBLE_EQ(video.calculateStorageCost(2.0), 2.0);
    EXPECT_DOUBLE_EQ(video.calculateStorageCost(0.0), 0.0);
    EXPECT_DOUBLE_EQ(video.calculateStorageCost(-1.0), -1.0);
    
    Video largeVideo("VID002", property, "/videos/large.mp4", "Large", "Description",
                    currentTime, 300.0, "1920x1080", 3221225472, true, "walkthrough", 
                    false, "/thumbnails/large.jpg");
    
    EXPECT_DOUBLE_EQ(largeVideo.calculateStorageCost(0.10), 0.3);
    EXPECT_DOUBLE_EQ(largeVideo.calculateStorageCost(0.25), 0.75);
}

TEST_F(VideoTest, GetVideoQuality) {
    Video standardVideo("VID001", property, "/videos/standard.mp4", "Standard", "Description",
                       currentTime, 300.0, "640x480", 268435456, true, "walkthrough", 
                       false, "/thumbnails/standard.jpg");
    EXPECT_EQ(standardVideo.getVideoQuality(), "standard");
    
    Video hdVideo("VID002", property, "/videos/hd.mp4", "HD", "Description",
                 currentTime, 300.0, "1280x720", 536870912, true, "walkthrough", 
                 false, "/thumbnails/hd.jpg");
    EXPECT_EQ(hdVideo.getVideoQuality(), "ultra_hd");
    
    Video fullHDVideo("VID003", property, "/videos/fullhd.mp4", "Full HD", "Description",
                     currentTime, 300.0, "1920x1080", 1073741824, true, "walkthrough", 
                     false, "/thumbnails/fullhd.jpg");
    EXPECT_EQ(fullHDVideo.getVideoQuality(), "ultra_hd");
    
    Video ultraHDVideo("VID004", property, "/videos/ultrahd.mp4", "Ultra HD", "Description",
                      currentTime, 300.0, "1920x1080", 3221225472, true, "walkthrough", 
                      false, "/thumbnails/ultrahd.jpg");
    EXPECT_EQ(ultraHDVideo.getVideoQuality(), "ultra_hd");
    
    Video ultraHDVideo2("VID005", property, "/videos/ultrahd2.mp4", "Ultra HD 2", "Description",
                       currentTime, 300.0, "3840x2160", 2147483648, true, "walkthrough", 
                       false, "/thumbnails/ultrahd2.jpg");
    EXPECT_EQ(ultraHDVideo2.getVideoQuality(), "standard");
    
    Video lowBitrateHDVideo("VID006", property, "/videos/lowbitratehd.mp4", "Low Bitrate HD", "Description",
                           currentTime, 600.0, "1920x1080", 1073741824, true, "walkthrough", 
                           false, "/thumbnails/lowbitratehd.jpg");
    EXPECT_EQ(lowBitrateHDVideo.getVideoQuality(), "ultra_hd");
    
    Video emptyResVideo("VID007", property, "/videos/empty.mp4", "Empty", "Description",
                       currentTime, 300.0, "", 1073741824, true, "walkthrough", 
                       false, "/thumbnails/empty.jpg");
    EXPECT_EQ(emptyResVideo.getVideoQuality(), "standard");
}

TEST_F(VideoTest, CopyConstructor) {
    Video original("VID001", property, "/videos/original.mp4", "Original", "Description",
                  currentTime, 300.0, "1920x1080", 1073741824, true, "walkthrough", 
                  true, "/thumbnails/original.jpg");
    
    original.setTitle("Updated Title");
    original.setDescription("Updated Description");
    
    Video copy(original);
    
    EXPECT_EQ(copy.getVideoId(), "VID001");
    EXPECT_EQ(copy.getProperty(), property.get());
    EXPECT_EQ(copy.getFilePath(), "/videos/original.mp4");
    EXPECT_EQ(copy.getTitle(), "Updated Title");
    EXPECT_EQ(copy.getDescription(), "Updated Description");
    EXPECT_EQ(copy.getRecordedDate(), currentTime);
    EXPECT_DOUBLE_EQ(copy.getDuration(), 300.0);
    EXPECT_EQ(copy.getResolution(), "1920x1080");
    EXPECT_EQ(copy.getFileSize(), 1073741824);
    EXPECT_TRUE(copy.getHasAudio());
    EXPECT_EQ(copy.getVideoType(), "walkthrough");
    EXPECT_TRUE(copy.getIsVirtualTour());
    EXPECT_EQ(copy.getThumbnailPath(), "/thumbnails/original.jpg");
}

TEST_F(VideoTest, CopyAssignment) {
    Video original("VID001", property, "/videos/original.mp4", "Original", "Description",
                  currentTime, 300.0, "1920x1080", 1073741824, true, "walkthrough", 
                  true, "/thumbnails/original.jpg");
    
    Video assigned("VID002", nullptr, "/videos/assigned.mp4", "Assigned", "Description",
                  currentTime - std::chrono::hours(24*100), 600.0, "640x480", 
                  268435456, false, "promotional", false, "/thumbnails/assigned.jpg");
    
    assigned = original;
    
    EXPECT_EQ(assigned.getVideoId(), "VID001");
    EXPECT_EQ(assigned.getProperty(), property.get());
    EXPECT_EQ(assigned.getFilePath(), "/videos/original.mp4");
    EXPECT_EQ(assigned.getTitle(), "Original");
    EXPECT_EQ(assigned.getDescription(), "Description");
    EXPECT_EQ(assigned.getRecordedDate(), currentTime);
    EXPECT_DOUBLE_EQ(assigned.getDuration(), 300.0);
    EXPECT_EQ(assigned.getResolution(), "1920x1080");
    EXPECT_EQ(assigned.getFileSize(), 1073741824);
    EXPECT_TRUE(assigned.getHasAudio());
    EXPECT_EQ(assigned.getVideoType(), "walkthrough");
    EXPECT_TRUE(assigned.getIsVirtualTour());
    EXPECT_EQ(assigned.getThumbnailPath(), "/thumbnails/original.jpg");
}

TEST_F(VideoTest, MoveConstructor) {
    Video original("VID001", property, "/videos/original.mp4", "Original", "Description",
                  currentTime, 300.0, "1920x1080", 1073741824, true, "walkthrough", 
                  true, "/thumbnails/original.jpg");
    
    Video moved(std::move(original));
    
    EXPECT_EQ(moved.getVideoId(), "VID001");
    EXPECT_EQ(moved.getProperty(), property.get());
    EXPECT_EQ(moved.getFilePath(), "/videos/original.mp4");
    EXPECT_EQ(moved.getTitle(), "Original");
    EXPECT_EQ(moved.getDescription(), "Description");
    EXPECT_EQ(moved.getRecordedDate(), currentTime);
    EXPECT_DOUBLE_EQ(moved.getDuration(), 300.0);
    EXPECT_EQ(moved.getResolution(), "1920x1080");
    EXPECT_EQ(moved.getFileSize(), 1073741824);
    EXPECT_TRUE(moved.getHasAudio());
    EXPECT_EQ(moved.getVideoType(), "walkthrough");
    EXPECT_TRUE(moved.getIsVirtualTour());
    EXPECT_EQ(moved.getThumbnailPath(), "/thumbnails/original.jpg");
}

TEST_F(VideoTest, MoveAssignment) {
    Video original("VID001", property, "/videos/original.mp4", "Original", "Description",
                  currentTime, 300.0, "1920x1080", 1073741824, true, "walkthrough", 
                  true, "/thumbnails/original.jpg");
    
    Video moved("VID002", nullptr, "/videos/moved.mp4", "Moved", "Description",
               currentTime - std::chrono::hours(24*100), 600.0, "640x480", 
               268435456, false, "promotional", false, "/thumbnails/moved.jpg");
    
    moved = std::move(original);
    
    EXPECT_EQ(moved.getVideoId(), "VID001");
    EXPECT_EQ(moved.getProperty(), property.get());
    EXPECT_EQ(moved.getFilePath(), "/videos/original.mp4");
    EXPECT_EQ(moved.getTitle(), "Original");
    EXPECT_EQ(moved.getDescription(), "Description");
    EXPECT_EQ(moved.getRecordedDate(), currentTime);
    EXPECT_DOUBLE_EQ(moved.getDuration(), 300.0);
    EXPECT_EQ(moved.getResolution(), "1920x1080");
    EXPECT_EQ(moved.getFileSize(), 1073741824);
    EXPECT_TRUE(moved.getHasAudio());
    EXPECT_EQ(moved.getVideoType(), "walkthrough");
    EXPECT_TRUE(moved.getIsVirtualTour());
    EXPECT_EQ(moved.getThumbnailPath(), "/thumbnails/original.jpg");
}

TEST_F(VideoTest, SelfAssignment) {
    Video video("VID001", property, "/videos/video.mp4", "Video", "Description",
               currentTime, 300.0, "1920x1080", 1073741824, true, "walkthrough", 
               true, "/thumbnails/video.jpg");
    
    video.setTitle("Updated Title");
    video.setDescription("Updated Description");
    
    video = video;
    
    EXPECT_EQ(video.getVideoId(), "VID001");
    EXPECT_EQ(video.getProperty(), property.get());
    EXPECT_EQ(video.getFilePath(), "/videos/video.mp4");
    EXPECT_EQ(video.getTitle(), "Updated Title");
    EXPECT_EQ(video.getDescription(), "Updated Description");
    EXPECT_EQ(video.getRecordedDate(), currentTime);
    EXPECT_DOUBLE_EQ(video.getDuration(), 300.0);
    EXPECT_EQ(video.getResolution(), "1920x1080");
    EXPECT_EQ(video.getFileSize(), 1073741824);
    EXPECT_TRUE(video.getHasAudio());
    EXPECT_EQ(video.getVideoType(), "walkthrough");
    EXPECT_TRUE(video.getIsVirtualTour());
    EXPECT_EQ(video.getThumbnailPath(), "/thumbnails/video.jpg");
}

TEST_F(VideoTest, EdgeCases) {
    Video emptyVideo("", nullptr, "", "", "", std::chrono::system_clock::time_point(), 
                    0.0, "", 0, false, "", false, "");
    
    EXPECT_EQ(emptyVideo.getVideoId(), "");
    EXPECT_EQ(emptyVideo.getProperty(), nullptr);
    EXPECT_EQ(emptyVideo.getFilePath(), "");
    EXPECT_EQ(emptyVideo.getTitle(), "");
    EXPECT_EQ(emptyVideo.getDescription(), "");
    EXPECT_EQ(emptyVideo.getDuration(), 0.0);
    EXPECT_EQ(emptyVideo.getResolution(), "");
    EXPECT_EQ(emptyVideo.getFileSize(), 0);
    EXPECT_FALSE(emptyVideo.getHasAudio());
    EXPECT_EQ(emptyVideo.getVideoType(), "");
    EXPECT_FALSE(emptyVideo.getIsVirtualTour());
    EXPECT_EQ(emptyVideo.getThumbnailPath(), "");
    
    EXPECT_FALSE(emptyVideo.isHDVideo());
    EXPECT_EQ(emptyVideo.calculateFileSizeGB(), 0);
    EXPECT_FALSE(emptyVideo.isRecentVideo(currentTime));
    EXPECT_FALSE(emptyVideo.isWalkthrough());
    EXPECT_FALSE(emptyVideo.isPromotional());
    EXPECT_DOUBLE_EQ(emptyVideo.calculateBitrate(), 0.0);
    EXPECT_FALSE(emptyVideo.requiresCompression());
    EXPECT_TRUE(emptyVideo.canBeStreamed());
    EXPECT_DOUBLE_EQ(emptyVideo.calculateStorageCost(1.0), 0.0);
    EXPECT_EQ(emptyVideo.getVideoQuality(), "standard");
}

TEST_F(VideoTest, MixedFunctionalMethods) {
    Video video("VID001", property, "/videos/mixed.mp4", "Mixed", "Description",
               currentTime - std::chrono::hours(24*50), 450.0, "1920x1080", 
               2147483648, true, "walkthrough", true, "/thumbnails/mixed.jpg");
    
    EXPECT_TRUE(video.isHDVideo());
    EXPECT_EQ(video.calculateFileSizeGB(), 2);
    EXPECT_TRUE(video.isRecentVideo(currentTime));
    EXPECT_TRUE(video.isWalkthrough());
    EXPECT_FALSE(video.isPromotional());
    
    double expectedBitrate = (2147483648 * 8) / (450.0 * 1000);
    EXPECT_DOUBLE_EQ(video.calculateBitrate(), expectedBitrate);
    
    EXPECT_TRUE(video.requiresCompression());
    EXPECT_FALSE(video.canBeStreamed());
    
    EXPECT_DOUBLE_EQ(video.calculateStorageCost(0.25), 0.5);
    EXPECT_EQ(video.getVideoQuality(), "ultra_hd");
}

TEST_F(VideoTest, PropertyMethods) {
    Video video("VID001", property, "/videos/video.mp4", "Video", "Description",
               currentTime, 300.0, "1920x1080", 1073741824, true, "walkthrough", 
               true, "/thumbnails/video.jpg");
    
    EXPECT_EQ(video.getProperty(), property.get());
    
    Video videoWithoutProperty("VID002", nullptr, "/videos/noproperty.mp4", "No Property", "Description",
                              currentTime, 300.0, "1920x1080", 1073741824, true, "walkthrough", 
                              true, "/thumbnails/noproperty.jpg");
    
    EXPECT_EQ(videoWithoutProperty.getProperty(), nullptr);
}