#ifndef ANALYTICSENGINE_H
#define ANALYTICSENGINE_H

#include "../core/RentalSystem.h"
#include "../business/ReservationSystem.h"
#include "../business/ReportGenerator.h"
#include <memory>
#include <vector>
#include <string>

class AnalyticsEngine {
private:
    std::shared_ptr<RentalSystem> rentalSystem;
    std::shared_ptr<ReservationSystem> reservationSystem;
    std::shared_ptr<ReportGenerator> reportGenerator;

public:
    AnalyticsEngine(std::shared_ptr<RentalSystem> rentalSys,
                   std::shared_ptr<ReservationSystem> resSys,
                   std::shared_ptr<ReportGenerator> repGen);
    
    double predictDemandTrend(const std::string& category, int daysAhead) const;
    std::vector<std::string> identifyUnderperformingItems(double threshold) const;
    double calculateCustomerLifetimeValue(const std::string& customerId) const;
    std::string generateSeasonalAnalysis() const;
    double optimizePricingStrategy(const std::string& category) const;
    std::vector<std::string> getMaintenancePredictions() const;
};

#endif