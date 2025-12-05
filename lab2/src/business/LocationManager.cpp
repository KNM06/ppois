#include "../../include/business/LocationManager.h"
#include <cmath>

LocationManager::LocationManager(std::shared_ptr<Inventory> inv, std::shared_ptr<ReservationSystem> resSys)
    : inventory(inv), reservationSystem(resSys) {}

void LocationManager::addLocation(const Location& location) {
    locations[location.locationId] = location;
}

bool LocationManager::assignItemToLocation(const std::string& itemId, const std::string& locationId) {
    if (locations.find(locationId) == locations.end()) {
        return false;
    }
    
    itemLocations[itemId] = locationId;
    return true;
}

std::vector<std::string> LocationManager::findAvailableItemsNearby(double latitude, double longitude, double radius, const std::string& category) const {
    std::vector<std::string> availableItems;
    
    for (const auto& [itemId, locationId] : itemLocations) {
        if (locations.find(locationId) != locations.end()) {
            const Location& loc = locations.at(locationId);
            double distance = std::sqrt(std::pow(loc.latitude - latitude, 2) + std::pow(loc.longitude - longitude, 2));
            
            if (distance <= radius && inventory->isItemAvailable(itemId)) {
                availableItems.push_back(itemId);
            }
        }
    }
    
    return availableItems;
}

double LocationManager::calculateTransportationCost(const std::string& fromLocationId, const std::string& toLocationId) const {
    if (locations.find(fromLocationId) == locations.end() || locations.find(toLocationId) == locations.end()) {
        return 0.0;
    }
    
    const Location& from = locations.at(fromLocationId);
    const Location& to = locations.at(toLocationId);
    
    double distance = std::sqrt(std::pow(from.latitude - to.latitude, 2) + std::pow(from.longitude - to.longitude, 2));
    double baseCostPerUnit = 0.5;
    
    return distance * baseCostPerUnit;
}

std::string LocationManager::getOptimalLocationForItem(const std::string& itemId) const {
    auto demandForecast = reservationSystem->getDemandForecast(30);
    std::string bestLocation;
    double maxDemand = 0.0;
    
    for (const auto& [locationId, location] : locations) {
        double totalDemand = 0.0;
        for (const auto& category : location.availableCategories) {
            if (demandForecast.find(category) != demandForecast.end()) {
                totalDemand += demandForecast.at(category);
            }
        }
        
        if (totalDemand > maxDemand) {
            maxDemand = totalDemand;
            bestLocation = locationId;
        }
    }
    
    return bestLocation;
}

int LocationManager::getLocationUtilization(const std::string& locationId) const {
    if (locations.find(locationId) == locations.end()) {
        return 0;
    }
    
    int itemsAtLocation = 0;
    for (const auto& [itemId, locId] : itemLocations) {
        if (locId == locationId) {
            itemsAtLocation++;
        }
    }
    
    const Location& location = locations.at(locationId);
    double utilization = static_cast<double>(itemsAtLocation) / location.capacity;
    
    return static_cast<int>(utilization * 100);
}