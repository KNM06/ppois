#ifndef RESERVATIONSYSTEM_H
#define RESERVATIONSYSTEM_H

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

struct Reservation {
    std::string reservationId;
    std::string customerId;
    std::string itemId;
    std::string startDate;
    std::string endDate;
    double estimatedCost;
    bool isConfirmed;
};

class ReservationSystem {
private:
    std::unordered_map<std::string, Reservation> reservations;
    std::unordered_map<std::string, int> seasonalDemand;
    std::unordered_map<std::string, std::vector<std::string>> waitingLists;

public:
    ReservationSystem();
    
    bool makeReservation(const std::string& customerId, const std::string& itemId, 
                        const std::string& startDate, const std::string& endDate);
    double calculateReservationFee(const std::string& itemId, int durationDays) const;
    bool hasConflictingReservations(const std::string& itemId) const;
    double getSeasonalDemandFactor() const;
    std::unordered_map<std::string, double> getDemandForecast(int daysForward) const;
    void processWaitingList(const std::string& itemId);
    bool cancelReservation(const std::string& reservationId);
    double calculateCancellationPenalty(const std::string& reservationId) const;
};

#endif