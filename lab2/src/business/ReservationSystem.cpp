#include "../../include/business/ReservationSystem.h"
#include <random>
#include <cmath>
#include <algorithm>

ReservationSystem::ReservationSystem() {
    seasonalDemand["SPRING"] = 85;
    seasonalDemand["SUMMER"] = 120;
    seasonalDemand["AUTUMN"] = 95;
    seasonalDemand["WINTER"] = 70;
}

bool ReservationSystem::makeReservation(const std::string& customerId, const std::string& itemId, 
                                       const std::string& startDate, const std::string& endDate) {
    if (hasConflictingReservations(itemId)) {
        waitingLists[itemId].push_back(customerId);
        return false;
    }
    
    Reservation reservation;
    reservation.reservationId = "RES" + std::to_string(reservations.size() + 1000);
    reservation.customerId = customerId;
    reservation.itemId = itemId;
    reservation.startDate = startDate;
    reservation.endDate = endDate;
    reservation.isConfirmed = true;
    
    int durationDays = 7; 
    reservation.estimatedCost = calculateReservationFee(itemId, durationDays);
    
    reservations[reservation.reservationId] = reservation;
    return true;
}

double ReservationSystem::calculateReservationFee(const std::string& itemId, int durationDays) const {
    double baseFee = 25.0; 
    
    double categoryMultiplier = 1.0;
    if (itemId.find("VEH") != std::string::npos) categoryMultiplier = 2.0;
    else if (itemId.find("ELEC") != std::string::npos) categoryMultiplier = 1.5;
    else if (itemId.find("CONST") != std::string::npos) categoryMultiplier = 2.5;
    
    double seasonalMultiplier = getSeasonalDemandFactor();
    
    double durationMultiplier = 1.0 + (durationDays * 0.01);
    
    return baseFee * categoryMultiplier * seasonalMultiplier * durationMultiplier;
}

bool ReservationSystem::hasConflictingReservations(const std::string& itemId) const {
    int activeReservations = 0;
    for (const auto& [id, reservation] : reservations) {
        if (reservation.itemId == itemId && reservation.isConfirmed) {
            activeReservations++;
        }
    }
    
    return activeReservations >= 3;
}

double ReservationSystem::getSeasonalDemandFactor() const {
    std::string currentSeason = "SUMMER";
    
    auto it = seasonalDemand.find(currentSeason);
    if (it != seasonalDemand.end()) {
        return it->second / 100.0; 
    }
    
    return 1.0;
}

std::unordered_map<std::string, double> ReservationSystem::getDemandForecast(int daysForward) const {
    std::unordered_map<std::string, double> forecast;
    
    double baseDemand = 50.0; 
    double seasonalFactor = getSeasonalDemandFactor();
    
    forecast["VEHICLE"] = baseDemand * 1.5 * seasonalFactor;
    forecast["ELECTRONICS"] = baseDemand * 1.2 * seasonalFactor;
    forecast["CONSTRUCTION"] = baseDemand * 0.8 * seasonalFactor;
    forecast["TOOLS"] = baseDemand * 1.1 * seasonalFactor;
    forecast["SPORTS"] = baseDemand * 1.3 * seasonalFactor;
    
    double trendFactor = 1.0 + (daysForward * 0.001); 
    
    for (auto& [category, demand] : forecast) {
        demand *= trendFactor;
    }
    
    return forecast;
}

void ReservationSystem::processWaitingList(const std::string& itemId) {
    auto it = waitingLists.find(itemId);
    if (it != waitingLists.end() && !it->second.empty()) {
        std::string nextCustomerId = it->second.front();
        it->second.erase(it->second.begin());
        
    }
}

bool ReservationSystem::cancelReservation(const std::string& reservationId) {
    auto it = reservations.find(reservationId);
    if (it != reservations.end() && it->second.isConfirmed) {
        it->second.isConfirmed = false;
        
        processWaitingList(it->second.itemId);
        return true;
    }
    return false;
}

double ReservationSystem::calculateCancellationPenalty(const std::string& reservationId) const {
    auto it = reservations.find(reservationId);
    if (it == reservations.end() || !it->second.isConfirmed) {
        return 0.0;
    }
    
    const Reservation& reservation = it->second;
    double penaltyRate = 0.0;
    
  
    int daysUntilStart = 5;
    
    if (daysUntilStart < 1) {
        penaltyRate = 0.8; 
    } else if (daysUntilStart < 3) {
        penaltyRate = 0.5;
    } else if (daysUntilStart < 7) {
        penaltyRate = 0.25; 
    } else {
        penaltyRate = 0.1; 
    }
    
    return reservation.estimatedCost * penaltyRate;
}