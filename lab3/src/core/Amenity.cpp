#include "../../include/core/Amenity.h"
#include <algorithm>

Amenity::Amenity(const std::string& id, const std::string& name, const std::string& type,
                 const std::string& description, bool shared, bool available24_7,
                 double usageFee, int capacity, const std::string& operatingHours,
                 bool requiresReservation, const std::string& accessibility)
    : amenityId(id), name(name), type(type), description(description),
      isShared(shared), isAvailable24_7(available24_7), usageFee(usageFee),
      capacity(capacity), operatingHours(operatingHours),
      requiresReservation(requiresReservation), accessibility(accessibility) {}

Amenity::Amenity(const Amenity& other)
    : amenityId(other.amenityId), name(other.name), type(other.type),
      description(other.description), isShared(other.isShared),
      isAvailable24_7(other.isAvailable24_7), usageFee(other.usageFee),
      capacity(other.capacity), operatingHours(other.operatingHours),
      requiresReservation(other.requiresReservation), accessibility(other.accessibility) {}

Amenity& Amenity::operator=(const Amenity& other) {
    if (this != &other) {
        amenityId = other.amenityId;
        name = other.name;
        type = other.type;
        description = other.description;
        isShared = other.isShared;
        isAvailable24_7 = other.isAvailable24_7;
        usageFee = other.usageFee;
        capacity = other.capacity;
        operatingHours = other.operatingHours;
        requiresReservation = other.requiresReservation;
        accessibility = other.accessibility;
    }
    return *this;
}

Amenity::Amenity(Amenity&& other) noexcept
    : amenityId(std::move(other.amenityId)), name(std::move(other.name)),
      type(std::move(other.type)), description(std::move(other.description)),
      isShared(other.isShared), isAvailable24_7(other.isAvailable24_7),
      usageFee(other.usageFee), capacity(other.capacity),
      operatingHours(std::move(other.operatingHours)),
      requiresReservation(other.requiresReservation),
      accessibility(std::move(other.accessibility)) {}

Amenity& Amenity::operator=(Amenity&& other) noexcept {
    if (this != &other) {
        amenityId = std::move(other.amenityId);
        name = std::move(other.name);
        type = std::move(other.type);
        description = std::move(other.description);
        isShared = other.isShared;
        isAvailable24_7 = other.isAvailable24_7;
        usageFee = other.usageFee;
        capacity = other.capacity;
        operatingHours = std::move(other.operatingHours);
        requiresReservation = other.requiresReservation;
        accessibility = std::move(other.accessibility);
    }
    return *this;
}

Amenity::~Amenity() {}

std::string Amenity::getAmenityId() const { return amenityId; }
std::string Amenity::getName() const { return name; }
std::string Amenity::getType() const { return type; }
std::string Amenity::getDescription() const { return description; }
bool Amenity::getIsShared() const { return isShared; }
bool Amenity::getIsAvailable24_7() const { return isAvailable24_7; }
double Amenity::getUsageFee() const { return usageFee; }
int Amenity::getCapacity() const { return capacity; }
std::string Amenity::getOperatingHours() const { return operatingHours; }
bool Amenity::getRequiresReservation() const { return requiresReservation; }
std::string Amenity::getAccessibility() const { return accessibility; }

void Amenity::setUsageFee(double newFee) {
    usageFee = std::max(0.0, newFee);
}

void Amenity::setOperatingHours(const std::string& newHours) {
    operatingHours = newHours;
}

bool Amenity::isFreeToUse() const {
    return usageFee == 0.0;
}

bool Amenity::hasCapacityFor(int people) const {
    return capacity >= people;
}

double Amenity::calculateMonthlyRevenue(int usageCount) const {
    return usageFee * usageCount;
}

bool Amenity::isAccessible() const {
    return accessibility == "fully_accessible" || accessibility == "wheelchair_accessible";
}

bool Amenity::requiresMaintenanceStaff() const {
    return type == "pool" || type == "gym" || type == "spa" || type == "elevator";
}

bool Amenity::isRecreational() const {
    return type == "pool" || type == "gym" || type == "tennis_court" || 
           type == "playground" || type == "sports_field";
}

double Amenity::calculateMaintenanceCost() const {
    double baseCost = 0.0;
    if (type == "pool") baseCost = 500.0;
    else if (type == "gym") baseCost = 300.0;
    else if (type == "elevator") baseCost = 200.0;
    else if (isRecreational()) baseCost = 150.0;
    else baseCost = 50.0;
    
    return baseCost + (usageFee * 0.1);
}

bool Amenity::isEssential() const {
    return type == "parking" || type == "laundry" || type == "elevator" || 
           type == "security" || name.find("essential") != std::string::npos;
}

std::string Amenity::getAmenityTier() const {
    if (usageFee > 100.0 || !isShared) return "premium";
    if (usageFee > 0.0 || requiresReservation) return "standard";
    return "basic";
}