#include "../../include/core/PropertyFeature.h"
#include <algorithm>

PropertyFeature::PropertyFeature(const std::string& id, const std::string& name,
                                 const std::string& description, const std::string& category,
                                 bool standard, bool premium, double additionalCost,
                                 const std::string& maintenance, int lifespan, bool professionalInstallation)
    : featureId(id), name(name), description(description), category(category),
      isStandard(standard), isPremium(premium), additionalCost(additionalCost),
      maintenanceRequirements(maintenance), lifespanYears(lifespan),
      requiresProfessionalInstallation(professionalInstallation) {}

PropertyFeature::PropertyFeature(const PropertyFeature& other)
    : featureId(other.featureId), name(other.name), description(other.description),
      category(other.category), isStandard(other.isStandard), isPremium(other.isPremium),
      additionalCost(other.additionalCost), maintenanceRequirements(other.maintenanceRequirements),
      lifespanYears(other.lifespanYears), requiresProfessionalInstallation(other.requiresProfessionalInstallation) {}

PropertyFeature& PropertyFeature::operator=(const PropertyFeature& other) {
    if (this != &other) {
        featureId = other.featureId;
        name = other.name;
        description = other.description;
        category = other.category;
        isStandard = other.isStandard;
        isPremium = other.isPremium;
        additionalCost = other.additionalCost;
        maintenanceRequirements = other.maintenanceRequirements;
        lifespanYears = other.lifespanYears;
        requiresProfessionalInstallation = other.requiresProfessionalInstallation;
    }
    return *this;
}

PropertyFeature::PropertyFeature(PropertyFeature&& other) noexcept
    : featureId(std::move(other.featureId)), name(std::move(other.name)),
      description(std::move(other.description)), category(std::move(other.category)),
      isStandard(other.isStandard), isPremium(other.isPremium),
      additionalCost(other.additionalCost), maintenanceRequirements(std::move(other.maintenanceRequirements)),
      lifespanYears(other.lifespanYears), requiresProfessionalInstallation(other.requiresProfessionalInstallation) {}

PropertyFeature& PropertyFeature::operator=(PropertyFeature&& other) noexcept {
    if (this != &other) {
        featureId = std::move(other.featureId);
        name = std::move(other.name);
        description = std::move(other.description);
        category = std::move(other.category);
        isStandard = other.isStandard;
        isPremium = other.isPremium;
        additionalCost = other.additionalCost;
        maintenanceRequirements = std::move(other.maintenanceRequirements);
        lifespanYears = other.lifespanYears;
        requiresProfessionalInstallation = other.requiresProfessionalInstallation;
    }
    return *this;
}

PropertyFeature::~PropertyFeature() {}

std::string PropertyFeature::getFeatureId() const { return featureId; }
std::string PropertyFeature::getName() const { return name; }
std::string PropertyFeature::getDescription() const { return description; }
std::string PropertyFeature::getCategory() const { return category; }
bool PropertyFeature::getIsStandard() const { return isStandard; }
bool PropertyFeature::getIsPremium() const { return isPremium; }
double PropertyFeature::getAdditionalCost() const { return additionalCost; }
std::string PropertyFeature::getMaintenanceRequirements() const { return maintenanceRequirements; }
int PropertyFeature::getLifespanYears() const { return lifespanYears; }
bool PropertyFeature::getRequiresProfessionalInstallation() const { return requiresProfessionalInstallation; }

void PropertyFeature::setAdditionalCost(double newCost) {
    additionalCost = std::max(0.0, newCost);
}

void PropertyFeature::setMaintenanceRequirements(const std::string& newRequirements) {
    maintenanceRequirements = newRequirements;
}

double PropertyFeature::calculateAnnualMaintenanceCost() const {
    if (lifespanYears <= 0) return 0.0;
    return additionalCost * 0.02;
}

bool PropertyFeature::isEnergyEfficient() const {
    return category == "energy" || category == "sustainability" || 
           name.find("efficient") != std::string::npos;
}

bool PropertyFeature::increasesPropertyValue() const {
    return isPremium || additionalCost > 1000.0 || isEnergyEfficient();
}

double PropertyFeature::calculateROI(double propertyValue) const {
    if (propertyValue <= 0 || additionalCost <= 0) return 0.0;
    double valueIncrease = additionalCost * (isPremium ? 1.5 : 1.2);
    return (valueIncrease / additionalCost) * 100.0;
}

bool PropertyFeature::requiresRegularMaintenance() const {
    return maintenanceRequirements.find("monthly") != std::string::npos ||
           maintenanceRequirements.find("quarterly") != std::string::npos ||
           maintenanceRequirements.find("annual") != std::string::npos;
}

bool PropertyFeature::isSafetyFeature() const {
    return category == "safety" || category == "security" ||
           name.find("alarm") != std::string::npos ||
           name.find("safety") != std::string::npos;
}

double PropertyFeature::calculateDepreciation() const {
    if (lifespanYears <= 0) return 0.0;
    return additionalCost / lifespanYears;
}

bool PropertyFeature::isLuxuryFeature() const {
    return isPremium && additionalCost > 5000.0;
}

std::string PropertyFeature::getFeatureGrade() const {
    if (isPremium && additionalCost > 10000.0) return "luxury";
    if (isPremium) return "premium";
    if (isStandard) return "standard";
    return "basic";
}