#ifndef VEHICLE_H
#define VEHICLE_H

#include "../../include/core/RentalItem.h"
#include <string>

class Vehicle : public RentalItem {
private:
    std::string licensePlate;
    std::string vehicleType;
    int year;
    double mileage;
    std::string fuelType;
    double fuelEfficiency;
    int passengerCapacity;
    std::string transmissionType;

public:
    Vehicle(const std::string& id, const std::string& name, 
            const std::string& license, const std::string& type, int year);
    
    double calculateFuelCostForTrip(double distance) const;
    bool requiresEmissionCheck() const;
    double calculateDepreciation() const override;
    bool validateDriverAge(int driverAge) const;
    double calculateInsurancePremium() const override;
    int getRecommendedServiceInterval() const;
    
    std::string getLicensePlate() const { return licensePlate; }
    std::string getVehicleType() const { return vehicleType; }
    double getMileage() const { return mileage; }
};

#endif