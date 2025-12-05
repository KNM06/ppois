#include "../../include/core/PropertyType.h"
#include <algorithm>

PropertyType::PropertyType(const std::string& code, const std::string& category,
                           const std::string& desc, int minBeds, int maxBeds,
                           double minArea, double maxArea, bool specialLicense,
                           const std::string& zoning, double taxRate)
    : typeCode(code), category(category), description(desc),
      minBedrooms(minBeds), maxBedrooms(maxBeds), minArea(minArea),
      maxArea(maxArea), requiresSpecialLicense(specialLicense),
      zoningCategory(zoning), baseTaxRate(taxRate) {}
PropertyType::PropertyType(const std::string& code)
    : typeCode(code),
      category(code),           
      description(""),
      minBedrooms(0),
      maxBedrooms(10),
      minArea(0.0),
      maxArea(10000.0),
      requiresSpecialLicense(false),
      zoningCategory("any"),
      baseTaxRate(1.0) {}
PropertyType::PropertyType(const std::string& code, const std::string& category)
    : typeCode(code), category(category), description(""),
      minBedrooms(0), maxBedrooms(10), minArea(0.0), maxArea(10000.0),
      requiresSpecialLicense(false), zoningCategory("any"), baseTaxRate(1.0) {}

PropertyType::PropertyType(const PropertyType& other)
    : typeCode(other.typeCode), category(other.category), description(other.description),
      minBedrooms(other.minBedrooms), maxBedrooms(other.maxBedrooms),
      minArea(other.minArea), maxArea(other.maxArea),
      requiresSpecialLicense(other.requiresSpecialLicense),
      zoningCategory(other.zoningCategory), baseTaxRate(other.baseTaxRate) {}
PropertyType::PropertyType() :
    typeCode(""), category(""), description(""),
    minBedrooms(0), maxBedrooms(0),
    minArea(0.0), maxArea(0.0),
    requiresSpecialLicense(false),
    zoningCategory(""), baseTaxRate(0.0) {}

PropertyType& PropertyType::operator=(const PropertyType& other) {
    if (this != &other) {
        typeCode = other.typeCode;
        category = other.category;
        description = other.description;
        minBedrooms = other.minBedrooms;
        maxBedrooms = other.maxBedrooms;
        minArea = other.minArea;
        maxArea = other.maxArea;
        requiresSpecialLicense = other.requiresSpecialLicense;
        zoningCategory = other.zoningCategory;
        baseTaxRate = other.baseTaxRate;
    }
    return *this;
}

PropertyType::PropertyType(PropertyType&& other) noexcept
    : typeCode(std::move(other.typeCode)), category(std::move(other.category)),
      description(std::move(other.description)), minBedrooms(other.minBedrooms),
      maxBedrooms(other.maxBedrooms), minArea(other.minArea), maxArea(other.maxArea),
      requiresSpecialLicense(other.requiresSpecialLicense),
      zoningCategory(std::move(other.zoningCategory)), baseTaxRate(other.baseTaxRate) {}

PropertyType& PropertyType::operator=(PropertyType&& other) noexcept {
    if (this != &other) {
        typeCode = std::move(other.typeCode);
        category = std::move(other.category);
        description = std::move(other.description);
        minBedrooms = other.minBedrooms;
        maxBedrooms = other.maxBedrooms;
        minArea = other.minArea;
        maxArea = other.maxArea;
        requiresSpecialLicense = other.requiresSpecialLicense;
        zoningCategory = std::move(other.zoningCategory);
        baseTaxRate = other.baseTaxRate;
    }
    return *this;
}

PropertyType::~PropertyType() {}

std::string PropertyType::getTypeCode() const { return typeCode; }
std::string PropertyType::getCategory() const { return category; }
std::string PropertyType::getDescription() const { return description; }
int PropertyType::getMinBedrooms() const { return minBedrooms; }
int PropertyType::getMaxBedrooms() const { return maxBedrooms; }
double PropertyType::getMinArea() const { return minArea; }
double PropertyType::getMaxArea() const { return maxArea; }
bool PropertyType::getRequiresSpecialLicense() const { return requiresSpecialLicense; }
std::string PropertyType::getZoningCategory() const { return zoningCategory; }
double PropertyType::getBaseTaxRate() const { return baseTaxRate; }

void PropertyType::setDescription(const std::string& newDesc) {
    description = newDesc;
}

void PropertyType::setBaseTaxRate(double newRate) {
    baseTaxRate = std::max(0.0, newRate);
}

bool PropertyType::isValidConfiguration(int bedrooms, double area) const {
    return bedrooms >= minBedrooms && bedrooms <= maxBedrooms &&
           area >= minArea && area <= maxArea;
}

double PropertyType::calculateTaxAmount(double propertyValue) const {
    return propertyValue * (baseTaxRate / 100.0);
}

bool PropertyType::isCommercial() const {
    return category == "commercial" || category == "industrial" || category == "retail";
}

bool PropertyType::isResidential() const {
    return category == "residential" || category == "apartment" || category == "house";
}

bool PropertyType::requiresAdditionalPermits(const std::string& intendedUse) const {
    if (isCommercial() && intendedUse == "restaurant") return true;
    if (isCommercial() && intendedUse == "medical") return true;
    if (requiresSpecialLicense) return true;
    return false;
}

bool PropertyType::canBeConvertedTo(const PropertyType& targetType) const {
    if (category == targetType.category) return true;
    if (isCommercial() && targetType.isResidential()) return true;
    if (isResidential() && targetType.isCommercial()) 
    return zoningCategory == "mixed_use";
    if (category == "mixed_use") 
    return targetType.isResidential() || targetType.isCommercial();
    return false;
}

double PropertyType::calculateInsurancePremium(double propertyValue) const {
    double basePremium = propertyValue * 0.001;
    if (isCommercial()) basePremium *= 1.5;
    if (requiresSpecialLicense) basePremium *= 1.2;
    return basePremium;
}

std::string PropertyType::getRiskCategory() const {
    if (requiresSpecialLicense) return "high_risk";  
    if (isCommercial()) return "medium_risk";
    return "low_risk";
}

bool PropertyType::meetsZoningRequirements(const std::string& localZoning) const {
    return zoningCategory == localZoning || localZoning == "any" || zoningCategory == "mixed_use";
}