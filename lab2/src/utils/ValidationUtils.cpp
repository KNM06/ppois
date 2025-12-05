#include "../../include/utils/ValidationUtils.h"
#include <regex>
#include <algorithm>
#include <cctype>

bool ValidationUtils::isValidEmail(const std::string& email) {
    std::regex emailPattern(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");
    return std::regex_match(email, emailPattern);
}

bool ValidationUtils::isValidPhoneNumber(const std::string& phone) {
    std::regex phonePattern(R"(^\+?[1-9]\d{1,14}$)");
    
    std::string cleanPhone;
    for (char c : phone) {
        if (std::isdigit(c) || c == '+') {
            cleanPhone += c;
        }
    }
    
    return std::regex_match(cleanPhone, phonePattern) && cleanPhone.length() >= 10;
}

bool ValidationUtils::isValidCreditCard(const std::string& cardNumber) {
    std::string cleanNumber;
    for (char c : cardNumber) {
        if (std::isdigit(c)) {
            cleanNumber += c;
        }
    }
    
    if (cleanNumber.length() < 13 || cleanNumber.length() > 19) {
        return false;
    }
    
    int sum = 0;
    bool alternate = false;
    
    for (int i = cleanNumber.length() - 1; i >= 0; i--) {
        int digit = cleanNumber[i] - '0';
        
        if (alternate) {
            digit *= 2;
            if (digit > 9) {
                digit = (digit % 10) + 1;
            }
        }
        
        sum += digit;
        alternate = !alternate;
    }
    
    return (sum % 10) == 0;
}

bool ValidationUtils::isValidLicenseNumber(const std::string& license) {
    if (license.length() < 5 || license.length() > 20) {
        return false;
    }
    
    bool hasLetter = false;
    bool hasDigit = false;
    
    for (char c : license) {
        if (std::isalpha(c)) hasLetter = true;
        if (std::isdigit(c)) hasDigit = true;
        if (!std::isalnum(c) && c != ' ' && c != '-') return false;
    }
    
    return hasLetter && hasDigit;
}

bool ValidationUtils::isValidCustomerAge(int age, const std::string& itemCategory) {
    if (age < 18 || age > 100) {
        return false; 
    }
    
    if (itemCategory == "VEHICLE" && age < 21) {
        return false; 
    }
    
    if (itemCategory == "CONSTRUCTION" && age < 25) {
        return false; 
    }
    
    if (itemCategory == "LUXURY" && age < 25) {
        return false; 
    }
    
    if (itemCategory == "SPECIALIZED" && age < 30) {
        return false; 
    }
    
    return true;
}

bool ValidationUtils::isValidRentalDuration(int days, const std::string& category) {
    if (days <= 0 || days > 365) {
        return false; 
    }
    
    if (category == "VEHICLE" && days > 30) {
        return false; 
    }
    
    if (category == "ELECTRONICS" && days > 14) {
        return false; 
    }
    
    if (category == "CONSTRUCTION" && days > 90) {
        return false; 
    }
    
    if (category == "LUXURY" && days > 7) {
        return false; 
    }
    
    return true;
}

bool ValidationUtils::isValidItemId(const std::string& itemId) {
    if (itemId.length() < 3 || itemId.length() > 20) {
        return false;
    }
    
    std::vector<std::string> validPrefixes = {"VEH", "ELEC", "CONST", "TOOL", "SPORT", "PARTY", "LUX"};
    bool hasValidPrefix = false;
    
    for (const auto& prefix : validPrefixes) {
        if (itemId.find(prefix) == 0) {
            hasValidPrefix = true;
            break;
        }
    }
    
    if (!hasValidPrefix) {
        return false;
    }
    
    for (char c : itemId) {
        if (!std::isalnum(c) && c != '_') {
            return false;
        }
    }
    
    return true;
}

bool ValidationUtils::isValidCustomerId(const std::string& customerId) {
    if (customerId.length() < 5 || customerId.length() > 15) {
        return false;
    }
    
    std::vector<std::string> validPrefixes = {"CUST", "VIP", "BUS", "STU"};
    bool hasValidPrefix = false;
    
    for (const auto& prefix : validPrefixes) {
        if (customerId.find(prefix) == 0) {
            hasValidPrefix = true;
            break;
        }
    }
    
    if (!hasValidPrefix) {
        return false;
    }
    
    for (char c : customerId) {
        if (!std::isalnum(c)) {
            return false;
        }
    }
    
    return true;
}

bool ValidationUtils::isValidPrice(double price) {
    return price >= 0.0 && price <= 100000.0; 
}

bool ValidationUtils::isValidQuantity(int quantity) {
    return quantity > 0 && quantity <= 1000; 
}

bool ValidationUtils::hasSpecialCharacters(const std::string& text) {
    std::vector<char> dangerousChars = {'<', '>', '&', '\'', '"', ';', '(', ')', '[', ']', '{', '}'};
    
    for (char c : text) {
        if (std::find(dangerousChars.begin(), dangerousChars.end(), c) != dangerousChars.end()) {
            return true;
        }
    }
    
    return false;
}

bool ValidationUtils::isStrongPassword(const std::string& password) {
    if (password.length() < 8) {
        return false;
    }
    
    bool hasUpper = false;
    bool hasLower = false;
    bool hasDigit = false;
    bool hasSpecial = false;
    
    for (char c : password) {
        if (std::isupper(c)) hasUpper = true;
        else if (std::islower(c)) hasLower = true;
        else if (std::isdigit(c)) hasDigit = true;
        else if (!std::isalnum(c)) hasSpecial = true;
    }
    
    int criteriaMet = (hasUpper ? 1 : 0) + (hasLower ? 1 : 0) + 
                     (hasDigit ? 1 : 0) + (hasSpecial ? 1 : 0);
    
    return criteriaMet >= 3;
}