#include "../../include/business/TransportationManager.h"
#include <algorithm>
#include <cmath>

TransportationManager::TransportationManager(std::shared_ptr<Inventory> inv, std::shared_ptr<LocationManager> locManager)
    : inventory(inv), locationManager(locManager) {
    
    vehicleCosts["VAN"] = 0.5;
    vehicleCosts["TRUCK"] = 0.8;
    vehicleCosts["SPECIAL"] = 1.2;
    vehicleCosts["CAR"] = 0.3;
    
    itemTransportMultipliers["VEHICLE"] = 2.0;
    itemTransportMultipliers["CONSTRUCTION"] = 1.8;
    itemTransportMultipliers["ELECTRONICS"] = 1.3;
    itemTransportMultipliers["LUXURY"] = 1.5;
}

bool TransportationManager::scheduleTransport(const std::string& itemId, const std::string& fromLocation, const std::string& toLocation) {
    if (!inventory->isItemAvailable(itemId)) {
        return false;
    }
    
    double distance = 0.0;
    if (locationManager) {
        distance = locationManager->calculateTransportationCost(fromLocation, toLocation) / 0.5;
    }
    
    TransportRoute route;
    route.routeId = "TRANS" + std::to_string(transportRoutes.size() + 1000);
    route.startLocation = fromLocation;
    route.endLocation = toLocation;
    route.distance = distance;
    route.estimatedTime = estimateTransportTime(distance, "VAN");
    route.cost = calculateTransportCost(itemId, distance);
    route.itemsTransported = {itemId};
    
    transportRoutes[route.routeId] = route;
    return true;
}

double TransportationManager::calculateTransportCost(const std::string& itemId, double distance) const {
    std::string category = "GENERAL";
    if (itemId.find("VEH") != std::string::npos) category = "VEHICLE";
    else if (itemId.find("CONST") != std::string::npos) category = "CONSTRUCTION";
    
    auto multiplierIt = itemTransportMultipliers.find(category);
    double categoryMultiplier = multiplierIt != itemTransportMultipliers.end() ? multiplierIt->second : 1.0;
    
    double baseCostPerKm = 0.5;
    double vehicleCost = baseCostPerKm * distance;
    
    double handlingCost = 25.0;
    if (category == "ELECTRONICS" || category == "LUXURY") {
        handlingCost *= 1.5;
    }
    
    return (vehicleCost * categoryMultiplier) + handlingCost;
}

double TransportationManager::estimateTransportTime(double distance, const std::string& vehicleType) const {
    double averageSpeed = 50.0;
    
    if (vehicleType == "TRUCK") averageSpeed = 40.0;
    else if (vehicleType == "SPECIAL") averageSpeed = 30.0;
    else if (vehicleType == "CAR") averageSpeed = 60.0;
    
    double travelTime = distance / averageSpeed;
    double loadingTime = 0.5;
    double unloadingTime = 0.5;
    
    return travelTime + loadingTime + unloadingTime;
}

std::vector<std::string> TransportationManager::optimizeTransportRoute(const std::vector<std::string>& locations) const {
    std::vector<std::string> optimizedRoute = locations;
    
    if (locations.size() > 2) {
        std::sort(optimizedRoute.begin() + 1, optimizedRoute.end() - 1);
    }
    
    return optimizedRoute;
}

bool TransportationManager::validateTransportFeasibility(const std::string& itemId, const std::string& vehicleType) const {
    std::string category = "GENERAL";
    if (itemId.find("VEH") != std::string::npos) category = "VEHICLE";
    else if (itemId.find("CONST") != std::string::npos) category = "CONSTRUCTION";
    
    if (category == "CONSTRUCTION" && vehicleType == "CAR") {
        return false;
    }
    
    if (category == "LUXURY" && vehicleType != "SPECIAL") {
        return false;
    }
    
    auto vehicleIt = vehicleCosts.find(vehicleType);
    if (vehicleIt == vehicleCosts.end()) {
        return false;
    }
    
    return true;
}

double TransportationManager::calculateFuelCost(double distance, const std::string& vehicleType) const {
    double fuelEfficiency = 8.0;
    double fuelPrice = 1.75;
    
    if (vehicleType == "TRUCK") fuelEfficiency = 5.0;
    else if (vehicleType == "VAN") fuelEfficiency = 7.0;
    else if (vehicleType == "CAR") fuelEfficiency = 12.0;
    
    double fuelConsumed = distance / 100.0 * fuelEfficiency;
    return fuelConsumed * fuelPrice;
}

std::vector<std::string> TransportationManager::getTransportSchedule() const {
    std::vector<std::string> schedule;
    
    for (const auto& [routeId, route] : transportRoutes) {
        std::string scheduleEntry = "Route " + routeId + ": " + route.startLocation + " to " + 
                                   route.endLocation + " - Cost: $" + std::to_string(route.cost);
        schedule.push_back(scheduleEntry);
    }
    
    return schedule;
}

void TransportationManager::updateVehicleCost(const std::string& vehicleType, double cost) {
    vehicleCosts[vehicleType] = std::max(0.0, cost);
}

std::shared_ptr<Inventory> TransportationManager::getInventory() const {
    return inventory;
}

std::shared_ptr<LocationManager> TransportationManager::getLocationManager() const {
    return locationManager;
}

double TransportationManager::getVehicleCost(const std::string& vehicleType) const {
    auto it = vehicleCosts.find(vehicleType);
    return it != vehicleCosts.end() ? it->second : 0.5;
}

void TransportationManager::setInventory(std::shared_ptr<Inventory> inv) {
    inventory = inv;
}

void TransportationManager::setLocationManager(std::shared_ptr<LocationManager> manager) {
    locationManager = manager;
}

void TransportationManager::setItemTransportMultiplier(const std::string& itemCategory, double multiplier) {
    itemTransportMultipliers[itemCategory] = std::max(0.1, multiplier);
}