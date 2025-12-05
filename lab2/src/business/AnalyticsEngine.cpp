#include "../../include/business/AnalyticsEngine.h"
#include <algorithm>
#include <cmath>

AnalyticsEngine::AnalyticsEngine(
    std::shared_ptr<RentalSystem> rentalSys,
    std::shared_ptr<ReservationSystem> resSys,
    std::shared_ptr<ReportGenerator> repGen)
    : rentalSystem(rentalSys),
      reservationSystem(resSys),
      reportGenerator(repGen) {}

double AnalyticsEngine::predictDemandTrend(const std::string& category, int daysAhead) const {
    double baseDemand = 50.0;

    if (reservationSystem) {
        auto demandForecast = reservationSystem->getDemandForecast(daysAhead);
        if (demandForecast.find(category) != demandForecast.end()) {
            baseDemand = demandForecast.at(category);
        }
    }

    if (daysAhead < 0) {
        daysAhead = std::abs(daysAhead);
    }

    double growthRate = 1.0 + (static_cast<double>(daysAhead) * 0.001);
    double seasonalFactor = reservationSystem ? reservationSystem->getSeasonalDemandFactor() : 1.0;

    return baseDemand * growthRate * seasonalFactor;
}

std::vector<std::string> AnalyticsEngine::identifyUnderperformingItems(double threshold) const {
    std::vector<std::string> underperforming;

    double utilization = rentalSystem ? rentalSystem->calculateSystemUtilizationRate() : 0.0;

    if (utilization < threshold) {
        underperforming.push_back("Low system utilization detected");
    }

    if (reportGenerator) {
        auto topCategories = reportGenerator->getTopPerformingCategories(5);
        if (topCategories.size() < 3) {
            underperforming.push_back("Limited high-performing categories");
        }
    } else {
        underperforming.push_back("Limited high-performing categories");
    }

    return underperforming;
}

double AnalyticsEngine::calculateCustomerLifetimeValue(const std::string&) const {
    double avgRentalValue = 100.0;
    double rentalFrequency = 4.0;
    double customerLifespan = 3.0;
    double retentionProbability = 0.7;
    double referralValue = 50.0;

    double baseLTV = avgRentalValue * rentalFrequency * customerLifespan;
    double adjustedLTV = baseLTV * retentionProbability + referralValue;

    return adjustedLTV;
}

std::string AnalyticsEngine::generateSeasonalAnalysis() const {
    double seasonalFactor = reservationSystem ? reservationSystem->getSeasonalDemandFactor() : 1.0;

    std::string season =
        seasonalFactor > 1.2 ? "High" :
        seasonalFactor < 0.8 ? "Low" :
                               "Moderate";

    return "Seasonal Analysis: " + season +
           " season detected with factor " +
           std::to_string(seasonalFactor);
}

double AnalyticsEngine::optimizePricingStrategy(const std::string& category) const {
    double basePrice = 50.0;

    double demandMultiplier = predictDemandTrend(category, 30) / 50.0;
    double competitionFactor = 0.9;

    double optimizedPrice = basePrice * demandMultiplier * competitionFactor;

    return std::max(10.0, optimizedPrice);
}

std::vector<std::string> AnalyticsEngine::getMaintenancePredictions() const {
    std::vector<std::string> predictions;

    double utilization = rentalSystem ? rentalSystem->calculateSystemUtilizationRate() : 0.0;

    if (utilization > 0.8) {
        predictions.push_back("High utilization - schedule preventive maintenance");
    }

    predictions.push_back("Regular maintenance check recommended");
    predictions.push_back("Equipment rotation suggested for optimal usage");

    return predictions;
}
