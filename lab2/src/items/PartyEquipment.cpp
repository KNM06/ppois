#include "../../include/items/PartyEquipment.h"
#include <algorithm>
#include <cmath>

PartyEquipment::PartyEquipment(const std::string& id, const std::string& name, 
                               const std::string& type, int capacity)
    : RentalItem(id, name, "PARTY", 45.0),
      partyType(type), guestCapacity(capacity), powerRequirements(0.0),
      requiresSetup(true), requiresSupervision(false), soundLevel("MODERATE") {
    
    if (partyType == "SOUND_SYSTEM") {
        basePricePerDay = 75.0;
        powerRequirements = 1000.0;
        requiresSupervision = true;
        soundLevel = "HIGH";
        includedAccessories = {"Speakers", "Mixer", "Microphones"};
    } else if (partyType == "LIGHTING") {
        basePricePerDay = 60.0;
        powerRequirements = 800.0;
        includedAccessories = {"Light Controller", "Cables", "Mounts"};
    } else if (partyType == "TENT") {
        basePricePerDay = 120.0;
        powerRequirements = 200.0;
        requiresSetup = true;
        includedAccessories = {"Poles", "Anchors", "Sidewalls"};
    } else if (partyType == "TABLES_CHAIRS") {
        basePricePerDay = 35.0;
        requiresSetup = true;
        includedAccessories = {"Tablecloths", "Chair Covers"};
    }
}

double PartyEquipment::calculateSetupCost(int setupComplexity) const {
    double baseSetupCost = 50.0;
    
    double typeMultiplier = 1.0;
    if (partyType == "SOUND_SYSTEM") {
        typeMultiplier = 1.5;
    } else if (partyType == "LIGHTING") {
        typeMultiplier = 1.3;
    } else if (partyType == "TENT") {
        typeMultiplier = 2.0;
    } else if (partyType == "TABLES_CHAIRS") {
        typeMultiplier = 0.8;
    }
    
    double complexityMultiplier = 1.0 + (setupComplexity * 0.2);
    
    double guestMultiplier = 1.0 + (guestCapacity / 100.0);
    
    return baseSetupCost * typeMultiplier * complexityMultiplier * guestMultiplier;
}

bool PartyEquipment::validateVenueCompatibility(const std::string& venueType, double venueSize) const {
    double requiredSpace = 0.0;
    if (partyType == "SOUND_SYSTEM") {
        requiredSpace = 10.0; 
    } else if (partyType == "LIGHTING") {
        requiredSpace = 5.0;
    } else if (partyType == "TENT") {
        requiredSpace = guestCapacity * 1.5; 
    } else if (partyType == "TABLES_CHAIRS") {
        requiredSpace = guestCapacity * 1.2;
    }
    
    if (venueSize < requiredSpace) {
        return false; 
    }
    
    if (venueType == "INDOOR" && partyType == "TENT") {
        return false; 
    }
    
    if (venueType == "OUTDOOR" && partyType == "SOUND_SYSTEM" && soundLevel == "HIGH") {
        return true; 
    }
    
    if (powerRequirements > 500.0 && venueType == "OUTDOOR") {
        return true; 
    }
    
    return true;
}

double PartyEquipment::calculatePowerConsumption(int hours) const {
    double baseConsumption = powerRequirements * hours / 1000.0; 
    
    double accessoryConsumption = 0.0;
    for (const auto& accessory : includedAccessories) {
        if (accessory == "Mixer" || accessory == "Light Controller") {
            accessoryConsumption += 0.1 * hours; 
        }
    }
    
    double efficiencyFactor = 0.9; 
    
    return (baseConsumption + accessoryConsumption) * efficiencyFactor;
}

int PartyEquipment::getRecommendedGuestCount() const {
    int baseGuestCount = guestCapacity;
    
    if (partyType == "SOUND_SYSTEM") {
        baseGuestCount = static_cast<int>(guestCapacity * 1.2);
    } else if (partyType == "TABLES_CHAIRS") {
        baseGuestCount = guestCapacity;
    } else if (partyType == "TENT") {
        baseGuestCount = static_cast<int>(guestCapacity * 0.8);
    }
    
    if (soundLevel == "HIGH" && baseGuestCount > 100) {
        baseGuestCount = 100; 
    }
    
    return baseGuestCount;
}

double PartyEquipment::calculateDamageRisk(const std::string& eventType) const {
    double baseRisk = 0.05; 
    
    double eventMultiplier = 1.0;
    if (eventType == "WEDDING") {
        eventMultiplier = 0.8; 
    } else if (eventType == "CORPORATE") {
        eventMultiplier = 0.7; 
    } else if (eventType == "CONCERT") {
        eventMultiplier = 1.8; 
    } else if (eventType == "FESTIVAL") {
        eventMultiplier = 2.0; 
    } else if (eventType == "BIRTHDAY") {
        eventMultiplier = 1.2; 
    }
    
    double equipmentMultiplier = 1.0;
    if (partyType == "SOUND_SYSTEM") {
        equipmentMultiplier = 1.5;
    } else if (partyType == "LIGHTING") {
        equipmentMultiplier = 1.3;
    } else if (partyType == "TENT") {
        equipmentMultiplier = 1.8;
    } else if (partyType == "TABLES_CHAIRS") {
        equipmentMultiplier = 1.1;
    }
    
    double guestMultiplier = 1.0 + (guestCapacity / 200.0);
    
    return baseRisk * eventMultiplier * equipmentMultiplier * guestMultiplier;
}

bool PartyEquipment::requiresLicensedOperator() const {
    
    if (partyType == "SOUND_SYSTEM" && soundLevel == "HIGH") {
        return true; 
    }
    
    if (partyType == "PYROTECHNICS" || partyType == "SPECIAL_EFFECTS") {
        return true; 
    }
    
    if (powerRequirements > 2000.0) {
        return true; 
    }
    
    if (guestCapacity > 200) {
        return true; 
    }
    
    return false;
}