#ifndef PARTYEQUIPMENT_H
#define PARTYEQUIPMENT_H

#include "../../include/core/RentalItem.h"
#include <string>
#include <vector>

class PartyEquipment : public RentalItem {
private:
    std::string partyType;
    int guestCapacity;
    double powerRequirements;
    std::vector<std::string> includedAccessories;
    bool requiresSetup;
    bool requiresSupervision;
    std::string soundLevel;

public:
    PartyEquipment(const std::string& id, const std::string& name, 
                   const std::string& type, int capacity);
    
    double calculateSetupCost(int setupComplexity) const;
    bool validateVenueCompatibility(const std::string& venueType, double venueSize) const;
    double calculatePowerConsumption(int hours) const;
    int getRecommendedGuestCount() const;
    double calculateDamageRisk(const std::string& eventType) const;
    bool requiresLicensedOperator() const;
    
    std::string getPartyType() const { return partyType; }
    int getGuestCapacity() const { return guestCapacity; }
    bool getRequiresSetup() const { return requiresSetup; }
};

#endif