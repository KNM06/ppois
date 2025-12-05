#ifndef PROPERTYTYPE_H
#define PROPERTYTYPE_H

#include <string>
#include <memory>

class PropertyType {
private:
    std::string typeCode;
    std::string category;
    std::string description;
    int minBedrooms;
    int maxBedrooms;
    double minArea;
    double maxArea;
    bool requiresSpecialLicense;
    std::string zoningCategory;
    double baseTaxRate;

public:
    PropertyType();
    PropertyType(const std::string& code, const std::string& category);
    PropertyType(const std::string& code);
    PropertyType(const std::string& code, const std::string& category,
                 const std::string& desc, int minBeds, int maxBeds,
                 double minArea, double maxArea, bool specialLicense,
                 const std::string& zoning, double taxRate);
    
    PropertyType(const PropertyType& other);
    PropertyType& operator=(const PropertyType& other);
    PropertyType(PropertyType&& other) noexcept;
    PropertyType& operator=(PropertyType&& other) noexcept;
    ~PropertyType();

    std::string getTypeCode() const;
    std::string getCategory() const;
    std::string getDescription() const;
    int getMinBedrooms() const;
    int getMaxBedrooms() const;
    double getMinArea() const;
    double getMaxArea() const;
    bool getRequiresSpecialLicense() const;
    std::string getZoningCategory() const;
    double getBaseTaxRate() const;

    void setDescription(const std::string& newDesc);
    void setBaseTaxRate(double newRate);

    bool isValidConfiguration(int bedrooms, double area) const;
    double calculateTaxAmount(double propertyValue) const;
    bool isCommercial() const;
    bool isResidential() const;
    bool requiresAdditionalPermits(const std::string& intendedUse) const;
    bool canBeConvertedTo(const PropertyType& targetType) const;
    double calculateInsurancePremium(double propertyValue) const;
    std::string getRiskCategory() const;
    bool meetsZoningRequirements(const std::string& localZoning) const;
};

#endif