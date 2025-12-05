#ifndef LAND_H
#define LAND_H

#include <string>
#include <memory>
#include "../../include/core/Property.h"
#include "../../include/core/Address.h"
#include "../../include/core/PropertyStatus.h"
#include "../../include/core/PropertyType.h"

class Land : public Property {
private:
    std::string landUse;
    bool hasUtilities;
    bool isAgricultural;
    double frontage;
    double depth;
    std::string topography;
    bool hasWaterAccess;
    std::string soilType;
    double annualTax;

public:
    Land(const std::string& propId, double propArea, double price,
         std::unique_ptr<Address> propAddress,
         std::unique_ptr<PropertyStatus> propStatus,
         std::unique_ptr<PropertyType> propType,
         const std::string& landUse, bool utilities, bool agricultural,
         double frontage, double depth, const std::string& topography,
         bool waterAccess, const std::string& soilType, double tax);
    
    Land(const Land& other);
    Land& operator=(const Land& other);
    Land(Land&& other) noexcept;
    Land& operator=(Land&& other) noexcept;
    ~Land();

    std::string getLandUse() const;
    bool getHasUtilities() const;
    bool getIsAgricultural() const;
    double getFrontage() const;
    double getDepth() const;
    std::string getTopography() const;
    bool getHasWaterAccess() const;
    std::string getSoilType() const;
    double getAnnualTax() const;

    void setLandUse(const std::string& newLandUse);
    void setAnnualTax(double newTax);

    double calculatePerimeter() const;
    bool isSuitableForConstruction() const;
    bool isSuitableForFarming() const;
    double calculateFrontageToDepthRatio() const;
    bool hasDevelopmentPotential() const;
    double calculateLandValuePerAcre() const;
    bool requiresZoningChange(const std::string& intendedUse) const;
    double calculateInfrastructureCost() const;
};

#endif