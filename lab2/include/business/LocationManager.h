#ifndef LOCATIONMANAGER_H
#define LOCATIONMANAGER_H

#include "../core/Inventory.h"
#include "../business/ReservationSystem.h"
#include <memory>
#include <vector>
#include <string>
#include <unordered_map>

struct Location {
    std::string locationId;
    std::string name;
    std::string address;
    double latitude;
    double longitude;
    int capacity;
    std::vector<std::string> availableCategories;
};

class LocationManager {
private:
    std::shared_ptr<Inventory> inventory;
    std::shared_ptr<ReservationSystem> reservationSystem;
    std::unordered_map<std::string, Location> locations;
    std::unordered_map<std::string, std::string> itemLocations;

public:
    LocationManager(std::shared_ptr<Inventory> inv, std::shared_ptr<ReservationSystem> resSys);
    
    void addLocation(const Location& location);
    bool assignItemToLocation(const std::string& itemId, const std::string& locationId);
    std::vector<std::string> findAvailableItemsNearby(double latitude, double longitude, double radius, const std::string& category) const;
    double calculateTransportationCost(const std::string& fromLocationId, const std::string& toLocationId) const;
    std::string getOptimalLocationForItem(const std::string& itemId) const;
    int getLocationUtilization(const std::string& locationId) const;
};

#endif