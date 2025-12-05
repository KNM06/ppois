#include "../../include/core/RentalSystem.h"
#include "../../include/core/Inventory.h"
#include "../../include/business/CustomerManager.h"
#include "../../include/business/ReservationSystem.h"
#include "../../include/business/PaymentProcessor.h"
#include "../../include/business/PricingStrategy.h"
#include "../../include/core/RentalAgreement.h"
#include "../../include/exceptions/RentalItemUnavailableException.h"
#include "../../include/exceptions/CustomerBlacklistedException.h"
#include <algorithm>
#include <cmath>

RentalSystem::RentalSystem() 
    : totalRevenue(0.0), totalRentalsProcessed(0) {}

RentalSystem::RentalSystem(std::unique_ptr<Inventory> inv,
                         std::unique_ptr<CustomerManager> custMgr,
                         std::unique_ptr<ReservationSystem> resSys,
                         std::unique_ptr<PaymentProcessor> payProc,
                         std::unique_ptr<PricingStrategy> priceStrat)
    : inventory(std::move(inv)),
      customerManager(std::move(custMgr)),
      reservationSystem(std::move(resSys)),
      paymentProcessor(std::move(payProc)),
      pricingStrategy(std::move(priceStrat)),
      totalRevenue(0.0),
      totalRentalsProcessed(0) {}

std::shared_ptr<RentalAgreement> RentalSystem::processRental(const std::string& customerId, 
                                                           const std::string& itemId, 
                                                           int durationDays) {
    if (!customerManager->isCustomerEligible(customerId)) {
        throw CustomerBlacklistedException("Customer is not eligible for rental");
    }
    
    if (!inventory->isItemAvailable(itemId)) {
        throw RentalItemUnavailableException("Requested item is not available");
    }
    
    auto agreement = std::make_shared<RentalAgreement>(customerId, itemId, durationDays);
    
    double amount = paymentProcessor->calculateTotalAmount(itemId, durationDays, customerId);
    if (paymentProcessor->processPayment(customerId, amount)) {
        activeRentals[agreement->getAgreementId()] = agreement;
        inventory->markItemAsRented(itemId);
        totalRevenue += amount;
        totalRentalsProcessed++;
        
        rentalHistory[customerId].push_back(agreement->getAgreementId());
        return agreement;
    }
    
    return nullptr;
}

double RentalSystem::calculateSystemUtilizationRate() const {
    int totalItems = inventory->getTotalItemCount();
    int rentedItems = inventory->getRentedItemCount();
    
    if (totalItems == 0) return 0.0;
    
    double utilization = static_cast<double>(rentedItems) / totalItems;
    double seasonalMultiplier = reservationSystem->getSeasonalDemandFactor();
    
    return std::min(1.0, utilization * seasonalMultiplier);
}

void RentalSystem::optimizeInventoryDistribution() {
    auto utilizationByCategory = inventory->getUtilizationByCategory();
    auto demandForecast = reservationSystem->getDemandForecast(30);
    
    for (const auto& [category, utilization] : utilizationByCategory) {
        double forecastDemand = demandForecast[category];
        
        if (utilization > 0.8 && forecastDemand > utilization) {
            inventory->adjustCategoryCapacity(category, 0.2);
        } else if (utilization < 0.3 && forecastDemand < utilization) {
            inventory->adjustCategoryCapacity(category, -0.15);
        }
    }
}

double RentalSystem::predictRevenueForPeriod(int daysForward) const {
    double baseRevenue = totalRevenue / totalRentalsProcessed * 30;
    auto demandForecast = reservationSystem->getDemandForecast(daysForward);
    
    double predictedRevenue = 0.0;
    for (const auto& [category, demand] : demandForecast) {
        double avgPrice = inventory->getAveragePriceForCategory(category);
        predictedRevenue += avgPrice * demand * 0.7;
    }
    
    double seasonalFactor = reservationSystem->getSeasonalDemandFactor();
    double growthTrend = 1.0 + (totalRentalsProcessed * 0.0001);
    
    return predictedRevenue * seasonalFactor * growthTrend;
}

void RentalSystem::applySeasonalPricingAdjustments() {
    double seasonalFactor = reservationSystem->getSeasonalDemandFactor();
    auto allCategories = inventory->getAllCategories();
    
    for (const auto& category : allCategories) {
        double adjustment = 1.0;
        if (seasonalFactor > 1.2) {
            adjustment = 1.15;
        } else if (seasonalFactor < 0.8) {
            adjustment = 0.9;
        }
        
        inventory->adjustCategoryPricing(category, adjustment);
    }
}

bool RentalSystem::validateRentalFeasibility(const std::string& itemId, const std::string& customerId) const {
    bool itemAvailable = inventory->isItemAvailable(itemId);
    bool customerEligible = customerManager->isCustomerEligible(customerId);
    bool hasPendingReservations = reservationSystem->hasConflictingReservations(itemId);
    
    return itemAvailable && customerEligible && !hasPendingReservations;
}

double RentalSystem::calculateCustomerLifetimeValue(const std::string& customerId) const {
    return customerManager->calculateCustomerLifetimeValue(customerId);
}

std::vector<std::string> RentalSystem::generateBusinessReport() const {
    std::vector<std::string> report;
    
    report.push_back("Total Revenue: " + std::to_string(totalRevenue));
    report.push_back("Total Rentals Processed: " + std::to_string(totalRentalsProcessed));
    report.push_back("System Utilization: " + std::to_string(calculateSystemUtilizationRate()));
    report.push_back("Active Rentals: " + std::to_string(activeRentals.size()));
    
    auto utilization = inventory->getUtilizationByCategory();
    for (const auto& [category, util] : utilization) {
        report.push_back(category + " Utilization: " + std::to_string(util));
    }
    
    return report;
}

bool RentalSystem::processReturn(const std::string& agreementId, const std::string& itemCondition) {
    auto agreementIt = activeRentals.find(agreementId);
    if (agreementIt != activeRentals.end()) {
        auto agreement = agreementIt->second;
        std::string itemId = agreement->getAgreementId();
        
        inventory->markItemAsReturned(itemId);
        activeRentals.erase(agreementIt);
        
        if (itemCondition != "EXCELLENT") {
            double damageCost = 0.0;
            agreement->applyDamagePenalty(damageCost);
        }
        
        return true;
    }
    return false;
}

std::shared_ptr<Inventory> RentalSystem::getInventory() const {
    return std::shared_ptr<Inventory>(inventory.get());
}

std::shared_ptr<CustomerManager> RentalSystem::getCustomerManager() const {
    return std::shared_ptr<CustomerManager>(customerManager.get());
}

void RentalSystem::setInventory(std::unique_ptr<Inventory> inv) {
    inventory = std::move(inv);
}

void RentalSystem::setCustomerManager(std::unique_ptr<CustomerManager> mgr) {
    customerManager = std::move(mgr);
}