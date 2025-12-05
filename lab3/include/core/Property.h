#ifndef PROPERTY_H
#define PROPERTY_H

#include <string>
#include <memory>
#include <vector>
#include "../../include/core/Address.h"
#include "../../include/core/PropertyStatus.h"
#include "../../include/core/PropertyType.h"

class Property {
private:
    std::string id;
    double area;
    double rentalPrice;
    std::unique_ptr<Address> address;
    std::unique_ptr<PropertyStatus> status;
    std::unique_ptr<PropertyType> type;


public:
    Property(const std::string& propId, double propArea, double price,
             std::unique_ptr<Address> propAddress,
             std::unique_ptr<PropertyStatus> propStatus,
             std::unique_ptr<PropertyType> propType);
    Property(const std::string& propId,
         const std::string& addressStr,
         double area);


    Property(const Property& other);
    Property& operator=(const Property& other);
    Property(Property&& other) noexcept;
    Property& operator=(Property&& other) noexcept;
    ~Property();

    std::string getId() const;
    double getArea() const;
    double getRentalPrice() const;
    Address* getAddress() const;
    PropertyStatus* getStatus() const;
    PropertyType* getType() const;

    void setRentalPrice(double newPrice);
    void updateArea(double newArea);

    double calculateRentalYield(double annualRevenue) const;
    bool isAvailableForRent() const;
    double calculatePricePerSqMeter() const;
    bool meetsAreaRequirements(double minArea, double maxArea) const;
};

#endif