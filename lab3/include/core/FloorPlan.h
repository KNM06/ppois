#ifndef FLOORPLAN_H
#define FLOORPLAN_H

#include <string>
#include <memory>
#include "../../include/core/Property.h"

class FloorPlan {
private:
    std::string floorPlanId;
    std::shared_ptr<Property> property;
    std::string planName;
    std::string filePath;
    double scale;
    std::string format;
    int floorLevel;
    double totalArea;
    int numberOfRooms;
    int numberOfBathrooms;
    std::string orientation;
    bool hasMeasurements;
    std::string architect;

public:
    FloorPlan(const std::string& id, std::shared_ptr<Property> prop,
              const std::string& planName, const std::string& filePath,
              double scale, const std::string& format, int floorLevel,
              double totalArea, int numberOfRooms, int numberOfBathrooms,
              const std::string& orientation, bool hasMeasurements,
              const std::string& architect);
    
    FloorPlan(const FloorPlan& other);
    FloorPlan& operator=(const FloorPlan& other);
    FloorPlan(FloorPlan&& other) noexcept;
    FloorPlan& operator=(FloorPlan&& other) noexcept;
    ~FloorPlan();

    std::string getFloorPlanId() const;
    Property* getProperty() const;
    std::string getPlanName() const;
    std::string getFilePath() const;
    double getScale() const;
    std::string getFormat() const;
    int getFloorLevel() const;
    double getTotalArea() const;
    int getNumberOfRooms() const;
    int getNumberOfBathrooms() const;
    std::string getOrientation() const;
    bool getHasMeasurements() const;
    std::string getArchitect() const;

    void setPlanName(const std::string& newName);
    void setTotalArea(double newArea);
    void setHasMeasurements(bool hasMeasurements);

    bool isToScale() const;
    double calculateRoomDensity() const;
    bool isDetailed() const;
    double calculateEfficiencyRatio() const;
    bool isGroundFloor() const;
    bool hasMultipleBathrooms() const;
    double calculateAverageRoomSize() const;
    bool isArchitecturalPlan() const;
    std::string getPlanComplexity() const;
};

#endif