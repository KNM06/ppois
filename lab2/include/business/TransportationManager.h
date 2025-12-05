#ifndef TRANSPORTATIONMANAGER_H
#define TRANSPORTATIONMANAGER_H

#include "../../include/core/Inventory.h"
#include "../../include/business/LocationManager.h"
#include <memory>
#include <unordered_map>
#include <vector>
#include <string>

struct TransportRoute {
    std::string routeId;
    std::string startLocation;
    std::string endLocation;
    double distance;
    double estimatedTime;
    double cost;
    std::vector<std::string> itemsTransported;
};

class TransportationManager {
private:
    std::shared_ptr<Inventory> inventory;
    std::shared_ptr<LocationManager> locationManager;
    std::unordered_map<std::string, TransportRoute> transportRoutes;
    std::unordered_map<std::string, double> vehicleCosts;
    std::unordered_map<std::string, double> itemTransportMultipliers;

public:
    TransportationManager(std::shared_ptr<Inventory> inv, std::shared_ptr<LocationManager> locManager);
    
    bool scheduleTransport(const std::string& itemId, const std::string& fromLocation, const std::string& toLocation);
    double calculateTransportCost(const std::string& itemId, double distance) const;
    double estimateTransportTime(double distance, const std::string& vehicleType) const;
    std::vector<std::string> optimizeTransportRoute(const std::vector<std::string>& locations) const;
    bool validateTransportFeasibility(const std::string& itemId, const std::string& vehicleType) const;
    double calculateFuelCost(double distance, const std::string& vehicleType) const;
    std::vector<std::string> getTransportSchedule() const;
    void updateVehicleCost(const std::string& vehicleType, double cost);
    
    std::shared_ptr<Inventory> getInventory() const;
    std::shared_ptr<LocationManager> getLocationManager() const;
    double getVehicleCost(const std::string& vehicleType) const;
    
    void setInventory(std::shared_ptr<Inventory> inv);
    void setLocationManager(std::shared_ptr<LocationManager> manager);
    void setItemTransportMultiplier(const std::string& itemCategory, double multiplier);
};

#endif