#ifndef VIDEO_H
#define VIDEO_H

#include <string>
#include <memory>
#include <chrono>
#include "../../include/core/Property.h"

class Video {
private:
    std::string videoId;
    std::shared_ptr<Property> property;
    std::string filePath;
    std::string title;
    std::string description;
    std::chrono::system_clock::time_point recordedDate;
    double duration;
    std::string resolution;
    long fileSize;
    bool hasAudio;
    std::string videoType;
    bool isVirtualTour;
    std::string thumbnailPath;

public:
    Video(const std::string& id, std::shared_ptr<Property> prop,
          const std::string& filePath, const std::string& title,
          const std::string& description, const std::chrono::system_clock::time_point& recordedDate,
          double duration, const std::string& resolution, long fileSize,
          bool hasAudio, const std::string& videoType, bool virtualTour,
          const std::string& thumbnailPath);
    
    Video(const Video& other);
    Video& operator=(const Video& other);
    Video(Video&& other) noexcept;
    Video& operator=(Video&& other) noexcept;
    ~Video();

    std::string getVideoId() const;
    Property* getProperty() const;
    std::string getFilePath() const;
    std::string getTitle() const;
    std::string getDescription() const;
    std::chrono::system_clock::time_point getRecordedDate() const;
    double getDuration() const;
    std::string getResolution() const;
    long getFileSize() const;
    bool getHasAudio() const;
    std::string getVideoType() const;
    bool getIsVirtualTour() const;
    std::string getThumbnailPath() const;

    void setTitle(const std::string& newTitle);
    void setDescription(const std::string& newDescription);
    void setThumbnailPath(const std::string& newPath);

    bool isHDVideo() const;
    long calculateFileSizeGB() const;
    bool isRecentVideo(const std::chrono::system_clock::time_point& currentDate) const;
    bool isWalkthrough() const;
    bool isPromotional() const;
    double calculateBitrate() const;
    bool requiresCompression() const;
    bool canBeStreamed() const;
    double calculateStorageCost(double costPerGB) const;
    std::string getVideoQuality() const;
};

#endif