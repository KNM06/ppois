#include "../../include/core/Photo.h"
#include <chrono>
#include <algorithm>
#include <cmath>

Photo::Photo(const std::string& id, std::shared_ptr<Property> prop,
             const std::string& filePath, const std::string& caption,
             const std::chrono::system_clock::time_point& takenDate,
             const std::string& resolution, long fileSize, bool isPrimary,
             const std::string& roomType, const std::string& angle,
             bool approved, const std::string& photographer)
    : photoId(id), property(prop), filePath(filePath), caption(caption),
      takenDate(takenDate), resolution(resolution), fileSize(fileSize),
      isPrimary(isPrimary), roomType(roomType), angle(angle),
      isApproved(approved), photographer(photographer) {}

Photo::Photo(const Photo& other)
    : photoId(other.photoId), property(other.property), filePath(other.filePath),
      caption(other.caption), takenDate(other.takenDate), resolution(other.resolution),
      fileSize(other.fileSize), isPrimary(other.isPrimary), roomType(other.roomType),
      angle(other.angle), isApproved(other.isApproved), photographer(other.photographer) {}

Photo& Photo::operator=(const Photo& other) {
    if (this != &other) {
        photoId = other.photoId;
        property = other.property;
        filePath = other.filePath;
        caption = other.caption;
        takenDate = other.takenDate;
        resolution = other.resolution;
        fileSize = other.fileSize;
        isPrimary = other.isPrimary;
        roomType = other.roomType;
        angle = other.angle;
        isApproved = other.isApproved;
        photographer = other.photographer;
    }
    return *this;
}

Photo::Photo(Photo&& other) noexcept
    : photoId(std::move(other.photoId)), property(std::move(other.property)),
      filePath(std::move(other.filePath)), caption(std::move(other.caption)),
      takenDate(other.takenDate), resolution(std::move(other.resolution)),
      fileSize(other.fileSize), isPrimary(other.isPrimary),
      roomType(std::move(other.roomType)), angle(std::move(other.angle)),
      isApproved(other.isApproved), photographer(std::move(other.photographer)) {}

Photo& Photo::operator=(Photo&& other) noexcept {
    if (this != &other) {
        photoId = std::move(other.photoId);
        property = std::move(other.property);
        filePath = std::move(other.filePath);
        caption = std::move(other.caption);
        takenDate = other.takenDate;
        resolution = std::move(other.resolution);
        fileSize = other.fileSize;
        isPrimary = other.isPrimary;
        roomType = std::move(other.roomType);
        angle = std::move(other.angle);
        isApproved = other.isApproved;
        photographer = std::move(other.photographer);
    }
    return *this;
}

Photo::~Photo() {}

std::string Photo::getPhotoId() const { return photoId; }
Property* Photo::getProperty() const { return property.get(); }
std::string Photo::getFilePath() const { return filePath; }
std::string Photo::getCaption() const { return caption; }
std::chrono::system_clock::time_point Photo::getTakenDate() const { return takenDate; }
std::string Photo::getResolution() const { return resolution; }
long Photo::getFileSize() const { return fileSize; }
bool Photo::getIsPrimary() const { return isPrimary; }
std::string Photo::getRoomType() const { return roomType; }
std::string Photo::getAngle() const { return angle; }
bool Photo::getIsApproved() const { return isApproved; }
std::string Photo::getPhotographer() const { return photographer; }

void Photo::setIsPrimary(bool primary) {
    isPrimary = primary;
}

void Photo::setIsApproved(bool approved) {
    isApproved = approved;
}

void Photo::setCaption(const std::string& newCaption) {
    caption = newCaption;
}

bool Photo::isHighResolution() const {
    return resolution.find("4K") != std::string::npos || 
           resolution.find("3840") != std::string::npos ||
           resolution.find("2160") != std::string::npos;
}

long Photo::calculateFileSizeMB() const {
    return fileSize / (1024 * 1024);
}

bool Photo::isRecent(const std::chrono::system_clock::time_point& currentDate) const {
    auto duration = std::chrono::duration_cast<std::chrono::hours>(currentDate - takenDate);
    return duration.count() < (24 * 90);
}

bool Photo::isExteriorShot() const {
    return roomType == "exterior" || roomType == "front" || roomType == "back" ||
           roomType == "garden" || roomType == "pool";
}

bool Photo::isInteriorShot() const {
    return !isExteriorShot() && roomType != "aerial" && roomType != "drone";
}

bool Photo::canBeUsedInMarketing() const {
    return isApproved && isHighResolution() && isRecent(std::chrono::system_clock::now());
}

double Photo::calculateAspectRatio() const {
    size_t xPos = resolution.find('x');
    if (xPos == std::string::npos) return 0.0;
    
    try {
        int width = std::stoi(resolution.substr(0, xPos));
        int height = std::stoi(resolution.substr(xPos + 1));
        if (height == 0) return 0.0;
        return static_cast<double>(width) / height;
    } catch (...) {
        return 0.0;
    }
}

bool Photo::requiresEditing() const {
    return !isApproved || calculateFileSizeMB() > 10 || !isHighResolution();
}

std::string Photo::getPhotoQuality() const {
    if (!isHighResolution()) return "low";
    if (calculateFileSizeMB() < 2) return "medium";
    if (isApproved && isPrimary) return "premium";
    return "high";
}