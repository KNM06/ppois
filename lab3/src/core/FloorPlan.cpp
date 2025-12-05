#include "../../include/core/FloorPlan.h"
#include <algorithm>
#include <cmath>

FloorPlan::FloorPlan(const std::string& id, std::shared_ptr<Property> prop,
                     const std::string& planName, const std::string& filePath,
                     double scale, const std::string& format, int floorLevel,
                     double totalArea, int numberOfRooms, int numberOfBathrooms,
                     const std::string& orientation, bool hasMeasurements,
                     const std::string& architect)
    : floorPlanId(id), property(prop), planName(planName), filePath(filePath),
      scale(scale), format(format), floorLevel(floorLevel), totalArea(totalArea),
      numberOfRooms(numberOfRooms), numberOfBathrooms(numberOfBathrooms),
      orientation(orientation), hasMeasurements(hasMeasurements), architect(architect) {}

FloorPlan::FloorPlan(const FloorPlan& other)
    : floorPlanId(other.floorPlanId), property(other.property),
      planName(other.planName), filePath(other.filePath), scale(other.scale),
      format(other.format), floorLevel(other.floorLevel), totalArea(other.totalArea),
      numberOfRooms(other.numberOfRooms), numberOfBathrooms(other.numberOfBathrooms),
      orientation(other.orientation), hasMeasurements(other.hasMeasurements),
      architect(other.architect) {}

FloorPlan& FloorPlan::operator=(const FloorPlan& other) {
    if (this != &other) {
        floorPlanId = other.floorPlanId;
        property = other.property;
        planName = other.planName;
        filePath = other.filePath;
        scale = other.scale;
        format = other.format;
        floorLevel = other.floorLevel;
        totalArea = other.totalArea;
        numberOfRooms = other.numberOfRooms;
        numberOfBathrooms = other.numberOfBathrooms;
        orientation = other.orientation;
        hasMeasurements = other.hasMeasurements;
        architect = other.architect;
    }
    return *this;
}

FloorPlan::FloorPlan(FloorPlan&& other) noexcept
    : floorPlanId(std::move(other.floorPlanId)), property(std::move(other.property)),
      planName(std::move(other.planName)), filePath(std::move(other.filePath)),
      scale(other.scale), format(std::move(other.format)), floorLevel(other.floorLevel),
      totalArea(other.totalArea), numberOfRooms(other.numberOfRooms),
      numberOfBathrooms(other.numberOfBathrooms), orientation(std::move(other.orientation)),
      hasMeasurements(other.hasMeasurements), architect(std::move(other.architect)) {}

FloorPlan& FloorPlan::operator=(FloorPlan&& other) noexcept {
    if (this != &other) {
        floorPlanId = std::move(other.floorPlanId);
        property = std::move(other.property);
        planName = std::move(other.planName);
        filePath = std::move(other.filePath);
        scale = other.scale;
        format = std::move(other.format);
        floorLevel = other.floorLevel;
        totalArea = other.totalArea;
        numberOfRooms = other.numberOfRooms;
        numberOfBathrooms = other.numberOfBathrooms;
        orientation = std::move(other.orientation);
        hasMeasurements = other.hasMeasurements;
        architect = std::move(other.architect);
    }
    return *this;
}

FloorPlan::~FloorPlan() {}

std::string FloorPlan::getFloorPlanId() const { return floorPlanId; }
Property* FloorPlan::getProperty() const { return property.get(); }
std::string FloorPlan::getPlanName() const { return planName; }
std::string FloorPlan::getFilePath() const { return filePath; }
double FloorPlan::getScale() const { return scale; }
std::string FloorPlan::getFormat() const { return format; }
int FloorPlan::getFloorLevel() const { return floorLevel; }
double FloorPlan::getTotalArea() const { return totalArea; }
int FloorPlan::getNumberOfRooms() const { return numberOfRooms; }
int FloorPlan::getNumberOfBathrooms() const { return numberOfBathrooms; }
std::string FloorPlan::getOrientation() const { return orientation; }
bool FloorPlan::getHasMeasurements() const { return hasMeasurements; }
std::string FloorPlan::getArchitect() const { return architect; }

void FloorPlan::setPlanName(const std::string& newName) {
    planName = newName;
}

void FloorPlan::setTotalArea(double newArea) {
    totalArea = std::max(0.0, newArea);
}

void FloorPlan::setHasMeasurements(bool measurements) {
    hasMeasurements = measurements;
}

bool FloorPlan::isToScale() const {
    return scale > 0.0 && scale <= 1.0;
}

double FloorPlan::calculateRoomDensity() const {
    if (totalArea <= 0) return 0.0;
    return numberOfRooms / totalArea;
}

bool FloorPlan::isDetailed() const {
    return hasMeasurements && isToScale() && !architect.empty();
}

double FloorPlan::calculateEfficiencyRatio() const {
    if (totalArea <= 0) return 0.0;
    double livingArea = totalArea - (numberOfBathrooms * 6.0);
    return (livingArea / totalArea) * 100.0;
}

bool FloorPlan::isGroundFloor() const {
    return floorLevel == 0 || floorLevel == 1;
}

bool FloorPlan::hasMultipleBathrooms() const {
    return numberOfBathrooms > 1;
}

double FloorPlan::calculateAverageRoomSize() const {
    if (numberOfRooms <= 0) return 0.0;
    return totalArea / numberOfRooms;
}

bool FloorPlan::isArchitecturalPlan() const {
    return format == "CAD" || format == "DWG" || format == "vector";
}

std::string FloorPlan::getPlanComplexity() const {
    if (numberOfRooms > 8 || floorLevel > 3) return "complex";
    if (numberOfRooms > 4 || !isGroundFloor()) return "moderate";
    return "simple";
}