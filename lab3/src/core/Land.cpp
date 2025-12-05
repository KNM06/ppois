#include "../../include/core/Land.h"
#include <algorithm>

Land::Land(const std::string& propId, double propArea, double price,
           std::unique_ptr<Address> propAddress,
           std::unique_ptr<PropertyStatus> propStatus,
           std::unique_ptr<PropertyType> propType,
           const std::string& landUse, bool utilities, bool agricultural,
           double frontage, double depth, const std::string& topography,
           bool waterAccess, const std::string& soilType, double tax)
    : Property(propId, propArea, price, std::move(propAddress),
               std::move(propStatus), std::move(propType)),
      landUse(landUse), hasUtilities(utilities), isAgricultural(agricultural),
      frontage(frontage), depth(depth), topography(topography),
      hasWaterAccess(waterAccess), soilType(soilType), annualTax(tax) {}

Land::Land(const Land& other)
    : Property(other), landUse(other.landUse), hasUtilities(other.hasUtilities),
      isAgricultural(other.isAgricultural), frontage(other.frontage),
      depth(other.depth), topography(other.topography),
      hasWaterAccess(other.hasWaterAccess), soilType(other.soilType),
      annualTax(other.annualTax) {}

Land& Land::operator=(const Land& other) {
    if (this != &other) {
        Property::operator=(other);
        landUse = other.landUse;
        hasUtilities = other.hasUtilities;
        isAgricultural = other.isAgricultural;
        frontage = other.frontage;
        depth = other.depth;
        topography = other.topography;
        hasWaterAccess = other.hasWaterAccess;
        soilType = other.soilType;
        annualTax = other.annualTax;
    }
    return *this;
}

Land::Land(Land&& other) noexcept
    : Property(std::move(other)), landUse(std::move(other.landUse)),
      hasUtilities(other.hasUtilities), isAgricultural(other.isAgricultural),
      frontage(other.frontage), depth(other.depth),
      topography(std::move(other.topography)), hasWaterAccess(other.hasWaterAccess),
      soilType(std::move(other.soilType)), annualTax(other.annualTax) {}

Land& Land::operator=(Land&& other) noexcept {
    if (this != &other) {
        Property::operator=(std::move(other));
        landUse = std::move(other.landUse);
        hasUtilities = other.hasUtilities;
        isAgricultural = other.isAgricultural;
        frontage = other.frontage;
        depth = other.depth;
        topography = std::move(other.topography);
        hasWaterAccess = other.hasWaterAccess;
        soilType = std::move(other.soilType);
        annualTax = other.annualTax;
    }
    return *this;
}

Land::~Land() {}

std::string Land::getLandUse() const { return landUse; }
bool Land::getHasUtilities() const { return hasUtilities; }
bool Land::getIsAgricultural() const { return isAgricultural; }
double Land::getFrontage() const { return frontage; }
double Land::getDepth() const { return depth; }
std::string Land::getTopography() const { return topography; }
bool Land::getHasWaterAccess() const { return hasWaterAccess; }
std::string Land::getSoilType() const { return soilType; }
double Land::getAnnualTax() const { return annualTax; }

void Land::setLandUse(const std::string& newLandUse) {
    landUse = newLandUse;
}

void Land::setAnnualTax(double newTax) {
    annualTax = std::max(0.0, newTax);
}

double Land::calculatePerimeter() const {
    return 2 * (frontage + depth);
}

bool Land::isSuitableForConstruction() const {
    return hasUtilities && topography != "mountainous" && soilType != "wetland";
}

bool Land::isSuitableForFarming() const {
    return isAgricultural && hasWaterAccess && 
           (soilType == "loam" || soilType == "clay" || soilType == "silt");
}

double Land::calculateFrontageToDepthRatio() const {
    if (depth <= 0) return 0.0;
    return frontage / depth;
}

bool Land::hasDevelopmentPotential() const {
    return hasUtilities && frontage >= 30.0 && getArea() >= 1000.0;
}

double Land::calculateLandValuePerAcre() const {
    double acres = getArea() / 4046.86;
    if (acres <= 0) return 0.0;
    return getRentalPrice() / acres;
}

bool Land::requiresZoningChange(const std::string& intendedUse) const {
    return landUse != intendedUse && intendedUse != "conservation";
}

double Land::calculateInfrastructureCost() const {
    double baseCost = 0.0;
    if (!hasUtilities) baseCost += getArea() * 5.0;
    if (topography == "mountainous") baseCost += getArea() * 10.0;
    if (!hasWaterAccess) baseCost += 5000.0;
    return baseCost;
}