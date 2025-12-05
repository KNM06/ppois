#ifndef PROPERTYFEATURE_H
#define PROPERTYFEATURE_H

#include <string>
#include <memory>

class PropertyFeature {
private:
    std::string featureId;
    std::string name;
    std::string description;
    std::string category;
    bool isStandard;
    bool isPremium;
    double additionalCost;
    std::string maintenanceRequirements;
    int lifespanYears;
    bool requiresProfessionalInstallation;

public:
    PropertyFeature(const std::string& id, const std::string& name,
                   const std::string& description, const std::string& category,
                   bool standard, bool premium, double additionalCost,
                   const std::string& maintenance, int lifespan, bool professionalInstallation);
    
    PropertyFeature(const PropertyFeature& other);
    PropertyFeature& operator=(const PropertyFeature& other);
    PropertyFeature(PropertyFeature&& other) noexcept;
    PropertyFeature& operator=(PropertyFeature&& other) noexcept;
    ~PropertyFeature();

    std::string getFeatureId() const;
    std::string getName() const;
    std::string getDescription() const;
    std::string getCategory() const;
    bool getIsStandard() const;
    bool getIsPremium() const;
    double getAdditionalCost() const;
    std::string getMaintenanceRequirements() const;
    int getLifespanYears() const;
    bool getRequiresProfessionalInstallation() const;

    void setAdditionalCost(double newCost);
    void setMaintenanceRequirements(const std::string& newRequirements);

    double calculateAnnualMaintenanceCost() const;
    bool isEnergyEfficient() const;
    bool increasesPropertyValue() const;
    double calculateROI(double propertyValue) const;
    bool requiresRegularMaintenance() const;
    bool isSafetyFeature() const;
    double calculateDepreciation() const;
    bool isLuxuryFeature() const;
    std::string getFeatureGrade() const;
};

#endif