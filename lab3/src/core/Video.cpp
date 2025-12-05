#include "../../include/core/Video.h"
#include <chrono>
#include <algorithm>
#include <cmath>

Video::Video(const std::string& id, std::shared_ptr<Property> prop,
             const std::string& filePath, const std::string& title,
             const std::string& description, const std::chrono::system_clock::time_point& recordedDate,
             double duration, const std::string& resolution, long fileSize,
             bool hasAudio, const std::string& videoType, bool virtualTour,
             const std::string& thumbnailPath)
    : videoId(id), property(prop), filePath(filePath), title(title),
      description(description), recordedDate(recordedDate), duration(duration),
      resolution(resolution), fileSize(fileSize), hasAudio(hasAudio),
      videoType(videoType), isVirtualTour(virtualTour), thumbnailPath(thumbnailPath) {}

Video::Video(const Video& other)
    : videoId(other.videoId), property(other.property), filePath(other.filePath),
      title(other.title), description(other.description), recordedDate(other.recordedDate),
      duration(other.duration), resolution(other.resolution), fileSize(other.fileSize),
      hasAudio(other.hasAudio), videoType(other.videoType), isVirtualTour(other.isVirtualTour),
      thumbnailPath(other.thumbnailPath) {}

Video& Video::operator=(const Video& other) {
    if (this != &other) {
        videoId = other.videoId;
        property = other.property;
        filePath = other.filePath;
        title = other.title;
        description = other.description;
        recordedDate = other.recordedDate;
        duration = other.duration;
        resolution = other.resolution;
        fileSize = other.fileSize;
        hasAudio = other.hasAudio;
        videoType = other.videoType;
        isVirtualTour = other.isVirtualTour;
        thumbnailPath = other.thumbnailPath;
    }
    return *this;
}

Video::Video(Video&& other) noexcept
    : videoId(std::move(other.videoId)), property(std::move(other.property)),
      filePath(std::move(other.filePath)), title(std::move(other.title)),
      description(std::move(other.description)), recordedDate(other.recordedDate),
      duration(other.duration), resolution(std::move(other.resolution)),
      fileSize(other.fileSize), hasAudio(other.hasAudio),
      videoType(std::move(other.videoType)), isVirtualTour(other.isVirtualTour),
      thumbnailPath(std::move(other.thumbnailPath)) {}

Video& Video::operator=(Video&& other) noexcept {
    if (this != &other) {
        videoId = std::move(other.videoId);
        property = std::move(other.property);
        filePath = std::move(other.filePath);
        title = std::move(other.title);
        description = std::move(other.description);
        recordedDate = other.recordedDate;
        duration = other.duration;
        resolution = std::move(other.resolution);
        fileSize = other.fileSize;
        hasAudio = other.hasAudio;
        videoType = std::move(other.videoType);
        isVirtualTour = other.isVirtualTour;
        thumbnailPath = std::move(other.thumbnailPath);
    }
    return *this;
}

Video::~Video() {}

std::string Video::getVideoId() const { return videoId; }
Property* Video::getProperty() const { return property.get(); }
std::string Video::getFilePath() const { return filePath; }
std::string Video::getTitle() const { return title; }
std::string Video::getDescription() const { return description; }
std::chrono::system_clock::time_point Video::getRecordedDate() const { return recordedDate; }
double Video::getDuration() const { return duration; }
std::string Video::getResolution() const { return resolution; }
long Video::getFileSize() const { return fileSize; }
bool Video::getHasAudio() const { return hasAudio; }
std::string Video::getVideoType() const { return videoType; }
bool Video::getIsVirtualTour() const { return isVirtualTour; }
std::string Video::getThumbnailPath() const { return thumbnailPath; }

void Video::setTitle(const std::string& newTitle) {
    title = newTitle;
}

void Video::setDescription(const std::string& newDescription) {
    description = newDescription;
}

void Video::setThumbnailPath(const std::string& newPath) {
    thumbnailPath = newPath;
}

bool Video::isHDVideo() const {
    return resolution.find("1080") != std::string::npos || 
           resolution.find("720") != std::string::npos ||
           resolution.find("HD") != std::string::npos;
}

long Video::calculateFileSizeGB() const {
    return fileSize / (1024 * 1024 * 1024);
}

bool Video::isRecentVideo(const std::chrono::system_clock::time_point& currentDate) const {
    auto duration = std::chrono::duration_cast<std::chrono::hours>(currentDate - recordedDate);
    return duration.count() < (24 * 180);
}

bool Video::isWalkthrough() const {
    return videoType == "walkthrough" || videoType == "tour" || isVirtualTour;
}

bool Video::isPromotional() const {
    return videoType == "promotional" || videoType == "marketing" || videoType == "commercial";
}

double Video::calculateBitrate() const {
    if (duration <= 0) return 0.0;
    return (fileSize * 8) / (duration * 1000);
}

bool Video::requiresCompression() const {
    return calculateFileSizeGB() > 1.0 || calculateBitrate() > 5000.0;
}

bool Video::canBeStreamed() const {
    return calculateBitrate() <= 5000.0 && duration <= 600.0;
}

double Video::calculateStorageCost(double costPerGB) const {
    return calculateFileSizeGB() * costPerGB;
}

std::string Video::getVideoQuality() const {
    if (!isHDVideo()) return "standard";
    if (calculateBitrate() > 8000.0) return "ultra_hd";
    if (calculateBitrate() > 4000.0) return "full_hd";
    return "hd";
}