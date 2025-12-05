#ifndef RENTALSYSTEM_H
#define RENTALSYSTEM_H

#include "../../include/core/Inventory.h"
#include "../../include/business/CustomerManager.h"
#include "../../include/business/ReservationSystem.h"
#include "../../include/business/PaymentProcessor.h"
#include "../../include/business/PricingStrategy.h"
#include "../../include/core/RentalAgreement.h"
#include <memory>
#include <vector>
#include <unordered_map>

class Inventory;
class CustomerManager;
class ReservationSystem;
class PaymentProcessor;
class PricingStrategy;

class RentalSystem {
private:
    std::unique_ptr<Inventory> inventory;
    std::unique_ptr<CustomerManager> customerManager;
    std::unique_ptr<ReservationSystem> reservationSystem;
    std::unique_ptr<PaymentProcessor> paymentProcessor;
    std::unique_ptr<PricingStrategy> pricingStrategy;
    std::unordered_map<std::string, std::shared_ptr<RentalAgreement>> activeRentals;
    std::unordered_map<std::string, std::vector<std::string>> rentalHistory;
    double totalRevenue;
    int totalRentalsProcessed;

public:
    RentalSystem();
    RentalSystem(std::unique_ptr<Inventory> inv,
                 std::unique_ptr<CustomerManager> custMgr,
                 std::unique_ptr<ReservationSystem> resSys,
                 std::unique_ptr<PaymentProcessor> payProc,
                 std::unique_ptr<PricingStrategy> priceStrat);
    
    std::shared_ptr<RentalAgreement> processRental(const std::string& customerId, 
                                                  const std::string& itemId, 
                                                  int durationDays);
    double calculateSystemUtilizationRate() const;
    void optimizeInventoryDistribution();
    double predictRevenueForPeriod(int daysForward) const;
    void applySeasonalPricingAdjustments();
    bool validateRentalFeasibility(const std::string& itemId, const std::string& customerId) const;
    double calculateCustomerLifetimeValue(const std::string& customerId) const;
    std::vector<std::string> generateBusinessReport() const;
    bool processReturn(const std::string& agreementId, const std::string& itemCondition);
    
    double getTotalRevenue() const { return totalRevenue; }
    int getTotalRentalsProcessed() const { return totalRentalsProcessed; }
    std::shared_ptr<Inventory> getInventory() const;
    std::shared_ptr<CustomerManager> getCustomerManager() const;
    
    void setInventory(std::unique_ptr<Inventory> inv);
    void setCustomerManager(std::unique_ptr<CustomerManager> mgr);
};

#endif