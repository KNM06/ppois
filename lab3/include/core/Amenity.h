#ifndef AMENITY_H
#define AMENITY_H

#include <string>
#include <memory>

class Amenity {
private:
    std::string amenityId;
    std::string name;
    std::string type;
    std::string description;
    bool isShared;
    bool isAvailable24_7;
    double usageFee;
    int capacity;
    std::string operatingHours;
    bool requiresReservation;
    std::string accessibility;

public:
    Amenity(const std::string& id, const std::string& name, const std::string& type,
            const std::string& description, bool shared, bool available24_7,
            double usageFee, int capacity, const std::string& operatingHours,
            bool requiresReservation, const std::string& accessibility);
    
    Amenity(const Amenity& other);
    Amenity& operator=(const Amenity& other);
    Amenity(Amenity&& other) noexcept;
    Amenity& operator=(Amenity&& other) noexcept;
    ~Amenity();

    std::string getAmenityId() const;
    std::string getName() const;
    std::string getType() const;
    std::string getDescription() const;
    bool getIsShared() const;
    bool getIsAvailable24_7() const;
    double getUsageFee() const;
    int getCapacity() const;
    std::string getOperatingHours() const;
    bool getRequiresReservation() const;
    std::string getAccessibility() const;

    void setUsageFee(double newFee);
    void setOperatingHours(const std::string& newHours);

    bool isFreeToUse() const;
    bool hasCapacityFor(int people) const;
    double calculateMonthlyRevenue(int usageCount) const;
    bool isAccessible() const;
    bool requiresMaintenanceStaff() const;
    bool isRecreational() const;
    double calculateMaintenanceCost() const;
    bool isEssential() const;
    std::string getAmenityTier() const;
};

#endif