#ifndef VALIDATIONUTILS_H
#define VALIDATIONUTILS_H

#include <string>
#include <vector>

class ValidationUtils {
public:
    static bool isValidEmail(const std::string& email);
    static bool isValidPhoneNumber(const std::string& phone);
    static bool isValidCreditCard(const std::string& cardNumber);
    static bool isValidLicenseNumber(const std::string& license);
    static bool isValidCustomerAge(int age, const std::string& itemCategory);
    static bool isValidRentalDuration(int days, const std::string& category);
    static bool isValidItemId(const std::string& itemId);
    static bool isValidCustomerId(const std::string& customerId);
    static bool isValidPrice(double price);
    static bool isValidQuantity(int quantity);
    static bool hasSpecialCharacters(const std::string& text);
    static bool isStrongPassword(const std::string& password);
    
private:
    ValidationUtils() = delete; 
};

#endif