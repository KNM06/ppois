#include "../../include/core/Property.h"
#include <cmath>

Property::Property(const std::string& propId, double propArea, double price,
                   std::unique_ptr<Address> propAddress,
                   std::unique_ptr<PropertyStatus> propStatus,
                   std::unique_ptr<PropertyType> propType)
    : id(propId), area(propArea), rentalPrice(price),
      address(std::move(propAddress)),
      status(std::move(propStatus)),
      type(std::move(propType)) {}

Property::Property(const std::string& propId,
                   const std::string& addressStr,
                   double propArea)
    : id(propId),
      area(propArea),
      rentalPrice(0.0), 
      address(std::make_unique<Address>(addressStr)),
      status(std::make_unique<PropertyStatus>("AVAILABLE")),
      type(std::make_unique<PropertyType>("UNKNOWN")) {}



Property::Property(const Property& other)
    : id(other.id), area(other.area), rentalPrice(other.rentalPrice),
      address(other.address ? std::make_unique<Address>(*other.address) : nullptr),
      status(other.status ? std::make_unique<PropertyStatus>(*other.status) : nullptr),
      type(other.type ? std::make_unique<PropertyType>(*other.type) : nullptr) {}

Property& Property::operator=(const Property& other) {
    if (this != &other) {
        id = other.id;
        area = other.area;
        rentalPrice = other.rentalPrice;
        address = other.address ? std::make_unique<Address>(*other.address) : nullptr;
        status = other.status ? std::make_unique<PropertyStatus>(*other.status) : nullptr;
        type = other.type ? std::make_unique<PropertyType>(*other.type) : nullptr;
    }
    return *this;
}

Property::Property(Property&& other) noexcept
    : id(std::move(other.id)), area(other.area), rentalPrice(other.rentalPrice),
      address(std::move(other.address)),
      status(std::move(other.status)),
      type(std::move(other.type)) {}

Property& Property::operator=(Property&& other) noexcept {
    if (this != &other) {
        id = std::move(other.id);
        area = other.area;
        rentalPrice = other.rentalPrice;
        address = std::move(other.address);
        status = std::move(other.status);
        type = std::move(other.type);
    }
    return *this;
}

Property::~Property() {}

std::string Property::getId() const { return id; }
double Property::getArea() const { return area; }
double Property::getRentalPrice() const { return rentalPrice; }
Address* Property::getAddress() const { return address.get(); }
PropertyStatus* Property::getStatus() const { return status.get(); }
PropertyType* Property::getType() const { return type.get(); }

void Property::setRentalPrice(double newPrice) {
    rentalPrice = newPrice;
}

void Property::updateArea(double newArea) {
    area = newArea;
}

double Property::calculateRentalYield(double annualRevenue) const {
    if (rentalPrice <= 0) return 0.0;
    return (annualRevenue / (rentalPrice * 12)) * 100;
}

bool Property::isAvailableForRent() const {
    return status && status->getStatusCode() == "AVAILABLE";
}

double Property::calculatePricePerSqMeter() const {
    if (area <= 0) return 0.0;
    return rentalPrice / area;
}

bool Property::meetsAreaRequirements(double minArea, double maxArea) const {
    return area >= minArea && area <= maxArea;
}