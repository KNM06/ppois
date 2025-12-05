#ifndef PHOTO_H
#define PHOTO_H

#include <string>
#include <memory>
#include <chrono>
#include "../../include/core/Property.h"

class Photo {
private:
    std::string photoId;
    std::shared_ptr<Property> property;
    std::string filePath;
    std::string caption;
    std::chrono::system_clock::time_point takenDate;
    std::string resolution;
    long fileSize;
    bool isPrimary;
    std::string roomType;
    std::string angle;
    bool isApproved;
    std::string photographer;

public:
    Photo(const std::string& id, std::shared_ptr<Property> prop,
          const std::string& filePath, const std::string& caption,
          const std::chrono::system_clock::time_point& takenDate,
          const std::string& resolution, long fileSize, bool isPrimary,
          const std::string& roomType, const std::string& angle,
          bool approved, const std::string& photographer);
    
    Photo(const Photo& other);
    Photo& operator=(const Photo& other);
    Photo(Photo&& other) noexcept;
    Photo& operator=(Photo&& other) noexcept;
    ~Photo();

    std::string getPhotoId() const;
    Property* getProperty() const;
    std::string getFilePath() const;
    std::string getCaption() const;
    std::chrono::system_clock::time_point getTakenDate() const;
    std::string getResolution() const;
    long getFileSize() const;
    bool getIsPrimary() const;
    std::string getRoomType() const;
    std::string getAngle() const;
    bool getIsApproved() const;
    std::string getPhotographer() const;

    void setIsPrimary(bool primary);
    void setIsApproved(bool approved);
    void setCaption(const std::string& newCaption);

    bool isHighResolution() const;
    long calculateFileSizeMB() const;
    bool isRecent(const std::chrono::system_clock::time_point& currentDate) const;
    bool isExteriorShot() const;
    bool isInteriorShot() const;
    bool canBeUsedInMarketing() const;
    double calculateAspectRatio() const;
    bool requiresEditing() const;
    std::string getPhotoQuality() const;
};

#endif